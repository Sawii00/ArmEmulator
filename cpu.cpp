#include "cpu.h"

BYTE cpu::readB(u32 pos)
{
	ASSERT(pos < 0 || pos >= RAM_SIZE, "Invalid ram address\n");
	return ram[pos];


}

WORD cpu::readW(u32 pos)
{
	
	ASSERT(pos < 0 || pos + 1 >= RAM_SIZE, "Invalid ram address\n");
	WORD res = 0;
	res = ram[pos] << 8 | ram[pos + 1];
	return res;
}

DWORD cpu::readD(u32 pos)
{
	ASSERT(pos < 0 || pos + 3 >= RAM_SIZE, "Invalid ram address\n");
	DWORD res = 0;
	res = ram[pos] << 24 | ram[pos + 1] << 16 | ram[pos + 2] << 8 | ram[pos + 3];
	return res;
}


void cpu::writeB(u32 pos, BYTE val)
{
	ASSERT(pos < 0 || pos >= RAM_SIZE, "Invalid ram address\n");
	ram[pos] = val;
}

void cpu::writeW(u32 pos, WORD val)
{
	ASSERT(pos < 0 || pos + 1 >= RAM_SIZE, "Invalid ram address\n");
	ram[pos] = val & 0xFF;
	ram[pos + 1] = val >> 8 & 0xFF;
}

void cpu::writeD(u32 pos, DWORD val)
{
	ASSERT(pos < 0 || pos + 3 >= RAM_SIZE, "Invalid ram address\n");
	ram[pos] = val & 0xFF;
	ram[pos + 1] = val >> 8 & 0xFF;
	ram[pos + 2] = val >> 16 & 0xFF;
	ram[pos + 3] = val >> 24 & 0xFF;
}

DWORD cpu::extract(DWORD& instruction, u8 pos, u8 n)
{
	ASSERT(n > 32 || pos >= 31, "Invalid extraction\n");
    //maybe check if pos + n <= 32
	if(n == 0)return 0;
	static DWORD mask_table[32] = {0x1, 0x3, 0x7, 0xF, 0x1F, 0x3F, 0x7F, 0xFF, 0x1FF, 0x3FF, 0x7FF, 0xFFF, 0x1FFF, 0x3FFF, 0x7FFF, 0xFFFF, 0x1FFFF, 0x3FFFF, 0x7FFFF, 0xFFFFF, 0x1FFFFF, 0x3FFFFF, 0x7FFFFF, 0xFFFFFF, 0x1FFFFFF, 0x3FFFFFF, 0x7FFFFFF, 0xFFFFFFFF};
	DWORD mask = mask_table[n - 1];
	DWORD res = (instruction >> pos) & mask;
	return res;

}


inline void cpu::fetch()
{
	curr_instruction = readD(PC++);
}

void cpu::execute()
{

	DWORD condition = extract(curr_instruction, 28, 4);
	switch(condition)
	{
		case(0):
		{
			if(!status.z)
					return;
            break;
		}
		case(1):
		{
			if(status.z)
					return;
            break;
		}
		case(2):
		{
			if(!status.c)
					return;
            break;
		}
		case(3):
		{
			if(status.c)
					return;
            break;
		}
		case(4):
		{
			if(!status.n)
					return;
            break;
		}
		case(5):
		{
			if(status.n)
					return;
            break;
        }
        case(6):
        {
            if(!status.v)
                return;
            break;
        }
        case(7):
        {
            if(status.v)
                return;
            break;
        }
        case(8):
        {
            if(!(status.c && !status.z))
                return;
            break;
        }
        case(9):
        {
            if(!(!status.c && status.z))
                return;
            break;
        }
        case(10):
        {
            if(status.n != status.v)
                return;
            break;
        }
        case(11):
        {
            if(status.n == status.v)
                return;
            break;
        }
        case(12):
        {
            if(status.z || status.n != status.v)
                return;
            break;
        }
        case(13):
        {
            if(!(status.z || status.n != status.v))
                return;
            break;
        }
        case(14):
        {
            //always continue;	
            break;
        }
        case(15):
        {

            ERROR("Invalid Condition Code\n");
        }
	
	}

    //we reached this point if the instruction is to be executed

    DWORD bits_27_25 = extract(curr_instruction, 25, 3);

    switch(bits_27_25)
    {
        case(0):
            {

                handle_000();

            }
        case(1):
            {

                handle_001();

            }
        case(2):
            {

                handle_010();

            }
        case(3):
            {

                handle_011();

            }
        case(4):
            {

                handle_100();

            }
        case(5):
            {

                handle_101();

            }
        case(6):
            {

                handle_110();

            }
        case(7):
            {

                handle_111();

            }



    }






}


