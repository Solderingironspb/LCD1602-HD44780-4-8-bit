/**
 ******************************************************************************
 *  @file lcd1602_4bit.c
 *  @brief Работа с дисплеем lcd1602 в 4 битном режиме
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
 * |  4   |   RS   |    INSTRUCTION/data     | RS=0:  INSTRUCTION REGISTER |
 * |      |        |   REGISTER SELECTION    | RS=1:  REGISTER READ        |
 * |  5   |   R/W  |  READ/WRITE SELECTION   | R/W=0: REGISTER WRITE       |
 * |      |        |                         | R/W=1: REGISTER READ        |
 * |  6   |    E   |      ENABLE SIGNAL      |                             |
 * |  7   |   DB0  |                         |                             |
 * |  8   |   DB1  |                         |                             |
 * |  9   |   DB2  |                         |                             |
 * |  10  |   DB3  |                         |                             |
 * |  11  |   DB4  | data INPUT/OUTPUT LINES | +                           |
 * |  12  |   DB5  |                         | +                           |
 * |  13  |   DB6  |                         | +                           |
 * |  14  |   DB7  |                         | +                           |
 * |  15  |  LED+  | SUPPLY VOLTAGE FOR LED+ | +3.3v/+5v                   |
 * |__16__|__LED-__|_SUPPLY_VOLTAGE_FOR_LED-_|_GND_________________________|
 *
 ******************************************************************************
 */
#include "lcd1602_4_bit.h"

/**
 ***************************************************************************************
 *  @breif Конфигурация GPIO для ножек RS, RW, E
 *  Перед настройкой (GPIOs and AFIOs) нужно включить тактирование порта.
 ***************************************************************************************
 */
void lcd1602_init_GPIO_RS_RW_E(void) {
    SET_BIT(RCC->APB2PCENR, RCC_APB2Periph_GPIOC);                               // Включим тактирование порта C
    MODIFY_REG(GPIOC->CFGHR, GPIO_CFGHR_MODE13, 0b11 << GPIO_CFGHR_MODE13_Pos);  // 11: Output mode, maximum speed: 50MHz;
    MODIFY_REG(GPIOC->CFGHR, GPIO_CFGHR_CNF13, 0b00 << GPIO_CFGHR_CNF13_Pos);    // 00: General push-pull output mode;

    MODIFY_REG(GPIOC->CFGHR, GPIO_CFGHR_MODE14, 0b11 << GPIO_CFGHR_MODE14_Pos);  // 11: Output mode, maximum speed: 50MHz;
    MODIFY_REG(GPIOC->CFGHR, GPIO_CFGHR_CNF14, 0b00 << GPIO_CFGHR_CNF14_Pos);    // 00: General push-pull output mode;

    MODIFY_REG(GPIOC->CFGHR, GPIO_CFGHR_MODE15, 0b11 << GPIO_CFGHR_MODE15_Pos);  // 11: Output mode, maximum speed: 50MHz;
    MODIFY_REG(GPIOC->CFGHR, GPIO_CFGHR_CNF15, 0b00 << GPIO_CFGHR_CNF15_Pos);    // 00: General push-pull output mode;
}

/**
 ***************************************************************************************
 *  @breif Конфигурация GPIO для ножек D4-D7
 *  Перед настройкой (GPIOs and AFIOs) нужно включить тактирование порта.
 *  @param  Mode - Настройка GPIO на вход или выход
 ***************************************************************************************
 */
