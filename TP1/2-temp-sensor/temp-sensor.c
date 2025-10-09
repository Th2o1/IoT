#include "temperature-sensor.h"
#include "etimer.h"
#include "contiki.h"
#include "sys/log.h"
#include <stdio.h>

/*---------------------------------------------------------------------------*/
PROCESS(read_temp_process, "Read Temperature Process");
AUTOSTART_PROCESSES(&read_temp_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(read_temp_process, ev, data)
{
    // TODO1 déclaration du temporisateur
    static struct etimer t;

    PROCESS_BEGIN();

    // TODO1 armement du temporisateur à 5 sec.

    etimer_set(&t, 5 * CLOCK_SECOND);

    // TODO2 activer le capteur

    SENSORS_ACTIVATE(temperature_sensor);

    while (1)
    {

        // TODO1 attente passive jusqu'à l'expiration du temporisateur
        // puis affichage d'un message
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&t));
        printf("BOO!");
        // TODO2: Get and display raw sensor value
        int raw_value = temperature_sensor.value(0);
        printf("Raw temperature value: %d\n", raw_value);

        // TODO3: Convert and display temperature in Celsius
        // According to nRF52840 documentation: each increment = 0.25°C
        int temp_celsius = raw_value * 0.25;
        printf("Temperature: %d °C\n", temp_celsius);

        // TODO1 réarmez le temporisateur pour le cycle suivant
        etimer_reset(&t);
    }

    // TODO2 désactivez le capteur
    SENSORS_DEACTIVATE(temperature_sensor);

    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