void handle_000()
{
    DWORD opcode = extract(curr_instruction, 21, 4);

    switch(opcode)
    {
        case(AND):
            {
                handle_AND();
                break;
            }

        case(EOR):
            {
                handle_EOR();
                break;
            }

        case(SUB):
            {
                handle_SUB();
                break;
            }

        case(RSB):
            {
                handle_RSB();
                break;
            }

        case(ADD):
            {
                handle_ADD();
                break;
            }

        case(ADC):
            {
                handle_ADC();
                break;
            }

        case(SBC):
            {
                handle_SBC();
                break;
            }

        case(RSC):
            {
                handle_RSC();
                break;
            }

        case(TST):
            {
                DWORD s_bit = extract(curr_instruction, 20, 1);
                if(!s_bit)
                    handle_MISC();
                handle_TST();
                break;
            }

        case(TEQ):
            {
                DWORD s_bit = extract(curr_instruction, 20, 1);
                if(!s_bit)
                    handle_MISC();

                handle_TEQ();
                break;
            }

        case(CMP):
            {
                DWORD s_bit = extract(curr_instruction, 20, 1);
                if(!s_bit)
                    handle_MISC();
                handle_CMP();
                break;
            }

        case(CMN):
            {
                DWORD s_bit = extract(curr_instruction, 20, 1);
                if(!s_bit)
                    handle_MISC();
                handle_CMN();
                break;
            }

        case(ORR):
            {
                handle_ORR();
                break;
            }

        case(MOV):
            {
                handle_MOV();
                break;
            }

        case(BIC):
            {
                handle_BIC();
                break;
            }

        case(MVN):
            {
                handle_MVN();
                break;
            }


    }

}

