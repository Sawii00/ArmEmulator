#pragma once
#include <array>
#include "types.h"
#include "utils.h"
#define RAM_SIZE 4096


#define R0 regs[0]; 
#define R1 regs[1]; 
#define R2 regs[2]; 
#define R3 regs[3]; 
#define R4 regs[4]; 
#define R5 regs[5]; 
#define R6 regs[6]; 
#define R7 regs[7]; 
#define R8 regs[8]; 
#define R9 regs[9]; 
#define R10 regs[10]; 
#define R11 regs[11]; 
#define R12 regs[12]; 
#define SP regs[13]; 
#define LR regs[14]; 
#define PC regs[15]; 

struct cpsr
{
	BYTE n : 1;
	BYTE z : 1;
	BYTE c : 1;
	BYTE v : 1;
	BYTE q : 1;
	BYTE res : 2;
	BYTE j : 1;
	BYTE res2 : 4;
	BYTE ge : 4;
	BYTE res3 : 6;
	BYTE e : 1;
	BYTE a : 1;
	BYTE i : 1;
	BYTE f : 1;
	BYTE t : 1;
	BYTE m : 5;
};

enum DATA_PROCESSING_INSTRUCTIONS
{
    AND, EOR, SUB, RSB, ADD, ADC, SBC, RSC, TST, TEQ, CMP, CMN, ORR, MOV, BIC, MVN
};


class cpu
{

	private:
		std::array<BYTE, RAM_SIZE> ram;
		std::array<DWORD, 31> regs;
		cpsr status;
		DWORD curr_instruction;

//needs to be little endian
		BYTE readB(u32 pos);
		WORD readW(u32 pos);
		DWORD readD(u32 pos);

		void writeB(u32 pos, BYTE val);
		void writeW(u32 pos, WORD val);
		void writeD(u32 pos, DWORD val);


		WORD extract(DWORD& instruction, u8 pos, u8 n); 
		inline void fetch();
		void execute();


        //INSTRUCTION DECODING
        void handle_000();

        //written, not tested
        void compute_operands_data_processing(DWORD& first_operand, DWORD& second_operand, BYTE& carry_out);

        //data processing instructions
        void handle_AND();
        void handle_EOR();
        void handle_SUB();
        void handle_RSB();
        void handle_ADD();
        void handle_ADC();
        void handle_SBC();
        void handle_RSC();
        void handle_TST();
        void handle_TEQ();
        void handle_CMP();
        void handle_CMN();
        void handle_ORR();
        void handle_MOV();
        void handle_BIC();
        void handle_MVN();

        void handle_001();
        void handle_010();
        void handle_011();
        void handle_100();
        void handle_101();
        void handle_110();
        void handle_111();

	public:

		void init();




};
