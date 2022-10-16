#include "ComandSystem.h"

#include "..\Logging\Logging.h"

int InitHeader(Header* header, int comands_number)
{
    CHECK(header == nullptr, "Null header", -1);

    header->signature        = SIGNATURE;
    header->version          = ASM_VERSION;
    header->comands_number   = comands_number;

    return 0;
}

void DumpCPU(CPU* cpu)
{
    CHECK(cpu == nullptr, "Error in dump\n", (void)0);

    LogPrintf("\nnumber_comands = %d\n", cpu->number_comands);
    LogPrintf("pc               = %d\n", cpu->pc);
    LogPrintf("code             = %p\n", cpu->code);

    LogPrintf("regs:\n{\n");
    for(int i = 0; i < REG_N; i++)
        LogPrintf("\tr%cx = %d\n", i + 'a', cpu->regs[i + 1]);
    LogPrintf("}\n");

    //LogPrintf("ram:\n{\n");
    //for(int i = 0; i < RAM_SIZE; i++)
//        LogPrintf("\t[%d] = %d\n", i, cpu->ram[i]);
    //LogPrintf("}\n");

    DUMP_STACK(cpu->stk);
}