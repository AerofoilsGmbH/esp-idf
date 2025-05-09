/**
 * SPDX-FileCopyrightText: 2024-2025 Espressif Systems (Shanghai) CO LTD
 *
 *  SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

/** Group: configuration register */
/** Type of strap register
 *  pad strapping register
 */
typedef union {
    struct {
        /** strapping : RO; bitpos: [15:0]; default: 0;
         *  pad strapping register
         */
        uint32_t strapping:16;
        uint32_t reserved_16:16;
    };
    uint32_t val;
} gpio_strap_reg_t;

/** Type of out register
 *  GPIO output register for GPIO0-29
 */
typedef union {
    struct {
        /** out_data_orig : R/W/SC/WTC; bitpos: [29:0]; default: 0;
         *  GPIO output register for GPIO0-29
         */
        uint32_t out_data_orig:30;
        uint32_t reserved_30:2;
    };
    uint32_t val;
} gpio_out_reg_t;

/** Type of out_w1ts register
 *  GPIO output set register for GPIO0-29
 */
typedef union {
    struct {
        /** out_w1ts : WT; bitpos: [29:0]; default: 0;
         *  GPIO output set register for GPIO0-29
         */
        uint32_t out_w1ts:30;
        uint32_t reserved_30:2;
    };
    uint32_t val;
} gpio_out_w1ts_reg_t;

/** Type of out_w1tc register
 *  GPIO output clear register for GPIO0-29
 */
typedef union {
    struct {
        /** out_w1tc : WT; bitpos: [29:0]; default: 0;
         *  GPIO output clear register for GPIO0-29
         */
        uint32_t out_w1tc:30;
        uint32_t reserved_30:2;
    };
    uint32_t val;
} gpio_out_w1tc_reg_t;

/** Type of enable register
 *  GPIO output enable register for GPIO0-29
 */
typedef union {
    struct {
        /** enable_data : R/W/WTC; bitpos: [29:0]; default: 0;
         *  GPIO output enable register for GPIO0-29
         */
        uint32_t enable_data:30;
        uint32_t reserved_30:2;
    };
    uint32_t val;
} gpio_enable_reg_t;

/** Type of enable_w1ts register
 *  GPIO output enable set register for GPIO0-29
 */
typedef union {
    struct {
        /** enable_w1ts : WT; bitpos: [29:0]; default: 0;
         *  GPIO output enable set register for GPIO0-29
         */
        uint32_t enable_w1ts:30;
        uint32_t reserved_30:2;
    };
    uint32_t val;
} gpio_enable_w1ts_reg_t;

/** Type of enable_w1tc register
 *  GPIO output enable clear register for GPIO0-29
 */
typedef union {
    struct {
        /** enable_w1tc : WT; bitpos: [29:0]; default: 0;
         *  GPIO output enable clear register for GPIO0-29
         */
        uint32_t enable_w1tc:30;
        uint32_t reserved_30:2;
    };
    uint32_t val;
} gpio_enable_w1tc_reg_t;

/** Type of in register
 *  GPIO input register for GPIO0-29
 */
typedef union {
    struct {
        /** in_data_next : RO; bitpos: [29:0]; default: 0;
         *  GPIO input register for GPIO0-29
         */
        uint32_t in_data_next:30;
        uint32_t reserved_30:2;
    };
    uint32_t val;
} gpio_in_reg_t;

/** Type of status register
 *  GPIO interrupt status register for GPIO0-29
 */
typedef union {
    struct {
        /** status_interrupt : R/W/WTC; bitpos: [29:0]; default: 0;
         *  GPIO interrupt status register for GPIO0-29
         */
        uint32_t status_interrupt:30;
        uint32_t reserved_30:2;
    };
    uint32_t val;
} gpio_status_reg_t;

/** Type of status_w1ts register
 *  GPIO interrupt status set register for GPIO0-29
 */
typedef union {
    struct {
        /** status_w1ts : WT; bitpos: [29:0]; default: 0;
         *  GPIO interrupt status set register for GPIO0-29
         */
        uint32_t status_w1ts:30;
        uint32_t reserved_30:2;
    };
    uint32_t val;
} gpio_status_w1ts_reg_t;

/** Type of status_w1tc register
 *  GPIO interrupt status clear register for GPIO0-29
 */
