#ifndef TASK_2_DEFINITIONS_H
#define TASK_2_DEFINITIONS_H

#define NO_EXCEPTION 0
#define ALL 1


#define TRY \
{CTryCatchBlock _block; \
_excManager.RegisterTryCatchBlock( _block ); \
_block.exc = _setjmp( _block.env ); \
if( _block.exc == NO_EXCEPTION )


#define YRT }


#define CATCH( exceptionType ) \
else if( _block.CheckType( (exceptionType) ) )


#define THROW( exceptionType ) \
_excManager.Throw( (exceptionType) )


#define REGISTER_EXCEPTION( exceptionName ) \
const int exceptionName = _excManager.AddNewException()


#define IMPORT_EXCEPTION( exceptionName ) \
extern const int (exceptionName)


#endif //TASK_2_DEFINITIONS_H
