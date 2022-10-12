DEF_CMD(HLT, 0, 0,                                                  \
{                                                                   \
    return;                                                         \
})

DEF_CMD(PUSH, 1, 1,                                                 \
{                                                                   \
    int arg = 0;                                                    \
    if (GetPushArg(&arg, cmd, cpu) != 0)                            \
        return;                                                     \
    PUSH(arg);                                                      \
})

DEF_CMD(ADD, 2, 0,                                                  \
{                                                                   \
    Elem a1 = 0, a2 = 0;                                            \
    POP(a1);                                                        \
    POP(a2);                                                        \
    PUSH(a2 + a1);                                                  \
})

DEF_CMD(SUB, 3, 0,                                                  \
{                                                                   \
    Elem a1 = 0, a2 = 0;                                            \
    POP(a1);                                                        \
    POP(a2);                                                        \
    PUSH(a2 - a1);                                                  \
})

DEF_CMD(MUL, 4, 0,                                                  \
{                                                                   \
    Elem a1 = 0, a2 = 0;                                            \
    POP(a1);                                                        \
    POP(a2);                                                        \
    PUSH(a2 * a1);                                                  \
})

DEF_CMD(DIV, 5, 0,                                                  \
{                                                                   \
    Elem a1 = 0, a2 = 0;                                            \
    POP(a1);                                                        \
    POP(a2);                                                        \
    CHECK(a1 == 0, "Divison by zero\n", (void)-1);                 \
    PUSH(a2 / a1);                                                  \
})

DEF_CMD(OUT, 6, 0,                                                  \
{                                                                   \
    Elem a1 = 0;                                                    \
    POP(a1);                                                        \
    printf("%d\n", a1);                                             \
})

DEF_CMD(POP, 7, 1,                                                  \
{                                                                   \
    Elem a1 = 0;                                                    \
    POP(a1);                                                        \
    int arg = 0;                                                    \
    int write_to = 0;                                               \
    if (GetPopArg(&arg, &write_to, cmd, cpu) != 0)                  \
        return;                                                     \
                                                                    \
    if (write_to == ARG_MEM)                                        \
        cpu->ram[arg] = a1;                                         \
    if (write_to == ARG_REG)                                        \
        cpu->regs[arg] = a1;                                        \
})

DEF_CMD(DUMP, 8, 0,                                                 \
{                                                                   \
    DumpCPU(cpu);                                                   \
})

DEF_CMD(JMP, 9, 2,                                                   \
{                                                                    \
    cpu->pc = cpu->code[cpu->pc++];                                  \
})