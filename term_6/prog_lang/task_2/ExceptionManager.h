#ifndef TASK_2_EXCEPTIONMANAGER_H
#define TASK_2_EXCEPTIONMANAGER_H

#include "TryCatchBlock.h"


class CExceptionManager {

public:
    CExceptionManager() noexcept;

    void RegisterTryCatchBlock( CTryCatchBlock& block );
    void UnregisterTryCatchBlock();
    void Throw( const int exceptionType );
    int AddNewException() noexcept;

private:
    CTryCatchBlock* head;
    int nextAvailExcType;

    void unstageHead( CObject* end );
};

#endif //TASK_2_EXCEPTIONMANAGER_H
