#include "z80.h"
#include "z80_opcodes.h"

#include <sstream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>

namespace usr{

const char* regsTable[] = {"b", "c", "d", "e", "h", "l", "(hl)", "a"};
const char* pairRegsTable[] = {"bc","de","hl","sp"};
const char* pairRegsTable2[] = {"bc","de","hl","af"};
const char* conditionsTable[] = {"nz", "z", "nc", "c", "po", "pe", "p", "m",};
const char* aluOpcodesTable[] = {"add a", "adc a", "sub", "sbc a", "and", "xor", "or", "cp"};
const char* rotationTable[] = {"rlc ", "rrc ", "rl  ", "rr  ", "sla ", "sra ", "sll ", "slr "};


Z80_Disassembler::Z80_Disassembler(BeyeContext& bc,const Bin_Format& b,binary_stream& h,DisMode& _parent )
		:Disassembler(bc,b,h,_parent)
		,bctx(bc)
		,parent(_parent)
		,main_handle(h)
		,bin_format(b)
{
    outstr = new char[1024];
}


Z80_Disassembler::~Z80_Disassembler()
{
    delete[] outstr;
}


static DisasmRet Prefix_CB_Disassembler(int code, int prevPrefix, unsigned char* buffer)
{
    DisasmRet ret;
    std::ostringstream os;
    int type = (code & 0xC0) >> 6;
    int operand1 = (code & 0x38) >> 3;
    int operand2 = (code & 0x07);
    
    std::ostringstream subos;
    if(prevPrefix == 0) subos << regsTable[operand2];
    else if(prevPrefix == 0xDD) subos << "(ix+" << std::setfill ('0') << std::setw(2) << std::hex << (int)buffer[3] << "h)";
    else if (prevPrefix == 0xFD) subos << "(iy+" << std::setfill ('0') << std::setw(2) << std::hex << (int)buffer[3] << "h)";

    if(type == 0) {
        os << rotationTable[operand1] << subos.str();
	if(prevPrefix != 0 && operand2 != 6) os << ", " << regsTable[operand2];
    } else if(type == 1) {
        os << "bit  " << operand1 << ", " << subos.str();
    } else if(type == 2) {
        os << "res  " << operand1 << ", " << subos.str();
	if(prevPrefix != 0 && operand2 != 6) os << ", " << regsTable[operand2];
    } else {
        os << "set  " << operand1 << ", " << subos.str();
	if(prevPrefix != 0 && operand2 != 6) os << ", " << regsTable[operand2];
    }
    
    ret.str = os.str();
    ret.codelen = 2;
    if(prevPrefix != 0) ret.codelen += 2;

    return ret;
}

static DisasmRet Prefix_DD_FD_Disassembler(int code, int prefix, MBuffer buffer)
{
    DisasmRet ret;
    size_t position = 0;
    const z80_opcode_t *opcode = &z80_opcodes[code];
    std::string regName;
    bool illegal = true;
    
    if(code == 0xEB) { //No command
        ret.str = "NONI";
        ret.codelen = 2;
        return ret;
    }
    if(prefix == 0xDD) regName = "ix";
    else if(prefix == 0xFD) regName = "iy";
    
    std::string str = std::string(opcode->name);
    std::ostringstream localos;

    for(int i = 0 ; i < 2; ++i) {
        localos.clear();
        if((position = str.find("(hl)")) != std::string::npos) {
            if(code != 0xE9)
                localos << "(" << regName << "+" << std::setfill ('0') << std::setw(2)  << std::hex  << (int)buffer[2] << "h)";
            else
                localos << "(" << regName << ")";
        str.replace(position, 4, "");
        str.insert(position, localos.str());
	ret.codelen += 1;
        illegal = false;
        } else if((position = str.find(" hl")) != std::string::npos) {
            str.replace(position, 3, " ");
            str.insert(position + 1, regName);
            illegal = false;
        } else if((position = str.find(" h"))  != std::string::npos) {
            str.replace(position, 2, " ");        
            str.insert(position + 1, regName + "h");
            illegal = false;
        } else if((position = str.find(" l"))  != std::string::npos) {
            str.replace(position, 2, " ");
            str.insert(position + 1, regName + "l");
            illegal = false;
        }
        if(code >= 0x70 && code < 0x80) break;
    }

    if((position = str.find("%02Xh")) != std::string::npos) {
        localos.clear();
        localos << std::setfill ('0') << std::setw(2)  << std::hex  << (int)buffer[2] << "h";
        str.replace(position, 5, "");
        str.insert(position, localos.str());
    }
    if((position = str.find("%04Xh")) != std::string::npos) {
        localos.clear();
        localos << std::setfill ('0') << std::setw(4)  << std::hex  << (int)(buffer[2] | (buffer[3] << 8)) << "h";
        str.replace(position, 5, "");
        str.insert(position, localos.str());
    }

    if(illegal) str = "";
    ret.codelen += opcode->length + 1;
    ret.str = str;

    return ret;
}

DisasmRet Z80_Disassembler::disassembler(__filesize_t ulShift,
					MBuffer buffer,
					unsigned flags)
{
    DisasmRet ret;
    int outpos = 0;
    int arg = 0;
    const z80_opcode_t *opcode = NULL;
    int prefix = buffer[0];
    int code = 0;

    if(   prefix == 0xCB
       || prefix == 0xDD
       || prefix == 0xED
       || prefix == 0xFD) {
        code = buffer[1];
    } else {
        code = prefix;
        prefix = 0;
    }

    if(prefix == 0xCB) {
        return Prefix_CB_Disassembler(code, 0, buffer);
    } else if(prefix == 0xED) {
       if(code >= 0x40 && code < 0xA0) opcode = &z80_ED_opcodes[code - 0x40];
       else if(code >= 0xA0 && code < 0xC0) opcode = &z80_ED_opcodes[code - 0x60];
    } else if(prefix == 0xDD || prefix == 0xFD) {
       if(code == 0xCB) {
           code = buffer[2];
           return Prefix_CB_Disassembler(code, prefix, buffer);
       }
       return Prefix_DD_FD_Disassembler(code, prefix, buffer);
    } else {
        opcode = &z80_opcodes[code];
    }    

    if(opcode != NULL && opcode->length > 1) {
	int prefixOffset = 0;
	if(prefix > 0) prefixOffset = 1;
        if(opcode->length == 2) arg = buffer[1 + prefixOffset];
        else arg = buffer[1 + prefixOffset] | (buffer[2 + prefixOffset] << 8);
    }

    if(opcode == NULL) {
        ret.str = sprintf(outstr + outpos, "???  ???, ???      ; nor realize");
        ret.codelen = 1;
        return ret;
    } else {
        if(opcode->length > 1) outpos += sprintf(outstr + outpos, opcode->name, arg);
        else outpos += sprintf(outstr + outpos, "%s", opcode->name);
    }
    
    ret.str = outstr;
    ret.codelen = opcode->length;

    return ret;
}

void Z80_Disassembler::show_short_help() const
{
}

bool Z80_Disassembler::action_F1()
{
    return false;
}

int Z80_Disassembler::max_insn_len() const { return 13; }

ColorAttr Z80_Disassembler::get_insn_color( unsigned long clone )
{
  UNUSED(clone);
  return disasm_cset.cpu_cset[0].clone[0];
}

Bin_Format::bitness Z80_Disassembler::get_bitness() const { return Bin_Format::Use16; }

char Z80_Disassembler::clone_short_name( unsigned long clone )
{
  UNUSED(clone);
  return ' ';
}

void Z80_Disassembler::read_ini( Ini_Profile& ini ) { UNUSED(ini); }
void Z80_Disassembler::save_ini( Ini_Profile& ini ) { UNUSED(ini); }

const char* Z80_Disassembler::prompt(unsigned idx) const {
    switch(idx) {
	case 0: return "Z80Hlp"; break;
	default: break;
    }
    return "";
}

static Disassembler* query_interface(BeyeContext& bc,const Bin_Format& b,binary_stream& h,DisMode& _parent) { return new(zeromem) Z80_Disassembler(bc,b,h,_parent); }
extern const Disassembler_Info z80_disassembler_info = {
    DISASM_CPU_Z80,
    "Zilog ~Z80",	/**< plugin name */
    query_interface
};
}