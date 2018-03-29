#! /bin/bash

# Компилируем исходники виртуальной машины
echo "g++ -std=c++11 -o vm vm.cpp"
g++ -std=c++11 -o vm vm.cpp

# Создаем фаил образа
echo "./assembly.py -i prog.asm -o prog.vm"
./assembly.py -i prog.asm -o prog.vm

# Запускаем его
echo "./vm prog.vm"
./vm prog.vm

# Дизассемблируем фаил образа
echo "./disassembly.py -i prog.vm -o prog_disassembled.asm"
./disassembly.py -i prog.vm -o prog_disassembled.asm

# Создаем фаил образа из дизассемблированного фаила
echo "./assembly.py -i prog_disassembled.asm -o prog_disassembled.vm"
./assembly.py -i prog_disassembled.asm -o prog_disassembled.vm

# Запускаем его
echo "./vm prog_disassembled.vm"
./vm prog_disassembled.vm

# Удаляем созданные фаилы
echo "rm -f vm prog.vm prog_disassembled.asm prog_disassembled.vm "
rm -f vm prog.vm prog_disassembled.asm prog_disassembled.vm 
