DEF_CMD(HLT, 0, 0,                                                  \
{                                                                   \
    return;                                                         \
})

DEF_CMD(PUSH, 1, 1,                                                 \
{                                                                   \
    int arg = 0;                                                    \
    if (GetPushArg(&arg, cmd, cpu) != 0)                            \
        return;                                                     \
    StackPush(&cpu->stk, arg);                                      \
})

DEF_CMD(ADD, 2, 0,                                                  \
{                                                                   \
    size_t error = 0;                                                  \
    Elem a1 = StackPop(&(cpu->stk), &error);                         \
    CHECK(error != NO_ERROR, "Error during stack pop", (void)0);    \
    Elem a2 = StackPop(&(cpu->stk), &error);                         \
    CHECK(error != NO_ERROR, "Error during stack pop", (void)0);    \
    StackPush(&(cpu->stk), a2 + a1);                                \
})

DEF_CMD(SUB, 3, 0,                                                  \
{                                                                   \
    size_t error = 0;                                                      \
    Elem a1 = StackPop(&(cpu->stk), &error);                             \
    CHECK(error != NO_ERROR, "Error during stack pop", (void)0);    \
    Elem a2 = StackPop(&(cpu->stk), &error);                             \
    CHECK(error != NO_ERROR, "Error during stack pop", (void)0);    \
    StackPush(&(cpu->stk), a2 - a1);                             \
})

DEF_CMD(MUL, 4, 0,                                                  \
{                                                                   \
    size_t error = 0;                                                      \
    Elem a1 = StackPop(&(cpu->stk), &error);                             \
    CHECK(error != NO_ERROR, "Error during stack pop", (void)0);    \
    Elem a2 = StackPop(&(cpu->stk), &error);                             \
    CHECK(error != NO_ERROR, "Error during stack pop", (void)0);    \
    StackPush(&(cpu->stk), a2 * a1);                             \
})

DEF_CMD(DIV, 5, 0,                                                  \
{                                                                   \
    size_t error = 0;                                                      \
    size_t a1 = StackPop(&(cpu->stk), &error);                             \
    CHECK(error != NO_ERROR, "Error during stack pop", (void)0);    \
    Elem a2 = StackPop(&(cpu->stk), &error);                             \
    CHECK(error != NO_ERROR, "Error during stack pop", (void)0);    \
    CHECK(a2 == 0, "Division by zero", (void)0);                    \
    StackPush(&(cpu->stk), a2 / a1);                                \
})

DEF_CMD(OUT, 6, 0,                                                  \
{                                                                   \
    size_t error = 0;                                                  \
    Elem   a1    = StackPop(&(cpu->stk), &error);                         \
    CHECK(error != NO_ERROR, "Error during out", (void)0);          \
    printf("%d\n", a1);                                             \
})

DEF_CMD(POP, 7, 1,                                                  \
{                                                                   \
    size_t error = 0;                                               \
    Elem   a1    = StackPop(&(cpu->stk), &error);                   \
    CHECK(error != NO_ERROR, "Error during stack pop", (void)0);    \
                                                                    \
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
{                                                                   \
    cpu->pc = cpu->code[cpu->pc++]; ;                               \
})