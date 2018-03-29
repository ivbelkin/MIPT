import numpy as np

class AsmMapping:
    
    def __init__(self):
        self.reg2ad = {
            "EIP": 0, "ESP": 2, "ESI": 4, "EBP": 6,
            "EAX": 8, "EBX": 10, "ECX": 12, "EDX": 14,
        }
        self.ad2reg = {v:k for k,v in self.reg2ad.items()}
        
        self.command_names = [ "END", "MOV", "ADD", "SUB", "PUSH", "CALL", "RET", 
                               "SET", "PRINTC", "POP", "LOOP", "JMP", "JZ", "INPUT", "PRINTI" ]
        self.command_argnum = [ 0, 2, 2, 2, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1 ]
        self.command_argnum = dict( zip( self.command_names, self.command_argnum ) )
        self.com2code = dict( zip( self.command_names, 
            np.arange( len( self.command_names ), dtype=np.int8 ) ) )
        self.code2com = {v:k for k,v in self.com2code.items()}
        self.word_size = 5
        self.types = {
            "BYTE": { "size": 1, "type": np.uint8, "label": 0b10000000 },
            "INT":  { "size": 2, "type": np.int16, "label": 0b01000000 }
        }
        self.goto_commands = ["CALL", "LOOP", "JMP", "JZ"]
        self.expected_arg_type = {
            "END": [],
            "MOV": ["address", "address"],
            "ADD": ["address", "value"],
            "SUB": ["address", "value"],
            "PUSH": ["value"],
            "CALL": ["label"],
            "RET": ["value"],
            "SET": ["address", "value"],
            "PRINTC": ["address"],
            "POP": ["address"],
            "LOOP": ["label"],
            "JMP": ["label"],
            "JZ": ["label", "address"],
            "INPUT": ["address"],
            "PRINTI": ["address"]
        }
            
        self.start_addr = 30