void compute_operands_data_processing(DWORD& first_operand, DWORD& second_operand, BYTE& carry_out)
{

    DWORD i_bit = extract(curr_instruction, 25, 1);
    DWORD first_operand_reg = extract(curr_instruction, 16, 4);
    
    //to check it gets updated by every path
    carry_out = 99;

    if(i_bit)
    {
        DWORD rotate_imm = extract(curr_instruction, 8, 4);
        DWORD immed_8 = extract(curr_instruction, 0, 8);

        second_operand = rotr32(immed_8,  rotate_imm * 2); 
        if(first_operand_reg == 15)
            first_operand = readD(PC + 8);
        else 
            first_operand = regs[first_operand_reg];

        if(!rotate_imm)
            carry_out = status.c;
        else
            carry_out = extract(second_operand, 31, 1);

    }
    else
    {

        //register as second operand
        if(extract(curr_instruction, 4, 8) == 0x0)
        {
            DWORD second_operand_reg = extract(curr_instruction, 0, 4);

            if(first_operand_reg == 15)
                first_operand = readD(PC + 8);
            else 
                first_operand = regs[first_operand_reg];

            if(second_operand_reg == 15)
                second_operand = readD(PC + 8);
            else
                second_operand = regs[second_operand_reg];

            carry_out = status.c;

        }
        //logical shift left by immediate
        else if(extract(curr_instruction, 4,3) == 0x0)
        {

            DWORD second_operand_reg = extract(curr_instruction, 0, 4);
            DWORD shift_imm = extract(curr_instruction, 7, 4);

            if(shift_imm > 31)
                ERROR("Invalid shift left by immediate\n");

            if(first_operand_reg == 15)
                first_operand = readD(PC + 8);
            else 
                first_operand = regs[first_operand_reg];

            if(second_operand_reg == 15)
                second_operand = readD(PC + 8);
            else
                second_operand = regs[second_operand_reg];

            if(shift_imm)
            {
                carry_out = extract(second_operand, 32 - shift_imm, 1); 
            }
            else
            {
                carry_out = status.c;
            }

            second_operand << shift_imm;
        }


        //logical shift left by register
        else if(extract(curr_instruction, 4, 4) == 0x1)
        {

            DWORD second_operand_reg = extract(curr_instruction, 0, 4);

            DWORD shift_reg = extract(curr_instruction, 8, 4);
            DWORD shift_imm = regs[shift_reg];

            if(first_operand_reg == 15 || second_operand_reg == 15 || shift_reg == 15 || extract(curr_instruction, 12, 4) == 15)
                ERROR("Undefined... addressing mode data processing instruction\n");

            if(shift_imm > 31)
            {
                carry_out = 0;
            }
            else if(shift_imm == 0)
            {
                carry_out = status.c;
            }
            else
            {
                carry_out = extract(regs[second_operand_reg], 31 - regs[shift_reg]);
            }
            second_operand = regs[second_operand_reg] << regs[shift_reg];
            first_operand = regs[first_operand_reg];


        }
        //logical shift right by immediate
        else if(extract(curr_instruction, 4,3) == 0x2)
        {

            DWORD second_operand_reg = extract(curr_instruction, 0, 4);
            DWORD shift_imm = extract(curr_instruction, 7, 4);

            if(first_operand_reg == 15)
                first_operand = readD(PC + 8);
            else 
                first_operand = regs[first_operand_reg];

            if(!shift_imm)
            {
                second_operand = 0;
                carry_out = extract(regs[second_operand_reg], 31, 1);
            }
            else if (shift_imm <= 32)
            {
                if(second_operand_reg == 15)
                    second_operand = readD(PC + 8);
                else
                    second_operand = regs[second_operand_reg];

                second_operand >> shift_imm;
                carry_out = extract(regs[second_operand], shift_imm - 1, 1);


            }
            else
            {
                ERROR("Invalid immediate right shift\n");
            }
        }
        //logical shift right by register
        else if(extract(curr_instruction, 4, 4) == 0x3)
        {

            DWORD second_operand_reg = extract(curr_instruction, 0, 4);

            DWORD shift_reg = extract(curr_instruction, 8, 4);
            BYTE shift_imm = extract(regs[shift_reg], 0, 8);

            if(first_operand_reg == 15 || second_operand_reg == 15 || shift_reg == 15 || extract(curr_instruction, 12, 4) == 15)
                ERROR("Undefined... addressing mode data processing instruction\n");

            first_operand = regs[first_operand_reg];

            if(!shift_imm)
            {
                second_operand = regs[second_operand_reg];
                carry_out = status.c;
            }
            else if(shift_imm < 32)
            {
                second_operand = regs[second_operand_reg] >> shift_imm; 
                carry_out = extract(regs[second_operand_reg], shift_imm - 1);
            }
            else if(shift_imm == 32)
            {
                second_operand = 0;
                carry_out = extract(regs[second_operand_reg], 31, 1);
            }
            else
            {
                second_operand = 0;
                carry_out = 0;
            }



        }
        //arithmetic shift right by immediate
        else if(extract(curr_instruction 4, 3) == 0x4)
        {

            DWORD second_operand_reg = extract(curr_instruction, 0, 4);
            DWORD shift_imm = extract(curr_instruction, 7, 4);

            if(first_operand_reg == 15)
                first_operand = readD(PC + 8);
            else 
                first_operand = regs[first_operand_reg];

            if(second_operand_reg == 15)
                second_operand = readD(PC + 8);
            else
                second_operand = regs[second_operand_reg];

            if(!shift_imm)
            {
                //not a typo.. i need 31st bit
                if(second_operand >> 31)
                {
                    second_operand = 0xFFFFFFFF;
                }
                else
                {   
                    second_operand = 0;
                }
                
                carry_out = extract(regs[second_operand_reg], 31, 1);
            }
            else if(shift_imm <= 32)
            {
                second_operand = asr32(second_operand, shift_imm);
                carry_out = extract(regs[second_operand_reg], shift_imm - 1);

            }
            else
            {
                ERROR("Invalid arithmetic shift right immediate\n");
            }

        }
        //arithmetic shift right by register
        else if(extract(curr_instruction, 4,4) == 0x5)
        {

            DWORD second_operand_reg = extract(curr_instruction, 0, 4);

            DWORD shift_reg = extract(curr_instruction, 8, 4);
            BYTE shift_imm = regs[shift_reg];

            if(first_operand_reg == 15 || second_operand_reg == 15 || shift_reg == 15 || extract(curr_instruction, 12, 4) == 15)
                ERROR("Undefined... addressing mode data processing instruction\n");

            if(shift_imm>= 32)
            {
                BYTE second_operand_31st_bit = extract(regs[second_operand_reg], 31, 1);
                if(!second_operand_31st_bit)
                {
                    second_operand = 0;
                }
                else
                {
                    second_operand = 0xFFFFFFFF;
                    //HANDLE CARRY OUT
                }
                carry_out = second_operand_31st_bit; 

            }
            else if(shift_imm == 0)
            {
                second_operand = regs[second_operand_reg];
                carry_out = status.c;
            }
            else
            {
                second_operand = asr32(regs[second_operand_reg], shift_imm);
                carry_out = extract(regs[second_operand_reg], shift_imm - 1, 1);

            }

            first_operand = regs[first_operand_reg];
        }
        //rotate right with extend
        else if(extract(curr_instruction, 4, 8) == 0x6)
        {

            DWORD second_operand_reg = extract(curr_instruction, 0, 4);
            
            if(first_operand_reg == 15)
                first_operand = readD(PC + 8);
            else 
                first_operand = regs[first_operand_reg];

            if(second_operand_reg == 15)
                second_operand = readD(PC + 8);
            else
                second_operand = regs[second_operand_reg];

            carry_out = extract(regs[second_operand_reg], 0, 1);
            second_operand = second_operand >> 1 | status.c << 31;

        }
        //rotate right by register
        else if(extract(curr_instruction, 4, 4) == 0x7)
        {

            DWORD second_operand_reg = extract(curr_instruction, 0, 4);

            DWORD shift_reg = extract(curr_instruction, 8, 4);

            if(first_operand_reg == 15 || second_operand_reg == 15 || shift_reg == 15 || extract(curr_instruction, 12, 4) == 15)
                ERROR("Undefined... addressing mode data processing instruction\n");

            if(extract(regs[shift_reg], 0, 8) == 0)
            {
                second_operand = regs[second_operand_reg];
                carry_out = status.c;
            }
            else if(extract(regs[shift_reg], 0, 5) == 0)
            {
                second_operand = regs[second_operand_reg];
                carry_out = extract(regs[second_operand_reg], 31, 1); 
            }
            else
            {
                second_operand = rotr32(regs[second_operand_reg], extract(regs[shift_reg], 0, 5));
                carry_out = extract(regs[second_operand_reg], extract(regs[shift_reg], 0 , 5) - 1 , 1);
            }
            
            first_operand = regs[first_operand_reg];

        }
        //rotate right by immediate
        else if(extract(curr_instruction, 4,3) == 0x6)
        {


            DWORD second_operand_reg = extract(curr_instruction, 0, 4);
            DWORD shift_imm = extract(curr_instruction, 7, 4);

            if(shift_imm > 31)
                ERROR("Invalid Rotate Immediate\n");

            if(first_operand_reg == 15)
                first_operand = readD(PC + 8);
            else 
                first_operand = regs[first_operand_reg];

            if(second_operand_reg == 15)
                second_operand = readD(PC + 8);
            else
                second_operand = regs[second_operand_reg];

              second_operand = rotr32(second_operand, shift_imm);
              carry_out = extract(regs[second_operand_reg], shift_imm - 1);

        }
        else
        {
            ERROR("Error addressing mode data processing\n")
        }


    }
    
    if(carry_out > 1)
        ERROR("Error setting up carry out data processing\n");
}