void lcd1602_init_GPIO_D0_D7(uint8_t Mode) {
    if (Mode == LCD1602_GPIO_MODE_INPUT) {
        SET_BIT(RCC->APB2PCENR, RCC_APB2Periph_GPIOA);                             // Включим тактирование порта А
      
        MODIFY_REG(GPIOA->CFGLR, GPIO_CFGLR_MODE4, 0b00 << GPIO_CFGLR_MODE4_Pos);  // 00: Input mode;
        MODIFY_REG(GPIOA->CFGLR, GPIO_CFGLR_CNF4, 0b10 << GPIO_CFGLR_CNF4_Pos);    // 01: Floating input mode;

        MODIFY_REG(GPIOA->CFGLR, GPIO_CFGLR_MODE5, 0b00 << GPIO_CFGLR_MODE5_Pos);  // 00: Input mode;
        MODIFY_REG(GPIOA->CFGLR, GPIO_CFGLR_CNF5, 0b10 << GPIO_CFGLR_CNF5_Pos);    // 01: Floating input mode;

        MODIFY_REG(GPIOA->CFGLR, GPIO_CFGLR_MODE6, 0b00 << GPIO_CFGLR_MODE6_Pos);  // 00: Input mode;
        MODIFY_REG(GPIOA->CFGLR, GPIO_CFGLR_CNF6, 0b10 << GPIO_CFGLR_CNF6_Pos);    // 01: Floating input mode;

        MODIFY_REG(GPIOA->CFGLR, GPIO_CFGLR_MODE7, 0b00 << GPIO_CFGLR_MODE7_Pos);  // 00: Input mode;
        MODIFY_REG(GPIOA->CFGLR, GPIO_CFGLR_CNF7, 0b10 << GPIO_CFGLR_CNF7_Pos);    // 01: Floating input mode;

    } else if (Mode == LCD1602_GPIO_MODE_OUTPUT) {
        SET_BIT(RCC->APB2PCENR, RCC_APB2Periph_GPIOA);  // Включим тактирование порта А

        MODIFY_REG(GPIOA->CFGLR, GPIO_CFGLR_MODE4, 0b11 << GPIO_CFGLR_MODE4_Pos);  // 11: Output mode, maximum speed: 50MHz;
        MODIFY_REG(GPIOA->CFGLR, GPIO_CFGLR_CNF4, 0b00 << GPIO_CFGLR_CNF4_Pos);    // 00: General push-pull output mode;

        MODIFY_REG(GPIOA->CFGLR, GPIO_CFGLR_MODE5, 0b11 << GPIO_CFGLR_MODE5_Pos);  // 11: Output mode, maximum speed: 50MHz;
        MODIFY_REG(GPIOA->CFGLR, GPIO_CFGLR_CNF5, 0b00 << GPIO_CFGLR_CNF5_Pos);    // 00: General push-pull output mode;

        MODIFY_REG(GPIOA->CFGLR, GPIO_CFGLR_MODE6, 0b11 << GPIO_CFGLR_MODE6_Pos);  // 11: Output mode, maximum speed: 50MHz;
        MODIFY_REG(GPIOA->CFGLR, GPIO_CFGLR_CNF6, 0b00 << GPIO_CFGLR_CNF6_Pos);    // 00: General push-pull output mode;

        MODIFY_REG(GPIOA->CFGLR, GPIO_CFGLR_MODE7, 0b11 << GPIO_CFGLR_MODE7_Pos);  // 11: Output mode, maximum speed: 50MHz;
        MODIFY_REG(GPIOA->CFGLR, GPIO_CFGLR_CNF7, 0b00 << GPIO_CFGLR_CNF7_Pos);    // 00: General push-pull output mode;
    }
}

/**
 ***************************************************************************************
 *  @breif Инициализация дисплея
 ***************************************************************************************
 */
void lcd1602_init_display(void) {
    /*Сбросим все в 0*/
    LCD1602_RS_CLEAR_BIT;
    LCD1602_RW_CLEAR_BIT;
    LCD1602_E_CLEAR_BIT;
    GPIOA->OUTDR = 0x00;
    Delay_ms(20);  // Ожидать не менее 20 мс

    LCD1602_E_SET_BIT;
    lcd1602_set_data(0x3);
    LCD1602_E_CLEAR_BIT;
    Delay_ms(4);  // Ожидать не менее 20 мс

    LCD1602_E_SET_BIT;
    lcd1602_set_data(0x3);
    LCD1602_E_CLEAR_BIT;
    Delay_ms(1);  // Ожидать не менее 20 мс

    LCD1602_E_SET_BIT;
    lcd1602_set_data(0x3);
    LCD1602_E_CLEAR_BIT;
    Delay_ms(1);  // Ожидать не менее 20 мс

    // Переключение в 4-битный режим
    
    LCD1602_E_SET_BIT;
    lcd1602_set_data(0x02);
    LCD1602_E_CLEAR_BIT;
    Delay_ms(1);

    lcd1602_send_command(0x28);  // FUNCTION SET: 4 бит, 2 строки, шрифт 5x8 точек
    Delay_ms(1);                 // Ожидать не менее 20 мс

    lcd1602_display_switch(0, 0, 0);  // DISPLAY SWITCH Выключить дисплей (D=0, C=0, B=0)
    lcd1602_display_switch(1, 0, 0);  // DISPLAY SWITCH Включить дисплей (D=1), курсор выкл (C=0), мигание выкл (B=0)
    lcd1602_screen_clear();           // Очистим дисплей

    Delay_ms(5);
}

/**
 ***************************************************************************************
 *  @breif Настройка дисплея (вкл/выкл, настройка курсора)
 *  @param  on_off - вкл/выкл дисплей
 *  @param  cursor_on_off - вкл/выкл курсор
 *  @param  cursor_blink - вкл/выкл мигание курсора
 ***************************************************************************************
 */
void lcd1602_display_switch(uint8_t on_off, uint8_t cursor_on_off, uint8_t cursor_blink) {
    uint8_t data = 0x8;
    data = data | (on_off << 2) | (cursor_on_off << 1) | cursor_blink;
    lcd1602_send_command(data);
}

