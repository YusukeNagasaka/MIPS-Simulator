#include <stdio.h>
#include <mips.h>

void mux(Signal in1, Signal in2, Signal ctl, Signal *out1)
{
    Signal inner1, inner2, inner3;
    not_gate(ctl, &inner1);
    and_gate(inner1, in1, &inner2);
    and_gate(ctl, in2, &inner3);
    or_gate(inner2, inner3, out1);
}

void mux4(Signal in1, Signal in2, Signal in3, Signal in4, Signal ctl1, Signal ctl2, Signal *out1)
{
    Signal inner1[3], inner2[3], inner3[3], inner4[3], inner5[4];

    /* ctl1 != true && ctl2 != true) */
    not_gate(ctl1, inner1 + 0);
    not_gate(ctl2, inner1 + 1);
    inner1[2] = in1;
    andn_gate(inner1, 3, inner5 + 0);
    /* ctl1 == true && ctl2 != true) */
    inner2[0] = ctl1;
    not_gate(ctl2, inner2 + 1);
    inner2[2] = in2;
    andn_gate(inner2, 3, inner5 + 1);
    /* (ctl1 != true && ctl2 == true) */
    not_gate(ctl1, inner3 + 0);
    inner3[1] = ctl2;
    inner3[2] = in3;
    andn_gate(inner3, 3, inner5 + 2);
    /* (ctl1 == true && ctl2 == true) */
    inner4[0] = ctl1;
    inner4[1] = ctl2;
    inner4[2] = in4;
    andn_gate(inner4, 3, inner5 + 3);
    
    orn_gate(inner5, 4, out1);
}

void alu(Signal *ops, Signal a, Signal b, Signal less, Signal carry_in, Signal *s, Signal *carry_out)
{
    /* Exercise 6-1 */
    /* ver. 1 */
    /* Signal inner1, inner2, inner3; */
    /* and_gate(a, b, &inner1); */
    /* or_gate(a, b, &inner2); */
    /* full_adder(a, b, carry_in, &inner3, carry_out); */
    /* mux4(inner1, inner2, inner3, false, ops[0], ops[1], s); */

    /* ver. 2 */
    Signal inner1, inner2, inner3, inner4, inner5;
    not_gate(b, &inner4);
    mux(b, inner4, ops[2], &inner5);

    and_gate(a, inner5, &inner1);
    or_gate(a, inner5, &inner2);
    full_adder(a, inner5, carry_in, &inner3, carry_out);
    mux4(inner1, inner2, inner3, less, ops[0], ops[1], s);
    
}

void alu_msb(Signal *ops, Signal a, Signal b, Signal less, Signal carry_in, Signal *s, Signal *carry_out, Signal *set)
{
    /* ver. 3 */
    Signal inner1, inner2, inner3, inner4, inner5;
    not_gate(b, &inner4);
    mux(a, inner4, ops[2], &inner5);

    and_gate(a, inner5, &inner1);
    or_gate(a, inner5, &inner2);
    full_adder(a, inner5, carry_in, &inner3, carry_out);
    mux4(inner1, inner2, inner3, less, ops[0], ops[1], s);
    *set = inner3;
}

void alu32(Signal *ops, Word a, Word b, Signal carry_in, Word *s, Signal *zero)
{
    /* Exercise 6-1 */
    int i;
    Signal c[33];
    Signal set;
    c[0] = ops[2];
    for (i = 0; i < 31; ++i) {
        alu(ops, a.bit[i], b.bit[i], false, c[i], &(s->bit[i]), &(c[i + 1]));
    }
    alu_msb(ops, a.bit[31], b.bit[31], false, c[31], &(s->bit[31]), &(c[32]), &set);
    alu(ops, a.bit[0], b.bit[0], set, c[0], &(s->bit[0]), &(c[1]));
    
    /* Signal inner; */
    Signal inner;
    orn_gate(s->bit, 32, &inner);
    not_gate(inner, zero);
}

