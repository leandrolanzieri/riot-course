# name of your application
APPLICATION = gnrc_lorawan

RIOTBASE ?= ../../../../RIOT
BOARD ?= b-l072z-lrwan1

USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif
USEMODULE += gnrc_lorawan

# Use SX1272 LoRa module
USEMODULE += sx1272

# Use BMP280 sensor with I2C configuration
USEMODULE += bme280_i2c

# Use Cayenne LPP package
USEPKG += cayenne-lpp

# Turn on developer helpers
DEVELHELP ?= 1

DEVEUI ?= 0000000000000000
APPEUI ?= 0000000000000000
APPKEY ?= 00000000000000000000000000000000

# OTAA compile time configuration keys
CFLAGS += -DDEVEUI=\"$(DEVEUI)\"
CFLAGS += -DAPPEUI=\"$(APPEUI)\"
CFLAGS += -DAPPKEY=\"$(APPKEY)\"

# Use OTAA join procedure by default
CFLAGS += -DLORAMAC_DEFAULT_JOIN_PROCEDURE=LORAMAC_JOIN_OTAA

# Set default messages to unconfirmable
CFLAGS += -DLORAMAC_DEFAULT_TX_MODE=LORAMAC_TX_UNCNF

include $(RIOTBASE)/Makefile.include