void handle_AND()
{

    DWORD s_bit = extract(curr_instruction, 20, 1);
    DWORD dest_reg = extract(curr_instruction, 12, 4);
    DWORD first_operand, second_operand;
    BYTE carry_out;

    compute_operands_data_processing(first_operand, second_operand, carry_out);

    DWORD res = first_operand & second_operand;

    regs[dest_reg] = res;

    if(s_bit)
    {
        if(dest_reg == 15)
        {
            //@TODO(sawii): modes
        }
        else
        {
            status.n = extract(res, 31, 1);            
            status.z = res == 0 ? 1 : 0;
            status.c = carry_out;
        }
    }
}

void handle_EOR()
{
    

    DWORD s_bit = extract(curr_instruction, 20, 1);
    DWORD dest_reg = extract(curr_instruction, 12, 4);
    DWORD first_operand, second_operand;
    BYTE carry_out;

    compute_operands_data_processing(first_operand, second_operand, carry_out);

    DWORD res = first_operand | second_operand;

    regs[dest_reg] = res;

    if(s_bit)
    {
        if(dest_reg == 15)
        {
            //@TODO(sawii): modes
        }
        else
        {
            status.n = extract(res, 31, 1);            
            status.z = res == 0 ? 1 : 0;
            status.c = carry_out;
        }
    }

}