typedef union {
    struct {
        /** status_w1tc : WT; bitpos: [29:0]; default: 0;
         *  GPIO interrupt status clear register for GPIO0-29
         */
        uint32_t status_w1tc:30;
        uint32_t reserved_30:2;
    };
    uint32_t val;
} gpio_status_w1tc_reg_t;

/** Type of procpu_int register
 *  GPIO_PROCPU_INT interrupt status register for GPIO0-29
 */
typedef union {
    struct {
        /** procpu_int : RO; bitpos: [29:0]; default: 0;
         *  GPIO_PROCPU_INT interrupt status register for GPIO0-29
         */
        uint32_t procpu_int:30;
        uint32_t reserved_30:2;
    };
    uint32_t val;
} gpio_procpu_int_reg_t;

/** Type of procpu_nmi_int register
 *  GPIO_PROCPU_NMI_INT interrupt status register for GPIO0-29
 */
typedef union {
    struct {
        /** procpu_nmi_int : RO; bitpos: [29:0]; default: 0;
         *  GPIO_PROCPU_NMI_INT interrupt status register for GPIO0-29
         */
        uint32_t procpu_nmi_int:30;
        uint32_t reserved_30:2;
    };
    uint32_t val;
} gpio_procpu_nmi_int_reg_t;

/** Type of status_next register
 *  GPIO interrupt source register for GPIO0-29
 */
typedef union {
    struct {
        /** status_interrupt_next : RO; bitpos: [29:0]; default: 0;
         *  GPIO interrupt source register for GPIO0-29
         */
        uint32_t status_interrupt_next:30;
        uint32_t reserved_30:2;
    };
    uint32_t val;
} gpio_status_next_reg_t;

/** Type of pinn register
 *  GPIO pin configuration register
 */
typedef union {
    struct {
        /** pinn_sync2_bypass : R/W; bitpos: [1:0]; default: 0;
         *  set GPIO input_sync2 signal mode. 0:disable. 1:trigger at negedge. 2or3:trigger at
         *  posedge.
         */
        uint32_t pinn_sync2_bypass:2;
        /** pinn_pad_driver : R/W; bitpos: [2]; default: 0;
         *  set this bit to select pad driver. 1:open-drain. 0:normal.
         */
        uint32_t pinn_pad_driver:1;
        /** pinn_sync1_bypass : R/W; bitpos: [4:3]; default: 0;
         *  set GPIO input_sync1 signal mode. 0:disable. 1:trigger at negedge. 2or3:trigger at
         *  posedge.
         */
        uint32_t pinn_sync1_bypass:2;
        uint32_t reserved_5:2;
        /** pinn_int_type : R/W; bitpos: [9:7]; default: 0;
         *  set this value to choose interrupt mode. 0:disable GPIO interrupt. 1:trigger at
         *  posedge. 2:trigger at negedge. 3:trigger at any edge. 4:valid at low level. 5:valid
         *  at high level
         */
        uint32_t pinn_int_type:3;
        /** pinn_wakeup_enable : R/W; bitpos: [10]; default: 0;
         *  set this bit to enable GPIO wakeup.(can only wakeup CPU from Light-sleep Mode)
         */
        uint32_t pinn_wakeup_enable:1;
        uint32_t reserved_11:2;
        /** pinn_int_ena : R/W; bitpos: [17:13]; default: 0;
         *  set bit 13 to enable gpio_procpu_int. set bit 14 to enable gpio_procpu_nmi_int.
         */
        uint32_t pinn_int_ena:5;
        uint32_t reserved_18:14;
    };
    uint32_t val;
} gpio_pinn_reg_t;

/** Group: Input Configuration Registers */
/** Type of func0_in_sel_cfg register
 *  GPIO input function configuration register
 */
typedef union {
    struct {
        /** func0_in_sel : R/W; bitpos: [5:0]; default: 48;
         *  set this value: s=0-29: connect GPIO[s] to this port. s=0x20: set this port always
         *  high level. s=0x30: set this port always low level.
         */
        uint32_t func_in_sel:6;
        /** func0_in_inv_sel : R/W; bitpos: [6]; default: 0;
         *  set this bit to invert input signal. 1:invert. 0:not invert.
         */
        uint32_t func_in_inv_sel:1;
        /** sig0_in_sel : R/W; bitpos: [7]; default: 0;
         *  set this bit to bypass GPIO. 1:do not bypass GPIO. 0:bypass GPIO.
         */
        uint32_t sig_in_sel:1;
        uint32_t reserved_8:24;
    };
    uint32_t val;
} gpio_func_in_sel_cfg_reg_t;

