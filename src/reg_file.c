#include <stdio.h>
#include <mips.h>

void register_run(Register *reg, Signal wctl, Word wdata, Word *rdata)
{
    word_set_value(rdata, reg->val);
    if (wctl) {
        reg->val = word_get_value(wdata);
    }
}

void register_set_value(Register *reg, int val)
{
    reg->val = val;
}

int register_get_value(Register *reg)
{
    return reg->val;
}

void decorder5(Signal a[5], Word *b)
{
    int i, val;
    val = 0;
    for (i = 0; i < 5; ++i) {
        if (a[i]) {
            val += (1 << i);
        }
    }
    for (i = 0; i < 32; ++i) {
        if (i == val) {
            b->bit[i] = true;
        }
        else {
            b->bit[i] = false;
        }
    }
}

void mux32(Word ins[32], Signal ctls[5], Word *out)
{
    int i, val;
    val = 0;
    for (i = 0; i < 5; ++i) {
        if (ctls[i]) {
            val += (1 << i);
        }
    }
    *out = ins[val];
}

void register_file_run(RegisterFile *rf, Signal register_write, Signal *read1, Signal *read2, Signal *write1, Word wdata, Word *rdata1, Word *rdata2)
{
    int i;
    Word inner1, inner2, inner3[32];
    decorder5(write1, &inner1);
    register_run(rf->r, false, wdata, inner3);
    for (i = 1; i < 32; ++i) {
        and_gate(register_write, inner1.bit[i], inner2.bit + i);
        register_run(rf->r + i, inner2.bit[i], wdata, inner3 + i);
        /* printf("%d, %d\n", inner2.bit[i], word_get_value(inner3[i])); */
    }
    mux32(inner3, read1, rdata1);
    mux32(inner3, read2, rdata2);
}

void test_register_file()
{
    Signal register_write;
    Signal read1[5] = {true, false, false, false, false};
    Signal read2[5] = {true, false, false, false, false};
    Signal write1[5] = {true, false, false, false, false};
    Word wdata, rdata1, rdata2;
    RegisterFile rf;

    register_write = true;
    word_set_value(&wdata, 100);

    register_file_run(&rf, register_write, read1, read2, write1, wdata, &rdata1, &rdata2);
    
    printf("old data of rdata1: %d\n", word_get_value(rdata1));
    register_file_run(&rf, register_write, read1, read2, write1, wdata, &rdata1, &rdata2);
    printf("new data of rdata1: %d\n", word_get_value(rdata1));

}
