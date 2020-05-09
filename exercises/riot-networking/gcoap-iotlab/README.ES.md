## Exponer datos de un sensor via CoAP

### Introducción

En este ejercicio proponemos exponer los valores de temperatura y presión
atmosférica de un sensor via CoAP.
Se comenzará un experimento con 2 placas iotlab-m3 en el testbed FIT IoT-LAB.
Las placas iotlab-m3 proveen un sensor LPS331ap, ya soportado por RIOT, asíque
será utilizado para leer la temperatura y la presión atmosférica.

### Pre-requisitos

1. Seguir los
  [pre-requisitos para FIT IoT-LAB](https://riot-os.github.io/riot-course/slides/prerequisites/#4)
  si aún no se lo ha hecho.
2. [Iniciar un experimento](https://github.com/riot-os/riot-course/tree/master/exercises/riot-networking/networking-iotlab#setup-an-experiment-on-fit-iot-lab)

Durante el resto del ejerccio se asume que se han reservado 2 placas `iotlab-m3`
en el sitio `paris`:
- m3-10.paris.iot-lab.info
- m3-11.paris.iot-lab.info

### Programar la aplicación de ejemplo para RIOT gcoap

Esta aplicación está disponible en GitHub
[aquí](https://github.com/riot-os/riot-course/tree/master/exercises/RIOT/examples/gcoap).
En la máquina virtual, se puede acceder a él en `~/riot-course/RIOT/examples/gcoap`.

1. En la primera placa (aquí **m3-10**), compilar, programar y abrir una terminal
   de la aplicación de ejemplo `gcoap`:
  ```sh
  $ make BOARD=iotlab-m3 IOTAB_NODE=m3-10.paris.iot-lab.info -C ~/riot-course/RIOT/examples/gcoap flash term
  ```

2. Asegurarse de que el shell funciona:
```
> help
Command              Description
---------------------------------------
coap                 CoAP example
reboot               Reboot the node
ps                   Prints information about running threads.
ping6                Ping via ICMPv6
random_init          initializes the PRNG
random_get           returns 32 bit of pseudo randomness
nib                  Configure neighbor information base
ifconfig             Configure network interfaces
> coap
usage: coap <get|post|put|info>
```

### Escribir la aplicación de servidor CoAP

1. Posicionarse en `~/riot-course/riot-networking/gcoap-iotlab`

2. Editar el `Makefile` de la aplicación: agregar los módulos faltantes para
   el driver del sensor, ej. `lps331ap`.

3. Editar el archivo `main.c` de la aplicación:
  - Agregar las inclusiones para el driver del sensor:
  ```c
  #include "lpsxxx.h"
  #include "lpsxxx_params.h"
  ```
  - Declarar la variable del sensor:
  ```c
  static lpsxxx_t sensor;
  ```
  - Implementar la función `_riot_pressure_handler`:
    - Iniciar la respuesta coap:
    ```c
    gcoap_resp_init(pdu, buf, len, COAP_CODE_CONTENT);
    coap_opt_add_format(pdu, COAP_FORMAT_TEXT);
    size_t resp_len = coap_opt_finish(pdu, COAP_OPT_FINISH_PAYLOAD);
    ```
    - Formar el mensaje de respuesta:
    ```c
    char response[32];
    uint16_t pres;
    lpsxxx_read_pres(&sensor, &pres);
    sprintf(response, "%ihPa", pres);
    ```
    - Retornar la respuesta al que realizó la solicitud
    ```c
    if (pdu->payload_len >= strlen(response)) {
        memcpy(pdu->payload, response, strlen(response));
        return resp_len + strlen(response);
    }
    else {
        puts("gcoap: msg buffer too small");
        return gcoap_response(pdu, buf, len, COAP_CODE_INTERNAL_SERVER_ERROR);
    }
    ```
  - Seguir la misma estrategia para implementar la función
    `_riot_temperature_handler`.

4. Verificar que compile y programarla en la segunda placa (aquí **m3-11**):
```sh
  $ make BOARD=iotlab-m3 IOTAB_NODE=m3-11.paris.iot-lab.info flash term
```

5. Observar la dirección IP link-local de `m3-11` que se imprime al inicio:
```
  Configured network interfaces:
  Iface  7  HWaddr: 3E:26  Channel: 26  Page: 0  NID: 0x23
      Long HWaddr: 15:11:6B:10:65:FB:BE:26 
        TX-Power: 0dBm  State: IDLE  max. Retrans.: 3  CSMA Retries: 4 
      AUTOACK  ACK_REQ  CSMA  L2-PDU:102 MTU:1280  HL:64  6LO  
      IPHC  
      Source address length: 8
      Link type: wireless
      inet6 addr: fe80::1711:6b10:65fb:be26  scope: local  VAL
      inet6 group: ff02::1
```
  En este caso la dirección es **fe80::1711:6b10:65fb:be26**.

6. Leer la temperatura y presión desde la primera placa (aquí **m3-10**):
```
  > coap get fe80::1711:6b10:65fb:be26 5683 /temperature
  gcoap: response Success, code 2.05, 8 bytes
  39.36°C
  > coap get fe80::1711:6b10:65fb:be26 5683 /pressure
  gcoap: response Success, code 2.05, 7 bytes
  1013hPa
```

Ahora sigamos con las
[diapositivas de red](https://riot-os.github.io/riot-course/slides/04-networking-in-riot/ES/#25).
