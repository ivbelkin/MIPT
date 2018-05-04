#include <iostream>
#include <cstring>
using namespace std;

template <typename T>
class Array {
public:
    ~Array();

    void Add( const T& element );
    int Size() const;

    T& operator[]( int pos );
    const T& operator[]( int pos ) const;

private:
    static constexpr int initialCapacity = 4;
    T* buffer = new T[initialCapacity];
    int size = 0;
    int capacity = initialCapacity;
};

template<typename T>
void Array<T>::Add( const T& element )
{
    if( size >= capacity ) {
        capacity *= 2;
        T* tmp = new T[capacity];
        memcpy( tmp, buffer, sizeof(T) * capacity );
        delete [] buffer;
        buffer = tmp;
    }
    buffer[size++] = element;
}

template<typename T>
int Array<T>::Size() const
{
    return size;
}

template<typename T>
T& Array<T>::operator[]( int pos )
{
    std::cout << "non const";
    return buffer[pos];
}

template<typename T>
const T& Array<T>::operator[]( int pos ) const
{
    std::cout << "const";
    return buffer[pos];
}

template<typename T>
Array<T>::~Array() {
    delete [] buffer;
}

int main() {
    Array<int> arr;
    for (int i = 0; i < 10; ++i) {
        arr.Add(i);
    }
    for (int i = 0; i < arr.Size(); ++i) {
        std::cout << arr[i] << std::endl;
    }
    return 0;
}