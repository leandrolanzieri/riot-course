
#include <stdio.h>

#include "shell.h"
#include "net/gcoap.h"
#include "chat_contacts.h"

static ssize_t chat_handler(coap_pkt_t *pdu, uint8_t *buf, size_t len, void *ctx);

/* CoAP resources. Must be sorted by path (ASCII order). */
static const coap_resource_t _resources[] = {
    /* Define the list of CoAP resources here, sorted by path (ASCII alphabetical order) */

};

/* Declare the gcoap listener */


static ssize_t chat_handler(coap_pkt_t *pdu, uint8_t *buf, size_t len, void *ctx)
{
    (void)ctx;

    /* Get the method of the request */

    /* Implement the chat POST handler */

}

static int chat_command_handler(int argc, char **argv)
{
    coap_pkt_t pdu;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    size_t len = 0;

    if (argc != 3) {
        printf("usage: %s <contact> <msg>\n", argv[0]);
        return 1;
    }

    /* Look for the requested contact */


    /* Build a new CoAP request */


    ipv6_addr_t addr;
    sock_udp_ep_t remote;

    /* Initialize the remote endpoint */

    return gcoap_req_send(buf, len, &remote, NULL, NULL);
}


static int contact_command_handler(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    puts("=== Registered contacts ===");
    for (unsigned i = 0; i < chat_contacts_get_amount(); i++) {
        printf("- %s\t%s\n", chat_contacts[i].nickname, chat_contacts[i].addr);
    }

    return 0;
}

static const shell_command_t shell_commands[] = {
    { "chat", "send a chat message", chat_command_handler },
    { "contacts", "show the contact list", contact_command_handler },
    { NULL, NULL, NULL }
};

int main(void)
{
    /* register the gcoap listener */

    /* start the shell */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