void test_alu()
{
    Signal a, b, carry_in, s, carry_out;
    Signal ops[3];
    
    /* AND */
    ops[0] = false;
    ops[1] = false;
    ops[2] = false;
    for (carry_in = 0; carry_in <= 1; ++carry_in) {
        for (a = 0; a <= 1; ++a) {
            for (b = 0; b <= 1; ++b) {
                alu(ops, a, b, false, carry_in, &s, &carry_out);
                printf("ALU(AND, %d, %d, %d) => (%d, %d)\n", a, b, carry_in, s, carry_out);
            }
        }
    }
    /* OR */
    ops[0] = true;
    ops[1] = false;
    ops[2] = false;
    for (carry_in = 0; carry_in <= 1; ++carry_in) {
        for (a = 0; a <= 1; ++a) {
            for (b = 0; b <= 1; ++b) {
                alu(ops, a, b, false, carry_in, &s, &carry_out);
                printf("ALU(OR, %d, %d, %d) => (%d, %d)\n", a, b, carry_in, s, carry_out);
            }
        }
    }
    /* ADD */
    ops[0] = false;
    ops[1] = true;
    ops[2] = false;
    for (carry_in = 0; carry_in <= 1; ++carry_in) {
        for (a = 0; a <= 1; ++a) {
            for (b = 0; b <= 1; ++b) {
                alu(ops, a, b, false, carry_in, &s, &carry_out);
                printf("ALU(ADD, %d, %d, %d) => (%d, %d)\n", a, b, carry_in, s, carry_out);
            }
        }
    }
    /* SUB */
    ops[0] = false;
    ops[1] = true;
    ops[2] = true;
    for (carry_in = 0; carry_in <= 1; ++carry_in) {
        for (a = 0; a <= 1; ++a) {
            for (b = 0; b <= 1; ++b) {
                alu(ops, a, b, false, carry_in, &s, &carry_out);
                printf("ALU(SUB, %d, %d, %d) => (%d, %d)\n", a, b, carry_in, s, carry_out);
            }
        }
    }
}

void test_alu32(int val1, int val2)
{
    Signal ops[3];
    Word in1, in2, out1;
    Signal carry_in, carry_out;
    word_set_value(&in1, val1);
    word_set_value(&in2, val2);
    carry_in = false;
    
    /* AND */
    ops[0] = false;
    ops[1] = false;
    ops[2] = false;
    alu32(ops, in1, in2, carry_in, &out1, &carry_out);
    printf("ALU32(AND, %x, %x, %d) => (%x, %d)\n", val1, val2, carry_in, word_get_value(out1), carry_out);

    /* OR */
    ops[0] = true;
    ops[1] = false;
    ops[2] = false;
    alu32(ops, in1, in2, carry_in, &out1, &carry_out);
    printf("ALU32(OR, %x, %x, %d) => (%x, %d)\n", val1, val2, carry_in, word_get_value(out1), carry_out);

    /* ADD */
    ops[0] = false;
    ops[1] = true;
    ops[2] = false;
    alu32(ops, in1, in2, carry_in, &out1, &carry_out);
    printf("ALU32(ADD, %d, %d, %d) => (%d, %d)\n", val1, val2, carry_in, word_get_value(out1), carry_out);

    /* SUB */
    ops[0] = false;
    ops[1] = true;
    ops[2] = true;
    alu32(ops, in1, in2, carry_in, &out1, &carry_out);
    printf("ALU32(SUB, %d, %d, %d) => (%d, %d)\n", val1, val2, carry_in, word_get_value(out1), carry_out);
    alu32(ops, in1, in1, carry_in, &out1, &carry_out);
    printf("ALU32(SUB, %d, %d, %d) => (%d, %d)\n", val1, val1, carry_in, word_get_value(out1), carry_out);

    /* SLT */
    ops[0] = true;
    ops[1] = true;
    ops[2] = true;
    alu32(ops, in1, in2, carry_in, &out1, &carry_out);
    printf("ALU32(SLT, %d, %d, %d) => (%x, %d)\n", val1, val2, carry_in, word_get_value(out1), carry_out);
    alu32(ops, in2, in1, carry_in, &out1, &carry_out);
    printf("ALU32(SLT, %d, %d, %d) => (%d, %d)\n", val2, val1, carry_in, word_get_value(out1), carry_out);

}
