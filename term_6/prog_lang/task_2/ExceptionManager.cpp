#include <csetjmp>
#include <cstdlib>
#include <iostream>
#include <cassert>

#include "ExceptionManager.h"
#include "Object.h"
#include "Definitions.h"

extern CExceptionManager _excManager;

CExceptionManager::CExceptionManager() :
        head( nullptr ),
        nextAvailExcType( 2 )
{}

void CExceptionManager::RegisterTryCatchBlock( CTryCatchBlock& block )
{
    block.prev = head;
    block.headObj = CObject::GetHead();
    head = &block;
}

void CExceptionManager::UnregisterTryCatchBlock()
{
    assert( head != nullptr );
    head = head->prev;
}

void CExceptionManager::Throw( const int exceptionType )
{
    if( head != nullptr ) {
        if( head->status == CTryCatchBlock::IN_CATCH ) {
            unstageHead( head->headObj );
            head = head->prev;
        }
        if( head != nullptr ) {
            unstageHead( head->headObj );
            head->status = CTryCatchBlock::IN_CATCH;
            longjmp( head->env, exceptionType );
        } else {
            // такое исключение не было никем захвачено
            unstageHead( nullptr );
            exit( exceptionType );
        }
    }
}

int CExceptionManager::AddNewException()
{
    if( nextAvailExcType != NO_EXCEPTION ) {
        return nextAvailExcType++;
    } else {
        return ALL;
    }
}

void CExceptionManager::unstageHead( CObject* end )
{
    for( CObject* headObj = CObject::GetHead();
         headObj != end;
         headObj = CObject::GetHead() )
    {
        TRY {
            headObj->~CObject();
        } CATCH( ALL ) {
            std::cerr << "Exception in destructor!\n";
            exit( EXIT_FAILURE );
        } YRT
    }
}