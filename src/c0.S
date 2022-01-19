#include <regdef.h>
#include <cp0defs.h>
#include <pic32m.h>

       .set nomips16
       .set nomicromips
       .section .reset,"ax",@progbits
       .set noreorder
       .align 4
       .ent _reset
_reset:
        jal _startup
        nop
        .align 2
        .end _reset
        .globl _reset
        .size _reset, .-_reset

        .section .reset.startup,"ax",@progbits
        .set noreorder
        .align 4
        .globl _startup
        .set noreorder
        .ent _startup
_startup:
        # If entered because of an NMI, jump to the NMI handler.
        mfc0    k0,_CP0_STATUS
        ext     k0,k0,19,1              # Extract NMI bit
        beqz    k0,_no_nmi
        nop
        la      k0,_nmi_handler
        jr      k0
        nop
_no_nmi:
        la      sp,_stack
        la      gp,_gp
        .extern __initializeCpu
        la      t0,__initializeCpu
        beq     t0,$0,7f
        nop
        jalr    t0
        nop
7:

        ##################################################################
        # Clear uninitialized data sections, heap and stack
        ##################################################################
_start_bss_init:
        la      t0,__bss_start
#        la      t1,_end
        b       _bss_check
        nop
_bss_init:
        sw      zero,0x0(t0)
        addu    t0,4
_bss_check:
        bltu    t0,sp,_bss_init
        nop

        ##################################################################
        # Copy initialized data sections
        ##################################################################
_start_data_init:
        la      t0,_fdata
        la      t1,_edata
        la      t2,_tdata
        b       _data_check
        nop
_data_init:
        lw      t3,0x0(t2)
        sw      t3,0x0(t0)
        addu    t0,4
        addu    t2,4
_data_check:
        bltu    t0,t1,_data_init
        nop

        # Initialize CP0 registers
        # Initialize Count register
        mtc0    zero,_CP0_COUNT
        # Initialize Compare register
        li      t2,-1
        mtc0    t2,_CP0_COMPARE
        # Ensure BEV set and Initialize EBase register
        li      t0, (1<<22)
        mfc0    t2,_CP0_STATUS
        or      t2,t0,t2               # Set BEV bit 22
        mtc0    t2,_CP0_STATUS

        la      t1,_ebase_address
        ehb
        mtc0    t1,_CP0_EBASE

        # DONT TOUCH PRISS, it is not your job
        # _on_bootstrap should set this

        # Initialize IntCtl register with vector spacing = 1
        # pic32mm has INTCON.VS, todo
        la      t1,1
        li      t2,0                    # Clear t2 and
        ins     t2,t1,5,5               # shift value to VS field
        mtc0    t2,_CP0_INTCTL

        ##################################################################
        # Initialize CAUSE registers
        # - Enable counting of Count register <DC = 0>
        # - Use special exception vector <IV = 1>
        # - Clear pending software interrupts <IP1:IP0 = 0>
        ##################################################################
        li      t1,0x00800000
        mtc0    t1,_CP0_CAUSE

        ##################################################################
        # Initialize STATUS register
        # - Access to Coprocessor 0 not allowed in user mode <CU0 = 0>
        # - User mode uses configured endianness <RE = 0>
        # - Preserve Bootstrap Exception vectors <BEV>
        # - Preserve soft reset <SR> and non-maskable interrupt <NMI>
        # - CorExtend enabled based on whether CorExtend User Defined
        #   Instructions have been implemented <CEE = Config<UDI>>
        # - Disable any pending interrupts <IM7..IM2 = 0, IM1..IM0 = 0>
        # - Disable hardware interrupts <IPL7:IPL2 = 0>
        # - Base mode is Kernel mode <UM = 0>
        # - Error level is normal <ERL = 0>
        # - Exception level is normal <EXL = 0>
        # - Interrupts are disabled <IE = 0>
        # - DSPr2 ASE is enabled for devices that support it <MX = 1>
        # - FPU64 is enabled for devices that support it <CU1=1> & <FR=1>
        ##################################################################
        mfc0    t0,_CP0_CONFIG
        ext     t1,t0,22,1              # Extract UDI from Config register
        sll     t1,t1,17                # Move UDI to Status.CEE location
        mfc0    t0,_CP0_STATUS
        and     t0,t0,0x00580000        # Preserve SR, NMI, and BEV

        or      t0,t1,t0                # Include Status.CEE (from UDI)
        mtc0    t0,_CP0_STATUS
        ehb

        # Initialize Status<BEV> for normal exception vectors
        mfc0    t0,_CP0_STATUS
        and     t0,t0,0xffbfffff        # Clear BEV
        mtc0    t0,_CP0_STATUS
        ehb

        # Call main
        and     a0,a0,0
        and     a1,a1,0
        la      t0,_main_entry
        jr      t0
        nop

        .end _startup


        # Boot Exception Vector Handler
        .section .bev_handler,"ax",@progbits
        .align 2
        .set noreorder
        .ent _bev_exception
