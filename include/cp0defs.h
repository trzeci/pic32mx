#pragma once

#ifndef __CP0DEFS__H__
#define __CP0DEFS__H__

#ifdef __LANGUAGE_ASSEMBLY__

#define _CP0_INDEX                             $0, 0
#define _CP0_ENTRYLO0                          $2, 0
#define _CP0_TLBLO0                            $2, 0
#define _CP0_ENTRYLO1                          $3, 0
#define _CP0_PAGEMASK                          $5, 0
#define _CP0_WIRED                             $6, 0
#define _CP0_COUNT                             $9, 0
#define _CP0_ENTRYHI                           $10, 0
#define _CP0_COMPARE                           $11, 0
#define _CP0_STATUS                            $12, 0
#define _CP0_INTCTL                            $12, 1
#define _CP0_CAUSE                             $13, 0
#define _CP0_EPC                               $14, 0
#define _CP0_EBASE                             $15, 1
#define _CP0_CONFIG                            $16, 0
#define _CP0_CONFIG1                           $16, 1

#else

#define _CP0_PAGEMASK                          5
#define _CP0_PAGEMASK_SELECT                   0
#define _CP0_COUNT                             9
#define _CP0_COUNT_SELECT                      0
#define _CP0_COMPARE                           11
#define _CP0_COMPARE_SELECT                    0
#define _CP0_STATUS                            12
#define _CP0_STATUS_SELECT                     0
#define _CP0_INTCTL                            12
#define _CP0_INTCTL_SELECT                     1
#define _CP0_CAUSE                             13
#define _CP0_CAUSE_SELECT                      0
#define _CP0_EPC                               14
#define _CP0_EPC_SELECT                        0
#define _CP0_EBASE                             15
#define _CP0_EBASE_SELECT                      1

#define _CP0_GET_COMPARE()     _mfc0 (_CP0_COMPARE, _CP0_COMPARE_SELECT)
#define _CP0_SET_COMPARE(val)  _mtc0 (_CP0_COMPARE, _CP0_COMPARE_SELECT, val)

#define _CP0_GET_PAGEMASK()       _mfc0 (_CP0_PAGEMASK, _CP0_PAGEMASK_SELECT)
#define _CP0_SET_PAGEMASK(val)    _mtc0 (_CP0_PAGEMASK, _CP0_PAGEMASK_SELECT, val)

#define _CP0_GET_STATUS()      _mfc0 (_CP0_STATUS, _CP0_STATUS_SELECT)
#define _CP0_SET_STATUS(val)   _mtc0 (_CP0_STATUS, _CP0_STATUS_SELECT, val)
#define _CP0_XCH_STATUS(val)   _mxc0 (_CP0_STATUS, _CP0_STATUS_SELECT, val)
#define _CP0_BIC_STATUS(clr)   _bcc0 (_CP0_STATUS, _CP0_STATUS_SELECT, clr)
#define _CP0_BIS_STATUS(set)   _bsc0 (_CP0_STATUS, _CP0_STATUS_SELECT, set)
#define _CP0_BCS_STATUS(c,s)   _bcsc0(_CP0_STATUS, _CP0_STATUS_SELECT, c, s)

#define _CP0_GET_INTCTL()      _mfc0 (_CP0_INTCTL, _CP0_INTCTL_SELECT)
#define _CP0_SET_INTCTL(val)   _mtc0 (_CP0_INTCTL, _CP0_INTCTL_SELECT, val)
#define _CP0_XCH_INTCTL(val)   _mxc0 (_CP0_INTCTL, _CP0_INTCTL_SELECT, val)

#define _CP0_GET_CAUSE()       _mfc0 (_CP0_CAUSE, _CP0_CAUSE_SELECT)
#define _CP0_SET_CAUSE(val)    _mtc0 (_CP0_CAUSE, _CP0_CAUSE_SELECT, val)
#define _CP0_XCH_CAUSE(val)    _mxc0 (_CP0_CAUSE, _CP0_CAUSE_SELECT, val)
#define _CP0_BIC_CAUSE(clr)    _bcc0 (_CP0_CAUSE, _CP0_CAUSE_SELECT, clr)
#define _CP0_BIS_CAUSE(set)    _bsc0 (_CP0_CAUSE, _CP0_CAUSE_SELECT, set)
#define _CP0_BCS_CAUSE(c,s)    _bcsc0(_CP0_CAUSE, _CP0_CAUSE_SELECT, c, s)

