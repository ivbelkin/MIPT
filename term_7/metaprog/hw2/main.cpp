#include <iostream>
#include <vector>
#include <list>

//-----------------------------------------------------------------------------
//                                 TypeList
//-----------------------------------------------------------------------------

struct NullType {};

template<typename... Args>
struct TypeList {
    using head = NullType;
    using tail = NullType;
};

template<typename H, typename... T>
struct TypeList<H, T...> {
    using head = H;
    using tail = TypeList<T...>;
};

template<typename H>
struct TypeList<H> {
    using head = H;
    using tail = NullType;
};

using EmptyList = TypeList<>;

//-----------------------------------------------------------------------------
//                            PushFront<TypeList, TT>
//-----------------------------------------------------------------------------

template<typename TList, typename TT>
struct PushFront;

template<typename H, typename... T, typename TT>
struct PushFront<TypeList<H, T...>, TT> {
    using Result = TypeList<TT, H, T...>;
};

template<typename TT>
struct PushFront<EmptyList, TT> {
    using Result = TypeList<TT>;
};

//-----------------------------------------------------------------------------
//                             GetPrefix<TypeList, N>
//-----------------------------------------------------------------------------

template<typename TList, int N>
struct GetPrefix {
    using Result = EmptyList;
};

template<typename H, typename... T, int N>
struct GetPrefix<TypeList<H, T...>, N> {
    using Result = typename PushFront<
            typename GetPrefix<TypeList<T...>, N - 1>::Result, H>::Result;
};

template<int N>
struct GetPrefix<EmptyList, N> {
    using Result = EmptyList;
};

template<typename H, typename... T>
struct GetPrefix<TypeList<H, T...>, 0> {
    using Result = EmptyList;
};

//-----------------------------------------------------------------------------
//                            SkipPrefix<TypeList, N>
//-----------------------------------------------------------------------------

template<typename TList, int N>
struct SkipPrefix;

template<typename H, typename... T, int N>
struct SkipPrefix<TypeList<H, T...>, N> {
    using Result = typename SkipPrefix<TypeList<T...>, N - 1>::Result;
};

template<int N>
struct SkipPrefix<EmptyList, N> {
    using Result = EmptyList;
};

template<typename H, typename... T>
struct SkipPrefix<TypeList<H, T...>, 0> {
    using Result = TypeList<H, T...>;
};

//-----------------------------------------------------------------------------
//                            GetSlice<TypeList, S, L>
//-----------------------------------------------------------------------------

template<typename TList, int S, int L>
struct GetSlice;

template<typename... Args, int S, int L>
struct GetSlice<TypeList<Args...>, S, L> {
    using Result = typename GetPrefix<
            typename SkipPrefix<TypeList<Args...>, S - 1>::Result, L>::Result;
};

template<typename... Args, int L>
struct GetSlice<TypeList<Args...>, 0, L> {
    using Result = typename GetPrefix<TypeList<Args...>, L>::Result;
};

//-----------------------------------------------------------------------------
//                             Fibonacci Numbers
//-----------------------------------------------------------------------------

// 1 1 2 3 5 8  13 21
template<int N>
struct FiboCum {
    constexpr static int value = FiboCum<N - 1>::value + FiboCum<N - 2>::value;
};

template<>
struct FiboCum<1> {
    constexpr static int value = 1;
};

template<>
struct FiboCum<2> {
    constexpr static int value = 1;
};

//-----------------------------------------------------------------------------
//                              Linear Hierarchy
//-----------------------------------------------------------------------------

template<class, class Base>
class Unit : public Base {};

template<class TList, class Root = NullType>
class GenLinearHierarchy;

template <typename H, typename... T, class Root>
class GenLinearHierarchy<TypeList<H, T...>, Root> :
        public Unit<H, GenLinearHierarchy<TypeList<T...>, Root>>
{
    //
};

template<class Root>
class GenLinearHierarchy<EmptyList, Root>
{
    //
};

//-----------------------------------------------------------------------------
//                              Fibonacci-Scatter Hierarchy
//-----------------------------------------------------------------------------

template<class T, int N = 1>
class GenFiboHierarchy : public T
{
    //
};

template<class H, typename... T, int N>
class GenFiboHierarchy<TypeList<H, T...>, N> :
    public GenLinearHierarchy< typename GetPrefix<TypeList<H, T...>, FiboCum<N>::value + 1>::Result >,
    public GenFiboHierarchy< typename SkipPrefix<TypeList<H, T...>, FiboCum<N>::value + 1>::Result, N + 1 >
{
    //
};

template<int N>
class GenFiboHierarchy<EmptyList, N>
{
    //
};

int main()
{
    using T = GenFiboHierarchy<TypeList<int, std::vector<int>, double, std::list<char>, int, float>>;
    std::cout << sizeof( T );
    return 0;
}