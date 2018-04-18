#include <stdio.h>
#include <mips.h>

void and_gate(Signal in1, Signal in2, Signal *out1)
{
    *out1 = in1 && in2;
}

void or_gate(Signal in1, Signal in2, Signal *out1)
{
    /* Exercise 5-1 */
    *out1 = in1 || in2;
}

void not_gate(Signal in1, Signal *out1)
{
    *out1 = !in1;
}

void nand_circuit(Signal in1, Signal in2, Signal *out1)
{
    Signal inner;
    and_gate(in1, in2, &inner);
    not_gate(inner, out1);
}

void xor_circuit(Signal in1, Signal in2, Signal *out1)
{
    /* Exercise 5-1 */
    Signal inner1, inner2, inner3, inner4;
    not_gate(in1, &inner1);
    not_gate(in2, &inner2);
    and_gate(in1, inner2, &inner3);
    and_gate(in2, inner1, &inner4);
    or_gate(inner3, inner4, out1);
}

void andn_gate(Signal *in1, int n, Signal *out1)
{
    int i;
    Signal s = true;
    for (i = 0; i < n; ++i) {
        if (in1[i] == false) {
            s = false;
            break;
        }
    }
    *out1 = s;
}

void orn_gate(Signal *in1, int n, Signal *out1)
{
    int i;
    Signal s = false;
    for (i = 0; i < n; ++i) {
        if (in1[i] == true) {
            s = true;
            break;
        }
    }
    *out1 = s;
}

void test_gate()
{
    Signal a, b, out;
    /* AND Gate Test*/
    for (a = 0; a <= 1; ++a) {
        for (b = 0; b <= 1; ++b) {
            and_gate(a, b, &out);
            printf("AND(%d, %d) => %d\n", a, b, out);
        }
    }
    /* OR Gate Test*/
    for (a = 0; a <= 1; ++a) {
        for (b = 0; b <= 1; ++b) {
            or_gate(a, b, &out);
            printf("OR(%d, %d) => %d\n", a, b, out);
        }
    }
    /* NOT Gate Test*/
    for (a = 0; a <= 1; ++a) {
        not_gate(a, &out);
        printf("NOT(%d) => %d\n", a, out);
    }
    /* NAND Circuit Test*/
    for (a = 0; a <= 1; ++a) {
        for (b = 0; b <= 1; ++b) {
            nand_circuit(a, b, &out);
            printf("NAND(%d, %d) => %d\n", a, b, out);
        }
    }
    /* XOR Circuit Test*/
    for (a = 0; a <= 1; ++a) {
        for (b = 0; b <= 1; ++b) {
            xor_circuit(a, b, &out);
            printf("XOR(%d, %d) => %d\n", a, b, out);
        }
    }
}
