/*
 * Copyright (C) 2019 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     tests
 * @{
 * @file
 * @brief       Test application for GNRC LoRaWAN
 *
 * @author      José Ignacio Alamos <jose.alamos@haw-hamburg.de>
 * @}
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "thread.h"
#include "xtimer.h"
#include "fmt.h"
#include "shell.h"
#include "shell_commands.h"

#include "board.h"

#include "cayenne_lpp.h"

#include "net/gnrc/netapi.h"
#include "net/gnrc/netif.h"

#include "net/gnrc/pktbuf.h"
#include "net/gnrc/netreg.h"

#include "bmx280_params.h"
#include "bmx280.h"

#define _STACKSIZE      (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)

#define LORAWAN_PORT (1U)  /**< LoRaWAN reception port */
#define LORAWAN_DR   (3U)  /**< LoRaWAN datarate */
#define PERIOD (20U)       /**< TX period */
#define JOIN_DELAY (7U)    /**< Delay for OTAA */

/* Declaration of LoRaWAN keys */
static uint8_t deveui[LORAMAC_DEVEUI_LEN];
static uint8_t appeui[LORAMAC_APPEUI_LEN];
static uint8_t appkey[LORAMAC_APPKEY_LEN];

/* Stack size for receiver thread */
static char stack[_STACKSIZE];
kernel_pid_t _recv_pid;

/* BME280 device descriptor */
bmx280_t dev;

void *receive_thread(void *arg)
{
    (void) arg;

    gnrc_pktsnip_t *pkt;

    /* Init message queue for receiver thread */
    msg_t msg, msg_queue[8];
    msg_init_queue(msg_queue, sizeof(msg_queue));

    while(true) {
        /* Wait for any incoming message */
        msg_receive(&msg);
        switch(msg.type) {
            /* gnrc_netapi_dispatch_receive internally sends a message with
             * type GNRC_NETAPI_MSG_TYPE_RCV */
            case GNRC_NETAPI_MSG_TYPE_RCV:

                /* Print received packet */

                /* Release packet */

            default:
                break;
        }
    }
}

void send_packet(kernel_pid_t interface)
{
    /* Declare and reset Cayenne LPP buffer */

    /* Read temperature and humidity */

    /* Add temperature and humidity to Cayenne LPP buffer */

    /* Allocate GNRC packet snip for sending Cayenne LPP payload */

    /* Send packet to LoRaWAN interface */

    /* No need to release the packet on send */
}

void configure_lorawan(kernel_pid_t interface)
{
    /* Convert from hex string representation to bytes */
    fmt_hex_bytes(deveui, DEVEUI); 
    fmt_hex_bytes(appeui, APPEUI); 
    fmt_hex_bytes(appkey, APPKEY); 

    /* Set LoRaWAN keys */

    /* Set LoRaWAN datarate */
}

void join(kernel_pid_t interface)
{
    netopt_enable_t link = NETOPT_DISABLE;

    /* Loop until the link is up */
    do {

        /* Join LoRaWAN network */

        /* Wait for the OTAA Join Procedure to finish */
        xtimer_sleep(JOIN_DELAY);

        /* Get link information */
        gnrc_netapi_get(interface, NETOPT_LINK, 0, &link, sizeof(link));

    }
    while(link != NETOPT_ENABLE);
}

int main(void)
{
    /* start the shell */
    puts("Initialization successful - starting the shell now");

    /* Init BMP280 sensor */

    /* Create thread for receiving LoRaWAN packets */
    _recv_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                              THREAD_CREATE_STACKTEST, receive_thread, NULL,
                              "recv_thread");

    /* Register receiver thread to receive LoRaWAN packets */
    /* Use LORAWAN_PORT as demux context */

    /* Get the first (and only) LoRaWAN interface */
    gnrc_netif_t *netif = gnrc_netif_iter(NULL);
    kernel_pid_t interface = netif->pid;

    /* Call configuration routine */
    configure_lorawan(interface);

    /* Call join routine */
    join(interface);

    /* Send packets every PERIOD seconds */
    while(true) {
        send_packet(interface);
        xtimer_sleep(PERIOD);
    }

    return 0;
}
