/***************************************************************/
/*                                                             */
/*   RISC-V RV32 Instruction Level Simulator                   */
/*                                                             */
/*   ECEN 4243                                                 */
/*   Oklahoma State University                                 */
/*                                                             */
/***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include "isa.h"

char *byte_to_binary(int x) {

  static char b[9];
  b[0] = '\0';

  int z;
  for (z = 128; z > 0; z >>= 1) {
    strcat(b, ((x & z) == z) ? "1" : "0");
  }

  return b;
}

char *byte_to_binary32(int x) {

  static char b[33];
  b[0] = '\0';

  unsigned int z;
  for (z = 2147483648; z > 0; z >>= 1) {
    strcat(b, ((x & z) == z) ? "1" : "0");
  }

  return b;
}

int bchar_to_int(char* rsa) {

  int i = 0;
  int result = 0;
  int t = 0;
  while(rsa[i] != '\0')i++;
  while(i>0)
    {
      --i;
      // printf("%d\n", (rsa[i]-'0')<<t);
      result += (rsa[i] - '0')<<t;
      t++;
    }
  return result;
}

int r_process(char* i_) { // Done

  char d_opcode[8]; // Defining the size of opcode as 8 bits, 1 bit is empty
  d_opcode[0] = i_[31-6]; 
  d_opcode[1] = i_[31-5]; 
  d_opcode[2] = i_[31-4]; 
  d_opcode[3] = i_[31-3];
  d_opcode[4] = i_[31-2]; 
  d_opcode[5] = i_[31-1]; 
  d_opcode[6] = i_[31-0];
  d_opcode[7] = '\0';
  char rd[6]; rd[5] = '\0';
  char funct3[4]; funct3[3] = '\0';
  char rs1[6]; rs1[5] = '\0';	// Creating registers and assigning last bit 0	   
  char rs2[6]; rs2[5] = '\0';
  char funct7[8]; funct7[7] = '\0';
  
  for(int i = 0; i < 5; i++) { // Assigning registers their values
    rs1[i] = i_[31-19+i];
    rs2[i] = i_[31-24+i];            
    rd[i] = i_[31-11+i];
  }
  for(int i = 0; i < 3; i++) {
    funct3[i] = i_[31-14+i];
  }
  for(int i = 0; i < 7; i++) {
    funct7[i] = i_[31-31+i];
  }
  int Rs1 = bchar_to_int(rs1);
  int Rs2 = bchar_to_int(rs2);		   
  int Rd = bchar_to_int(rd);
  int Funct3 = bchar_to_int(funct3);
  int Funct7 = bchar_to_int(funct7);
  printf ("Opcode = %s\n Rs1 = %d\n Rs2 = %d\n Rd = %d\n Funct3 = %d\n\n",
	  d_opcode, Rs1, Rs2, Rd, Funct3);
  printf("\n");

  /* Example - use and replicate */
  if(!strcmp(d_opcode,"0110011") && Funct3 == 0 && Funct7 == 0) {
    printf("--- This is an ADD instruction. \n");
    ADD(Rd, Rs1, Rs2);
    return 0;
  }
  if(!strcmp(d_opcode,"0110011") && Funct3 == 0 && Funct7 == 32) {
    printf("--- This is an SUB instruction. \n");
    SUB(Rd, Rs1, Rs2);
    return 0;
  }
  if(!strcmp(d_opcode,"0110011") && Funct3 == 1) {
    printf("--- This is an SLL instruction. \n");
    SLL(Rd, Rs1, Rs2);
    return 0;
  }
  if(!strcmp(d_opcode,"0110011") && Funct3 == 2) {
    printf("--- This is an SLT instruction. \n");
    SLT(Rd, Rs1, Rs2);
    return 0;
  }
  if(!strcmp(d_opcode,"0110011") && Funct3 == 3) {
    printf("--- This is an SLTU instruction. \n");
    SLTU(Rd, Rs1, Rs2);
    return 0;
  }
  if(!strcmp(d_opcode,"0110011") && Funct3 == 4) {
    printf("--- This is an XOR instruction. \n");
    XOR(Rd, Rs1, Rs2);
    return 0;
  }
  if(!strcmp(d_opcode,"0110011") && Funct3 == 5 && Funct7 == 0) {
    printf("--- This is an SRL instruction. \n");
    SRL(Rd, Rs1, Rs2);
    return 0;
  }
  if(!strcmp(d_opcode,"0110011") && Funct3 == 5 && Funct7 == 32) {
    printf("--- This is an SRA instruction. \n");
    SRA(Rd, Rs1, Rs2);
    return 0;
  }
  if(!strcmp(d_opcode,"0110011")) {
    printf("--- This is an OR instruction. \n");
    OR(Rd, Rs1, Rs2);
    return 0;
  }
  if(!strcmp(d_opcode,"0110011")) {
    printf("--- This is an AND instruction. \n");
    AND(Rd, Rs1, Rs2, Funct3);
    return 0;
  }

  return 1;	
}

