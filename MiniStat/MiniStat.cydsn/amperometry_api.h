/* ========================================
 *
 * NANOBIOELECTRONICS LAB UCSD, 2019
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF the company.
 *
 * ========================================
 */

#include <project.h>

// Constants

#define LMP_ADDR 0x48

#define LMP_REG_MODECN_ADDR 0x12
#define LMP_REG_REFCN_ADDR 0x11
#define LMP_REG_TIACN_ADDR 0x10

#define LMP_3_LEAD_AMPEROMETRIC_CELL_MODE 0x03 
#define LMP_INT_BIAS_DEFAULT 0x2D 
#define LMP_TIA_DEFAULT 0x03

// Functions

void lmp_setup();