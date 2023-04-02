OUTPUT_ARCH( "riscv" ) /* 定义输出文件的运行平台 */
ENTRY( _start )/* 定义入口，第一条指令 */
SECTIONS
{
  /* '.'表示当前地址，从0x80200000开始 */
  . = 0x80200000;
  start = .;

  .text : {
     stext = .;
        *(.text.entry)
        *(.text .text.*) /* 将输入文件中所有.text和.text.*的段放到该段 */
        . = ALIGN(4K); /* 4k对齐 */
        etext = .;
  }

  .data : {
        sdata = .;
        *(.data .data.*)
        edata = .;
  }

  .bss : {
        sbss = .;
        *(.bss .bss.*)
        ebss = .;
  }
  PROVIDE(end = .);/* 定义一个符号end,值为当前地址 */
}