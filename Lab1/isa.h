/***************************************************************/
/*                                                             */
/*   RISC-V RV32 Instruction Level Simulator                   */
/*                                                             */
/*   ECEN 4243                                                 */
/*   Oklahoma State University                                 */
/*                                                             */
/***************************************************************/

#ifndef _SIM_ISA_H_
#define _SIM_ISA_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

//
// MACRO: Check sign bit (sb) of (v) to see if negative
//   if no, just give number
//   if yes, sign extend (e.g., 0x80_0000 -> 0xFF80_0000)
//
// TODO: Fix sign extension
#define SIGNEXT(v, sb) ((v) | (((v) & (1 << (sb))) ? ~((1 << (sb))-1) : 0)); // Sign Extension

int ADD (int Rd, int Rs1, int Rs2) {

  int cur = 0;
  cur = CURRENT_STATE.REGS[Rs1] + CURRENT_STATE.REGS[Rs2];
  NEXT_STATE.REGS[Rd] = cur;
  return 0;

}

int ADDI (int Rd, int Rs1, int Imm) { //Add Immediate

  int cur = 0;
  cur = CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm,12);
  NEXT_STATE.REGS[Rd] = cur;
  return 0;

}

int BNE (int Rs1, int Rs2, int Imm) { //Branch if not equal

  int cur = 0;
  Imm = Imm << 1;
  if (CURRENT_STATE.REGS[Rs1] != CURRENT_STATE.REGS[Rs2])
    NEXT_STATE.PC = (CURRENT_STATE.PC - 4) + (SIGNEXT(Imm,13));
  return 0;

}

// I Instructions
int LB (int Rd, int Rs1, int Imm){ // Load Byte
  NEXT_STATE.REGS[Rd] = CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm,8);
  return 0;
};

int LH (int Rd, int Rs1, int Imm){ // load half
  NEXT_STATE.REGS[Rd] = CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm,16);
  return 0;
};

int LW (int Rd, int Rs1, int Imm){ // load word
  NEXT_STATE.REGS[Rd] = CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm,32);
  return 0;
};

int LBU (int Rd, int Rs1, int Imm){ // load byte unsigned
  NEXT_STATE.REGS[Rd] = (unsigned int)CURRENT_STATE.REGS[Rs1] + (unsigned int)SIGNEXT(Imm,8);
  return 0;
};

int LHU (int Rd, int Rs1, int Imm){ // load halfword unigned
  NEXT_STATE.REGS[Rd] = (unsigned int)CURRENT_STATE.REGS[Rs1] + (unsigned int)SIGNEXT(Imm,16);
  return 0;
};

int SLLI (int Rd, int Rs1, int uImm){ // shift left logical immediate
  int cur = 0;
  cur = CURRENT_STATE.REGS[Rs1] << CURRENT_STATE.REGS[uImm];
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
};

int SLTI (int Rd, int Rs1, int Imm){ // set less than immediate
  int cur = 0;
  cur = CURRENT_STATE.REGS[Rs1] < CURRENT_STATE.REGS[Imm];
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
};

int SLTIU (int Rd, int Rs1, int Imm){
  int cur = 0;
  cur = CURRENT_STATE.REGS[Rs1] < CURRENT_STATE.REGS[Imm];
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
};

int XORI (int Rd, int Rs1, int Imm){
  int cur = 0;
  cur = (unsigned int)CURRENT_STATE.REGS[Rs1] ^ (unsigned int)CURRENT_STATE.REGS[Imm];
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
};

int SRLI (int Rd, int Rs1, int uImm){
  int cur = 0;
  cur = (unsigned int)CURRENT_STATE.REGS[Rs1] >> (unsigned int)CURRENT_STATE.REGS[uImm];
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
};

int SRAI (int Rd, int Rs1, int uImm){
  int cur = 0;
  cur = CURRENT_STATE.REGS[Rs1] >> CURRENT_STATE.REGS[uImm];
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
};

