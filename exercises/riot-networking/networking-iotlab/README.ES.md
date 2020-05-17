## RIOT y redes en hardware real

### Introducción

Este ejercicio demuestra las características de interoperabilidad de RIOT: se
utilizará la misma aplicación de ejemplo y el mismo código escrito en los
ejercicios anteriores, pero esta vez en placas reales provistas por FIT IoT-LAB.

### Pre-requisitos

Seguir los
[requisitos para FIT IoT-LAB](https://riot-os.github.io/riot-course/slides/prerequisites/ES/#4)
del riot-course.

Listo, está todo listo para utilizar RIOT en el testbed FIT IoT-LAB.

### Configurar un experimento en FIT IoT-LAB

- Detener cualquier experimento que se esté corriendo (usar `-i` si ya hay
  múltiples experimentos corriendo):

    $ iotlab-experiment stop

- Reservar 2 placas iotlab-m3. Se pueden utilizar los sitios Saclay, Paris,
  Lille, Strasbourg o Grenoble:

    $ iotlab-experiment submit -n riot_course -d 120 -l 2,archi=m3:at86rf231+site=paris
    {
        "id": 164500
    }
    $ iotlab-experiment wait
    "Running"

  Si el subcomando `wait` toma mucho tiempo, verificar la
  [actividad del testbed](https://www.iot-lab.info/testbed/drawgantt). Quizás no
  quedan placas disponibles en el sitio elegido: seleccionar placas M3 en otro
  sitio (saclay, grenoble, lille, strasbourg).

- Verificar los dispositivos reservados en el experimentos (usar el
  identificador correspondiente al experimento):

    $ iotlab-experiment get -r -i 164500
    {
        "items": [
            {
                "archi": "m3:at86rf231",
                "mobile": "0",
                "mobility_type": " ",
                "network_address": "m3-10.paris.iot-lab.info",
                "site": "paris",
                "state": "Alive",
                "uid": "b375",
                "x": "1.63",
                "y": "0",
                "z": "0.55"
            },
            {
                "archi": "m3:at86rf231",
                "mobile": "0",
                "mobility_type": " ",
                "network_address": "m3-11.paris.iot-lab.info",
                "site": "paris",
                "state": "Alive",
                "uid": "3662",
                "x": "2.28",
                "y": "0",
                "z": "1.12"
            }
        ]
    }

  En este experimento 2 placas fueron reservadas (observar el campo
  **network_address**):
  - m3-10.paris.iot-lab.info
  - m3-11.paris.iot-lab.info

- Es posible programar una placa remotamente en FIT IoT-LAB utilizando el
  sistema de compilación de RIOT:

  > Asignar la dirección (network_address) de la placa a la variable 
  > `IOTLAB_NODE` al llamar el comando `make`

### Probando comandos básicos de redes en el shell

1. En una primera terminal, compilar, programar y abrir una terminal serie con
   la aplicación de ejemplo `gnrc_networking` para la placa `iotlab-m3`:

  ```sh
  $ make BOARD=iotlab-m3 IOTLAB_NODE=m3-10.paris.iot-lab.info -C ~/riot-course/RIOT/examples/gnrc_networking flash term
  ```

2. En una segunda terminal, adaptar el mismo comando con la dirección
   (`network_address`) de la segunda placa (en este caso
   `m3-11.paris.iot-lab.info`). Mantener ambas terminales abiertas.

3. Ahora se puede verificar la configuración de red en ambas placas:

  ```sh
  > ifconfig
  > ifconfig
  Iface  7  HWaddr: 29:22  Channel: 26  Page: 0  NID: 0x23
            Long HWaddr: 15:11:6B:10:65:F8:A9:22
             TX-Power: 0dBm  State: IDLE  max. Retrans.: 3  CSMA Retries: 4
            AUTOACK  ACK_REQ  CSMA  L2-PDU:102 MTU:1280  HL:64  RTR
            6LO  IPHC
            Source address length: 8
            Link type: wireless
            inet6 addr: fe80::1711:6b10:65f8:a922  scope: local  VAL
            inet6 group: ff02::2
            inet6 group: ff02::1
            inet6 group: ff02::1:fff8:a922
            inet6 group: ff02::1a

            Statistics for Layer 2
              RX packets 0  bytes 0
              TX packets 3 (Multicast: 3)  bytes 115
              TX succeeded 3 errors 0
            Statistics for IPv6
              RX packets 0  bytes 0
              TX packets 3 (Multicast: 3)  bytes 178
              TX succeeded 3 errors 0
  ```

## Intercambio de paquetes ICMPv6 (ping)

Verificar que el ping enre ambas placas funciona: usar la dirección IPv6 con
`scope: local`:
```sh
> ping6 fe80::1711:6b10:65f8:a922
12 bytes from fe80::1711:6b10:65f8:a922: icmp_seq=0 ttl=64 rssi=-63 dBm time=6.348 ms
12 bytes from fe80::1711:6b10:65f8:a922: icmp_seq=1 ttl=64 rssi=-63 dBm time=7.308 ms
12 bytes from fe80::1711:6b10:65f8:a922: icmp_seq=2 ttl=64 rssi=-63 dBm time=6.348 ms

--- fe80::1711:6b10:65f8:a922 PING statistics ---
3 packets transmitted, 3 packets received, 0% packet loss
round-trip min/avg/max = 6.348/6.668/7.308 ms
```

## Intercambio de paquetes UDP

1. Utilizar el comando `udp` para iniciar un servidor UDP en el puerto `8888` en
   la primera placa (aquí `m3-10`):
  ```sh
  > udp server start 8888
  Success: started UDP server on port 8888
  ```

2. Enviar un paquete UDP desde la segunda placa (`m3-11`):
  ```sh
  > udp send fe80::1711:6b10:65f8:a922 8888 "Hello Board"
  Success: sent 11 byte(s) to [fe80::1711:6b10:65f8:a922]:8888
  ```

3. Verificar que el paquete ha sido recibido en la primera placa. Un mensaje
   similar a lo siguiente debería aparecer:
  ```sh
  PKTDUMP: data received:
  ~~ SNIP  0 - size:  11 byte, type: NETTYPE_UNDEF (0)
  00000000  48  65  6C  6C  6F  20  42  6F  61  72  64
  ~~ SNIP  1 - size:   8 byte, type: NETTYPE_UDP (4)
     src-port:  8888  dst-port:  8888
     length: 19  cksum: 0x5a02
  ~~ SNIP  2 - size:  40 byte, type: NETTYPE_IPV6 (2)
  traffic class: 0x00 (ECN: 0x0, DSCP: 0x00)
  flow label: 0x00000
  length: 19  next header: 17  hop limit: 64
  source address: fe80::1711:6b10:65fb:be26
  destination address: fe80::1711:6b10:65f8:a922
  ~~ SNIP  3 - size:  24 byte, type: NETTYPE_NETIF (-1)
  if_pid: 7  rssi: -65  lqi: 255
  flags: 0x0
  src_l2addr: 15:11:6B:10:65:FB:BE:26
  dst_l2addr: 15:11:6B:10:65:F8:A9:22
  ~~ PKT    -  4 snips, total size:  83 byte
  ```

## Probando tu propia aplicación

De manera similar a la aplicación `gnrc_networking`, se pueden probar las
aplicaciones de los ejercicios [sock-udp](../sock-udp), [posix-udp](../posix-udp)
y [gcoap](../gcoap), en las 2 placas del experimento.

Ahora, continuemos con las
[diapositivas de redes](https://riot-os.github.io/riot-course/slides/04-networking-in-riot/ES/#28).
