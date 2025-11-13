// dest_addr, etimer and seq_num declarations before process begin
#include "contiki.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include <string.h>
#include <stdio.h>
#include "sys/log.h"
#include "../arch/platform/nrf52840/common/temperature-sensor.h"
#include "etimer.h"

#define PORT_SENDER 8765
#define PORT_RECV 4321

static void
udp_rx_callback(struct simple_udp_connection *c,
                const uip_ipaddr_t *sender_addr,
                uint16_t sender_port,
                const uip_ipaddr_t *receiver_addr,
                uint16_t receiver_port,
                const uint8_t *data,
                uint16_t datalen)
{
}

/*---------------------------------------------------------------------------*/
PROCESS(udp_sender, "Sender UDP");
AUTOSTART_PROCESSES(&udp_sender);
/*---------------------------------------------------------------------------*/

PROCESS_THREAD(udp_sender, ev, data)
{

  static uip_ipaddr_t dest;
  printf("123!");
  uip_ip6addr(&dest,
              0xfe80, 0x0000, 0x0000, 0x0000,
              0xf6ce, 0x365a, 0xb2be, 0x0274);

  // Clock
  static struct etimer t;
  etimer_set(&t, 5 * CLOCK_SECOND);

  SENSORS_ACTIVATE(temperature_sensor);

  PROCESS_BEGIN();
  static struct simple_udp_connection udp_conn;
  simple_udp_register(&udp_conn, PORT_RECV, &dest, PORT_SENDER, udp_rx_callback);
  simple_udp_sendto_port(&udp_conn, "123", 3, &dest, PORT_RECV);
  while (1)
  {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&t));
    printf("BOOOooooOOOOOOOOOOO!");
    int raw_value = temperature_sensor.value(0);
    int temp_celsius = raw_value * 0.25;
    simple_udp_sendto_port(&udp_conn, &temp_celsius, sizeof(temp_celsius), &dest, PORT_RECV);
    etimer_reset(&t);
  }

  SENSORS_DEACTIVATE(temperature_sensor);
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
