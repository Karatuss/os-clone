ENTRY(vector_start)
SECTIONS
{
    . = 0x0;  /* first section is located at 0x00000000 */


    .text :
    {
        *(vector_start)
        *(.text .rodata)
    }
    .data :
    {
        *(.data)
    }
    .bss :
    {
        *(.bss)
    }
}