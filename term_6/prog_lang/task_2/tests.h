#ifndef TASK_2_TESTS_H
#define TASK_2_TESTS_H

#include "Exceptions.h"

IMPORT_EXCEPTION( OUT_OF_RANGE );
IMPORT_EXCEPTION( ZERO_DEVISION );
IMPORT_EXCEPTION( DESTROY_FAIL );


class A : public CObject {
public:
    A( int _num ) :
            num( _num )
    {
        std::cerr << "A( " << num << " )\n";
    }

    A( const A& a ) :
            num( a.num )
    {
        std::cerr << "A( A( " << num << " ) )\n";
    }

    ~A()
    {
        std::cerr << "~A( " << num << " )\n";
        if( num == 2 ) {
            THROW( DESTROY_FAIL );
        }
    }

private:
    int num;
};

void test_1();
void test_2();
void test_3();
void test_4();
void test_5();
void test_6();
void test_7( A a1 );
void test_8();
void test_9();

#endif //TASK_2_TESTS_H
