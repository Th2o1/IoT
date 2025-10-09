#include "contiki.h"
#include "dev/button-hal.h"
#include "net/netstack.h"
#include "net/nullnet/nullnet.h"
#include <stdio.h>
#include <string.h>
#include <sys/log.h>

#define BUFSIZE 256

#define LOG_MODULE "Broadcast"
#define LOG_LEVEL LOG_LEVEL_INFO

// TODO1 création structure message
struct message
{
    uint8_t seq_num;
    char text[BUFSIZE];
} __attribute__((packed));
;

struct message m;

/*---------------------------------------------------------------------------*/
PROCESS(broadcast_button_process, "Broadcast on Button Release");
AUTOSTART_PROCESSES(&broadcast_button_process);
/*---------------------------------------------------------------------------*/

// fonction de réception des messages
void input_callback(const void *data, uint16_t len, const linkaddr_t *src,
                    const linkaddr_t *dst)
{

    // display the source and the content of the received message
    struct message *msg = (struct message *)data;
    LOG_INFO("Message received: ");
    LOG_INFO_LLADDR(src);
    LOG_INFO_(" - Text: %s, Sequence: %d\n", msg->text, msg->seq_num);
}

PROCESS_THREAD(broadcast_button_process, ev, data)
{
    PROCESS_BEGIN();

    // initialiser le callback de réception
    nullnet_set_input_callback(input_callback);

    printf("Processus de diffusion démarré, en attente du bouton.\n");

    // TODO2 créer le message à envoyer
    strcpy(m.text, "TEST");

    while (1)
    {
        // TODO3 attendre un événement de type "relâchement de bouton"
        PROCESS_WAIT_EVENT_UNTIL(button_hal_release_event);

        // TODO4 vérifier que l'événement est déclenché par le bouton 1 (par
        // ex.)
        button_hal_button_t *button = (button_hal_button_t *)data;
        uint8_t id = button->unique_id;
        if (id == 0)
        {

            // TODO5 positionnez les variables associées à la couche nullnet
            nullnet_buf = (uint8_t *)&m;
            nullnet_len = strlen(m.text) + 2;

            // envoi du message
            NETSTACK_NETWORK.output(NULL);
            LOG_INFO_("Message sent: %s (%d)\n", m.text, m.seq_num);
            m.seq_num++;
        }
    }

    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