/** Type of funcn_out_sel_cfg register
 *  GPIO output function select register
 */
typedef union {
    struct {
        /** funcn_out_sel : R/W/SC; bitpos: [8:0]; default: 256;
         *  The value of the bits: 0<=s<=256. Set the value to select output signal. s=0-127:
         *  output of GPIO[n] equals input of peripheral[s]. s=128: output of GPIO[n] equals
         *  GPIO_OUT_REG[n].
         */
        uint32_t funcn_out_sel:9;
        /** funcn_out_inv_sel : R/W/SC; bitpos: [9]; default: 0;
         *  set this bit to invert output signal.1:invert.0:not invert.
         */
        uint32_t funcn_out_inv_sel:1;
        /** funcn_oe_sel : R/W; bitpos: [10]; default: 0;
         *  set this bit to select output enable signal.1:use GPIO_ENABLE_REG[n] as output
         *  enable signal.0:use peripheral output enable signal.
         */
        uint32_t funcn_oe_sel:1;
        /** funcn_oe_inv_sel : R/W; bitpos: [11]; default: 0;
         *  set this bit to invert output enable signal.1:invert.0:not invert.
         */
        uint32_t funcn_oe_inv_sel:1;
        uint32_t reserved_12:20;
    };
    uint32_t val;
} gpio_funcn_out_sel_cfg_reg_t;

/** Type of clock_gate register
 *  GPIO clock gate register
 */
typedef union {
    struct {
        /** clk_en : R/W; bitpos: [0]; default: 1;
         *  set this bit to enable GPIO clock gate
         */
        uint32_t clk_en:1;
        uint32_t reserved_1:31;
    };
    uint32_t val;
} gpio_clock_gate_reg_t;

/** Type of date register
 *  GPIO version register
 */
typedef union {
    struct {
        /** date : R/W; bitpos: [27:0]; default: 37785872;
         *  version register
         */
        uint32_t date:28;
        uint32_t reserved_28:4;
    };
    uint32_t val;
} gpio_date_reg_t;


typedef struct {
    volatile gpio_strap_reg_t strap;
    volatile gpio_out_reg_t out;
    volatile gpio_out_w1ts_reg_t out_w1ts;
    volatile gpio_out_w1tc_reg_t out_w1tc;
    uint32_t reserved_010[9];
    volatile gpio_enable_reg_t enable;
    volatile gpio_enable_w1ts_reg_t enable_w1ts;
    volatile gpio_enable_w1tc_reg_t enable_w1tc;
    uint32_t reserved_040[9];
    volatile gpio_in_reg_t in;
    uint32_t reserved_068[3];
    volatile gpio_status_reg_t status;
    volatile gpio_status_w1ts_reg_t status_w1ts;
    volatile gpio_status_w1tc_reg_t status_w1tc;
    uint32_t reserved_080[9];
    volatile gpio_procpu_int_reg_t procpu_int;
    volatile gpio_procpu_nmi_int_reg_t procpu_nmi_int;
    uint32_t reserved_0ac[6];
    volatile gpio_status_next_reg_t status_next;
    uint32_t reserved_0c8[3];
    volatile gpio_pinn_reg_t pinn[30];
    uint32_t reserved_14c[98];
    volatile gpio_func_in_sel_cfg_reg_t func_in_sel_cfg[256];  //0-255. reserved: 0-6, 17-19, 19-28, 35-40, 42-45, 56-6, 73-77, 82-87, 95-97, 162-255;
    uint32_t reserved_4ac[256];
    volatile gpio_funcn_out_sel_cfg_reg_t funcn_out_sel_cfg[30];
    uint32_t reserved_b4c[171];
    volatile gpio_clock_gate_reg_t clock_gate;
    volatile gpio_date_reg_t date;
} gpio_dev_t;

extern gpio_dev_t GPIO;

#ifndef __cplusplus
_Static_assert(sizeof(gpio_dev_t) == 0xe00, "Invalid size of gpio_dev_t structure");
#endif

#ifdef __cplusplus
}
#endif
