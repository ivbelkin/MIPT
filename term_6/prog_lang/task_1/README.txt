
Пример запуска в фаиле run.sh


Разрядность адреса: 16 бит. Все операнды размерности 16 бит.

Все имена заменяются адресами с ними связанными на этапе ассемблирования.
Если операнд команды заключен в квадратные скобки (напр. [EAX]), 
то в качестве аргумента команда получит значение записанное по этому адресу.

Описание команд:
	1. Установить значение <value> в ячейку по адресу <address>
		SET <address> <value>

	2. Записать содержимое ячейки по адресу <source_addr> в ячейку по адресу <target_addr>
		MOV <target_addr> <source_addr>

	3. Прибавить к ячейке по адресу <addr> значение <value>
		ADD <addr> <value>
	
	4. Отнять от ячейки по адресу <addr значение <value>
		SUB <A addr> <value> ; A = A - B

	5. Положить на стек значение <value>
		PUSH <value>

	6. Вызвать процедуру с именем <addr>
		CALL <addr>

	7. Возврат из процедуры с перемешением указателя стека на <value> позиций
		RET <value>

	8. Снять значение со стека и положить по адресу <addr>
		POP <addr>

	9. Напечатать один символ, записанный по адресу <addr>
		PRINTC <addr>

	10. Цикл с возвратом на метку <addr> с использованием регистра ECX
		LOOP <addr>
	
		Пример:
			SET ECX, 10
		L:	...
			...
			LOOP L

	11. Безусловный переход на метку <addr>
		JMP <addr>

	12. Переход на метку <addr_to_jump> если значение по адресу <addr_of_var> равно нулю
		JZ <addr_to_jump>, <addr_of_var>

	13. Ввод 16 битного значения в ячейку по адресу <addr>
		INPUT <addr>

	14. Вывод 16 битного числа по адресу <addr>
		PRINTI <addr>


Структура программы:

.data							; Описание статических данных
		
	A INT 53					; 16 битная переменная
	String BYTE 'Hello, World!!!'			; Массив однобайтных переменных
   
.code

Procedure proc						; Процедура, описывается в начале секции .code
   	RET 1
Procedure endp

Procedure2 proc						; Еще одна процедура
   	RET 2
Procedure2 endp

program:						; Точка входа. Имя programm фиксировано
   	PUSH 1
L5:  
L3:   	CALL  Procedure
L4:   	PUSH 1
   	PUSH [EAX]
   	CALL  Procedure

END program