int ORI (int Rd, int Rs1, int Imm){
  int cur = 0;
  cur = CURRENT_STATE.REGS[Rs1] | CURRENT_STATE.REGS[Imm];
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
};

int ANDI (int Rd, int Rs1, int Imm){
  int cur = 0;
  cur = CURRENT_STATE.REGS[Rs1] & CURRENT_STATE.REGS[Imm];
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
};

// U Instruction
int AUIPC (int Rd, int upImm){ // add upper immediate to PC
  NEXT_STATE.REGS[Rd] = CURRENT_STATE.REGS[upImm] + CURRENT_STATE.PC;
};

int LUI (int Rd, int upImm){ // load upper immediate
  NEXT_STATE.REGS[Rd] = CURRENT_STATE.REGS[upImm];
};

// S Instruction
int SB (int Rs1, int Rs2, int Imm){ // store byte
  EffAddr = CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm, 12);
  int mask = 0x3;
  int offset = EffAddr & mask;
  int AlignedAddr = EffAddr & ~mask;
  int WordReadData = mem_read_32(AlignedAddr);
  int WriteData = CURRENT_STATE.REGS[Rs2] & 0xFF;
  WriteData = WriteData << 8 * offset;
  int ShiftWordReadData = WordReadData << (8 * offset);
  WordReadData = ShiftWordReadData & (0xFF);
  WordReadData = WordReadData >> (8 * offset);
  int MergedWriteData = WriteData | WordReadData;
  mem_write_32(EffAddr, MergedWriteData);
  
  return 0;
};

int SH (int Rs1, int Rs2, int Imm){ // store half
  EffAddr = CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm, 12);
  int mask = 0x5;
  int offset = EffAddr & mask;
  int AlignedAddr = EffAddr & ~mask;
  int WordReadData = mem_read_32(AlignedAddr);
  int WriteData = CURRENT_STATE.REGS[Rs2] & 0xFFFF;
  WriteData = WriteData << 8 * offset;
  int ShiftWordReadData = WordReadData << (8 * offset);
  WordReadData = ShiftWordReadData & (0xFFFF);
  WordReadData = WordReadData >> (8 * offset);
  int MergedWriteData = WriteData | WordReadData;
  mem_write_32(EffAddr, MergedWriteData);
  return 0;
};

int SW (int Rs2, int Imm_Rs1){ // store word
  EffAddr = CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm, 12);
  int mask = 0x9;
  int offset = EffAddr & mask;
  int AlignedAddr = EffAddr & ~mask;
  int WordReadData = mem_read_32(AlignedAddr);
  int WriteData = CURRENT_STATE.REGS[Rs2] & 0xFFFFFFFF;
  WriteData = WriteData << 8 * offset;
  int ShiftWordReadData = WordReadData << (8 * offset);
  WordReadData = ShiftWordReadData & (0xFFFFFFFF);
  WordReadData = WordReadData >> (8 * offset);
  int MergedWriteData = WriteData | WordReadData;
  mem_write_32(EffAddr, MergedWriteData);
  return 0;
};

// R instruction
int SUB (int Rd, int Rs1, int Rs2){
  int cur = 0;
  cur = CURRENT_STATE.REGS[Rs1] - CURRENT_STATE.REGS[Rs2];
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
};
int SLL (int Rd, int Rs1, int Rs2){ // Shift left logical
  int cur = 0;
  cur = CURRENT_STATE.REGS[Rs1] << CURRENT_STATE.REGS[Rs2];
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
};

int SLT (int Rd, int Rs1, int Rs2){ // set less than
  int cur = 0;
  cur = CURRENT_STATE.REGS[Rs1] < CURRENT_STATE.REGS[Rs2];
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
};

int SLTU (int Rd, int Rs1, int Rs2){ // set less than
  int cur = 0;
  cur = abs(CURRENT_STATE.REGS[Rs1]) < abs(CURRENT_STATE.REGS[Rs2]);
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
};