int i_process(char* i_) { // Done
  // Reading in the opcode
  char d_opcode[8];
  d_opcode[0] = i_[31-6]; 
  d_opcode[1] = i_[31-5]; 
  d_opcode[2] = i_[31-4]; 
  d_opcode[3] = i_[31-3];
  d_opcode[4] = i_[31-2]; 
  d_opcode[5] = i_[31-1]; 
  d_opcode[6] = i_[31-0]; 
  d_opcode[7] = '\0';

  char rd[6]; rd[5] = '\0';
  char funct3[4]; funct3[3] = '\0';
  char rs1[6]; rs1[5] = '\0';		   
  char imm[13]; imm[12] = '\0';
  char funct7[7]; funct7[6] = '\0';

  for(int i = 0; i < 5; i++) {
    rs1[i] = i_[31-19+i];
    rd[i] = i_[31-11+i];
  }
  for(int i = 0; i < 12; i++) {
    imm[i] = i_[31-31+i];
  }
  for(int i = 0; i < 3; i++) {
    funct3[i] = i_[31-14+i];
  }
  for(int i = 0; i < 6; i++) {
    funct7[i] = i_[31-31+i];
  }
  int Rs1 = bchar_to_int(rs1);
  int Rd = bchar_to_int(rd);
  int Funct3 = bchar_to_int(funct3);
  int Imm = bchar_to_int(imm);
  int Funct7 = bchar_to_int(funct7);
  printf ("Opcode = %s\n Rs1 = %d\n Imm = %d\n Rd = %d\n Funct3 = %d\n\n",
	  d_opcode, Rs1, Imm, Rd, Funct3);
  printf("\n");

  if(!strcmp(d_opcode,"0010011") && Funct3 == 0) {
    printf("--- This is an ADDI instruction. \n");
    ADDI(Rd, Rs1, Imm);
    return 0;
  }  
  if(!strcmp(d_opcode,"0010011") && Funct3 == 1) {
    printf("--- This is an SLLI instruction. \n");
    SLLI(Rd, Rs1, Imm);
    return 0;
  } 
  if(!strcmp(d_opcode,"0010011") && Funct3 == 2) {
    printf("--- This is an SLTI instruction. \n");
    SLTI(Rd, Rs1, Imm);
    return 0;
  } 
  if(!strcmp(d_opcode,"0010011") && Funct3 == 3) {
    printf("--- This is an SLTIU instruction. \n");
    SLTIU(Rd, Rs1, Imm);
    return 0;
  } 
  if(!strcmp(d_opcode,"0010011") && Funct3 == 4) {
    printf("--- This is an XORI instruction. \n");
    XORI(Rd, Rs1, Imm);
    return 0;
  } 
  if(!strcmp(d_opcode,"0010011") && Funct3 == 5 && Funct7 == 0) {
    printf("--- This is an SRLI instruction. \n");
    SRLI(Rd, Rs1, Imm);
    return 0;
  } 
  if(!strcmp(d_opcode,"0010011") && Funct3 == 5 && Funct7 == 16) {
    printf("--- This is an SRAI instruction. \n");
    SRAI(Rd, Rs1, Imm);
    return 0;
  } 
  if(!strcmp(d_opcode,"0010011") && Funct3 == 6) {
    printf("--- This is an ORI instruction. \n");
    ORI(Rd, Rs1, Imm);
    return 0;
  } 
  if(!strcmp(d_opcode,"0010011") && Funct3 == 7) {
    printf("--- This is an ANDI instruction. \n");
    ANDI(Rd, Rs1, Imm);
    return 0;
  } 
  if(!strcmp(d_opcode, "1110011") && Funct3 == 0) {
    printf("--- This is an ECALL instruction. \n");
    ECALL(Rd, Rs1, Imm);
    return 0;
  }
  if(!strcmp(d_opcode, "1100111")){
    printf("--- This is a JALR instruction. \n");
    JALR(Rd, Rs1, Imm);
    return 0;
  }
  return 1;	
}

