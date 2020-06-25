# Aplicación LoRaWAN con Cayenne LPP

## Objetivo

Escribir una aplicación LoRaWAN Clase A y activación OTAA con las siguiente
características:
- La aplicación debe enviar periódicamente datos de temperatura y humedad
  relativa a TTN utilizando formato Cayenne LPP
- La aplicación debe imprimir mensajes enviados desde el servidor de LoRaWAN.

## Instrucciones

- En el dashboard de la aplicación LoRaWAN (TTN), asignar formato de payload
  "Cayenne LPP" en la sección "Payload Formats"

- Registrar el thread de reception (`receiver_thread`) en GNRC NetReg para
  recibir mensajes de tipo LoRaWAN (`GNRC_NETTYPE_LORAWAN`). Utilizar
`LORAWAN_PORT` (1) como contexto de demultiplexación.

```c
    gnrc_netreg_entry_t lorawan_recv = GNRC_NETREG_ENTRY_INIT_PID(LORAWAN_PORT, _recv_pid);
    gnrc_netreg_register(GNRC_NETTYPE_LORAWAN, &lorawan_recv);
```

- Configurar las llaves de LoRaWAN para OTAA (DevEUI, AppEUI, AppKey) utilizando
  la API de GNRC NetAPI.

```c
    gnrc_netapi_set(interface, NETOPT_ADDRESS_LONG, 0, deveui, sizeof(deveui));
    gnrc_netapi_set(interface, NETOPT_LORAWAN_APPEUI, 0, appeui, sizeof(appeui));
    gnrc_netapi_set(interface, NETOPT_LORAWAN_APPKEY, 0, appkey, sizeof(appkey));
```

Las llaves serán asignadas en tiempo de compilación.

Nota: GNRC LoRaWAN utiliza `NETOPT_ADDRESS_LONG` como identificador para DevEUI.

- Asignar un valor Datarate para las transmisiones de datos.

Datarate es un índice que indica una configuración de factor de dispersión (SF)
y ancho de banda (BW).  La representación de cada Datarate depende de la región
de operación. El listado de datarates por región se encuentra en el [documento
de especificación regional de
LoRaWAN](https://lora-alliance.org/sites/default/files/2018-07/lorawan_regional_parameters_v1.0.3reva_0.pdf)

Para efectos del ejercicio, se utilizará Datarate 3 de la región EU868 (SF9,
BW125). Con esta configuración el tiempo en aire para un  7 bytes de datos es
de aproximadamente 185 milisegundos.

```c
    uint8_t dr = LORAWAN_DR;
    gnrc_netapi_set(interface, NETOPT_LORAWAN_DR, 0, &dr, sizeof(dr));
```

- Iniciar el procedimiento the Join a través de OTAA (configurado por defecto)
utilizando GNRC NetAPI.  Para ello, utilizar `NETOPT_LINK`.

```c
    netopt_enable_t en = true;
    gnrc_netapi_set(interface, NETOPT_LINK, 0, &en, sizeof(en));
```

El proceso de join demora alrededor de 6 segundos. Es posible verificar si es
que el procedimiento fue exitoso utilizando la opción `NETOPT_LINK`. Por
ejemplo:

```c
    gnrc_netapi_get(interface, NETOPT_LINK, 0, &link, sizeof(link));
    
    if (link == NETOPT_ENABLE) {
        puts("Se ha realizado el procedimiento OTAA con éxito");
    }
```

- Iniciar sensor BME280

```
    if (bmx280_init(&dev, &bmx280_params[0]) < 0) {
        puts("Error en la inicialización");
        return -EINVAL;
    }
```

- Leer datos de temperatura y humedad del sensor BME280.
Revisar [documentación del driver](https://doc.riot-os.org/bmx280_8h.html) para
conversión de representación internal del driver a valores físicos.

- Cargar valores de temperatura y humedad en buffer de Cayenne LPP.

Un buffer de Cayenne LPP es representado con la estructura `cayenne_lpp_t`.
Antes de utilizar el buffer, debe ser reseteado:

```c
    cayenne_lpp_t lpp;
    cayenne_lpp_reset(&lpp);
```

Para cargar valores de sensores en el buffer de Cayenne LPP, utilizar las
funciones `cayenne_lpp_add_xxx` disponibles en el [header de la
librería](https://github.com/aabadie/cayenne-lpp/blob/master/cayenne_lpp.h).

Para efectos del ejercicio interesan `cayenne_lpp_add_temperature` y
`cayenne_lpp_add_relative_humidity`

- Alocar un snip de GNRC PktBuf que contiene el paquete LoRaWAN a enviar.  Para
  acceder al buffer de Cayenne LPP y al largo de datos a enviar, utilizar
respectivamente los miembros `buffer` y `cursor` de la estructura
`cayenne_lpp_t`. El tipo de snip no es relevante para el envío, por lo que se
utiliza `GNRC_NETTYPE_UNDEF`.

```c
    gnrc_pktsnip_t *pkt = gnrc_pktbuf_add(NULL, lpp.buffer, lpp.cursor, GNRC_NETTYPE_UNDEF);
```

- Enviar paquete LoRaWAN utilizando GNRC NetAPI

```c
    gnrc_netapi_send(interface, pkt);
```

No es necesario liberar el snip luego de llamar a la función de envío.

- En el thread de recepción, imprimir paquetes recibidos.

El paquete recibido se encuentra dentro del contenido del mensaje:
```
    gnrc_pktsnip_t *recv_pkt = msg.content.ptr;
``` 

Para acceder al buffer del paquete recibido, utilizar los miembros `data` y
`size` para el puntero y el largo respectivamente.

No olvidar de liberar el snip después de su uso:

```c
    gnrc_pktbuf_release(pkt);
```

- Compilar y flashear la aplicación.
Las llaves de LoRaWAN se pueden configurar a través del Makefile:

```sh
DEVEUI ?= 0000000000000000
APPEUI ?= 0000000000000000
APPKEY ?= 00000000000000000000000000000000
```

O a través de la línea de comandos en la invocación de `make`:

```sh
DEVEUI=0000000000000000 \
APPEUI=0000000000000000 \
APPKEY=00000000000000000000000000000000 \
BOARD=<board> make all flash term
```

- Verificar en la consola de TTN que los datos se envían sin problemas y que el
  nodo es capaz de recibir datos desde TTN.  Para ello, utilizar sección
"Downlink" de la pestaña Overview en el dashboard del nodo.  No olvidar que la
aplicación está configurada para recibir en el puerto 1.

Debido a la clase A, el nodo recibirá el dato tras la siguiente transmisión.


Happy hacking!
