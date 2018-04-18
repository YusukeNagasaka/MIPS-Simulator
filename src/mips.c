#include <stdio.h>
#include <mips.h>

void mips_run(MIPS *m, int inst_num)
{
    Word npaddr, paddr;
    Word instr;
    Word wdata, rdata1, rdata2;
    Signal register_dst, register_write, alu_op1, ops[3];
    Signal zero, zero4;
    int i;
    for (i = 0; i < inst_num; ++i) {
        register_run(&(m->pc), true, npaddr, &paddr);
        printf("npaddr: %x, paddr: %x\n", word_get_value(npaddr), word_get_value(paddr));
        inst_memory_run(&(m->im), paddr, &instr);
    
        /* printf("$t0 = %x\n", register_get_value(m->rf.r + 8)); */
        control_unit(instr.bit + 26, &register_dst, &register_write, &alu_op1);
        alu_control_unit(instr.bit, alu_op1, ops);
        register_file_run(&(m->rf), register_write, instr.bit + 21, instr.bit + 16, instr.bit + 11, wdata, &rdata1, &rdata2);
        alu32(ops, rdata1, rdata2, ops[2], &wdata, &zero);
        printf("ops=(%d, %d, %d)\n", ops[0], ops[1], ops[2]);
        printf("wdata: %x\n", word_get_value(wdata));
        printf("rdata1: %x\n", word_get_value(rdata1));
        printf("rdata2: %x\n", word_get_value(rdata2));
        register_file_run(&(m->rf), register_write, instr.bit + 21, instr.bit + 16, instr.bit + 11, wdata, &rdata1, &rdata2);

        Signal pcadd[3] = {false, true, false};
        Word four;
        word_set_value(&four, 4);
        alu32(pcadd, paddr, four, pcadd[2], &npaddr, &zero4);
        register_run(&(m->pc), true, npaddr, &paddr);
        printf("$t0 = %x\n", register_get_value(m->rf.r + 8));
    }
}

void mips_init(MIPS *m, int inst[], int inst_num)
{
    int i;
    register_set_value(&(m->pc), 0x04000000);
    register_set_value((m->rf.r + 9), 0x100);
    register_set_value((m->rf.r + 10), 0x300);
    for (i = 0; i < inst_num; ++i) {
        inst_memory_set_inst(&(m->im), 0x04000000 + 4 * i, inst[i]);
    }
}

void test_mips()
{
    MIPS m;
    int inst[] = {0x012a4020, 0x012a4022, 0x012a4024, 0x012a4025, 0x012a402a};
    mips_init(&m, inst, 5);
    mips_run(&m, 5);
    
}
