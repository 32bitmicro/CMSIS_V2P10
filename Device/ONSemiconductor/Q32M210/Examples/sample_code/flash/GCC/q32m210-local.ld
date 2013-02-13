MEMORY
{
    FLASH_DATA (rx)   : ORIGIN = 0x00070000, LENGTH = 64K
}

SECTIONS
{
    .flashdata :
    {
        *(.flashdata)
    } > FLASH_DATA
}
