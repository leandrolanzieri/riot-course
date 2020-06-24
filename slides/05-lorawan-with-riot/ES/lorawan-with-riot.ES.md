class: center, middle

# LoRaWAN con RIOT

---

## Protocolos IoT

.center[
  iser  <img src="../images/iot-protocols-overview.png" alt="" style="width:800px;"/>
]

---

## Vista general de LoRaWAN

<br>

- Diseñado por Semtech y promovido por la LoRa Alliance


<br>
.center[
    <a href=https://www.semtech.com/><img src="../images/semtech.png" alt="" style="width: 200px;"/></a>
    <a href=http://www.lora-alliance.org/><img src="../images/lora-alliance.png" alt="" style="width: 200px;"/></a>
]

<br>

- Primer release 1.0 de la especificación LoRaWAN en 2015

- Última release 1.1 en 2018

- Basado en la modulación de radio de largo alcance, LoRa

- Topología de red estrella &#x21d2; los dispositivos hablan con la red a través de gateways

---

## Unas palabras sobre LoRa

<table style="width:100%;margin-top:-40px">
<tr>
  <td><ul>
  <li>Tecnología de radio de largo alcance<br/><br/></li>
  <li>Modulación de espectro disperso (spread spectrum):<br/><br/>&#x21d2; "Chirp Spread Spectrum"<br/><br/></li>
  <li>Robusto al ruido, multipath y efecto Doppler</li>
  </ul></td>
</tr>
<tr>
  <td><ul>
        <li>Aumentar el factor de dispersión (SF):
        <br/><br/>
            <ul>
                <li>aumenta el rango (a varios kilómetros)</li>
                <li>aumenta el tiempo en el aire</li>
                <li>aumenta el consumo de energía</li>
            </ul>
        </li>
    </ul>
  </td>
</tr>
</table>

---

## El protocolo LoRaWAN

.center[<img src="../images/lorawan-classes.jpg" alt="" style="width: 500px;"/><br/>]

<br>

- Diferentes bandas de frecuencias dependiendo de la región geográfica

- Utiliza la modulación LoRa

- 3 clases de dispositivos &#x21d2; A, B & C

- La capa de aplicación está directamente sobre la capa MAC

---

## Acceso a la capa física

.center[
    <img src="../images/lorawan-frequency.png" alt="" style="width: 600px;"/>
]

<br>

- Se usan **bandas ISM**, públicas y libres: EU868 (ETSI), US915, etc

- Las bandas están divididas en **canales** de 3 diferentes anchos: 125 KHz, 250 KHz 500 KHz

- Acceso con restricción de tiempo a la capa física &#x21d2; **Ciclo de trabajo** (1% / canal)

- Ejemplo: por lo menos 16 canales deben ser usados en la banda EU868

---

## Tiempo en el aire

Depende de múltiples factores: payload, ancho de banda, factor de dispersión, etc.

