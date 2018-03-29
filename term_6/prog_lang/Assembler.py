import numpy as np
import re

from AsmMapping import AsmMapping


class Assembler:
    
    def __init__(self, tree):
        self.map = AsmMapping()
        self.addr = self.map.start_addr
        self.word_size = self.map.word_size
        self.types = self.map.types
        
        self.label2ad = {}
        
        self.preprocess(tree)
        self.encode(tree)
        
        self.state = np.zeros((65536,), dtype=np.uint8)
        self.write_to_state(tree)
        
        
    def write_to_state(self, tree):
        self.write_data_to_state(tree[".data"])
        self.write_code_to_state(tree[".code"])
        
        
    def write_data_to_state(self, data):
        for record in data:
            addr = record["address"]
            self.state[addr] = self.types[record["type"]]["label"]
            addr += 1
            addr = self.write_value_to_state(len(record["values"]), self.types["INT"], addr)
            for value in record["values"]:
                addr = self.write_value_to_state(value, self.types[record["type"]], addr)
                
    
    def write_code_to_state(self, code):
        self.write_commands_to_state(code["entrypoint"], code["startaddress"])
        for proc in code["procedures"]:
            self.write_commands_to_state(proc["code"], proc["address"])
        self.write_value_to_state(code["startaddress"], self.types["INT"], self.map.reg2ad["EIP"])
        self.write_value_to_state(self.addr, self.types["INT"], self.map.reg2ad["ESP"])
      

    def write_commands_to_state(self, commands, addr):
        for cml in commands:
            if cml["command"]:
                self.state[addr] = cml["command"]
                assert len(cml["args"]) < 3
                for i, arg in enumerate(cml["args"]):
                    #self.write_value_to_state(arg, self.types["INT"], addr + 2*i + 1)
                    self.state[addr + 1 + 2 * i] = arg % 256
                    self.state[addr + 1 + 2 * i + 1] = arg // 256
                    
                addr += self.word_size

                
    def write_value_to_state(self, value, tp, addr):
        for i in range(tp["size"]):
            self.state[addr] = value % 256
            value //= 256
            addr += 1
        return addr
        
        
    def preprocess(self, tree):
        self.prepare_data(tree[".data"])
        self.extract_labels(tree[".code"])
        
        
    def prepare_data(self, data):
        for record in data:
            record["values"] = self.expand_values(record["values"], 
                                                  self.types[record["type"]])
        for record in data:
            record["address"] = self.addr
            self.addr += 3
            self.addr += len(record["values"]) * self.types[record["type"]]["size"]
            if self.addr % self.word_size != 0:
                self.addr += self.word_size - self.addr % self.word_size
        self.addr += self.word_size
        for record in data:
            self.label2ad[record["name"]] = record["address"]
            
            
    def expand_values(self, values, tp):
        expanded = []
        for value in values:
            try:
                val = [tp["type"](value)]
            except:
                val = list(value)
                for i, v in enumerate(val):
                    val[i] = tp["type"](ord(v))
            expanded += val
        return expanded
    
    
    def extract_labels(self, code):
        self.preprocess_procs(code["procedures"])
        code["startaddress"] = self.addr
        self.extract_labels_from_code(code["entrypoint"])
    
    
    def preprocess_procs(self, procedures):
        for proc in procedures:
            proc["address"] = self.addr
            self.label2ad[proc["name"]] = self.addr
            self.extract_labels_from_code(proc["code"])
        self.addr += self.word_size
                    
                    
    def extract_labels_from_code(self, code):
        for cml in code:
            if cml["label"]:
                self.label2ad[cml["label"]] = self.addr
            if cml["command"]:
                self.addr += self.word_size
            
                
        
    def encode(self, tree):
        for cml in tree[".code"]["entrypoint"]:
            self.encode_command_line(cml)
        for proc in tree[".code"]["procedures"]:
            for cml in proc["code"]:
                self.encode_command_line(cml)
    
    
    def encode_command_line(self, cml):
        if cml["command"]:
            cml["command"] = self.map.com2code[cml["command"]]
            cml["args"] += ["0"] * (2 - len(cml["args"]))
            for i in range(2):
                arg = cml["args"][i]
                cml["args"][i], as_address = self.encode_arg(arg)
                if as_address:
                    cml["command"] += 2 ** (7 - i)
                    
    
    def encode_arg(self, arg_line):
        if arg_line.startswith("["):
            as_address = True
        else:
            as_address = False
        arg_comps = arg_line.strip(" []").split()
        for i, arg_comp in enumerate(arg_comps):
            arg_comps[i] = str(self.encode_arg_comp(arg_comp))
        arg_line = " ".join(arg_comps)
        return eval(arg_line), as_address
    
    
    def encode_arg_comp(self, arg_comp):
        if arg_comp in self.map.reg2ad:
            return self.map.reg2ad[arg_comp]
        elif arg_comp in self.label2ad:
            return self.label2ad[arg_comp]
        else:
            return arg_comp

