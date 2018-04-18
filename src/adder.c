#include <stdio.h>
#include <mips.h>

void full_adder(Signal in1, Signal in2, Signal carry_in, Signal *out1, Signal *carry_out)
{
    /* Exercise 5-2 */
    Signal inner1, inner2, inner3;
    xor_circuit(in1, in2, &inner1);
    and_gate(in1, in2, &inner2);
    xor_circuit(inner1, carry_in, out1);
    and_gate(inner1, carry_in, &inner3);
    or_gate(inner2, inner3, carry_out);
}

void rca(Word in1, Word in2, Signal carry_in, Word *out1, Signal *carry_out)
{
    /* Excercise 5-3 */
    int i;
    Word carry;
    carry.bit[0] = carry_in;
    for (i = 0; i < 31; ++i) {
        full_adder(in1.bit[i], in2.bit[i], carry.bit[i], out1->bit + i, carry.bit + i + 1);
    }
    full_adder(in1.bit[31], in2.bit[31], carry.bit[31], out1->bit + 31, carry_out);
}

void test_full_adder()
{
    Signal a, b, out, carry_in, carry_out;
    for (carry_in = 0; carry_in <= 1; ++carry_in) {
        for (a = 0; a <= 1; ++a) {
            for (b = 0; b <= 1; ++b) {
                full_adder(a, b, carry_in, &out, &carry_out);
                printf("FULL_ADDER(%d, %d, %d) => (%d, %d)\n", a, b, carry_in, out, carry_out);
            }
        }
    }
}

void test_rca(int val1, int val2)
{
    Word in1, in2, out1;
    Signal carry_in, carry_out;
    word_set_value(&in1, val1);
    word_set_value(&in2, val2);
    carry_in = false;
    rca(in1, in2, carry_in, &out1, &carry_out);
    printf("RCA(%d, %d, %d) => (%d, %d)\n", val1, val2, carry_in, word_get_value(out1), carry_out);
}

