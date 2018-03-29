.data
    Intro BYTE 'Enter N (0 - 23) to compute N-th Fibonacci number F(N): '
    Res BYTE 'F(N) = '
.code

PRINT proc
    MOV EBP, ESP
    SUB EBP, 4
    MOV EAX, [EBP]
    
    ADD EAX, 1
    MOV ECX, [EAX]
    
    ADD EAX, 2
L4: PRINTC [EAX]
    ADD EAX, 1
    LOOP L4
    RET 1
PRINT endp

FIBO proc
    MOV EBP, ESP
    SUB EBP, 4
    MOV EBX, [EBP]
    
    SET ECX, 0
    
    MOV EDX, EBX
    JZ L2, EDX
    SUB EDX, 1
    JZ L2, EDX
    JMP L3
    
L2: SET ECX, 1
    JMP L1
    
L3: SUB EBX, 1
    PUSH [ECX]
    PUSH [EBX]
    PUSH [EBX]
    CALL FIBO
    POP EBX
    POP ECX
    ADD ECX, [EAX] 
    
    SUB EBX, 1
    PUSH [ECX]
    PUSH [EBX]
    PUSH [EBX]
    CALL FIBO
    POP EBX
    POP ECX
    ADD ECX, [EAX]
    
L1: MOV EAX, ECX
    RET 1
FIBO endp

program: 
    PUSH Intro
    CALL PRINT 
    
    INPUT EAX
    PUSH [EAX]
    CALL FIBO
    
    PUSH [EAX]
    PUSH Res
    CALL PRINT
    POP EAX
    PRINTI EAX
    
END program
