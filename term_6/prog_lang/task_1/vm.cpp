#include <iostream>
#include <fstream>
#include <cstdint>
#include <iomanip>
#include <string>
#include <vector>
#include <cstring>

#define MEM8( addr ) ( static_cast<uint8_t*>( memory ) + (addr) )
#define MEM16( addr ) ( reinterpret_cast<uint16_t*>( MEM8( (addr) ) ) )

class VirtualMachine {
public:
    VirtualMachine();
    ~VirtualMachine();

    void init( const char* filename );
    void run( bool debug = false );

private:
    enum Command {
        END, MOV, ADD, SUB, PUSH, CALL, RET, SET, PRINTC, POP, LOOP, JMP, JZ, INPUT, PRINTI
    };
    enum Register {
        EIP = 0, ESP = 2, ESI = 4, EBP = 6,
        EAX = 8, EBX = 10, ECX = 12, EDX = 14,
    };

    void print_debug( uint16_t stackStart );

    const uint8_t COMMSIZE = sizeof( uint8_t ) * 1;
    const uint8_t ADDRSIZE = sizeof( uint8_t ) * 2;
    const uint8_t WORDSIZE = sizeof( uint8_t ) * 5;

    inline Command nextCommand() { return static_cast<Command>( *MEM8( *MEM16( EIP ) ) & 0b00111111 ); }
    inline bool firstAsAddress() { return *MEM8( *MEM16( EIP ) ) & 0b10000000; }
    inline uint16_t firstOperand() {
        if( firstAsAddress() ) {
            return *MEM16( *MEM16( *MEM16( EIP ) + COMMSIZE ) );
        } else {
            return *MEM16( *MEM16( EIP ) + COMMSIZE );
        }
    }
    inline bool secondAsAddress() { return *MEM8( *MEM16( EIP ) ) & 0b01000000; }
    inline uint16_t secondOperand() {
        if( secondAsAddress() ) {
            return *MEM16( *MEM16( *MEM16( EIP ) + COMMSIZE + ADDRSIZE ) );
        } else {
            return *MEM16( *MEM16( EIP ) + COMMSIZE + ADDRSIZE );
        }

    }
    inline void mov() { 
        *MEM16( firstOperand() ) = *MEM16( secondOperand() );
        incEIP();
    }
    inline void add() { 
        *MEM16( firstOperand() ) += secondOperand();
        incEIP();
    }
    inline void sub() { 
        *MEM16( firstOperand() ) -= secondOperand();
        incEIP();
    }
    inline void push() {
        *MEM16( *MEM16( ESP ) ) = firstOperand();
        *MEM16( ESP ) += ADDRSIZE;
        incEIP();
    }
    inline void call() {
        *MEM16( *MEM16( ESP ) ) = *MEM16( EIP ) + WORDSIZE;
        *MEM16( ESP ) += ADDRSIZE;
        *MEM16( EIP ) = firstOperand();
    }
    inline void ret() {
        *MEM16( ESP ) -= ADDRSIZE;
        *MEM16( ESP ) -= firstOperand() * ADDRSIZE;
        *MEM16( EIP ) = *MEM16( *MEM16( ESP ) + firstOperand() * ADDRSIZE );
    }
    inline void set() {
        *MEM16( firstOperand() ) = secondOperand();
        incEIP();
    }
    inline void printc() {
        std::cout << *MEM8( firstOperand() );
        incEIP();
    }
    inline void pop() {
        *MEM16( ESP ) -= ADDRSIZE;
        *MEM16( firstOperand() ) = *MEM16( *MEM16( ESP ) );
        incEIP();
    }
    inline void loop() {
        *MEM16( ECX ) -= 1;
        if( *MEM16( ECX ) == 0 ) {
            incEIP();
        } else {
            *MEM16( EIP ) = firstOperand();
        }
    }
    inline void jmp() {
        *MEM16( EIP ) = firstOperand();
    }
    inline void jz() {
        if( *MEM16( secondOperand() ) == 0 ) {
            *MEM16( EIP ) = firstOperand();
        } else {
            incEIP();
        }
    }
    inline void input() {
        std::cin >> *MEM16( firstOperand() );
        incEIP();
    }
    inline void printi() {
        std::cout << *MEM16( firstOperand() );
        incEIP();
    }