#define _CP0_GET_EBASE()       _mfc0 (_CP0_EBASE, _CP0_EBASE_SELECT)
#define _CP0_SET_EBASE(val)    _mtc0 (_CP0_EBASE, _CP0_EBASE_SELECT, val)
#define _CP0_XCH_EBASE(val)    _mxc0 (_CP0_EBASE, _CP0_EBASE_SELECT, val)

#endif

#ifdef __cplusplus
extern "C" {
#endif

#define _CP0_STATUS_IE_POSITION                0x00000000
#define _CP0_STATUS_IE_MASK                    0x00000001
#define _CP0_STATUS_IE_LENGTH                  0x00000001

#define _CP0_STATUS_EXL_POSITION               0x00000001
#define _CP0_STATUS_EXL_MASK                   0x00000002
#define _CP0_STATUS_EXL_LENGTH                 0x00000001

#define _CP0_STATUS_ERL_POSITION               0x00000002
#define _CP0_STATUS_ERL_MASK                   0x00000004
#define _CP0_STATUS_ERL_LENGTH                 0x00000001

#define _CP0_STATUS_UM_POSITION                0x00000004
#define _CP0_STATUS_UM_MASK                    0x00000010
#define _CP0_STATUS_UM_LENGTH                  0x00000001

#define _CP0_STATUS_IM0_POSITION               0x00000008
#define _CP0_STATUS_IM0_MASK                   0x00000100
#define _CP0_STATUS_IM0_LENGTH                 0x00000001

#define _CP0_STATUS_IM1_POSITION               0x00000009
#define _CP0_STATUS_IM1_MASK                   0x00000200
#define _CP0_STATUS_IM1_LENGTH                 0x00000001

#define _CP0_STATUS_IPL_POSITION               0x0000000A
#define _CP0_STATUS_IPL_MASK                   0x0000FC00
#define _CP0_STATUS_IPL_LENGTH                 0x00000006

#define _CP0_STATUS_IM2_POSITION               0x0000000A
#define _CP0_STATUS_IM2_MASK                   0x00000400
#define _CP0_STATUS_IM2_LENGTH                 0x00000001

#define _CP0_STATUS_IM3_POSITION               0x0000000B
#define _CP0_STATUS_IM3_MASK                   0x00000800
#define _CP0_STATUS_IM3_LENGTH                 0x00000001

#define _CP0_STATUS_IM4_POSITION               0x0000000C
#define _CP0_STATUS_IM4_MASK                   0x00001000
#define _CP0_STATUS_IM4_LENGTH                 0x00000001

#define _CP0_STATUS_IM5_POSITION               0x0000000D
#define _CP0_STATUS_IM5_MASK                   0x00002000
#define _CP0_STATUS_IM5_LENGTH                 0x00000001

#define _CP0_STATUS_IM6_POSITION               0x0000000E
#define _CP0_STATUS_IM6_MASK                   0x00004000
#define _CP0_STATUS_IM6_LENGTH                 0x00000001

#define _CP0_STATUS_IM7_POSITION               0x0000000F
#define _CP0_STATUS_IM7_MASK                   0x00008000
#define _CP0_STATUS_IM7_LENGTH                 0x00000001

#define _CP0_STATUS_CEE_POSITION               0x00000011
#define _CP0_STATUS_CEE_MASK                   0x00020000
#define _CP0_STATUS_CEE_LENGTH                 0x00000001

#define _CP0_STATUS_NMI_POSITION               0x00000013
#define _CP0_STATUS_NMI_MASK                   0x00080000
#define _CP0_STATUS_NMI_LENGTH                 0x00000001

#define _CPO_STATUS_SR_POSITION                0x00000014
#define _CP0_STATUS_SR_MASK                    0x00100000
#define _CP0_STATUS_SR_LENGTH                  0x00000001

/* TLB Shutdown */
#define _CP0_STATUS_TS_POSITION                0x00000015
#define _CP0_STATUS_TS_MASK                    0x00200000
#define _CP0_STATUS_TS_LENGTH                  0x00000001

#define _CP0_STATUS_BEV_POSITION               0x00000016
#define _CP0_STATUS_BEV_MASK                   0x00400000
#define _CP0_STATUS_BEV_LENGTH                 0x00000001

/* enable MDMX/DSP ASE */
#define _CP0_STATUS_MX_POSITION                0x00000018
#define _CP0_STATUS_MX_MASK                    0x01000000
#define _CP0_STATUS_MX_LENGTH                  0x00000001

#define _CP0_STATUS_RE_POSITION                0x00000019
#define _CP0_STATUS_RE_MASK                    0x02000000
#define _CP0_STATUS_RE_LENGTH                  0x00000001

#define _CP0_STATUS_FR_POSITION                0x0000001A
#define _CP0_STATUS_FR_MASK                    0x04000000
#define _CP0_STATUS_FR_LENGTH                  0x00000001

#define _CP0_STATUS_RP_POSITION                0x0000001B
#define _CP0_STATUS_RP_MASK                    0x08000000
#define _CP0_STATUS_RP_LENGTH                  0x00000001

#define _CP0_STATUS_CU0_POSITION               0x0000001C
#define _CP0_STATUS_CU0_MASK                   0x10000000
#define _CP0_STATUS_CU0_LENGTH                 0x00000001

#define _CP0_STATUS_CU1_POSITION               0x0000001D
#define _CP0_STATUS_CU1_MASK                   0x20000000
#define _CP0_STATUS_CU1_LENGTH                 0x00000001

#define _CP0_STATUS_CU2_POSITION               0x0000001E
#define _CP0_STATUS_CU2_MASK                   0x40000000
#define _CP0_STATUS_CU2_LENGTH                 0x00000001

#define _CP0_STATUS_CU3_POSITION               0x0000001F
#define _CP0_STATUS_CU3_MASK                   0x80000000
#define _CP0_STATUS_CU3_LENGTH                 0x00000001

#define _CP0_INTCTL_VS_POSITION                0x00000005
#define _CP0_INTCTL_VS_MASK                    0x000003E0
#define _CP0_INTCTL_VS_LENGTH                  0x00000005

#define _CP0_INTCTL_IPPCI_POSITION             0x0000001A
#define _CP0_INTCTL_IPPCI_MASK                 0x1C000000
#define _CP0_INTCTL_IPPCI_LENGTH               0x00000003

#define _CP0_INTCTL_IPTI_POSITION              0x0000001D
#define _CP0_INTCTL_IPTI_MASK                  0xE0000000
#define _CP0_INTCTL_IPTI_LENGTH                0x00000003

#define _CP0_CAUSE_EXCCODE_POSITION            0x00000002
#define _CP0_CAUSE_EXCCODE_MASK                0x0000007C
#define _CP0_CAUSE_EXCCODE_LENGTH              0x00000005

#define _CP0_CAUSE_IP0_POSITION                0x00000008
#define _CP0_CAUSE_IP0_MASK                    0x00000100
#define _CP0_CAUSE_IP0_LENGTH                  0x00000001

#define _CP0_CAUSE_IP1_POSITION                0x00000009
#define _CP0_CAUSE_IP1_MASK                    0x00000200
#define _CP0_CAUSE_IP1_LENGTH                  0x00000001

#define _CP0_CAUSE_RIPL_POSITION               0x0000000A
#define _CP0_CAUSE_RIPL_MASK                   0x0000FC00
#define _CP0_CAUSE_RIPL_LENGTH                 0x00000006

#define _CP0_CAUSE_IP2_POSITION                0x0000000A
#define _CP0_CAUSE_IP2_MASK                    0x00000400
#define _CP0_CAUSE_IP2_LENGTH                  0x00000001

#define _CP0_CAUSE_IP3_POSITION                0x0000000B
#define _CP0_CAUSE_IP3_MASK                    0x00000800
#define _CP0_CAUSE_IP3_LENGTH                  0x00000001

#define _CP0_CAUSE_IP4_POSITION                0x0000000C
#define _CP0_CAUSE_IP4_MASK                    0x00001000
#define _CP0_CAUSE_IP4_LENGTH                  0x00000001

#define _CP0_CAUSE_IP5_POSITION                0x0000000D
#define _CP0_CAUSE_IP5_MASK                    0x00002000
#define _CP0_CAUSE_IP5_LENGTH                  0x00000001

#define _CP0_CAUSE_IP6_POSITION                0x0000000E
#define _CP0_CAUSE_IP6_MASK                    0x00004000
#define _CP0_CAUSE_IP6_LENGTH                  0x00000001

#define _CP0_CAUSE_IP7_POSITION                0x0000000F
#define _CP0_CAUSE_IP7_MASK                    0x00008000
#define _CP0_CAUSE_IP7_LENGTH                  0x00000001

#define _CP0_CAUSE_WP_POSITION                 0x00000016
#define _CP0_CAUSE_WP_MASK                     0x00400000
#define _CP0_CAUSE_WP_LENGTH                   0x00000001

#define _CP0_CAUSE_IV_POSITION                 0x00000017
#define _CP0_CAUSE_IV_MASK                     0x00800000
#define _CP0_CAUSE_IV_LENGTH                   0x00000001

#define _CP0_CAUSE_PCI_POSITION                0x0000001A
#define _CP0_CAUSE_PCI_MASK                    0x04000000
#define _CP0_CAUSE_PCI_LENGTH                  0x00000001

#define _CP0_CAUSE_DC_POSITION                 0x0000001B
#define _CP0_CAUSE_DC_MASK                     0x08000000
#define _CP0_CAUSE_DC_LENGTH                   0x00000001

#define _CP0_CAUSE_CE_POSITION                 0x0000001C
#define _CP0_CAUSE_CE_MASK                     0x30000000
#define _CP0_CAUSE_CE_LENGTH                   0x00000002

#define _CP0_CAUSE_TI_POSITION                 0x0000001E
#define _CP0_CAUSE_TI_MASK                     0x40000000
#define _CP0_CAUSE_TI_LENGTH                   0x00000001

#define _CP0_CAUSE_BD_POSITION                 0x0000001F
#define _CP0_CAUSE_BD_MASK                     0x80000000
#define _CP0_CAUSE_BD_LENGTH                   0x00000001

#define _CP0_EPC_ALL_POSITION                  0x00000000
#define _CP0_EPC_ALL_MASK                      0xFFFFFFFF
#define _CP0_EPC_ALL_LENGTH                    0x00000020

#define _CP0_EBASE_CPUNUM_POSITION             0x00000000
#define _CP0_EBASE_CPUNUM_MASK                 0x000003FF
#define _CP0_EBASE_CPUNUM_LENGTH               0x0000000A

#define _CP0_EBASE_EBASE_POSITION              0x0000000C
#define _CP0_EBASE_EBASE_MASK                  0x3FFFF000
#define _CP0_EBASE_EBASE_LENGTH                0x0000000E

/* Kseg0 coherency algorithm */
#define _CP0_CONFIG_K0_POSITION                0x00000000
#define _CP0_CONFIG_K0_MASK                    0x00000007
#define _CP0_CONFIG_K0_LENGTH                  0x00000003

/* MMU Type */
#define _CP0_CONFIG_MT_POSITION                0x00000007
#define _CP0_CONFIG_MT_MASK                    0x00000380
#define _CP0_CONFIG_MT_LENGTH                  0x00000003
#define   _CP0_CONFIG_MT_NONE                  (0<<7)
#define   _CP0_CONFIG_MT_TLB                   (1<<7)
#define   _CP0_CONFIG_MT_BAT                   (2<<7)
#define   _CP0_CONFIG_MT_NONSTD                (3<<7)

#define _CP0_CONFIG_AR_POSITION                0x0000000A
#define _CP0_CONFIG_AR_MASK                    0x00001C00
#define _CP0_CONFIG_AR_LENGTH                  0x00000003

#define _CP0_CONFIG_AT_POSITION                0x0000000D
#define _CP0_CONFIG_AT_MASK                    0x00006000
#define _CP0_CONFIG_AT_LENGTH                  0x00000002

#define _CP0_CONFIG_BE_POSITION                0x0000000F
#define _CP0_CONFIG_BE_MASK                    0x00008000
#define _CP0_CONFIG_BE_LENGTH                  0x00000001

#define _CP0_CONFIG_DS_POSITION                0x00000010
#define _CP0_CONFIG_DS_MASK                    0x00010000
#define _CP0_CONFIG_DS_LENGTH                  0x00000001

#define _CP0_CONFIG_MDU_POSITION               0x00000014
#define _CP0_CONFIG_MDU_MASK                   0x00100000
#define _CP0_CONFIG_MDU_LENGTH                 0x00000001

#define _CP0_CONFIG_SB_POSITION                0x00000015
#define _CP0_CONFIG_SB_MASK                    0x00200000
#define _CP0_CONFIG_SB_LENGTH                  0x00000001

#define _CP0_CONFIG_UDI_POSITION               0x00000016
#define _CP0_CONFIG_UDI_MASK                   0x00400000
#define _CP0_CONFIG_UDI_LENGTH                 0x00000001

#define _CP0_CONFIG_KU_POSITION                0x00000019
#define _CP0_CONFIG_KU_MASK                    0x70000000
#define _CP0_CONFIG_KU_LENGTH                  0x00000003

#define _CP0_CONFIG_M_POSITION                 0x0000001F
#define _CP0_CONFIG_M_MASK                     0x80000000
#define _CP0_CONFIG_M_LENGTH                   0x00000001

#define _CP0_CONFIG1_FP_POSITION               0x00000000
#define _CP0_CONFIG1_FP_MASK                   0x00000001
#define _CP0_CONFIG1_FP_LENGTH                 0x00000001

#define _CP0_CONFIG1_EP_POSITION               0x00000001
#define _CP0_CONFIG1_EP_MASK                   0x00000002
#define _CP0_CONFIG1_EP_LENGTH                 0x00000001

#define _CP0_CONFIG1_CA_POSITION               0x00000002
#define _CP0_CONFIG1_CA_MASK                   0x00000004
#define _CP0_CONFIG1_CA_LENGTH                 0x00000001

#define _CP0_CONFIG1_WR_POSITION               0x00000003
#define _CP0_CONFIG1_WR_MASK                   0x00000008
#define _CP0_CONFIG1_WR_LENGTH                 0x00000001

#define _CP0_CONFIG1_PC_POSITION               0x00000004
#define _CP0_CONFIG1_PC_MASK                   0x00000010
#define _CP0_CONFIG1_PC_LENGTH                 0x00000001

#define _CP0_CONFIG1_MD_POSITION               0x00000005
#define _CP0_CONFIG1_MD_MASK                   0x00000020
#define _CP0_CONFIG1_MD_LENGTH                 0x00000001

#define _CP0_CONFIG1_C2_POSITION               0x00000006
#define _CP0_CONFIG1_C2_MASK                   0x00000040
#define _CP0_CONFIG1_C2_LENGTH                 0x00000001

#define _CP0_CONFIG1_DA_POSITION               0x00000007
#define _CP0_CONFIG1_DA_MASK                   0x00000380
#define _CP0_CONFIG1_DA_LENGTH                 0x00000003

#define _CP0_CONFIG1_DL_POSITION               0x0000000A
#define _CP0_CONFIG1_DL_MASK                   0x00001C00
#define _CP0_CONFIG1_DL_LENGTH                 0x00000003

#define _CP0_CONFIG1_DS_POSITION               0x0000000D
#define _CP0_CONFIG1_DS_MASK                   0x0000E000
#define _CP0_CONFIG1_DS_LENGTH                 0x00000003

#define _CP0_CONFIG1_IA_POSITION               0x00000010
#define _CP0_CONFIG1_IA_MASK                   0x00070000
#define _CP0_CONFIG1_IA_LENGTH                 0x00000003

#define _CP0_CONFIG1_IL_POSITION               0x00000013
#define _CP0_CONFIG1_IL_MASK                   0x00380000
#define _CP0_CONFIG1_IL_LENGTH                 0x00000003

#define _CP0_CONFIG1_IS_POSITION               0x00000016
#define _CP0_CONFIG1_IS_MASK                   0x01C00000
#define _CP0_CONFIG1_IS_LENGTH                 0x00000003

#define _CP0_CONFIG1_MMUSIZE_POSITION          0x00000019
#define _CP0_CONFIG1_MMUSIZE_MASK              0x7E000000
#define _CP0_CONFIG1_MMUSIZE_LENGTH            0x00000006

#define _CP0_CONFIG1_M_POSITION                0x0000001F
#define _CP0_CONFIG1_M_MASK                    0x80000000
#define _CP0_CONFIG1_M_LENGTH                  0x00000001

#ifdef __cplusplus
}
#endif

#endif
