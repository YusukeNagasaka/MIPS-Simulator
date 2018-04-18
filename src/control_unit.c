#include <mips.h>

void control_unit(Signal opcode[6], Signal *register_dst, Signal *register_write, Signal *aluop1)
{
    int i;
    Signal inner[6], inner1;
    for (i = 0; i < 6; ++i) {
        not_gate(opcode[i], inner + i);
    }
    andn_gate(inner, 6, &inner1);
    *register_dst = inner1;
    *register_write = inner1;
    *aluop1 = inner1;
}

void alu_control_unit(Signal *funct, Signal aluop1, Signal *ops)
{
    Signal inner1, inner2, inner3;
    or_gate(funct[0], funct[3], &inner1);
    not_gate(funct[2], &inner2);
    not_gate(aluop1, &inner3);
    and_gate(inner1, aluop1, ops + 0);
    or_gate(inner2, inner3, ops + 1);
    and_gate(funct[1], aluop1, ops + 2);
}