int b_process(char* i_) { // done
  
  /* This function execute branch instruction */

  char d_opcode[8];
  d_opcode[0] = i_[31-6]; 
  d_opcode[1] = i_[31-5]; 
  d_opcode[2] = i_[31-4]; 
  d_opcode[3] = i_[31-3];
  d_opcode[4] = i_[31-2]; 
  d_opcode[5] = i_[31-1]; 
  d_opcode[6] = i_[31-0]; 
  d_opcode[7] = '\0';
  char rs1[6]; rs1[5] = '\0';
  char rs2[6]; rs2[5] = '\0';		     
  char funct3[4]; funct3[3] = '\0';
  char imm[13]; 
  for(int i = 0; i < 5; i++) {
    rs1[i] = i_[31-19+i];
    rs2[i] = i_[31-24+i];                
  }
  // Old-fashioned method but works :)
  imm[0] = i_[31-31]; // 12, 31
  imm[1] = i_[31-7];  // 11, 7
  imm[2] = i_[31-30]; // 10, 30
  imm[3] = i_[31-29]; // 9, 29
  imm[4] = i_[31-28]; // 8, 28
  imm[5] = i_[31-27]; // 7, 27
  imm[6] = i_[31-26]; // 6, 26
  imm[7] = i_[31-25]; // 5, 25
  imm[8] = i_[31-11]; // 4, 11
  imm[9] = i_[31-10]; // 3, 10
  imm[10] = i_[31-9]; // 2, 9
  imm[11] = i_[31-8]; // 1, 8
  imm[12] = '\0';  

  for(int i = 0; i < 3; i++) {
    funct3[i] = i_[31-14+i];
  }
  int Rs1 = bchar_to_int(rs1);
  int Rs2 = bchar_to_int(rs2);  
  int Funct3 = bchar_to_int(funct3);
  int Imm = bchar_to_int(imm);
  printf ("Opcode = %s\n Rs1 = %d\n Rs2 = %d\n Imm = %d\n Funct3 = %d\n\n",
	  d_opcode, Rs1, Rs2, Imm, Funct3);
  printf("\n");    

  if(!strcmp(d_opcode,"1100011" && Funct3 == 0)) {
    printf("--- This is an BEQ instruction. \n");
    BEQ(Rs1, Rs2, Imm);
    return 0;
  }	  
  if(!strcmp(d_opcode,"1100011") && Funct3 == 1) {
    printf("--- This is an BNE instruction. \n");
    BNE(Rs1, Rs2, Imm);
    return 0;
  }	  
  if(!strcmp(d_opcode,"1100011") && Funct3 == 4) {
    printf("--- This is an BLT instruction. \n");
    BLT(Rs1, Rs2, Imm);
    return 0;
  }	  
  if(!strcmp(d_opcode,"1100011") && Funct3 == 5) {
    printf("--- This is an BGE instruction. \n");
    BGE(Rs1, Rs2, Imm);
    return 0;
  }	 

  return 1;

}

