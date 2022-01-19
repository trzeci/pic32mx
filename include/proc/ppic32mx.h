#if !defined(__PPIC32MX_H__)
#define __PPIC32MX_H__


#ifndef __PIC32MX__
#  define __PIC32MX__ 1
#endif
#ifndef __PIC32_HAS_MIPS32R2
# define __PIC32_HAS_MIPS32R2 1
#endif
#ifndef __PIC32_HAS_MIPS16
# define __PIC32_HAS_MIPS16 1
#endif
#ifndef __PIC32_HAS_INIT_DATA
# define __PIC32_HAS_INIT_DATA 1
#endif
#ifndef __PIC32_SRS_SET_COUNT
# define __PIC32_SRS_SET_COUNT 2
#endif


/* FLASH Controller definitions */
#define NVMCON_NVMOP           	0x0000000f
#define NVMCON_ERASE            0x00000040
#define NVMCON_WRERR            0x00002000
#define NVMCON_WREN             0x00004000
#define NVMCON_WR               0x00008000

#define NVMCON_NVMOP0           0x00000001
#define NVMCON_NVMOP1           0x00000002
#define NVMCON_NVMOP2           0x00000004
#define NVMCON_NVMOP3           0x00000008

#define NVMCON_PROGOP           0x0000000F

#define NVMCON_PROGOP0          0x00000001
#define NVMCON_PROGOP1          0x00000002
#define NVMCON_PROGOP2          0x00000004
#define NVMCON_PROGOP3          0x00000008

/* System-wide IRQ numbers
 * Device-specific definitions now appear in processor header
 * files.
 */
#ifndef _CORE_TIMER_IRQ
#define _CORE_TIMER_IRQ                       0
#define _CORE_SOFTWARE_0_IRQ                  1
#define _CORE_SOFTWARE_1_IRQ                  2
#define _EXTERNAL_0_IRQ                       3
#define _TIMER_1_IRQ                          4
#define _INPUT_CAPTURE_1_IRQ                  5
#define _OUTPUT_COMPARE_1_IRQ                 6
#define _EXTERNAL_1_IRQ                       7
#define _TIMER_2_IRQ                          8
#define _INPUT_CAPTURE_2_IRQ                  9
#define _OUTPUT_COMPARE_2_IRQ                 10
#define _EXTERNAL_2_IRQ                       11
#define _TIMER_3_IRQ                          12
#define _INPUT_CAPTURE_3_IRQ                  13
#define _OUTPUT_COMPARE_3_IRQ                 14
#define _EXTERNAL_3_IRQ                       15
#define _TIMER_4_IRQ                          16
#define _INPUT_CAPTURE_4_IRQ                  17
#define _OUTPUT_COMPARE_4_IRQ                 18
#define _EXTERNAL_4_IRQ                       19
#define _TIMER_5_IRQ                          20
#define _INPUT_CAPTURE_5_IRQ                  21
#define _OUTPUT_COMPARE_5_IRQ                 22
#define _SPI1_ERR_IRQ                         23
#define _SPI1_TX_IRQ                          24
#define _SPI1_RX_IRQ                          25
#define _UART1_ERR_IRQ                        26
#define _UART1_RX_IRQ                         27
#define _UART1_TX_IRQ                         28
#define _I2C1_BUS_IRQ                         29
#define _I2C1_SLAVE_IRQ                       30
#define _I2C1_MASTER_IRQ                      31
#define _CHANGE_NOTICE_IRQ                    32
#define _ADC_IRQ                              33
#define _PMP_IRQ                              34
#define _COMPARATOR_1_IRQ                     35
#define _COMPARATOR_2_IRQ                     36
#define _SPI2_ERR_IRQ                         37
#define _SPI2_TX_IRQ                          38
#define _SPI2_RX_IRQ                          39
#define _UART2_ERR_IRQ                        40
#define _UART2_RX_IRQ                         41
#define _UART2_TX_IRQ                         42
#define _I2C2_BUS_IRQ                         43
#define _I2C2_SLAVE_IRQ                       44
#define _I2C2_MASTER_IRQ                      45
#define _FAIL_SAFE_MONITOR_IRQ                46
#define _RTCC_IRQ                             47
#define _DMA0_IRQ                             48
#define _DMA1_IRQ                             49
#define _DMA2_IRQ                             50
#define _DMA3_IRQ                             51
#define _FLASH_CONTROL_IRQ                    56
#endif /* _CORE_TIMER_IRQ */


#endif
