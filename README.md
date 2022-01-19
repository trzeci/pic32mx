# pic32mx

This is a toolchain for PIC32MX5xx/6xx/7xx series of processors.
It consists of gcc toolchain for MIPS m4k CPU, C runtime startup files,
small C++ library for PIC32MX hardware.

The library covers 75% of processor hardware and is easy extensible.

Building:

- Create /opt/cross directory and place build-bleeding-edge-toolchain.sh there.
  The script is a modification of famous Freddie Chopin's script used to generate ARM toolchains.

- Run the script, it should create gcc toolchain in /opt/cross/mipsel-pic32mx-elf.
  The compiler is configured for -mips16 code generation with -Os and malloc_nano newlib.
  You may change it in OPTS="... at the beginning of the file.

- There are example configurations for 6 processors in ./include/proc/PIC32MX

- There is a processor configuration setup in ./src/devcfg.c for update (DEVCFG1/2/3 registers).
  The example setup is:
    8MHz crystal with PLL setup to Fcy = 80Mhz, Fpb = 40MHz,
    WDT off, USB off, no protection, no debug,
    SRS assigned to priority level 7.
  The file should be linked in to executable, if needed.
  There is no need for the file in bootloader environment,
  if the bootloader does the setup.

- Update ./cc_env.sh to a target processor, e.g.: PART=32MX575F512H
- Run ./cc.sh, it creates c0_MX.o and c0_MX.a C/C++ runtime startup files.
  The files are independent of the processor version.

- There are linker scripts for a few example processors.

- cd test && ./cc.sh
  It creates uploadable tests to processor using programmer.

  test00_blink    simply blinks LED, update .cc file with LED's port/pin numbers.
  test01_piclib   simple check of hardware library - it blinks LED,
                  but using C++ objects
  test02_uart     sets uart as stdout in libc, blinks LED and writes messages to uart
  test03_libc     prints using printf, stdout is configured using function called from c0.S bootstrap,
                  before anything created in C++. Configuration on LED and uart is in libc.cc .
  test04_stl      test of STL: std::map, virtuals, dynamic new/delete, std::cout
  test05_exc      test of exceptions handling
  test06_fpu      check of soft-float library
  test07_isr      test of ISR using Timer1

- whole hardware library is enclosed in files:
  pic_hardware.h
  pic_hardware.cc
  pic_hardware_asm.S
  core_timer.h
  core_timer.cc

Plans:
- the toolchain works with FreeRTOS 10.3, will upload after cleaning
- simplest possible bootloader on UART
- classes to manage I2C
- CAN
- port to PIC32MZ series

Enjoy!

