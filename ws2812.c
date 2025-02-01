#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/timer.h"
#include "ws2812.pio.h"

#define LED_R_PIN 13 // LED RGB Vermelho
#define BUTTON_A 5 // Botão A - Incrementa
#define BUTTON_B 6 // Botão B - Decrementa
#define IS_RGBW false // Define se os LEDs são RGBW
#define NUM_PIXELS 25 // Número de LEds na matriz
#define WS2812_PIN 7 // Pino dos LEDs WS2812
#define BLINK_INTERVAL 100 // Intervalo para o LED piscar (50 ms ligado, 50 ms desligado)
#define DEBOUNCE_TIME 200000 // Tempo de debouncing em microssegundos (200 ms)

// Variável global para armazenar o número atual
volatile int current_number = 0;

// Variáveis para armazenar a cor (0-255)
uint8_t led_r = 0;    // Vermelho
uint8_t led_g = 0;    // Verde
uint8_t led_b = 100;  // Azul

// Varíaveis para debouncing
static volatile uint32_t last_time_buttonA = 0;
static volatile uint32_t last_time_buttonB = 0;

// Números de 0 a 9
const bool number_patterns[10][NUM_PIXELS] = {
    {1, 1, 1, 1, 1,
     1, 0, 0, 0, 1,
     1, 0, 0, 0, 1, 
     1, 0, 0, 0, 1, 
     1, 1, 1, 1, 1}, // 0
    {0, 1, 1, 1, 0, 
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 1, 1, 0, 0,
    0, 0, 1, 0, 0}, // 1
    {1, 1, 1, 1, 1,
    1, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 
    0, 0, 0, 0, 1, 
    1, 1, 1, 1, 1}, // 2
    {1, 1, 1, 1, 1, 
    0, 0, 0, 0, 1, 
    1, 1, 1, 0, 0, 
    0, 0, 0, 0, 1, 
    1, 1, 1, 1, 1}, // 3
    {1, 1, 0, 0, 0, 
    0, 0, 0, 1, 1, 
    1, 1, 1, 1, 1, 
    1, 0, 0, 1, 1, 
    1, 1, 0, 0, 1}, // 4
    {1, 1, 1, 1, 1, 
    0, 0, 0, 0, 1, 
    1, 1, 1, 1, 1, 
    1, 0, 0, 0, 0, 
    1, 1, 1, 1, 1}, // 5
    {1, 1, 1, 1, 1, 
    1, 0, 0, 0, 1, 
    1, 1, 1, 1, 1, 
    1, 0, 0, 0, 0, 
    1, 1, 1, 1, 1}, // 6
    {0, 0, 1, 0, 0, 
    0, 0, 1, 0, 0, 
    0, 1, 0, 0, 0, 
    0, 0, 0, 0, 1, 
    1, 1, 1, 1, 1}, // 7
    {1, 1, 1, 1, 1, 
    1, 0, 0, 0, 1, 
    1, 1, 1, 1, 1, 
    1, 0, 0, 0, 1, 
    1, 1, 1, 1, 1}, // 8
    {1, 1, 1, 1, 1, 
    0, 0, 0, 0, 1, 
    1, 1, 1, 1, 1, 
    1, 0, 0, 0, 1, 
    1, 1, 1, 1, 1}  // 9
};

// Função para enviar um pixel para a matriz de LEDs
static inline void put_pixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

//Função para converter RGB para formato de 32 bits
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)(g) << 16) | ((uint32_t)(r) << 8) | (uint32_t)(b);
}

// Função para exibir o número atual na matriz de LEDs
void set_one_led(uint8_t r, uint8_t g, uint8_t b)
{
    uint32_t color = urgb_u32(r, g, b);

    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (number_patterns[current_number][i])
        {
            put_pixel(color);
        }
        else
        {
            put_pixel(0);
        }
    }
}

// Função de interrupção para os botões
void gpio_irq_handler(uint gpio, uint32_t events)
{
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    if (gpio == BUTTON_A && (current_time - last_time_buttonA > DEBOUNCE_TIME))
    {
        last_time_buttonA = current_time;
        if (current_number <9)
        {
            current_number++;
        }
        
    }
    else if (gpio == BUTTON_B && (current_time - last_time_buttonB > DEBOUNCE_TIME))
    {
        last_time_buttonB = current_time;
        if (current_number > 0)
        {
            current_number--;
        }
    }
}

// Função principal
int main()
{
    stdio_init_all();
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);

    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);

    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    gpio_init(LED_R_PIN);
    gpio_set_dir(LED_R_PIN, GPIO_OUT);

    while (1)
    {
       // Piscar o LED vermelho 5 vezes por segundo
        gpio_put(LED_R_PIN, true); // Liga LED
        sleep_ms(BLINK_INTERVAL / 2);
        gpio_put(LED_R_PIN, false); // Desliga LED
        sleep_ms(BLINK_INTERVAL / 2);
        
    // Atualizar a matriz de LEDs com o número atual    
        set_one_led(led_r, led_g, led_b);
    }

    return 0;
}
