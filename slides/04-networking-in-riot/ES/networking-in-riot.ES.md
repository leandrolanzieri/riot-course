class: center, middle

# Redes en RIOT

---

## IoT en el mundo digital

.center[
    <img src="../images/iot-global-overview.png" alt="" style="width:800px;"/>
]

.right[<span style="font-style: italic;font-size:12px">Fuente: https://www.keysight.com/upload/cmc_upload/All/Slide_IOT_Part_1.pdf</span>]

---

## Panorama de las tecnologías de red para IoT

.center[
    <img src="../images/iot-protocols-overview.png" alt="" style="width:800px;"/>
]

---

## Mapeando las tecnologías IoT a las capas OSI

.center[
    <img src="../images/iot-stacks.png" alt="" style="width:700px;"/>
    <br><br>
    &#x21d2; RIOT soporta ZigBee (6LowPAN), Thread, BLE y WiFi
]

---

## Existen muchas especificaciones

- **Capa física**

  &#x21d2; PLC, BACnet, Z-WAze, BLE, IEEE 802.15.4, LoRa, SigFox, IEEE 802.11

- **Capa de red** interoperabilidad con redes IP

  &#x21d2; 6TiSCH, 6LowPAN, RPL, OLSRv2, AODVv2, LoRaWAN

- **Capa de aplicación** interoperabilidad con la Web

  &#x21d2; CoAP, LwM2M, MQTT, CBOR, DTLS, OSCOAP

- **Nuevos paradigmas**
  
  &#x21d2; Information centric network (ICN)

.right[
    <img src="../images/riot-802154-stack.png" alt="" style="width:210px;margin-top:-50px"/>
]

---

## Arquitectura de red en RIOT

&#x21d2; diseñada para integrar cualquier stack de red en RIOT

<table>
<tr>
  <td>
    <ul>
      <li>La capa de red está desacoplada del hardware mediante la API de <b>netdev</b><br><br></li>
      <li>La capa de aplicación está desacoplada del stack de red mediante la API de <b>sock</b><br/><br/></li>
      <li>Cada driver de dispositivo corre en su <b>propio thread</b><br/><br/></li>
      <li>Los drivers de las radios están implementados en `drivers`<br/><br/></li>
      <li>Las APIs de red están definidas en `sys/include/net` e implementadas en `sys/net`<br/><br/></li>
    </ul>
  </td>
  <td>
.center[
    <img src="../images/riot-network-stack.png" alt="" style="width:200px;"/>
]
  </td>
</tr>
</table>

---

## Stacks IP disponibles

- **GNRC**
  - Stack genérico propio

- **LwIP**
  - stack de red completo
  - provisto como un paquete en `pkg/lwip`

- **emb6**
  - port del stack de red de Contiki sin 'protothreads'
  - provisto como un paquete en `pkg/emb6`

- **OpenThread**
  - Implementación de código abierto de las epecificaciones Thread
  - provisto como un paquete en `pkg/openthread`

---

## Otros stacks

- Stack **Bluetooth Low Energy** (BLE)
  - basado en la biblioteca
    [Apache Mynewt NimBLE](https://github.com/apache/mynewt-nimble)

- Stack **Controller Area Network** (CAN)
  - Modo nativo (SocketCAN) + driver para TJA1042 + STM32 driver de periférico

- **LoRaWAN**:
  - basado en la implementación de referencia de Semtech
    [Loramac-node](https://github.com/Lora-net/LoRaMac-node)
  - provista como un paquete en `pkg/semtech-loramac`

---

## API del driver de dispositivo de red

<table>
<tr>
  <td>
.center[
    <img src="../images/netdev-rx.png" alt="" style="width:450px;"/>
]
<ul>
<li>Los eventos de radio generan interrupciones &#x21d2; contexto de interrupción (ISR)</li>

<li>Se usan mensajes de threads para manejarlos fácilmente<br/>
  &#x21d2; moverse rápidamente desde el contexto ISR al contexto del thread de netdev</li>

<li>
    <a href="http://doc.riot-os.org/group__drivers__netdev__api.html">
        http://doc.riot-os.org/group__drivers__netdev__api.html
    </a>
</li>
</ul>
  </td>
  <td>
.center[
    <img src="../images/riot-network-stack.png" alt="" style="width:200px;"/>
]
  </td>
</tr>
</table>

---

## El stack GNRC de RIOT

.center[
    <img src="../images/riot-gnrc-stack.svg" alt="" style="width:400px;"/>
]

- Los drivers de las radios son accedidos a través de la API **netdev**

- Los componentes internos utilizan una única API: **netapi**

- Las aplicaciones utilizan la API  **sock**

---

## Ejercicio: Manejo básico de red IPv6 con el shell

- **Ejercicio:** `~/riot-course/exercises/riot-networking/shell-basic-native`

Sigue las instrucciones en el
[README](https://github.com/riot-os/riot-course/tree/master/exercises/riot-networking/shell-basic-native/README.ES.md)
del ejercicio.

- **Ejercicio:** `~/riot-course/exercises/riot-networking/shell-udp-native`

Sigue las instrucciones en el
[README](https://github.com/riot-os/riot-course/tree/master/exercises/riot-networking/shell-udp-native/README.ES.md)
del ejercicio.

---

## Módulos de red

- **GNRC:** Todos los módulos de red son cargados con el módulo **gnrc_ipv6_default**

- **lwIP:**

  - Los módulos de red IPv4 son cargados con **lwip_ipv4**

  - Los módulos de red IPv6 son cargados con **lwip_ipv6**

- Módulos de drivers de dispositivos:

  - Drivers 802.15.4: <br/>
    &#x21d2; **at86rf2xx**, **cc2420**, **kw2xrf**, **mrf24j40**, **xbee**

  - Drivers Ethernet: <br>
    &#x21d2; **enc28j60**, **w5100**, **esp32**, **esp8266**

  - Para drivers usar **gnrc_netdev_default** y **auto_init_gnrc_netif**

---

## Manipulación de direcciones IPv6

- Las funciones de manipulación IPv6 son provistas por el módulo `ipv6_addr`

- Ver: http://doc.riot-os.org/group__net__ipv6__addr.html (sólo en inglés)

- Algunas funciones útiles:

```c
/* es loopback (::1) */
static bool ipv6_addr_is_loopback (const ipv6_addr_t *addr);

/* es mulicast (ff02::1, ff02::2, etc) */
static bool ipv6_addr_is_multicast (const ipv6_addr_t *addr);

/* es link local (fe80:x:x:z::/64) */
static bool ipv6_addr_is_link_local (const ipv6_addr_t *addr);

/* es global */
static bool ipv6_addr_is_global (const ipv6_addr_t *addr);

/* convertir de string */
ipv6_addr_t * ipv6_addr_from_str (ipv6_addr_t *result, const char *addr);

/* convertir a string */
char * ipv6_addr_to_str (char *result, const ipv6_addr_t *addr,
                         uint8_t result_len);
```

---

## La API Sock (1)

- Provee una abstracción común sobre cualquier stack IP

- Una aplicación construida sobre Sock es portable a cualquier stack de red

- Hay 3 tipos de básicos de sock provistos:<br/><br/>
  &#x21d2; **Raw IPv4/IPv6**, **UDP** y **TCP**

- Dependiendo del stack utilizado, importar los módulos correspondientes:<br/><br/>
  &#x21d2; con GNRC, los módulos son **gnrc_sock_ip**, **gnrc_sock_udp**, **gnrc_sock_tcp**

- Más información: http://doc.riot-os.org/group__net__sock.html (sólo en inglés)

---

## La API Sock (2)

- 3 tipos diferentes:

  - **sock_ip_t**, incluido desde `net/sock/ip.h`<br>
    &#x21d2; http://doc.riot-os.org/group__net__sock__ip.html

  - **sock_udp_t**, incluido desde `net/sock/udp.h`<br>
    &#x21d2; http://doc.riot-os.org/group__net__sock__udp.html

  - **sock_tcp_t**, incluido desde `net/sock/tcp.h`<br>
    &#x21d2; http://doc.riot-os.org/group__net__sock__tcp.html

- 4 funciones para utilizar (`<type>` puede ser `ip`, `udp` or `tcp`):

```c
/* Crear la instancia de sock */
int sock_<type>_create(sock_<type>_t *sock, const sock_<type>_ep_t *local,
                       const sock_<type>_ep_t *remote, ...);
/* Enviar un paquete utilizando la instancia de sock a un remoto */
ssize_t sock_<type>_send(sock_<type>_t *sock, const void *data, size_t len, ...,
                         const sock_<type>_ep_t *remote);
/* Esperar por paquetes entrantes desde un remoto */
ssize_t sock_<type>_recv(sock_<type>_t *sock, void *data, size_t max_len,
                         uint32_t timeout, sock_<type>_ep_t *remote);
/* Cerrar la instancia de sock */
void sock_<type>_close(sock_<type>_t *sock);
```

---

## Ejercicio: Aplicación cliente/servidor UDP

**Ejercicio:** `~/riot-course/exercises/riot-networking/sock-udp`

Sigue las instrucciones en el
[README](https://github.com/riot-os/riot-course/tree/master/exercises/riot-networking/sock-udp/README.ES.md)
del ejercicio.

---

## La API Posix Socket

- Provee una API de red compatible con Posix

- Importar el módulo **posix_sockets**

- Uso
```c
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
```

- Funciones incluidas en la API Posix Socket:
```c
/* Crear un socket */
int socket(int socket_family, int socket_type, int protocol);
/* Asignar una dirección al socket */
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
/* Esparar por paquetes entrantes */
ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
                 struct sockaddr *src_addr, socklen_t *addrlen);
/* Enviar paquetes */
ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
               const struct sockaddr *dest_addr, socklen_t addrlen);
```

---

## Ejercicio: Aplicación cliente/servidor UDP con Posix

**Ejercicio:** `~/riot-course/exercises/riot-networking/posix-udp`

Sigue las instrucciones en el
[README](https://github.com/riot-os/riot-course/tree/master/exercises/riot-networking/posix-udp/README.ES.md)
del ejercicio.

---

## El protocolo CoAP

- CoAP es un protocolo similar a HTTP diseñado para dispositivos muy pequeños<br/>
  &#x21d2; construido sobre el protocolo UDP, el puerto estándar es **5683**

- Estandar del CORE working group en la IETF<br>
  &#x21d2; RFC 7252 https://tools.ietf.org/html/rfc7252<br>
  &#x21d2; RFC 7959 https://tools.ietf.org/html/rfc7959<br>
  &#x21d2; RFC 8323 https://tools.ietf.org/html/rfc8323<br>

- 4 tipos básicos de consultas: `GET`, `POST`, `PUT`, `DELETE`

- La URI `.well-known/core` describe la lista de URIs en un servidor CoAP

- Otras opciones: `OBSERVE`, transferencias de a bloques (Block-wise transfer)

.center[
    <img src="../images/coap-header.png" alt="" style="width:500px;"/>
]

---

## Implementaciones de CoAP

- Existen múltiples implementaciones, en múltiples lenguajes:<br/><br/>
    &#x21d2; Ver http://coap.technology/impls.html (sólo en inglés)

- RIOT provee soporte para CoAP:

  - De manera native con los módulos **nanocoap** y **gcoap**

  - A través de paquetes con **microcoap**

- **nanocoap**

  - diseñado para ser eficiente en memoria
  - características limitadas

- **gcoap**

  - provee más características: `OBSERVE`, API de cliente CoAP
  - menos eficiente en memoria

---

## Ejercicio: Usando CoAP

- **Ejercicio:** `~/riot-course/exercises/riot-networking/shell-coap-native`

Sigue las instrucciones en el
[README](https://github.com/riot-os/riot-course/tree/master/exercises/riot-networking/shell-coap-native/README.ES.md)
del ejercicio.

---

## Usando la API de gcoap para configurar un servidor CoAP (1)

- Incluye el header requerido
```c
#include "net/gcoap.h"
```
- Define una lista de recursos en un arreglo de `coap_resource_t`:
```c
static const coap_resource_t _resources[] = {
        { "/coap/url", COAP_GET | COAP_PUT, _handler, NULL },
}
```
- La firma de la función que maneja el recurso es:
```c
static ssize_t _handler(coap_pkt_t* pdu, uint8_t *buf,
                            size_t len, void *ctx);
```
- Usa otras funciones de gcoap para formar la respuesta:
```c
/* Iniciar la respuesta */
gcoap_resp_init(coap_pkt_t *pdu, uint8_t *buf, size_t len, unsigned code);
/* Enviar la respuesta */
ssize_t gcoap_response(coap_pkt_t *pdu, uint8_t *buf,
```

---

## Usando la API de gcoap para configurar un servidor CoAP (2)

- Registra tus recursos en el 'listener' de gcoap:
```c
static gcoap_listener_t _listener = {
    &_resources[0],
    sizeof(_resources) / sizeof(_resources[0]),
    NULL
};
```
- Finalemente, inicia el 'listener' de gcoap:
```c
gcoap_register_listener(&_listener);
```

- Encuentra más detalles en http://doc.riot-os.org/group__net__gcoap.html (sólo en inglés)

---

## Ejercicio: Usando gcoap

- **Ejercicio:** `~/riot-course/exercises/riot-networking/gcoap`

Sigue las instrucciones en el
[README](https://github.com/riot-os/riot-course/tree/master/exercises/riot-networking/gcoap/README.ES.md)
del ejercicio.

---

## Operaciones de red con RIOT en hardware: usa IoT-LAB

.center[<a href=https://www.iot-lab.info>https://www.iot-lab.info</a><br><br>
IoT-LAB es un testbed de experimentación de gran escala]

  - Puede utilizarse para ensayos de redes de comunicación inalámbrica en pequeños dispositivos

  - Puede utilizarse para aprender programación de IoT y protocolos de comunicación

  - Puede utilizarse para ensayos de plataformas de software en hardware heterogéneo


.center[
      <img src="../images/iotlab-sites.png" alt="" style="width:220px;"/>&nbsp;
      <img src="../images/iotlab-nodes.png" alt="" style="width:220px;"/>&nbsp;
      <img src="../images/iotlab-photo.png" alt="" style="width:220px;"/>
  ]

---

## IoT-LAB en pocas palabras

- **2300** nodos programables, 4 arquitecturas
  <br> &#x21d2; Cortex-M, MSP430, Cortex-A8, AVR

- Plataformas heterogéneas
  <br> &#x21d2; RPI3, iotlab-m3, Atmel SAMR21-Xpro, ST B-L072Z-LRWAN1, nrf52, etc.

- Radios heterogéneas &#x21d2; 802.15.4, 802.15.4g, LoRa, BLE

- Aprende IoT con IoT-LAB en
  <a href=https://www.iot-lab.info/tutorials/>https://www.iot-lab.info/tutorials/</a>

.center[
      <img src="../images/use-iotlab.png" alt="" style="width:450px;"/>
  ]

---

## Ejercicio: Redes con RIOT en hardware

- **Ejercicio:** `~/riot-course/exercises/riot-networking/networking-iotlab`

Sigue las instrucciones en el
[README](https://github.com/riot-os/riot-course/tree/master/exercises/riot-networking/networking-iotlab/README.ES.md)
del ejercicio.

- **Ejercicio:** `~/riot-course/exercises/riot-networking/gcoap-iotlab`

Sigue las instrucciones en el
[README](https://github.com/riot-os/riot-course/tree/master/exercises/riot-networking/gcoap-iotlab)
del ejercicio.

---

## Ejercicio: Redes con IPv6 pública

Ver: https://www.iot-lab.info/tutorials/riot-public-ipv6-m3/ (sólo en inglés)

---

class: center, middle

[Volver al curso](https://github.com/riot-os/riot-course#content-of-the-course)
