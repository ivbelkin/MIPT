// Задание на макс. 8
// По всей видимости задачка с отсылкой к чтению таблиц, поэтому считаем
// что входные данные в csv формате. То есть делим строки на элементы по
// запятой, передаем полученные подстроки в декомпрессоры
// и складываем полученные элементы последовательно в буфер, который возвращаем

// Есть ElementDecompressor - в нем зашита логика выбора способа декомпрессии
// это шаблон от Type, CompressedType, TypeDecompressor

// Reader содержит в себе ElementDecompressor для первого типа в списке,
// соответственно его он может обработать самостоятельно. Обработку остальных
// элементов списка Reader делегирует своему предку от TypeList'ов
// без первых элементов

// Протестил на CompressedInt и IntDecompressor. Первый при декомпресии
// увеличивает число вдвое, второй меяет знак

// Для построчного вывода таблицы через пробел в stdout используется LinePrinter
// Вывод для фаила input.csv:
// 4 -3
// 346 0
// 18 2
// 0 0

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

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

using EmptyTypeList = TypeList<>;

//-----------------------------------------------------------------------------
//                          NoneType + NoDecompressor
//-----------------------------------------------------------------------------

struct NoneType {};
struct NoDecompressor {};

//-----------------------------------------------------------------------------
//                            ElementDecompressor
//-----------------------------------------------------------------------------

template<typename T, typename C, typename D>
class ElementDecompressor;

template<typename T, typename C>
class ElementDecompressor<T, C, NoDecompressor> {
public:
    T Decompress( const std::string& compressed )
    {
        return C( compressed ).Decompress();
    }
};

template<typename T, typename D>
class ElementDecompressor<T, NoneType, D> {
public:
    T Decompress( const std::string& compressed )
    {
        return D().Decompress( compressed );
    }
};

//-----------------------------------------------------------------------------
//                                   Reader
//-----------------------------------------------------------------------------

template<typename TList, typename DList, typename FList>
class Reader;

template<>
class Reader<EmptyTypeList, EmptyTypeList, EmptyTypeList> {
public:
    explicit Reader( const std::string& filename ) :
        fin( filename, std::ios_base::in ),
        lineSize( 0 )
    {
        //
    }

protected:
    std::ifstream fin;
    size_t lineSize;

    virtual void readLine( void* dst )
    {
        //
    }
};

template<typename T, typename... TList, typename C, typename... CList, typename D, typename... DList>
class Reader<TypeList<T, TList...>, TypeList<C, CList...>, TypeList<D, DList...>> :
    public Reader<TypeList<TList...>, TypeList<CList...>, TypeList<DList...>>
{
    using Base = Reader<TypeList<TList...>, TypeList<CList...>, TypeList<DList...>>;
public:

    explicit Reader( const std::string& filename ) :
        Base::Reader( filename )
    {
        lineSize = Base::lineSize + sizeof( T );
    }

    void* ReadNextLine()
    {
        if( !Base::fin ) {
            return nullptr;
        } else {
            void* buf = std::malloc( lineSize );
            readLine( buf );
            return buf;
        }
    }

protected:
    size_t lineSize;

    void readLine( void* dst ) override
    {
        std::string compressed = readElement();
        T element = elementDecompressor.Decompress( compressed );
        std::memcpy( dst, &element, sizeof( T ) );
        Base::readLine( static_cast<char*>( dst ) + sizeof( T ) );
    }

private:
    ElementDecompressor<T, C, D> elementDecompressor;

    std::string readElement()
    {
        std::string s;
        char c = 0;
        Base::fin.get( c );
        while( Base::fin && c != ',' && c != '\n' ) {
            s.push_back( c );
            Base::fin.get( c );
        }
        return s;
    }
};

//-----------------------------------------------------------------------------
//                               class Printer
//-----------------------------------------------------------------------------

template<typename TList>
class LinePrinter;

template<>
class LinePrinter<EmptyTypeList> {
public:
    virtual void Print( void* line )
    {
        std::cout << std::endl;
    }
};

template<typename H, typename... T>
class LinePrinter<TypeList<H, T...>> :
    public LinePrinter<TypeList<T...>>
{
    using Base = LinePrinter<TypeList<T...>>;
public:
    void Print( void* line ) override
    {
        std::cout << *static_cast<H*>( line ) << " ";
        Base::Print( static_cast<char*>( line ) + sizeof( H ) );
    }
};

//-----------------------------------------------------------------------------
//                            class CompressedInt
//-----------------------------------------------------------------------------

class CompressedInt {
public:
    explicit CompressedInt( const std::string& _compressed ) :
        compressed( _compressed )
    {
        //
    }

    int Decompress()
    {
        return 2 * atoi( compressed.c_str() );
    }

private:
    const std::string& compressed;
};

//-----------------------------------------------------------------------------
//                            class IntDecompressor
//-----------------------------------------------------------------------------

class IntDecompressor {
public:
    int Decompress( const std::string& compressed )
    {
        return -atoi( compressed.c_str() );
    }
};


int main()
{
    using LineType = TypeList<int, int>;

    Reader<LineType,
           TypeList<CompressedInt, NoneType>,
           TypeList<NoDecompressor, IntDecompressor>> reader( "input.csv" );

    LinePrinter<LineType> printer;

    void* line = nullptr;
    while( line = reader.ReadNextLine() ) {
        printer.Print( line );
    }

    return 0;
}