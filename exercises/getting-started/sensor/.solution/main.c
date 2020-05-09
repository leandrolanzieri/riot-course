/*
 * Copyright (C) 2018 Inria
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include <stdio.h>
#include <stdlib.h>

#include "shell.h"

#include "board.h"

#include "mpl3115a2.h"
#include "mpl3115a2_params.h"

static mpl3115a2_t dev;

static int temperature_handler(int argc, char **argv)
{
    if (argc != 1) {
        printf("usage: %s\n", argv[0]);
        return 1;
    }

    int16_t temp = 0;
    if (mpl3115a2_read_temp(&dev, &temp) != MPL3115A2_OK) {
        puts("failed to read mpl3115a2 temperature");
        return -1;
    }

    bool negative = (temp < 0);
    if (negative) {
        temp = -temp;
    }

    printf("temperature T:%c%u.%u°C\n",
           (negative ? '-' : ' '), (temp / 10), (temp % 10));
    return 0;
}

static int pressure_handler(int argc, char **argv)
{
    if (argc != 1) {
        printf("usage: %s\n", argv[0]);
        return 1;
    }

    uint32_t pressure;
    uint8_t status;
    if (mpl3115a2_read_pressure(&dev, &pressure, &status) != MPL3115A2_OK) {
        puts("failed to read mpl3115a2 pressure");
        return -1;
    }

    printf("pressure: %u Pa\n", (unsigned int)pressure);
    return 0;
}

static const shell_command_t shell_commands[] = {
    { "temperature", "print mpl3115a2 temperature", temperature_handler },
    { "pressure", "print mpl3115a2 pressure", pressure_handler },
    { NULL, NULL, NULL }
};

int main(void)
{
    puts("shell application reads a sensor");

    if (mpl3115a2_init(&dev, &mpl3115a2_params[0]) != MPL3115A2_OK) {
        puts("Cannot initialize mpl3115a2 sensor");
        return -1;
    }

    if (mpl3115a2_set_active(&dev) != MPL3115A2_OK) {
        puts("[FAILED] activate measurement!");
        return -2;
    }

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
}