int s_process(char* i_) {
  char d_opcode[8];
  d_opcode[0] = i_[31-6]; 
  d_opcode[1] = i_[31-5]; 
  d_opcode[2] = i_[31-4]; 
  d_opcode[3] = i_[31-3];
  d_opcode[4] = i_[31-2]; 
  d_opcode[5] = i_[31-1]; 
  d_opcode[6] = i_[31-0]; 
  d_opcode[7] = '\0';

  char imm[13]; imm[12] = '\0';
  char funct3[4]; funct3[3] = '\0';
  char rs1[6]; rs1[5] = '\0';
  char rs2[6]; rs2[5] = '\0';

  for(int i = 0; i < 5; i++){
    imm[i] = i_[31-11+i];
  }
  for(int i = 0; i < 7; i++){
    imm[i+5] = i_[31-31+1];
  }
  for(int i = 0; i < 5; i++){
    rs1[i] = i_[31-19+i];
    rs2[i] = i_[31-24+i];
  }
  for(int i = 0; i < 3; i++){
    funct3[i] = i_[31-14+i];
  }

  int Rs1 = bchar_to_int(rs1);
  int Rs2 = bchar_to_int(rs2);  
  int Funct3 = bchar_to_int(funct3);
  int Imm = bchar_to_int(imm);
  
  // SB, SH, SW
  if(!strcmp(d_opcode,"1100011") && Funct3 == 0) {
    printf("--- This is an SB instruction. \n");
    SB(Rs1, Rs2, Imm);
    return 0;
  }
  if(!strcmp(d_opcode,"0100011") && Funct3 == 1) {
    printf("--- This is an SH instruction. \n");
    SH(Rs1, Rs2, Imm);
    return 0;
  }
  if(!strcmp(d_opcode,"0100011") && Funct3 == 2) {
    printf("--- This is an SW instruction. \n");
    SW(Rs1, Rs2, Imm);
    return 0;
  }
  return 1;

}

int j_process(char* i_) {
  char d_opcode[8];
  d_opcode[0] = i_[31-6]; 
  d_opcode[1] = i_[31-5]; 
  d_opcode[2] = i_[31-4]; 
  d_opcode[3] = i_[31-3];
  d_opcode[4] = i_[31-2]; 
  d_opcode[5] = i_[31-1]; 
  d_opcode[6] = i_[31-0]; 
  d_opcode[7] = '\0';

  char rd[6]; rd[5] = '\0';
  char imm[21]; imm[20] = '\0';
  for(int i = 0; i < 5; i++) {
    rd[i] = i_[31-11+i];
  }
  imm[0] = i_[31-31]; // 20
  imm[1] = i_[31-19]; // 19
  imm[2] = i_[31-18]; // 18
  imm[3] = i_[31-17]; // 17
  imm[4] = i_[31-16]; // 16
  imm[5] = i_[31-15]; // 15
  imm[6] = i_[31-14]; // 14
  imm[7] = i_[31-13]; // 13
  imm[8] = i_[31-12]; // 12
  imm[9] = i_[31-20]; // 11
  imm[10] = i_[31-30]; //10
  imm[11] = i_[31-29]; // 9
  imm[12] = i_[31-28]; // 8
  imm[13] = i_[31-27]; // 7
  imm[14] = i_[31-26]; // 6
  imm[15] = i_[31-25]; // 5
  imm[16] = i_[31-24]; // 4
  imm[17] = i_[31-23]; // 3
  imm[18] = i_[31-22]; // 2
  imm[19] = i_[31-21]; // 1


  int Rd = bchar_to_int(rd);
  int Imm = bchar_to_int(imm);
 
  if(!strcmp(d_opcode, "1101111")){
    printf("--- This is a JAL instruction. \n");
    JAL(Rd, Imm);
    return 0;
  }

  return 1;

}

