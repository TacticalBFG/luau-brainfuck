#include <vector>
#include <string>
#include <sstream>
#include <iostream>

extern "C" {
#include "Lua/lobject.h"
#include "Lua/lopcode.h"
#include "Lua/lfunc.h"
}

std::string brainfuckTranslator(const char *s);

struct jumpInfo {
	int from;
	int to;
};

// uh credits to jayden for these structs
// if u want this to actually work figure out the luau structs urself lol
uintptr_t getLuaUOpCode(Instruction inst)
{
    return (inst >> 8);
}

struct luaRegisters
{
  OpCode op;
  uint8_t a;
  uint8t b;
  uint8_t c;
};

struct luaRegistersBx
{
  OpCode op;
  uint8_t a;
  uint16_t bx
};

Instruction generateABC(OpCode op, int a, int b, int c)
{
    luaRegisters reg;
    reg.a = a;
    reg.b = b;
    reg.c = c;
    return reinterpret_cast<Instruction>(&reg);
    
}

Instruction generateABx(OpCode op, int a, int bx)
{
    luaRegistersBx reg;
    reg.a = a;
    reg.bx= b;
    return reinterpret_cast<Instruction>(&reg);
}