int XOR (int Rd, int Rs1, int Rs2){ // XOR gate
  int cur = 0;
  cur = (unsigned int)CURRENT_STATE.REGS[Rs1] ^ (unsigned int)CURRENT_STATE.REGS[Rs2];
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
};

int SRL (int Rd, int Rs1, int Rs2){ // right shift logical
  int cur = 0;
  cur = (unsigned int)CURRENT_STATE.REGS[Rs1] >> (unsigned int)CURRENT_STATE.REGS[Rs2];
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
};

int SRA (int Rd, int Rs1, int Rs2){ // right shift arithmetic
  int cur = 0;
  cur = CURRENT_STATE.REGS[Rs1] >> CURRENT_STATE.REGS[Rs2];
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
};

int OR (int Rd, int Rs1, int Rs2){ // OR gate
  int cur = 0;
  cur = CURRENT_STATE.REGS[Rs1] | CURRENT_STATE.REGS[Rs2];
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
};
int AND (int Rd, int Rs1, int Rs2){ // AND gate
  int cur = 0;
  cur = CURRENT_STATE.REGS[Rs1] & CURRENT_STATE.REGS[Rs2];
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
};

// B instructions
int BEQ (int Rs1, int Rs2, int Imm){ // branch if =

  int cur = 0;
  Imm = Imm << 1;
  if (CURRENT_STATE.REGS[Rs1] == CURRENT_STATE.REGS[Rs2])
    NEXT_STATE.PC = (CURRENT_STATE.PC - 4) + (SIGNEXT(Imm,13));
  return 0;

};
int BLT (int Rs1, int Rs2, int Imm){ // branch if <

  int cur = 0;
  Imm = Imm << 1;
  if (CURRENT_STATE.REGS[Rs1] < CURRENT_STATE.REGS[Rs2])
    NEXT_STATE.PC = (CURRENT_STATE.PC - 4) + (SIGNEXT(Imm,13));
  return 0;

};
int BGE (int Rs1, int Rs2, int Imm){ // branch if >=

  int cur = 0;
  Imm = Imm << 1;
  if (CURRENT_STATE.REGS[Rs1] >= CURRENT_STATE.REGS[Rs2])
    NEXT_STATE.PC = (CURRENT_STATE.PC - 4) + (SIGNEXT(Imm,13));
  return 0;

};
int BLTU (int Rs1, int Rs2, int Imm){ // branch if < unsigned;

  int cur = 0;
  Imm = Imm << 1;
  if ((unsigned int)CURRENT_STATE.REGS[Rs1] < (unsigned int)CURRENT_STATE.REGS[Rs2])
    NEXT_STATE.PC = (CURRENT_STATE.PC - 4) + (SIGNEXT(Imm,13));
  return 0;
};
int BGEU (int Rs1, int Rs2, int Imm){ // branch if >= unsigned;

  int cur = 0;
  Imm = Imm << 1;
  if ((unsigned int)CURRENT_STATE.REGS[Rs1] >= (unsigned int)CURRENT_STATE.REGS[Rs2])
    NEXT_STATE.PC = (CURRENT_STATE.PC - 4) + (SIGNEXT(Imm,13));
  return 0;
}

// I instruction
int JALR (int Rd, int Rs1, int Imm){ // jump and link register
  NEXT_STATE.PC = CURRENT_STATE.REGS[Rs1] + (SIGNEXT(Imm,12));
  NEXT_STATE.REGS[Rd] = CURRENT_STATE.PC + 4;
  return 0;
};

// J instruction
int JAL (int Rd, int Imm){ // jump and link
  NEXT_STATE.PC = CURRENT_STATE.REGS[Imm];
  NEXT_STATE.REGS[Rd] = CURRENT_STATE.PC + 4;
  return 0;
};

int ECALL (int Rd, int Rs1, int Imm){
  RUN_BIT = CURRENT_STATE.REGS[Imm];
  return 0;
}

#endif