int u_process(char* i_) {
  char d_opcode[8];
  d_opcode[0] = i_[31-6]; 
  d_opcode[1] = i_[31-5]; 
  d_opcode[2] = i_[31-4]; 
  d_opcode[3] = i_[31-3];
  d_opcode[4] = i_[31-2]; 
  d_opcode[5] = i_[31-1]; 
  d_opcode[6] = i_[31-0]; 
  d_opcode[7] = '\0';

  char rd[6]; rd[5] = '\0';
  char imm[21]; imm[20] = '\0';
  for(int i = 0; i < 5; i++) {
    rd[i] = i_[31-11+i];
  }
  for(int i = 0; i < 20; i++) {
    imm[i] = i_[31-31+i];
  }
  int Rd = bchar_to_int(rd);
  int Imm = bchar_to_int(imm);
  if(!strcmp(d_opcode,"0010111")) {
    printf("--- This is an AUIPC instruction. \n");
    AUIPC(Rd, Rs1, Imm);
    return 0;
  } 
  if(!strcmp(d_opcode,"0110111")) {
    printf("--- This is an LUI instruction. \n");
    LUI(Rd, Rs1, Imm);
    return 0;
  } 
  return 1;

}

int interruption_process(char* i_) {

  ECALL(i_);
  RUN_BIT = 0;
  return 0;

}

int decode_and_execute(char* i_) {

  /* 
     This function decode the instruction and update 
     CPU_State (NEXT_STATE)
  */

  if((i_[25] == '0') && (i_[26] == '0') &&
     (i_[27] == '1') && (i_[28] == '0') &&
     (i_[29] == '0') && (i_[30] == '1') && (i_[31] == '1')) {
    printf("- This is an Immediate Type Instruction. \n");
    i_process(i_);
  }
  if((i_[25] == '0') && (i_[26] == '1') &&
     (i_[27] == '1') && (i_[28] == '0') &&
     (i_[29] == '0') && (i_[30] == '1') && (i_[31] == '1')) {
    printf("- This is an R Type Instruction. \n");
    r_process(i_);
  }    
  if((i_[25] == '1') && (i_[26] == '1') &&
     (i_[27] == '0') && (i_[28] == '0') &&
     (i_[29] == '0') && (i_[30] == '1') && (i_[31] == '1')) {
    printf("- This is a B Type Instruction. \n");
    b_process(i_);
  }
  if((i_[25] == '0') && (i_[26] == '1') &&
     (i_[27] == '0') && (i_[28] == '0') &&
     (i_[29] == '0') && (i_[30] == '1') && (i_[31] == '1')) {
    printf("- This is a S Type Instruction. \n");
    s_process(i_);
  }  
  if((i_[25] == '1') && (i_[26] == '1') &&
     (i_[27] == '0') && (i_[28] == '1') &&
     (i_[29] == '1') && (i_[30] == '1') && (i_[31] == '1')) {
    printf("- This is a J Type Instruction. \n");
    j_process(i_);
  }
  if((i_[25] == '0') && (i_[26] == '0') &&
     (i_[27] == '1') && (i_[28] == '0') &&
     (i_[29] == '1') && (i_[30] == '1') && (i_[31] == '1')) {
    printf("- This is a U Type Instruction. \n");
    u_process(i_);
  }  
  if((i_[25] == '1') && (i_[26] == '1') &&
     (i_[27] == '1') && (i_[28] == '0') &&
     (i_[29] == '0') && (i_[30] == '1') && (i_[31] == '1')) {
    printf("- This is a Software Interruption Instruction. \n");
    interruption_process(i_);
  }

  return 0;

}

unsigned int OPCODE (unsigned int i_word) {

  return ((i_word<<27)>>27);

}

void process_instruction() {

  /* 
     execute one instruction here. You should use CURRENT_STATE and modify
     values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     access memory. 
  */   

  unsigned int inst_word = mem_read_32(CURRENT_STATE.PC);
  printf("The instruction is: %x \n", inst_word);
  printf("33222222222211111111110000000000\n");
  printf("10987654321098765432109876543210\n");
  printf("--------------------------------\n");
  printf("%s \n", byte_to_binary32(inst_word));
  printf("\n");
  decode_and_execute(byte_to_binary32(inst_word));

  NEXT_STATE.PC += 4;

}