_bev_exception:
        la        k0,_bootstrap_exception_handler
        jr        k0
        nop
        .end _bev_exception


        # GEH
        .section .gen_handler,"ax",@progbits
        .align 2
        .set noreorder
        .ent _gen_exception
        .extern _general_exception_handler
_gen_exception:
0:      la      k0,_general_exception_handler
        jr      k0
        nop
        .end _gen_exception


        .section .text.main_entry,"ax",@progbits
        .align 2
        .ent _main_entry
_main_entry:
        .weak _on_bootstrap
        la      t0,_on_bootstrap
        beq     t0,$0,7f
        nop
        jalr    t0
        nop
7:

        # preinit_array + _init + init_array
        .extern __init_array_init
        la      t0,__init_array_init
        jalr    t0
        nop

        and     a0,a0,0
        and     a1,a1,0
        # Call main
        la      t0,main
        jalr    t0
        nop

        .weak exit
        jal exit
        nop
        la      t0,exit
        beq     t0,$0,7f
        nop
        jalr    t0
        nop
7:

        # infinite loop
        .weak __exception_handler_break
__crt0_exit:
1:
        la      v0,__exception_handler_break
        beq     v0,0,0f
        nop
        jalr    v0
        nop
0:      b       1b
        nop
        .globl __crt0_exit
        .end _main_entry


# highest vector number in PIC32MX series is 51, 2kB for vectors table

/*  vector spacing assumed = 1, 0x20  */

  .section .vector_table,"ax",@progbits
  .set nomips16
  .set nomicromips
  .align 2
  .ent __vector_table

  .altmacro
  .macro vector_in_table from, to
    .weak __vector_\from
    __vector_\from = __vector_default
    j __vector_\from
#if __PIC32MZ__
    nop
#elif __PIC32MX__
    nop;nop;nop;nop;nop;nop;nop
#endif
    .if \to-\from
      vector_in_table %from+1,\to
    .endif
  .endm

__vector_table:
    vector_in_table 0, 63
__vector_default:
    j __vector_default
    nop

  .end __vector_table
  .size __vector_table, .-__vector_table

  .section .sdata
  .global __dso_handle
  .weak   __dso_handle
  .balign 4
__dso_handle:
  .long   0
  .global __stack_end
__stack_end:
  .long   _stack

  #.section .devcfg,"a"
  #.balign 4
  #.long   0xffffffff # DEVCFG3  Assign irq priority 7 to shadow set, Ethernet MII enabled, Default Ethernet i/o pins,  Default CAN i/o pins, USBID pin: controlled by USB, VBuson pin: controlled by USB
  #.long   0xfff8ffd9 # DEVCFG2  PLL divider: 1/2, PLL multiplier: 20x, USB PLL divider: 1/12, Disable USB PLL, PLL postscaler: 1/1
  #.long   0xff6cde5b # DEVCFG1  Primary oscillator with PLL, Primary oscillator: HS, CLKO output disabled, Peripheral bus clock: SYSCLK / 2, Fail-safe clock monitor disable, Clock switching disable, Watchdog postscale: 1/4096
  #.long   0x7fffffff # DEVCFG0  Debugger disabled
