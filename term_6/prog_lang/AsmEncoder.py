import numpy as np
import re

from AsmMapping import AsmMapping

class AsmEncoder:
    """Генерация дерева разбора"""
    
    def __init__( self ):
        pass


    def parse(self, input_file):
        self.sections = {".data":[], ".code":[]}
        section = None
        with open(input_file, "r") as f:
            for line in f:
                line = line.strip()
                if line:
                    if line in self.sections:
                        section = line
                    elif section is not None:
                        self.sections[section].append(line)
        self.parse_data_section()
        self.parse_code_section()
        self.parse_procedures()
        self.parse_code()
        return self.sections
    
    
    def parse_data_section(self):
        data = []
        for line in self.sections[".data"]:
            name, tp, values = line.split(maxsplit=2)
            values = self.parse_values(values)
            data.append({
                "name": name,
                "type": tp,
                "values": values,
            })
        self.sections[".data"] = data
        
    
    def parse_code_section(self):
        code = {"procedures":[], "entrypoint":[]}
        section = "procedures"
        for line in self.sections[".code"]:
            line = line.strip()
            if line == "program:":
                section = "entrypoint"
            elif line == "END program":
                code[section].append("END")
                break
            else:
                code[section].append(line)
        self.sections[".code"] = code
        
        
    def parse_procedures(self):
        procedures = []
        procedure = {"name": "", "code": []}
        for line in self.sections[".code"]["procedures"]:
            tokens = line.strip().split()
            if len(tokens) == 2:
                if tokens[1] == "proc":
                    procedure["name"] = tokens[0]
                    continue
                elif tokens == [procedure["name"], "endp"]:
                    procedures.append(procedure)
                    procedure = {"name": "", "code": []}
            if procedure["name"]:
                procedure["code"].append(line)
        self.sections[".code"]["procedures"] = procedures
        
        
    def parse_code(self):
        parsed = self.parse_listing(self.sections[".code"]["entrypoint"])
        self.sections[".code"]["entrypoint"] = parsed
        for proc in self.sections[".code"]["procedures"]:
            parsed = self.parse_listing(proc["code"])
            proc["code"] = parsed
            
            
    def parse_args(self, args):
        return [a.strip() for a in args.split(",")]
    
    
    def parse_values(self, values):
        return [v.strip("'") for v in re.findall(r"-?\d+|'.*?'", values)]
    
    
    def parse_command(self, line):
        s = r"((?P<label>\w+):)?(\s*(?P<command>\w+))?(\s+(?P<args>.+))?"
        pattern = re.compile(s)
        parsed = re.match(pattern, line).groupdict()
        if parsed["args"]:
            parsed["args"] = self.parse_args(parsed["args"])
        else:
            parsed["args"] = []
        return parsed

    
    def parse_listing(self, listing):
        parsed = []
        for line in listing:
            parsed.append(self.parse_command(line))
        return parsed


