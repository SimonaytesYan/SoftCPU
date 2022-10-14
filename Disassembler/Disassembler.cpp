#include "..\Processor\Processor.h"
#include "..\Libs\ComandSystem\ComandSystem.h"

#define DEF_CMD(name, num, ...) \
    CMD_##name = num,

enum COMANDS
{
    #include "..\Cmd.h"
};

const char original_program[] = "DisAssebledProgram.txt";

void PushCommonArgs(int cmd, FILE* original_text_fp, CPU* cpu)
{
    if ((cmd & ARG_MEM) != 0)
        fprintf(original_text_fp, " [");
    if ((cmd & ARG_IMMED) != 0)
        fprintf(original_text_fp, " %d", cpu->code[cpu->pc++]);

    if ((cmd & ARG_IMMED) != 0 && (cmd & ARG_REG) != 0)
            fprintf(original_text_fp, " +");
                
    if ((cmd & ARG_REG) != 0)
    {
        const char* reg = "";
        fprintf(original_text_fp, " r%cx", cpu->code[cpu->pc++] + 'a' - 1);
    }
    if ((cmd & ARG_MEM) != 0)
        fprintf(original_text_fp, "]");
}

void AddLabel(int int_name, Label* labels, int cmd_index)
{
    int i = 0;
    char name[MAX_LABEL_LEN] = "";
    itoa(int_name, name, 10); 
    for(i = 0; i < MAX_LABELS; i++)
    {
        if (stricmp(name, labels[i].name) == 0)
            return;
        if (strlen(labels[i].name) == 0)
        {
            labels[i].cmd_to = cmd_index;
            strcpy(labels[i].name, name);
            return;
        }
    }
}

void PutLabel(CPU* cpu, Label* labels, FILE* original_text_fp)
{
    for(int i = 0; i < MAX_LABELS; i++)
    {
        if (cpu->pc == labels[i].cmd_to)
        {
            fprintf(original_text_fp, "%d:\n", labels[i].cmd_to);
            return;
        }
    }
}


int RecoverProgram(CPU* cpu, Label* labels)
{
    int   next_label       = 0;
    FILE* original_text_fp = fopen(original_program, "w");

#define DEF_CMD(name, num, arg, ...)                            \
    case num:                                                   \
        if (arg == COMMON_ARGS)                                 \
        {                                                       \
            if ((cmd & ARG_IMMED))                              \
                cpu->pc++;                                      \
            if ((cmd & ARG_REG))                                \
                cpu->pc++;                                      \
        }                                                       \
        else if (arg == JMP_ARGS)                               \
        {                                                       \
            int cmd_index = cpu->code[cpu->pc++];               \
            printf("cmd_ind = %d\n", cmd_index);                \
            AddLabel(cmd_index, labels, cmd_index);             \
        }                                                       \
    break;

    while (cpu->pc < cpu->number_comands)
    {
        int cmd = cpu->code[cpu->pc++];
        int arg = 0;

        switch(cmd & CMD_MASK)
        {
            #include "..\Cmd.h"
        }
    }

#undef DEF_CMD

    cpu->pc = 0;

#define DEF_CMD(name, num, arg, ...)                                                            \
    case num:                                                                                   \
    {                                                                                           \
        fprintf(original_text_fp, #name);                                                       \
        if (arg == COMMON_ARGS)                                                                 \
        {                                                                                       \
            fflush(original_text_fp);                                                           \
            PushCommonArgs(cmd, original_text_fp, cpu);                                         \
        }                                                                                       \
        else if (arg == JMP_ARGS)                                                               \
        {                                                                                       \
            fprintf(original_text_fp, " %d", cpu->code[cpu->pc++]);                            \
        }                                                                                       \
        fprintf(original_text_fp, "\n");                                                        \
    } break;

    while (cpu->pc < cpu->number_comands)
    {
        int arg = 0;
        PutLabel(cpu, labels, original_text_fp);

        int cmd = cpu->code[cpu->pc++];
                                                     
        switch(cmd & CMD_MASK)
        {
            #include "..\Cmd.h"
        }
    }
    PutLabel(cpu, labels, original_text_fp);
#undef DEF_CMD

    fclose(original_text_fp);
}

int main(int argc, char* argv[])
{
    OpenLogFile("Disassembler/Disassembler.txt");
    FILE *fp = nullptr;
    GetExecFileFromCLArgs(&fp, argc, argv);

    Header header = {};
    CheckHeaderFromFile(&header, fp);

    CPU cpu = {};

    GetCPUFromFile(&cpu, header.comands_number, fp);
    Label labels[MAX_LABELS] = {};
    if (RecoverProgram(&cpu, labels) != 0)
        return -1;

    CloseLogFile();
}