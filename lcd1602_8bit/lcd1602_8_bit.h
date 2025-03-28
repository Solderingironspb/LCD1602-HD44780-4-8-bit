/**
 ******************************************************************************
 *  @file lcd1602_8bit.h
 *  @brief Работа с дисплеем lcd1602 в 8 битном режиме
 *  @author Волков Олег
 *  @date 28.03.2025
 *
 ******************************************************************************
 * @attention
 *  YouTube: https://www.youtube.com/channel/UCzZKTNVpcMSALU57G1THoVw
 *  GitHub: https://github.com/Solderingironspb/STM32F103C8T6_CMSIS_notes
 *  Группа ВК: https://vk.com/solderingiron.stm32
 *  Работал по документации: https://www.waveshare.com/datasheet/LCD_en_PDF/LCD1602.pdf
 *
 * Номера пинов:
 * _______________________________________________________________________
 * |  1   |   VSS  |         GROUND          | GND                         |                         
 * |  2   |   VCC  |      POWER SUPPLY       | +5V                         |
 * |  3   | VEE/V0 |      LCD CONTRAST       | ПОТЕНЦИОМЕТР НА 10 кОм      |
 * |  4   |   RS   |    INSTRUCTION/DATA     | RS=0:  INSTRUCTION REGISTER |
 * |      |        |   REGISTER SELECTION    | RS=1:  REGISTER READ        |
 * |  5   |   R/W  |  READ/WRITE SELECTION   | R/W=0: REGISTER WRITE       |
 * |      |        |                         | R/W=1: REGISTER READ        |
 * |  6   |    E   |      ENABLE SIGNAL      |                             |
 * |  7   |   DB0  |                         |                             |
 * |  8   |   DB1  |                         |                             |
 * |  9   |   DB2  |                         |                             |
 * |  10  |   DB3  |                         |                             |
 * |  11  |   DB4  | DATA INPUT/OUTPUT LINES |                             |
 * |  12  |   DB5  |                         |                             |
 * |  13  |   DB6  |                         |                             |
 * |  14  |   DB7  |                         |                             |
 * |  15  |  LED+  | SUPPLY VOLTAGE FOR LED+ | +3.3v/+5v                   |
 * |__16__|__LED-__|_SUPPLY_VOLTAGE_FOR_LED-_|_GND_________________________|
 * 
 ******************************************************************************
 */

#include "main.h"
#include <string.h>
#include <stdio.h>



#define LCD1602_GPIO_MODE_INPUT         0
#define LCD1602_GPIO_MODE_OUTPUT        1


#define LCD1602_RS_SET_BIT      GPIOC->BSHR = GPIO_BSHR_BS13
#define LCD1602_RS_CLEAR_BIT    GPIOC->BSHR = GPIO_BSHR_BR13

#define LCD1602_RW_SET_BIT      GPIOC->BSHR = GPIO_BSHR_BS14
#define LCD1602_RW_CLEAR_BIT    GPIOC->BSHR = GPIO_BSHR_BR14

#define LCD1602_E_SET_BIT      GPIOC->BSHR = GPIO_BSHR_BS15
#define LCD1602_E_CLEAR_BIT    GPIOC->BSHR = GPIO_BSHR_BR15

void lcd1602_init_GPIO_RS_RW_E(void);
void lcd1602_init_GPIO_D0_D7(uint8_t mode);
void lcd1602_init_display(void);
void lcd1602_set_data(uint8_t data);
void lcd1602_send_command(uint8_t data);
void lcd1602_set_cursor(uint8_t x, uint8_t y);
void lcd1602_print_text(char *message);
void lcd1602_print_symbol(uint8_t data);
void lcd1602_screen_clear(void);
void lcd1602_display_shift(uint8_t s_c, uint8_t r_l);
void lcd1602_display_switch(uint8_t on_off, uint8_t cursor_on_off, uint8_t cursor_blink);