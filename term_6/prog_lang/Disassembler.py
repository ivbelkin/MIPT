import numpy as np
import re

from AsmMapping import AsmMapping


class Disassembler:
    
    def __init__( self ):
        self.map = AsmMapping()
        self.ad2name = self.map.ad2reg
        
        self.types = self.map.types
        
        self.addr = self.map.start_addr
        
        self.varname_template = "Var_{}"
        self.procname_template = "Proc_{}"
        self.label_template = "L{}"
        
        self.word_size = self.map.word_size


    def disassembly(self, input_file, output_file):
        self.sections = {".data":[], ".code":[]}

        with open(input_file, "rb") as f:
            self.state = np.array(list(f.read()))
            
        self.parse_data_section()
        self.parse_code_section()
        self.write_to_file(output_file)
    
    
    def parse_data_section(self):
        data = []
        var_num = 1
        while self.state[self.addr] != 0:
            address = self.addr
            for tp_name, type_ in self.types.items():
                if self.state[self.addr] == type_["label"]:
                    tp = tp_name        
            self.addr += 1
            size = self.read_value_from_state("INT")
            values = []
            for i in range(size):
                values.append(self.read_value_from_state(tp))
            name = self.varname_template.format(var_num)
            self.ad2name[address] = name
            if tp == "BYTE":
                values = "".join(map(chr, values))
            data.append({
                "name": name,
                "type": tp,
                "values": values,
                "address": address,
            })
            var_num += 1
            if self.addr % self.word_size != 0:
                self.addr += self.word_size - self.addr % self.word_size
        self.addr += self.word_size
        self.sections[".data"] = data
        
        
    def read_value_from_state(self, tp):
        base = 1
        value = 0
        for i in range(self.types[tp]["size"]):
            value += base * self.state[self.addr]
            self.addr += 1
            base *= 256
        return value
        
    
    def parse_code_section(self):
        self.sections[".code"] = {"procedures":[], "entrypoint":[]}
        self.parse_procedures()
        self.parse_entrypoint()
        self.decode_args()
        self.set_labels()
        
        
    def parse_procedures(self):
        proc_num = 1
        procedures = []
        while self.state[self.addr] != 0:
            address = self.addr
            code = []
            while self.state[self.addr] != self.map.com2code["RET"]:
                code.append(self.parse_command())
            code.append(self.parse_command())
            name = self.procname_template.format(proc_num)
            procedures.append({
                "address": address,
                "code": code,
                "name": name,
            })
            self.ad2name[address] = name
            proc_num += 1
        self.addr += self.word_size
        self.sections[".code"]["procedures"] = procedures
        
        
    def parse_entrypoint(self):
        self.sections[".code"]["startaddress"] = self.addr
        code = []
        while self.state[self.addr] != self.map.com2code["END"]:
            code.append(self.parse_command())
        self.sections[".code"]["entrypoint"] = code
        
    
    def parse_command(self):
        command = self.map.code2com[self.state[self.addr] & 0b00111111]
        as_address = [
            bool(self.state[self.addr] & 0b10000000),
            bool(self.state[self.addr] & 0b01000000)
        ]
        self.addr += 1
        args = [
            self.read_value_from_state("INT"),
            self.read_value_from_state("INT"),
        ]
        as_address = as_address[:len(self.map.expected_arg_type[command])]
        args = args[:len(self.map.expected_arg_type[command])]
        return {"args": args, "command": command, "as_address": as_address}
    
    
    def decode_args(self):
        self.label_num = 1
        for procedure in self.sections[".code"]["procedures"]:
            self.decode_listing(procedure["code"])
        self.decode_listing(self.sections[".code"]["entrypoint"])
    
    
    def decode_listing(self, listing):
        for cml in listing:
            for i in range(len(cml["args"])):
                self.decode_arg(cml, i)
            del cml["as_address"]
    
    
    def decode_arg(self, cml, i):
        atype = self.map.expected_arg_type[cml["command"]][i]
        if atype == "value":
            if cml["as_address"][i] and cml["args"][i] in self.ad2name:
                cml["args"][i] = self.ad2name[cml["args"][i]]
            else:
                cml["args"][i] = str(cml["args"][i])                                
        elif atype == "address":
            if cml["args"][i] in self.ad2name:
                cml["args"][i] = self.ad2name[cml["args"][i]]
            else:
                cml["args"][i] = str(cml["args"][i])
        elif atype == "label":
            if cml["args"][i] in self.ad2name:
                cml["args"][i] = self.ad2name[cml["args"][i]]
            else:
                name = self.label_template.format(self.label_num)
                self.label_num += 1
                self.ad2name[cml["args"][i]] = name
                cml["args"][i] = name
        if cml["as_address"][i]:
            cml["args"][i] = "[" + cml["args"][i] + "]"
            
    
    def set_labels(self):
        self.set_labels_in_listing(self.sections[".code"]["entrypoint"],
                                   self.sections[".code"]["startaddress"])
        for procedure in self.sections[".code"]["procedures"]:
            self.set_labels_in_listing(procedure["code"], procedure["address"])
        
                
    def set_labels_in_listing(self, listing, addr):
        for cml in listing:
            if addr in self.ad2name and self.ad2name[addr].startswith("L"):
                cml["label"] = self.ad2name[addr]
            else:
                cml["label"] = None
            addr += self.word_size
            
            
    def write_to_file(self, filename):
        with open(filename, "w") as f:
            self.write_data_to_file(f)
            self.write_code_to_file(f)
            
            
    def write_data_to_file(self, f):
        f.write(".data\n\n")
        for record in self.sections[".data"]:
            f.write("\t" + record["name"] + " ")
            f.write(record["type"] + " ")
            if record["type"] == "BYTE":
                f.write("'" + record["values"] + "'")
            else:
                f.write(", ".join(record["values"]))
            f.write("\n")
            
            
    def write_code_to_file(self, f):
        f.write("\n\n.code\n\n")
        for procedure in self.sections[".code"]["procedures"]:
            f.write(procedure["name"] + " proc\n")
            self.write_listing_to_file(f, procedure["code"])
            f.write(procedure["name"] + " endp\n\n")
        f.write("program:\n")
        self.write_listing_to_file(f, self.sections[".code"]["entrypoint"])
        f.write("END program\n")
        
    
    def write_listing_to_file(self, f, listing):
        for cml in listing:
            if cml["label"] is not None:
                f.write(cml["label"] + ":")
            f.write("\t")
            f.write(cml["command"] + " ")
            f.write(", ".join(cml["args"]))
            f.write("\n")

