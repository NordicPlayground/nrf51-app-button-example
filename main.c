/* Copyright (c) 2014 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/*
*  App_button example.
*
*/

#include <stdbool.h>
#include <string.h>
#include "app_button.h"
#include "boards.h"
#include "app_gpiote.h"
#include "app_timer.h"

#define APP_TIMER_PRESCALER             0  // Value of the RTC1 PRESCALER register.
#define APP_TIMER_MAX_TIMERS            1  // Maximum number of simultaneously created timers. 
#define APP_TIMER_OP_QUEUE_SIZE         2  // Size of timer operation queues. 
#define BUTTON_DEBOUNCE_DELAY			50 // Delay from a GPIOTE event until a button is reported as pushed. 
#define APP_GPIOTE_MAX_USERS            1  // Maximum number of users of the GPIOTE handler. 

/*
 * Handler to be called when button is pushed.
 * param[in]   pin_no 			The pin number where the event is genereated
 * param[in]   button_action 	Is the button pushed or released
 */
static void button_handler(uint8_t pin_no, uint8_t button_action)
{
    if(button_action == APP_BUTTON_PUSH)
    {
        switch(pin_no)
        {
            case BUTTON_1:
                nrf_gpio_pin_toggle(LED_1);
                break;
            case BUTTON_2:
                nrf_gpio_pin_toggle(LED_2);
                break;
            case BUTTON_3:
                nrf_gpio_pin_toggle(LED_3);
                break;
            case BUTTON_4:
                nrf_gpio_pin_toggle(LED_4);
                break;
            default:
                break;
        }
    }
}

/**
 * Initialize the clock.
 */
void init_clock()
{
    NRF_CLOCK->LFCLKSRC            = (CLOCK_LFCLKSRC_SRC_Xtal << CLOCK_LFCLKSRC_SRC_Pos);
    NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_LFCLKSTART    = 1;
    while (NRF_CLOCK->EVENTS_LFCLKSTARTED == 0); // Wait for clock to start
}	

/**
 * Initialize all four LEDs on the nRF51 DK.
 */
void init_leds()
{
    nrf_gpio_cfg_output(LED_1);
    nrf_gpio_cfg_output(LED_2);
    nrf_gpio_cfg_output(LED_3);
    nrf_gpio_cfg_output(LED_4);
    nrf_gpio_pin_set(LED_1);
    nrf_gpio_pin_set(LED_2);
    nrf_gpio_pin_set(LED_3);
    nrf_gpio_pin_set(LED_4);
}	

/**
 * Function for application main entry.
 */
int main(void)
{	
    init_leds();
    init_clock();

    uint32_t err_code;

    // Button configuration structure.
    static app_button_cfg_t p_button[] = {  {BUTTON_1, APP_BUTTON_ACTIVE_LOW, NRF_GPIO_PIN_PULLUP, button_handler},
                                            {BUTTON_2, APP_BUTTON_ACTIVE_LOW, NRF_GPIO_PIN_PULLUP, button_handler},
                                            {BUTTON_3, APP_BUTTON_ACTIVE_LOW, NRF_GPIO_PIN_PULLUP, button_handler},
                                            {BUTTON_4, APP_BUTTON_ACTIVE_LOW, NRF_GPIO_PIN_PULLUP, button_handler}};
        
    // Macro for initializing the application timer module.
    // It will handle dimensioning and allocation of the memory buffer required by the timer, making sure that the buffer is correctly aligned. It will also connect the timer module to the scheduler (if specified).
    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_MAX_TIMERS, APP_TIMER_OP_QUEUE_SIZE, NULL);

    // Macro for initializing the GPIOTE module.
    // It will handle dimensioning and allocation of the memory buffer required by the module, making sure that the buffer is correctly aligned.
    APP_GPIOTE_INIT(APP_GPIOTE_MAX_USERS);

    // Initializing the buttons.
    err_code = app_button_init(p_button, sizeof(p_button) / sizeof(p_button[0]), BUTTON_DEBOUNCE_DELAY);
    APP_ERROR_CHECK(err_code);
                                            
    // Enabling the buttons.										
    err_code = app_button_enable();
    APP_ERROR_CHECK(err_code);

    while(true)
    {
        // Do nothing.
    }
}