Calculando el tiempo en el aire:
- [LoRa Airtime Calculator spreadsheet](https://docs.google.com/spreadsheets/d/1voGAtQAjC1qBmaVuP1ApNKs1ekgUjavHuVQIXyYSvNc/edit#gid=0)
- [lorawan_toa package](https://github.com/tanupoo/lorawan_toa)
- [Equations from LoRa designer's guide](https://www.semtech.com/uploads/documents/LoraDesignGuide_STD.pdf)

---

## Dispositivos de clase A, B & C

<table style="width:100%">
<tr>
  <td><b>Dispositivo clase A</b>
  <ul>
  <li>Sólo puede recibir mensajes luego de enviar</li>
  <li>Menor consumo de energía</li>
  <li>Puede utilizarse a batería</li>
  </ul>
  
  </td>
  <td><img src="../images/lorawan-class-a.png" alt="" style="width: 350px;"/><br/>
  .center[<img src="../images/lorawan-consumption.png" alt="" style="width: 250px;"/><br/>
<span style="left-margin: 10px;font-style: italic;font-size:12px;text-align:right">Consumo de energía de un dispositivo clase A</span>]</td>
</tr>
</table>

<table style="width:100%">
<tr>
  <td><b>Dispositivo clase C</b>
  <ul>
  <li>Siempre escuchando: baja latencia</li>
  <li>Más consumo de energía</li>
  <li>No puede utilizarse a batería</li>
  </ul>
  </td>
  <td><img src="../images/lorawan-class-c.png" alt="" style="width: 350px;"/><br/></td>
</tr>
<table>

---

## Arquitectura de una red LoRaWAN

.center[
    <img src="../images/lorawan-system.png" alt="" style="width: 600px;"/><br/>
]

- **Dispositivos y gateways** intercambian mensajes utilizando comunicación LoRa

- Los **Gateways** se conectan con el 'network server' a través de protocolos de Internet normales

- Los usuarios pueden acceder a la información a través de una aplicación conectada al 'network server'

- La seguridad de los datos está garantizada por encripción **AES** (llaves simétricas)

---

## Vista estructural de las partes de la red

.center[
    <img src="../images/lorawan-network.png" alt="" style="width: 700px;"/><br/>
]

<table style="width:100%">
<tr>
  <td><b>Fabricantes de Gateways</b>
  <ul>
  <li>Gateway IMST Lite https://shop.imst.de</li>
  <li>Kerlink https://www.kerlink.fr/</li>
  <li>Multitech: https://www.multitech.com/</li>
  </ul>
  </td>
  <td><b>Implementaciones de 'Network servers'</b>
  <ul>
  <li>https://www.loraserver.io/ (código abierto)</li>
  <li>https://www.resiot.io/en/<br><br><br></li>
  </ul>
  </td>
</tr>
<table>

---

## Cómo programar un dispositivo

Implementaciones de código abierto existentes:

- Arduino LMIC https://github.com/matthijskooijman/arduino-lmic
  &#x21d2; casi no mantenida

- Loramac-node https://github.com/Lora-net/LoRaMac-node
  &#x21d2; implementación de referencia, utilizada para la certificación de LoRa Alliance

- GNRC LoRaWAN (RIOT)
  &#x21d2; implementación propia de RIOT

<br>

Soporte de alto nivel para el dispositivo final (generalmente basadas en Loramac-node):

- ARM mbedOS: https://www.mbed.com/en/platform/mbed-os/

- Mynewt: https://mynewt.apache.org/

- Micropython: https://pycom.io/

- RIOT: https://riot-os.org/

---

## Comunicación del dispositivo en la red

.center[
    <img src="../images/lorawan-encryption.png" alt="" style="width: 650px;"/><br/>
]

<br>

- Cada dispositivo es identificado con una dirección de 4 bytes

- "Network session key" &#x21d2; utilizada para encriptar los datos relacionados con la red (MAC)

- "Application session key" &#x21d2; utilizada para encriptar información relacionada con la aplicación

---

## Procedimientos de activación

.center[Para intercambiar datos, todos los dispositivos deben ser activados por la red]

&#x21d2; 2 tipos de procedimientos de activación:

- Over-The-Air Activación (OTAA)

- Activation By Personnalization (ABP)

.center[
    <img src="../images/loraotaa.jpg" alt="" style="width: 600px;"/><br/>
]

---

## Procedimientos de activación

- en **OTAA**:
   - Requiere 'Device EUI', 'Application EUI' y 'Application Key'

   - El dispositivo inicia el handshake con el servidor para obtener su dirección y un
   "nonce" &#x21d2; la dirección del dispositivo cambia luego de cada activación

   - Las dos llaves de sesión se derivan de la llave de la aplicación y el "nonce"

- en **ABP**:

   - Requiere 'Application session key', 'Network session key y
   'Device address'

   - No se requiere un handshake

---

## Operadores de red

Muchos operadores de red públicos:

- Actility https://www.actility.com/

- Loriot https://www.loriot.io/

- Objenious (Bouygues Telecom) http://objenious.com/

- Orbiwise  https://www.orbiwise.com/

- TheThingsNetwork https://www.thethingsnetwork.org/

.center[
    <img src="../images/ttn-logo.svg" alt="" style="width: 200px;"/><br/>
]

---

## TheThingsNetwork (TTN)

- El despliegue de la red es **comunitario**

- El stack de software es de código abierto

.center[
    <img src="../images/ttn-map.png" alt="" style="width: 650px;"/><br/>
]

- Acceso ilimitado al backend

   - no hay límites de dispositivos

   - no hay límites de mensaje (respecto al ciclo de trabajo)

   - API amigable (MQTT)

---

## Primeros pasos con TTN: práctica

1. Crear una cuenta<br>
  https://account.thethingsnetwork.org/register

  Los gateways y la aplicación se controlan desde la consola web: https://console.thethingsnetwork.org/

2. Controlando los gateways (opcional)<br>
  https://www.thethingsnetwork.org/docs/gateways/registration.html

3. Agregar una nueva aplicación<br>
  https://console.thethingsnetwork.org/applications

4. Registrar el dispositivo en la aplicación<br>
  https://console.thethingsnetwork.org/applications/tuaplicacion

---

## Probar TTN con RIOT: práctica (1)

- <a href="http://doc.riot-os.org/group__pkg__semtech-loramac.html">Documentación de LoRaWAN en RIOT</a>

- Compilar y correr la aplicación provista por RIOT (en `~/riot-course/RIOT`)

```sh
make -C examples/gnrc_lorawan flash term
```

- Obtener el número de la interfaz de red LoRaWAN utilizando el shell de RIOT

```sh
> ifconfig
```

- Configurar el dispositivo

```sh
> ifconfig 4 set deveui 0000000000000000
> ifconfig 4 set appeui 0000000000000000
> ifconfig 4 set appkey 00000000000000000000000000000000
```

- Unirse a la red utilizando el procedimiento de activación OTAA (activado por
  defecto)

```sh
> ifconfig 4 otaa
> ifconfig 4 up
```

---

- Luego de 7 segundos verificar la activación (`Link: up`)

```sh
> ifconfig
```

- Enviar (y eventualmente recibir) mensajes a la red

```sh
> send 4 "Hola mundo!"
Tx done
```

---

## Integración con servicios externos

- Utilizar las APIs http y/y MQTT API para obtener los datos

- Fácil de integrar

- Servicios disponibles

  - Dashboards personalizables con Cayenne<br>
    https://mydevices.com/

  - Servicio de localización con Collos<br>
    http://preview.collos.org/

  - Recolectar y analizar el uso de espacios de trabajo y sensores con OpenSensors<br>
    https://opensensors.com/

  - Guardar los datos IoT con TheThingsIndustries<br>
    https://www.thethingsindustries.com/

---

## Un ejemplo: Cayenne

https://mydevices.com/cayenne/docs/lora/#lora-the-things-network

- Crear dashboards con los datos LoRaWAN con unos clicks

- Acceder a los datos de los sensores desde donde sea

- Se debe utilizar el formato Low Power Payload (LPP)

  - Biblioteca para python/micropython:<br>
  https://github.com/jojo-/py-cayenne-lpp

  - Biblioteca para Arduino (C++):<br>
  https://github.com/sabas1080/CayenneLPP

  - Biblioteca genérica en C<br>
  https://github.com/riot-os/cayenne-lpp

---

## GNRC LoRaWAN

- Implementación propia de LoRaWAN en RIOT

- Clase A, región EU868, versión 1.0.3

- Integrado con componentes de GNRC (buffer de paquetes, registros, comunicación
  asincrónica, etc).

---

## Conceptos básicos de GNRC

- Paquetes son representados a través de un buffer de paquetes (GNRC Pktbuf).

- Cada componente de GNRC (`gnrc_ipv6`, `gnrc_udp`, `gnrc_netif`) es un thread

- Comunicación asincrónica entre threads a través de colas de mensajes
    - Interfaz única entre componentes (GNRC NetAPI)

- Un paquete puede ser ser dirigido a un componente en específico o
  a un grupo de componentes registrados a un tipo de paquete (GNRC Netreg).

---

## GNRC Pktbuf

- Un paquete está representado por uno o más "snips" (corte) representados
  como una lista enlazada

- Cada "snip" tiene un tipo, un puntero a un buffer y largo.

- Operaciones del buffer de paquetes:
    - Alocar un snip
    - Dividir un snips
    - Eliminar un snip

---

## GNRC Pktbuf

Algunas funciones útiles:

```c
/* Alocar un paquete (snip) */
gnrc_pktsnip_t *pkt = gnrc_pktbuf_add(NULL, buffer, 64, GNRC_NETTYPE_LORAWAN);

/* Acceder a la información asociada al snip */
uint8_t *packet = pkt->data;
size_t packet_size = pkt->size;
gnrc_pktsnip_t *next_snip = pkt->next;

/* Dividir un snip. `pkt` apunta el nuevo snip */
gnrc_pktsnip_t *ipv6_snip = gnrc_pktbuf_mark(pkt, 10, GNRC_NETTYPE_UDP);

/* Liberar un paquete (y todos los snips enlazados) */
gnrc_pktbuf_release(pkt);
```
Documentación:
https://doc.riot-os.org/group__net__gnrc__pktbuf.html

--- 

## GNRC NetAPI

Contiene funciones para:

- Transmitir paquetes entre components de GNRC

- Configurar un componente de GNRC a través de patrones Get/Set 

---

## GNRC NetAPI

Funciones interesantes para efectos del curso

```c
/* Delegar envío de un paquete (uplink) a un componente de GNRC */
gnrc_netapi_send(pid, pkt)

/* Obtener un parámetro de configuración de un componente de GNRC */
gnrc_netapi_get(netif_pid, NETOPT_CHANNEL, 0, &channel, sizeof(channel));

/* Setear un parámetro de configuración a un componente de GNRC */
gnrc_netapi_set(netif_pid, NETOPT_ADDRESS, 0, &addr, sizeof(addr));
```

Documentación
https://doc.riot-os.org/group__net__gnrc__netapi.html

Listado de opciones:
https://doc.riot-os.org/netopt_8h.html

---

## GNRC Netreg

- NetAPI permite delegar paquetes a un grupo de componentes (threads) que
estén registrados a un tipo de paquetes.

```c
/* Delegar paquete a uno o más thread registrados a paquetes de tipo LoRaWAN */
gnrc_netapi_dispatch(GNRC_NETTYPE_LORAWAN, demux_ctx, cmd, pkt);
```

- Para registrar un componente:

```c
gnrc_netreg_entry_t entry = GNRC_NETREG_ENTRY_INIT_PID(demux_ctx,
                                                      pid);
gnrc_netreg_register(GNRC_NETTYPE_LORAWAN, &entry);
```

---

## Ejercicio: Aplicación Cayenne LPP

- **Ejercicio:** `~/riot-course/exercises/riot-lorawan/lpp-node`

Seguir el [README del ejercicio](https://github.com/riot-os/riot-course/tree/master/exercises/riot-lorawan/lpp-node)

---

# The End

[Volver al curso](https://github.com/riot-os/riot-course#content-of-the-course)
