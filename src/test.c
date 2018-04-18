#include <stdio.h>
#include <mips.h>

int main()
{
    /* Exercise 5-1 */
    test_gate();

    /* Exercise 5-2 */
    test_full_adder();

    /* Exercise 5-3 */
    test_rca(100, 200);

    /* Exercise 6-1 */
    test_alu();
    test_alu32(100, 200);

    /* Exercise 7-1 */
    test_register_file();
    test_inst_memory();
    test_data_memory();

    /* Exercise 8-1 */
    test_mips();
    
    return 0;
}
