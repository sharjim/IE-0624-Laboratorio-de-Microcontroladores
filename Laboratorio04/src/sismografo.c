// Librerías utilizadas
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// Includes de LCD y periféricos
#include "lcd-spi.h"
#include "clock.h"
#include "console.h"
#include "sdram.h"
#include "gfx.h"
#include "grp.h"
#include "gyr_spi.c"
#include "gyr_spi.h"
#include "usart.h"
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/adc.h>

// Declaración de constantes y variables globales
#define VREF 3.3f
#define ADC_MAX_VALUE 4095.0f
#define CONVERSION_FACTOR 2.703f
#define NUM_SAMPLES 100
#define BATTERY_THRESHOLD 7.5f
#define LED_BLINK_INTERVAL 50000  // Temporización ajustada

float battery_voltage;

// Declaraciones de las funciones utilizadas en el código
void gpio_setup_for_ledR(void);
void gpio_setup_for_ledG(void);
static uint16_t read_adc_naiive(uint8_t channel);
uint16_t read_adc_average(uint8_t channel);
static void usart_setup(void);
void usart_send_string(char* str);
float adc_to_voltage(uint16_t adc_value);
void adc_setup(void);
void button_setup(void);
bool is_button_pressed(void);

// Implementación del `main`
int main(void) {
    char lcd_out[100];
    char usart_out[200]; 
    int16_t x, y, z;

    // Configuración inicial de periféricos
    clock_setup();
    console_setup(115200);
    sdram_init();
    gyr_setup();
    adc_setup();
    lcd_spi_init();
    gfx_init(lcd_draw_pixel, 240, 320);
    gpio_setup_for_ledR();
    gpio_setup_for_ledG();
    usart_setup();

    while (1) {
        // Limpiar pantalla y mostrar título
        gfx_fillScreen(LCD_WHITE);
        gfx_setTextSize(2);
        gfx_setCursor(25, 15);
        gfx_puts("Sismografo");

        // Lecturas del acelerómetro
        x = gyr_readX();
        y = gyr_readY();
        z = gyr_readZ();

        // Mostrar lecturas en pantalla
        sprintf(lcd_out, "X: %.2f", (float)x / 100);
        gfx_setCursor(15, 36);
        gfx_puts(lcd_out);

        sprintf(lcd_out, "Y: %.2f", (float)y / 100);
        gfx_setCursor(15, 90);
        gfx_puts(lcd_out);

        sprintf(lcd_out, "Z: %.2f", (float)z / 100);
        gfx_setCursor(15, 144);
        gfx_puts(lcd_out);

        // Lectura del voltaje de la batería mediante ADC
        uint16_t raw_adc_value = read_adc_average(7);
        battery_voltage = adc_to_voltage(raw_adc_value);

        // Control del LED según el voltaje de la batería
        if (battery_voltage < BATTERY_THRESHOLD) {
            gpio_toggle(GPIOG, GPIO14);
            for (int i = 0; i < LED_BLINK_INTERVAL; i++) {
                __asm__("nop");
            }
        } else {
            gpio_clear(GPIOG, GPIO14);
        }

        // Mostrar valores en pantalla
        sprintf(lcd_out, "Raw ADC: %d", raw_adc_value);
        gfx_setCursor(15, 198);
        gfx_puts(lcd_out);

        sprintf(lcd_out, "Voltaje: %.2fV", battery_voltage);
        gfx_setCursor(15, 250);
        gfx_puts(lcd_out);

        // Envío por USART si el botón es presionado
        if (is_button_pressed()) {
            sprintf(usart_out, "%.2f\t%.2f\t%.2f\t%.2f\r\n", 
                    (float)x / 100, (float)y / 100, (float)z / 100, battery_voltage);
            usart_send_string(usart_out);
        }

        // Actualización y espera
        lcd_show_frame();
        for (int i = 0; i < 8000; i++) {
            __asm__("nop");
        }
    }
    return 0;
}

// Implementación de las funciones

void gpio_setup_for_ledR(void) {
    rcc_periph_clock_enable(RCC_GPIOG);
    gpio_mode_setup(GPIOG, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO14);
}

void gpio_setup_for_ledG(void) {
    rcc_periph_clock_enable(RCC_GPIOG);
    gpio_mode_setup(GPIOG, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13);
}

static uint16_t read_adc_naiive(uint8_t channel) {
    uint8_t channel_array[16] = {channel};
    adc_set_regular_sequence(ADC1, 1, channel_array);
    adc_start_conversion_regular(ADC1);
    while (!adc_eoc(ADC1));
    return adc_read_regular(ADC1);
}

uint16_t read_adc_average(uint8_t channel) {
    uint32_t sum = 0;
    for (int i = 0; i < NUM_SAMPLES; i++) {
        sum += read_adc_naiive(channel);
    }
    return sum / NUM_SAMPLES;
}

static void usart_setup(void) {
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9);
    gpio_set_af(GPIOA, GPIO_AF7, GPIO9);
    usart_set_baudrate(USART1, 115200);
    usart_set_databits(USART1, 8);
    usart_set_stopbits(USART1, USART_STOPBITS_1);
    usart_set_mode(USART1, USART_MODE_TX);
    usart_set_parity(USART1, USART_PARITY_NONE);
    usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
    usart_enable(USART1);
}

void usart_send_string(char* str) {
    while (*str) {
        usart_send_blocking(USART1, *str++);
        gpio_toggle(GPIOG, GPIO13);  // Parpadeo durante transmisión
        for (int i = 0; i < LED_BLINK_INTERVAL; i++) {
            __asm__("nop");
        }
    }
}

float adc_to_voltage(uint16_t adc_value) {
    return ((adc_value / ADC_MAX_VALUE) * VREF) * CONVERSION_FACTOR;
}

void adc_setup(void) {
    rcc_periph_clock_enable(RCC_ADC1);
    gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO7);
    adc_power_off(ADC1);
    adc_enable_scan_mode(ADC1);
    adc_set_continuous_conversion_mode(ADC1);
    adc_disable_external_trigger_regular(ADC1);
    adc_set_right_aligned(ADC1);
    adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_28CYC);
    adc_power_on(ADC1);
}

void button_setup(void) {
    rcc_periph_clock_enable(RCC_GPIOA);
    gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO0);
}

bool is_button_pressed(void) {
    return gpio_get(GPIOA, GPIO0);
}
