#include "Definitions.h"
#include "TryCatchBlock.h"
#include "ExceptionManager.h"


extern CExceptionManager _excManager;

CTryCatchBlock::CTryCatchBlock() :
        exc( 0 ),
        env{ 0 },
        prev( nullptr ),
        headObj( nullptr ),
        status( IN_TRY )
{}

CTryCatchBlock::~CTryCatchBlock()
{
    if( exc != NO_EXCEPTION ) {
        _excManager.Throw( exc );
    } else {
        _excManager.UnregisterTryCatchBlock();
    }
}

bool CTryCatchBlock::CheckType( int exceptionType )
{
    if( exc == exceptionType || exceptionType == ALL ) {
        exc = NO_EXCEPTION;
        return true;
    } else {
        return false;
    }
}