void handle_SUB()
{


    DWORD s_bit = extract(curr_instruction, 20, 1);
    DWORD dest_reg = extract(curr_instruction, 12, 4);
    DWORD first_operand, second_operand;
    BYTE carry_out;

    compute_operands_data_processing(first_operand, second_operand, carry_out);

    SIGNED_QWORD res = (int)first_operand - second_operand;

    regs[dest_reg] = res;

    if(s_bit)
    {
        if(dest_reg == 15)
        {
            //@TODO(sawii): modes
        }
        else
        {
            BYTE first_sign = extract(first_operand, 31, 1);
            BYTE second_sign = extract(second_operand, 31, 1);

            status.n = extract(res, 31, 1);            
            status.z = res == 0 ? 1 : 0;
            status.c = res < 0 ? 0 : 1;
            if(first_sign != second_sign)
            {
                if(second_sign  == extract(res, 31, 1))
                    status.v = 1;
                else
                    status.v = 0;
            }
            else
            {
                status.v = 0;
            }
        }
    }
}

void handle_RSB()
{

}

void handle_ADD()
{

}

void handle_ADC()
{

}

void handle_SBC()
{

}

void handle_RSC()
{

}

void handle_TST()
{

}

void handle_TEQ()
{

}

void handle_CMP()
{

}

void handle_CMN()
{

}

void handle_ORR()
{

}

void handle_MOV()
{

}

void handle_BIC()
{

}

void handle_MVN()
{

}




void handle_001()
{

}

void handle_010()
{

}

void handle_011()
{

}

void handle_100()
{

}

void handle_101()
{

}

void handle_110()
{

}

void handle_111()
{

}





void cpu::init()
{
    *(DWORD*)&status = 0;
    std::cout << "TestLol:\n";
    DWORD t = 0x12345678;

	std::cout << extract(t, 0, 5) << '\n';
	std::cout << extract(t, 5, 3) << '\n';
	std::cout << extract(t, 14, 1) << '\n';
	std::cout << extract(t, 8, 16) << '\n';

}