/**
 ***************************************************************************************
 *  @breif Формирование выхода ножек D4-D7, в соответствии с data
 *  @param  data - Загружаемый байт данных, который выставит биты на ножках D4-D7
 ***************************************************************************************
 */
void lcd1602_set_data(uint8_t data) {
    GPIOA->OUTDR = (GPIOA->OUTDR) & 0xFF0F;
    GPIOA->OUTDR = (GPIOA->OUTDR) | (data << 4U);
}

/**
 ***************************************************************************************
 *  @breif Формирование выхода ножек D4-D7, в соответствии с data
 *  @param  data - Данные, которые хотим отправить на дисплей
 ***************************************************************************************
 */
void lcd1602_send_command(uint8_t data) {
    lcd1602_set_data(data >> 4U);
    LCD1602_E_SET_BIT;
    LCD1602_E_CLEAR_BIT;

    lcd1602_set_data(data & 0x0F);
    LCD1602_E_SET_BIT;
    LCD1602_E_CLEAR_BIT;
    lcd1602_init_GPIO_D0_D7(LCD1602_GPIO_MODE_INPUT);
    // 3. Чтение флага занятости (BF)
    LCD1602_RW_SET_BIT;    // Режим чтения (RW=1)
    LCD1602_RS_CLEAR_BIT;  // Чтение статуса (RS=0)
    do {
        LCD1602_E_SET_BIT;  // Импульс E для чтения
        for (volatile uint32_t i = 0; i < 144; i++) {
            __NOP();
        }
        LCD1602_E_CLEAR_BIT;
    } while (READ_BIT(GPIOA->INDR, GPIO_INDR_IDR3) == 1);  // Проверяем BF (PA7)

    // 3. Чтение флага занятости (AC3)
    LCD1602_RW_SET_BIT;    // Режим чтения (RW=1)
    LCD1602_RS_CLEAR_BIT;  // Чтение статуса (RS=0)
    LCD1602_E_SET_BIT;     // Импульс E для чтения
    for (volatile uint32_t i = 0; i < 144; i++) {
        __NOP();
    }
    LCD1602_E_CLEAR_BIT;

    // 4. Возврат в режим записи
    LCD1602_RW_CLEAR_BIT;  // RW=0 (запись)
    lcd1602_init_GPIO_D0_D7(LCD1602_GPIO_MODE_OUTPUT);
   // Delay_ms(5);
}

/**
 ***************************************************************************************
 *  @breif Очистка дисплея хардварным способом
 ***************************************************************************************
 */
void lcd1602_screen_clear(void) {
    lcd1602_send_command(0x1);
}

/**
 ***************************************************************************************
 *  @breif Установка курсора по осям x, y
 *  @param  x - Координата по оси x
 *  @param  y - Координата по оси y
 ***************************************************************************************
 */
void lcd1602_set_cursor(uint8_t x, uint8_t y) {
    uint8_t address;
    // Вычисляем адрес DDRAM в зависимости от строки
    switch (y) {
        case 0:
            address = x;
            break;
        case 1:
            address = 0x40 + x;
            break;
        case 2:
            address = 0x14 + x;
            break;  // Только для LCD2004
        case 3:
            address = 0x54 + x;
            break;  // Только для LCD2004
        default:
            address = x;
            break;
    }

    // Отправляем команду установки адреса
    uint8_t command = 0x80 | address;
    lcd1602_send_command(command);
}

/**
 ***************************************************************************************
 *  @breif Установка курсора по осям x, y
 *  @param  x - Координата по оси x
 *  @param  y - Координата по оси y
 ***************************************************************************************
 */
void lcd1602_print_symbol(uint8_t data) {
    LCD1602_RS_SET_BIT;          // RS=1 (режим данных)
    LCD1602_RW_CLEAR_BIT;        // RW=0 (режим записи данных)
    lcd1602_send_command(data);  // Используем вашу функцию отправки
    LCD1602_RS_CLEAR_BIT;        // Вернуть RS=0 (для будущих команд)
}

/**
 ***************************************************************************************
 *  @breif Вывести текст на дисплей
 *  @param  *message - текстовый массив данных
 ***************************************************************************************
 */
void lcd1602_print_text(char *message) {
    for (int i = 0; i < strlen(message); i++) {
        lcd1602_print_symbol(message[i]);
    }
}

/**
 ***************************************************************************************
 *  @breif Сдвинуть дисплей влево/вправо
 *  @param  s_c - Вкл/выкл сдвиг дисплея
 *  @param  r_l- 0 - Влево, 1 - Вправо
 ***************************************************************************************
 */
void lcd1602_display_shift(uint8_t s_c, uint8_t r_l) {
    uint8_t data = 0x10;
    data = data | (s_c << 3U) | (r_l << 2);
    lcd1602_send_command(data);
}