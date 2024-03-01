/* generated configuration header file - do not edit */
#ifndef BSP_PIN_CFG_H_
#define BSP_PIN_CFG_H_
#if __has_include("r_ioport.h")
#include "r_ioport.h"
#elif __has_include("r_ioport_b.h")
#include "r_ioport_b.h"
#endif

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

#define ENET_RMII_INT (BSP_IO_PORT_00_PIN_00)
#define GROVE2_AN102 (BSP_IO_PORT_00_PIN_02)
#define ARDUINO_A1 (BSP_IO_PORT_00_PIN_03)
#define ARDUINO_A0_MIKROBUS_AN000 (BSP_IO_PORT_00_PIN_04)
#define GROVE2_AN001 (BSP_IO_PORT_00_PIN_05)
#define PMOD1_IRQ11 (BSP_IO_PORT_00_PIN_06)
#define ARDUINO_A004 (BSP_IO_PORT_00_PIN_07)
#define USER_S2 (BSP_IO_PORT_00_PIN_08)
#define USER_S1 (BSP_IO_PORT_00_PIN_09)
#define MIKROBUS_IRQ14 (BSP_IO_PORT_00_PIN_10)
#define ARDUINO_A3 (BSP_IO_PORT_00_PIN_11)
#define ARDUINO_A5 (BSP_IO_PORT_00_PIN_15)
#define OSPI_DQ0 (BSP_IO_PORT_01_PIN_00)
#define OSPI_DQ3 (BSP_IO_PORT_01_PIN_01)
#define OSPI_DQ4 (BSP_IO_PORT_01_PIN_02)
#define OSPI_DQ2 (BSP_IO_PORT_01_PIN_03)
#define OSPI_CS (BSP_IO_PORT_01_PIN_04)
#define OSPI_INT (BSP_IO_PORT_01_PIN_05)
#define OSPI_RESET (BSP_IO_PORT_01_PIN_06)
#define LED3 (BSP_IO_PORT_01_PIN_07)
#define ETH_A_RMII_RMII_RXDV (BSP_IO_PORT_01_PIN_12)
#define SDRAM_CKE (BSP_IO_PORT_01_PIN_13)
#define ETH_A_LINKSTA (BSP_IO_PORT_01_PIN_14)
#define MPLX_CTRL (BSP_IO_PORT_01_PIN_15)
#define NMI (BSP_IO_PORT_02_PIN_00)
#define MD (BSP_IO_PORT_02_PIN_01)
#define MIPI_DSI_TE (BSP_IO_PORT_02_PIN_06)
#define CAN_STB (BSP_IO_PORT_02_PIN_07)
#define TDI (BSP_IO_PORT_02_PIN_08)
#define TDO (BSP_IO_PORT_02_PIN_09)
#define SWDIO (BSP_IO_PORT_02_PIN_10)
#define SWCLK (BSP_IO_PORT_02_PIN_11)
#define EXTAL (BSP_IO_PORT_02_PIN_12)
#define XTAL (BSP_IO_PORT_02_PIN_13)
#define ETH_A_RXER (BSP_IO_PORT_03_PIN_00)
#define ETH_A_RXD1 (BSP_IO_PORT_03_PIN_01)
#define ETH_A_RXD0 (BSP_IO_PORT_03_PIN_02)
#define ETH_A_REFCLK (BSP_IO_PORT_03_PIN_03)
#define ETH_A_TXD0 (BSP_IO_PORT_03_PIN_04)
#define ETH_A_TXD1 (BSP_IO_PORT_03_PIN_05)
#define ETH_A_TXEN (BSP_IO_PORT_03_PIN_06)
#define ETH_A_MDIO (BSP_IO_PORT_03_PIN_07)
#define ETH_A_MDC (BSP_IO_PORT_03_PIN_08)
#define ARDUINO_D0_MIKROBUS_RXD3 (BSP_IO_PORT_03_PIN_09)
#define ARDUINO_D1_MIKROBUS_TXD3 (BSP_IO_PORT_03_PIN_10)
#define CAN_RXD (BSP_IO_PORT_03_PIN_11)
#define CAN_TXD (BSP_IO_PORT_03_PIN_12)
#define I3C_SCL0_ARDUINO_MIKROBUS_PMOD1_3_qwiic (BSP_IO_PORT_04_PIN_00)
#define I3C_SDA0_ARDUINO_MIKROBUS_PMOD1_4_qwiic (BSP_IO_PORT_04_PIN_01)
#define ETH_B_MDIO (BSP_IO_PORT_04_PIN_02)
#define ETH_B_LINKSTA (BSP_IO_PORT_04_PIN_03)
#define DISP_BLEN (BSP_IO_PORT_04_PIN_04)
#define ETH_B_TXEN (BSP_IO_PORT_04_PIN_05)
#define ETH_B_TXD1 (BSP_IO_PORT_04_PIN_06)
#define USBFS_VBUS (BSP_IO_PORT_04_PIN_07)
#define USBHS_VBUSEN (BSP_IO_PORT_04_PIN_08)
#define USBHS_OVRCURA (BSP_IO_PORT_04_PIN_09)
#define MISOB_B_ARDUINO_MIKROBUS (BSP_IO_PORT_04_PIN_10)
#define MOSIB_B_ARDUINO_MIKROBUS (BSP_IO_PORT_04_PIN_11)
#define RSPCKB_B_ARDUINO_MIKROBUS (BSP_IO_PORT_04_PIN_12)
#define SSLB0_B_ARDUINO_D10_MIKROBUS (BSP_IO_PORT_04_PIN_13)
#define LED2 (BSP_IO_PORT_04_PIN_14)
#define CAM_D12 (BSP_IO_PORT_04_PIN_15)
#define USBFS_VBUS_EN (BSP_IO_PORT_05_PIN_00)
#define USBFS_OVERCURA (BSP_IO_PORT_05_PIN_01)
#define MIKROBUS_RESET (BSP_IO_PORT_05_PIN_02)
#define MIKROBUS_RES (BSP_IO_PORT_05_PIN_07)
#define PMOD2_7_IRQ1 (BSP_IO_PORT_05_PIN_08)
#define ARDUINO_D3 (BSP_IO_PORT_05_PIN_09)
#define DISP_INT (BSP_IO_PORT_05_PIN_10)
#define GROVE2_IIC_SDA1 (BSP_IO_PORT_05_PIN_11)
#define GROVE2_IIC_SCL1 (BSP_IO_PORT_05_PIN_12)
#define LCD_TCON2 (BSP_IO_PORT_05_PIN_13)
#define LCD_EXTCLK (BSP_IO_PORT_05_PIN_14)
#define LCD_TCON3 (BSP_IO_PORT_05_PIN_15)
#define BOOT_KEY (BSP_IO_PORT_06_PIN_00)
#define SDRAM_D0 (BSP_IO_PORT_06_PIN_01)
#define ARDUINO_D6 (BSP_IO_PORT_06_PIN_02)
#define SDRAM_D2 (BSP_IO_PORT_06_PIN_03)
#define SDRAM_D3 (BSP_IO_PORT_06_PIN_04)
#define SDRAM_D4 (BSP_IO_PORT_06_PIN_05)
#define SDRAM_D5 (BSP_IO_PORT_06_PIN_06)
#define SDRAM_D6 (BSP_IO_PORT_06_PIN_07)
#define PMOD1_3_MISO0_RXD0_SCL0 (BSP_IO_PORT_06_PIN_09)
#define PMOD1_2_MOSI0_TXD0 (BSP_IO_PORT_06_PIN_10)
#define PMOD1_4_SCK0 (BSP_IO_PORT_06_PIN_11)
#define PMOD1_1_SSL0_CTS_RTS (BSP_IO_PORT_06_PIN_12)
#define PMOD1_1_CTS0 (BSP_IO_PORT_06_PIN_13)
#define PMOD1_9_GPIO (BSP_IO_PORT_06_PIN_14)
#define PMOD1_10_GPIO (BSP_IO_PORT_06_PIN_15)
#define ETH_B_TXD0 (BSP_IO_PORT_07_PIN_00)
#define ETH_B_REFCLK (BSP_IO_PORT_07_PIN_01)
#define ETH_B_RXD0 (BSP_IO_PORT_07_PIN_02)
#define ETH_B_RXD1 (BSP_IO_PORT_07_PIN_03)
#define ETH_B_RXER (BSP_IO_PORT_07_PIN_04)
#define LCD_DATA23 (BSP_IO_PORT_07_PIN_07)
#define CAM_CLK (BSP_IO_PORT_07_PIN_08)
#define CAM_HD (BSP_IO_PORT_07_PIN_09)
#define CAM_VD (BSP_IO_PORT_07_PIN_10)
#define I3C_SDA0_PULLUP (BSP_IO_PORT_07_PIN_11)
#define LCD_DATA11 (BSP_IO_PORT_07_PIN_12)
#define LCD_DATA12 (BSP_IO_PORT_07_PIN_13)
#define LCD_DATA13 (BSP_IO_PORT_07_PIN_14)
#define LCD_DATA14 (BSP_IO_PORT_07_PIN_15)
#define OSPI_DQ5 (BSP_IO_PORT_08_PIN_00)
#define OSPI_DS (BSP_IO_PORT_08_PIN_01)
#define OSPI_DQ6 (BSP_IO_PORT_08_PIN_02)
#define OSPI_DQ1 (BSP_IO_PORT_08_PIN_03)
#define OSPI_DQ7 (BSP_IO_PORT_08_PIN_04)
#define LCD_TCON0 (BSP_IO_PORT_08_PIN_05)
#define LCD_CLK (BSP_IO_PORT_08_PIN_06)
#define LCD_TCON1 (BSP_IO_PORT_08_PIN_07)
#define OSPI_CK (BSP_IO_PORT_08_PIN_08)
#define PMOD2_8_RESET (BSP_IO_PORT_08_PIN_09)
#define PMOD2_9_GPIO (BSP_IO_PORT_08_PIN_10)
#define PMOD2_10_GPIO (BSP_IO_PORT_08_PIN_11)
#define ARDUINO_RESET (BSP_IO_PORT_08_PIN_13)
#define USBFS_P (BSP_IO_PORT_08_PIN_14)
#define USBFS_N (BSP_IO_PORT_08_PIN_15)
#define LCD_DATA8 (BSP_IO_PORT_09_PIN_02)
#define LCD_DATA7 (BSP_IO_PORT_09_PIN_03)
#define LCD_DATA6 (BSP_IO_PORT_09_PIN_04)
#define ARDUINO_D4 (BSP_IO_PORT_09_PIN_05)
#define ARDUINO_D2 (BSP_IO_PORT_09_PIN_06)
#define ARDUINO_D3_MIKROBUS_GTIOC13A (BSP_IO_PORT_09_PIN_07)
#define ARDUINO_D7 (BSP_IO_PORT_09_PIN_08)
#define ARDUINO_D8 (BSP_IO_PORT_09_PIN_09)
#define LCD_DATA2 (BSP_IO_PORT_09_PIN_10)
#define LCD_DATA3 (BSP_IO_PORT_09_PIN_11)
#define LCD_DATA4 (BSP_IO_PORT_09_PIN_12)
#define LCD_DATA5 (BSP_IO_PORT_09_PIN_13)
#define LCD_DATA0 (BSP_IO_PORT_09_PIN_14)
#define LCD_DATA1 (BSP_IO_PORT_09_PIN_15)
#define SDRAM_D7 (BSP_IO_PORT_10_PIN_00)
#define DISP_RST (BSP_IO_PORT_10_PIN_01)
#define PMOD2_3_MISO2_RXD2 (BSP_IO_PORT_10_PIN_02)
#define PMOD2_2_MOSI2_TXD2 (BSP_IO_PORT_10_PIN_03)
#define PMOD2_4_SCK2 (BSP_IO_PORT_10_PIN_04)
#define PMOD2_1_CTS_RTS_SSL2 (BSP_IO_PORT_10_PIN_05)
#define PMOD2_1_CTS2 (BSP_IO_PORT_10_PIN_06)
#define ARDUINO_D9 (BSP_IO_PORT_10_PIN_07)
#define PMOD1_8_RESET (BSP_IO_PORT_10_PIN_08)
#define SDRAM_SDCLK (BSP_IO_PORT_10_PIN_09)
#define SDRAM_DQM0 (BSP_IO_PORT_10_PIN_10)
#define JLOB_COMS_TX (BSP_IO_PORT_10_PIN_14)
#define JLOB_COMS_RX (BSP_IO_PORT_10_PIN_15)
#define I3C_SCL0_PULLUP (BSP_IO_PORT_11_PIN_00)
#define USBHS_VBUS (BSP_IO_PORT_11_PIN_01)
#define LCD_DATA21 (BSP_IO_PORT_11_PIN_02)
#define LCD_DATA20 (BSP_IO_PORT_11_PIN_03)
#define LCD_DATA19 (BSP_IO_PORT_11_PIN_04)
#define LCD_DATA17 (BSP_IO_PORT_11_PIN_05)
#define LCD_DATA16 (BSP_IO_PORT_11_PIN_06)
#define LCD_DATA15 (BSP_IO_PORT_11_PIN_07)
extern const ioport_cfg_t g_bsp_pin_cfg; /* RTT-RA8D1 */

void BSP_PinConfigSecurityInit();

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER
#endif /* BSP_PIN_CFG_H_ */
