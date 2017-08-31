/*
 * Copyright (C) 2008, 2009, 2010  Kaspar Schleiser <kaspar@schleiser.de>
 * Copyright (C) 2013 INRIA
 * Copyright (C) 2013 Ludwig Knüpfer <ludwig.knuepfer@fu-berlin.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       Default application that shows a lot of functionality of RIOT
 *
 * @author      Kaspar Schleiser <kaspar@schleiser.de>
 * @author      Oliver Hahm <oliver.hahm@inria.fr>
 * @author      Ludwig Knüpfer <ludwig.knuepfer@fu-berlin.de>
 *
 * @}
 */

#include <stdio.h>
#include "thread.h"
#include "led.h"
#include "xtimer.h"
#include "net/gnrc/netif.h"
#include "net/gnrc/ipv6/netif.h"
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"

#define MAIN_QUEUE_SIZE (8)
#define DATA_SIZE (64)

#define SERVER_MSG_QUEUE_SIZE (8)
#define SERVER_BUFFER_SIZE (128)

static sock_udp_t sock;
static char server_buffer[SERVER_BUFFER_SIZE];
static msg_t server_msg_queue[SERVER_MSG_QUEUE_SIZE];

int receiver(int argc, char **argv)
{
    (void)(argc);
    (void)(argv);

    sock_udp_ep_t server = {.port = 8888, .family = AF_INET6};
    msg_init_queue(server_msg_queue, SERVER_MSG_QUEUE_SIZE);

    if (sock_udp_create(&sock, &server, NULL, 0) < 0)
    {
        return -1;
    }

    while (1)
    {
        int res;

        LED0_ON;
        if ((res = sock_udp_recv(&sock, server_buffer,
                                 SERVER_BUFFER_SIZE - 1, SOCK_NO_TIMEOUT,
                                 NULL)) > 0)
        {
            LED1_ON;
            server_buffer[res] = '\0';
            printf("%s\n", server_buffer);
        }
        else
        {
            LED2_ON;
            puts("Receive failed");
        }

        LED0_OFF;
        LED1_OFF;
        LED2_OFF;
    }

    return 0;
}

int main(void)
{
    return receiver(0, NULL);
}