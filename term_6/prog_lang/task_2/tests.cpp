#include <iostream>

#include "Exceptions.h"
#include "tests.h"

REGISTER_EXCEPTION( OUT_OF_RANGE );
REGISTER_EXCEPTION( ZERO_DEVISION );
REGISTER_EXCEPTION( DESTROY_FAIL );


void test_1()
{
    TRY {
        std::cout << "TRY\n";
        THROW( OUT_OF_RANGE );
        std::cout << "unreachable\n";
    } CATCH( OUT_OF_RANGE ) {
        std::cout << "OUT_OF_RANGE\n";
        THROW( ZERO_DEVISION );
        std::cout << "unreachable\n";
    } CATCH( ZERO_DEVISION ) {
        std::cout << "unreachable\n";
    } YRT
}

void test_2()
{
    TRY {
        std::cout << "TRY\n";
        THROW( OUT_OF_RANGE );
        std::cout << "unreachable\n";
    } CATCH( ZERO_DEVISION ) {
        std::cout << "unreachable\n";
    } CATCH( ALL ) {
        std::cout << "ALL\n";
    } YRT
}

void test_3()
{
    TRY {
        TRY {
            THROW( ZERO_DEVISION );
            std::cout << "unreachable\n";
        } CATCH( OUT_OF_RANGE ) {
            std::cout << "unreachable\n";
        } YRT
        std::cout << "unreachable\n";
    } CATCH( OUT_OF_RANGE ) {
        std::cout << "unreachable\n";
    } CATCH( ZERO_DEVISION ) {
        std::cout << "ZERO_DIVISION\n";
        THROW( OUT_OF_RANGE );
    } YRT
}

void test_4()
{
    test_3();
}

void test_5()
{
    TRY {
        test_4();
    } CATCH( ZERO_DEVISION ) {
        std::cout << "unreachable\n";
    } CATCH( OUT_OF_RANGE ) {
        std::cout << "OUT_OF_RANGE\n";
    } YRT
    std::cout << "reachable\n";
}

void test_6()
{
    TRY {
        THROW( ZERO_DEVISION );
    } CATCH ( OUT_OF_RANGE ) {
        std::cout << "unreachable\n";
    } YRT
    std::cout << "unreachable\n";
}



void test_7( A a1 )
{
    A a2( 2 );
    TRY {
        A a3( 3 );
        THROW( OUT_OF_RANGE );
        std::cout << "unreachable\n";
    } CATCH( ZERO_DEVISION ) {
        A a4( 4 );
    } YRT
    std::cout << "unreachable\n";
}

void test_8()
{
    A a0( 0 );
    TRY {
        A a1( 1 );
        test_7( a1 );
        std::cout << "unreachable\n";
    } CATCH( ZERO_DEVISION ) {
        std::cout << "OUT_OF_RANGE\n";
    } YRT
}

void test_9()
{
    TRY {
        THROW( OUT_OF_RANGE );
        std::cout << "unreachable\n";
    } CATCH( ALL ) {
        std::cout << "ALL\n";
    } YRT
    std::cout << "reachable\n";
    TRY {
        THROW( OUT_OF_RANGE );
        std::cout << "unreachable\n";
    } CATCH( OUT_OF_RANGE ) {
        std::cout << "OUT_OF_RANGE\n";
    } YRT
    std::cout << "reachable\n";
}