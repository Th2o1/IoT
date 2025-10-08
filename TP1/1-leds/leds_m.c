#include "contiki.h"
#include "dev/button-hal.h"
#include "dev/gpio-hal.h"

/*---------------------------------------------------------------------------*/
PROCESS(leds_m_process, "Leds m process");
AUTOSTART_PROCESSES(&leds_m_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(leds_m_process, ev, data)
{

    PROCESS_BEGIN();

    while (1) {

        PROCESS_YIELD();
        // TODO1
        // l'événement est-il un bouton relaché ?
        if(ev == button_hal_release_event) {
            button_hal_button_t *button = (button_hal_button_t *)data;
            uint8_t id = button->unique_id;
            switch (id)
            {
            case 0 :
                gpio_hal_arch_toggle_pin(0,13);
                break;
            case 1 :
                gpio_hal_arch_toggle_pin(0,14);
                break;
            case 2 :
                gpio_hal_arch_toggle_pin(0,15);
                break;
            case 3 :
                gpio_hal_arch_toggle_pin(0,16);
                break;
            
            default:
                break;
            }
        }

        // TODO2
        // si oui, il faut identifier le bouton qui a été relaché
        

        // TODO3
        // il faut allumer ou éteindre la led correspondante au bouton

    }

    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
