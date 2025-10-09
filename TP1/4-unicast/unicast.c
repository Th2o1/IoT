#include "contiki.h"
#include "net/netstack.h"
#include "net/nullnet/nullnet.h"
#include "sys/log.h"
#include "etimer.h"
#include <stdio.h>
#include <string.h>

#define LOG_MODULE "Unicast"
#define LOG_LEVEL LOG_LEVEL_INFO

#define BUFSIZE 256

/**
 * @brief Structure of a message
 * @var seq_num ID of the message
 */
struct message
{
  uint8_t seq_num;
  char text[BUFSIZE];
} __attribute__((packed));

struct message m;

static linkaddr_t dest_addr = {{0xf5, 0xce,
                                0x36, 0x5a,
                                0xb2, 0xbe,
                                0x02, 0x74}};

/*---------------------------------------------------------------------------*/
PROCESS(unicast_process, "Unicast");
AUTOSTART_PROCESSES(&unicast_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(unicast_process, ev, data)
{
  static struct etimer t;

  PROCESS_BEGIN();
  strcpy(m.text, "TEST");
  nullnet_buf = (uint8_t *)&m;
  nullnet_len = strlen(m.text) + 2;

  etimer_set(&t, 5 * CLOCK_SECOND);

  while (1)
  {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&t));
    NETSTACK_NETWORK.output(&dest_addr);

    // Print info
    LOG_INFO_("Message sent: %s (%d) to", m.text, m.seq_num);
    LOG_INFO_LLADDR(&dest_addr);
    LOG_INFO_("\n");
    m.seq_num++;
  }

  PROCESS_END();
}
