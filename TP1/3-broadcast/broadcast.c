#include "contiki.h"
#include "dev/button-hal.h"
#include "net/netstack.h"
#include "net/nullnet/nullnet.h"
#include <stdio.h>
#include <string.h>

#define BUFSIZE 256

// TODO1 création structure message
typedef struct message_t {
    uint8_t id;
    char buf[BUFSIZE];
} message;
 
message m;

/*---------------------------------------------------------------------------*/
PROCESS(broadcast_button_process, "Broadcast on Button Release");
AUTOSTART_PROCESSES(&broadcast_button_process);
/*---------------------------------------------------------------------------*/

// fonction de réception des messages
void input_callback(const void *data, uint16_t len, const linkaddr_t *src,
                    const linkaddr_t *dst)
{

    // TODO6 afficher la source ainsi que les informations contenues dans le
    // message reçu
    printf("Reçu : %s De :", (char*)data);
    for (size_t i = 0; i < LINKADDR_SIZE; i++)
    {
        printf("%02x",src->u8[i]);
    }
 
    

}

PROCESS_THREAD(broadcast_button_process, ev, data)
{
    PROCESS_BEGIN();

    // initialiser le callback de réception
    nullnet_set_input_callback(input_callback);

    printf("Processus de diffusion démarré, en attente du bouton.\n");

    // TODO2 créer le message à envoyer
    strcpy(m.buf, "TEST");

    while (1) {
        // TODO3 attendre un événement de type "relâchement de bouton"
        PROCESS_WAIT_EVENT_UNTIL(button_hal_release_event);


        // TODO4 vérifier que l'événement est déclenché par le bouton 1 (par
        // ex.)
        button_hal_button_t *button = (button_hal_button_t *)data;
        uint8_t id = button->unique_id;
        if(id == 0){
      
            // TODO5 positionnez les variables associées à la couche nullnet
            nullnet_buf = (uint8_t*)&m;
            nullnet_len = strlen(m.buf)+2;

            // envoi du message
            NETSTACK_NETWORK.output(NULL);
            printf("Message envoyé : %s (%d)\n", nullnet_buf, nullnet_len);
        }
    }

    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
