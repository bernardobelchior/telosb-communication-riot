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
#include "xtimer.h"
#include "net/gnrc/netif.h"
#include "net/gnrc/ipv6/netif.h"
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"
#include "fmt.h"
#include "led.h"

#define DATA_SIZE (64)

int monitor(char *addr, int port)
{
    sock_udp_ep_t remote = {.family = AF_INET6};

    if (ipv6_addr_from_str((ipv6_addr_t *)&remote.addr, addr) == NULL)
    {
        return -1;
    }

    remote.port = port;

    while (1)
    {
        float temp_val, relhum_val, relhum_temp_val;
        temp_val = 43.21;
        relhum_val = 23.0;
        relhum_temp_val = 27.0;

        char data[DATA_SIZE];
        char temp[8], relhum[8], relhum_temp[8];

        fmt_float(temp, temp_val, 2);
        fmt_float(relhum, relhum_val, 2);
        fmt_float(relhum_temp, relhum_temp_val, 2);

        int data_len = sprintf(data, "%s\t%s\t%s", temp, relhum, relhum_temp);
        printf("%s", data);
        int res;
        if ((res = sock_udp_send(NULL, data, data_len, &remote)) <= 0)
        {
            LED0_ON;
            puts("Error sending packet!");
        }
        else
        {
            LED1_ON;
            printf("Sent packet with %d bytes!\n", res);
        }

        xtimer_sleep(2);
        LED0_OFF;
        LED1_OFF;
    }

    return 0;
}

int main(void)
{
    monitor("ff02::1", 8888);

    return 0;
}