#include <stdio.h>
#include <mips.h>

void inst_memory_run(InstMemory *im, Word addr, Word *inst)
{
    int val, offset;
    val = word_get_value(addr);
    offset = (int)(val - INST_MEM_START) / 4;
    word_set_value(inst, im->mem[offset]);
}

void inst_memory_set_inst(InstMemory *im, int addr, int inst)
{
    int offset;
    offset = (int)(addr - INST_MEM_START) / 4;
    im->mem[offset] = inst;
}

void data_memory_run(DataMemory *dm, Signal mem_write, Signal mem_read, Word addr, Word wdata, Word *rdata)
{
    int val, offset;
    val = word_get_value(addr);
    offset = (val - DATA_MEM_START) / 4;
    // Read from mem[offset]
    if (mem_read) {
        word_set_value(rdata, dm->mem[offset]);
    }
    else if (mem_write) {
        dm->mem[offset] = word_get_value(wdata);
    }
}

void test_inst_memory()
{
    Word addr, inst;
    InstMemory im;

    word_set_value(&addr, 0x04000000);
    inst_memory_set_inst(&im, 0x04000000, 350);
    inst_memory_run(&im, addr, &inst);

    printf("InstMemory[0x04000000] = %d\n", word_get_value(inst));

}

void test_data_memory()
{
    DataMemory dm;
    Signal mem_write, mem_read;
    Word addr, wdata, rdata;
    
    word_set_value(&rdata, 0);
    mem_write = true;
    mem_read = false;

    word_set_value(&addr, 0x10000004);
    word_set_value(&wdata, 100);
    data_memory_run(&dm, mem_write, mem_read, addr, wdata, &rdata);

    word_set_value(&addr, 0x10000008);
    word_set_value(&wdata, 200);
    data_memory_run(&dm, mem_write, mem_read, addr, wdata, &rdata);

    printf("rdata : %d\n", word_get_value(rdata));

    mem_write = false;
    mem_read = true;
    data_memory_run(&dm, mem_write, mem_read, addr, wdata, &rdata);
    printf("rdata : %d\n", word_get_value(rdata));

    word_set_value(&addr, 0x10000004);
    data_memory_run(&dm, mem_write, mem_read, addr, wdata, &rdata);
    printf("rdata : %d\n", word_get_value(rdata));

}