    inline void incEIP() { *MEM16( EIP ) += WORDSIZE; }

    const int MEMSIZE = 65536;
    void* memory;
};

int main(int argc, char** argv)
{
    VirtualMachine vm;
    if( argc < 2 ) {
        std::cout << "Usage: vm <filename>" << std::endl;
    } else {
        vm.init( argv[1] );
        bool debug = false;
        if( argc == 3 && strcmp( argv[2], "-d" ) == 0 ) {
            debug = true;
        }
        vm.run(debug);
        std::cout << std::endl;
    }

    return 0;
}

VirtualMachine::VirtualMachine()
{
    memory = malloc( MEMSIZE );
}

VirtualMachine::~VirtualMachine()
{
    free( memory );
}

void VirtualMachine::init( const char* filename )
{
    std::ifstream fin( filename, std::ios::binary );
    fin.read( static_cast<char*>( memory ), MEMSIZE );
}

void VirtualMachine::run( bool debug )
{
    uint16_t stackStart = *MEM16( ESP );
    while( true ) {
        if( debug ) {
            print_debug( stackStart );
        }
        switch( nextCommand() ) {
            case END:
                return;
            case MOV:
                mov(); break;
            case ADD:
                add(); break;
            case SUB:
                sub(); break;
            case PUSH:
                push(); break;
            case CALL:
                call(); break;
            case RET:
                ret(); break;
            case SET:
                set(); break;
            case PRINTC:
                printc(); break;
            case POP:
                pop(); break;
            case LOOP:
                loop(); break;
            case JMP:
                jmp(); break;
            case JZ:
                jz(); break;
            case INPUT:
                input(); break;
            case PRINTI:
                printi(); break;
            default:
                std::cout << "Unknown command\n Exit\n";
                return;
        }
    }
}

void VirtualMachine::print_debug( uint16_t stackStart )
{
    std::vector<std::string> names = {
            "END", "MOV", "ADD", "SUB", "PUSH", "CALL", "RET", "SET",
            "PRINTC", "POP", "LOOP", "JMP", "JZ", "INPUT", "PRINTI"
    };
    std::cerr << std::setw(10) << "EIP" << std::setw(10) << "ESP"
              << std::setw(10) << "ESI" << std::setw(10) << "EBP"
              << std::setw(10) << "EAX" << std::setw(10) << "EBX"
              << std::setw(10) << "ECX" << std::setw(10) << "EDX" << std::endl;
    std::cerr << std::setw(10) << int(*MEM16( EIP ))  << std::setw(10) << int(*MEM16( ESP ))
              << std::setw(10) << int(*MEM16( ESI )) << std::setw(10) << int(*MEM16( EBP ))
              << std::setw(10) << int(*MEM16( EAX )) << std::setw(10) << int(*MEM16( EBX ))
              << std::setw(10) << int(*MEM16( ECX )) << std::setw(10) << int(*MEM16( EDX )) << std::endl;

    std::cerr << "Stack top:\n";
    for( uint16_t i = stackStart; i < *MEM16( ESP ); i += ADDRSIZE) {
        std::cerr << std::setw(10) << int( i );
    }
    std::cerr << std::endl;
    for( uint16_t i = stackStart; i < *MEM16( ESP ); i += ADDRSIZE ) {
        std::cerr << std::setw(10) << int( *MEM16( i ) );
    }
    std::cerr << std::endl;
    std::cerr << "Command: " << std::setw(10) << names[nextCommand()] << " "
              << std::setw(10) << int( firstOperand() ) << " "
              << std::setw(10) << int( secondOperand() ) << " ";
    std::cerr << std::endl << std::endl;
}