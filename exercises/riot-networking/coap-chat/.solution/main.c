
#include <stdio.h>

#include "shell.h"
#include "net/gcoap.h"
#include "chat_contacts.h"

static ssize_t chat_handler(coap_pkt_t *pdu, uint8_t *buf, size_t len, void *ctx);

/* CoAP resources. Must be sorted by path (ASCII order). */
static const coap_resource_t _resources[] = {
    /* Define the list of CoAP resources here, sorted by path (ASCII alphabetical order) */
    { "/chat", COAP_POST, chat_handler, NULL },
};

/* Declare the gcoap listener */
static gcoap_listener_t _listener = {
    &_resources[0],
    sizeof(_resources) / sizeof(_resources[0]),
    gcoap_encode_link,
    NULL
};

static ssize_t chat_handler(coap_pkt_t *pdu, uint8_t *buf, size_t len, void *ctx)
{
    (void)ctx;

    /* Get the method of the request */
    unsigned method_flag = coap_method2flag(coap_get_code_detail(pdu));
    if (method_flag != COAP_POST) {
        puts("Invalid method in the chat request");
        goto bad_req;
    }

    /* Implement the chat POST handler */
    if (pdu->payload_len > 0) {
        printf("\n[ CHAT ] %.*s\n\n", pdu->payload_len, (char *)pdu->payload);
        return gcoap_response(pdu, buf, len, COAP_CODE_CHANGED);
    }

bad_req:
    return gcoap_response(pdu, buf, len, COAP_CODE_BAD_REQUEST);
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

    chat_contact_t *dst = chat_contacts_get_by_nickname(argv[1]);

    if (!dst) {
        puts("Contact not found");
        return 1;
    }

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_POST, "/chat");

    memcpy(pdu.payload, argv[2], strlen(argv[2]));

    len = gcoap_finish(&pdu, strlen(argv[2]), COAP_FORMAT_TEXT);

    ipv6_addr_t addr;
    sock_udp_ep_t remote;

    remote.family = AF_INET6;
    remote.port = 5683;
    remote.netif = (uint16_t)gnrc_netif_iter(NULL)->pid;

    /* parse destination address */
    if (ipv6_addr_from_str(&addr, dst->addr) == NULL) {
        puts("[CoAP] can't parse IPv6 address");
        return 0;
    }

    memcpy(&remote.addr.ipv6[0], &addr.u8[0], sizeof(addr.u8));

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
    gcoap_register_listener(&_listener);

    /* start the shell */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
