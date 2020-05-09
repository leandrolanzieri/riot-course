## Un servidor CoAP simple

### Introducción

En este ejercicio proponemos exponer diferentes tipos de recursos CoAP,
accesibles via solicitudes GET, PUT y POST:
- `/riot/board` retorna el nombre de la placa que corre RIOT
- `/riot/cpu` retorna el nombre del CPU que corre RIOT
- `/value` retorna un valor arbitrario, por defecto el valor 0. Este valor puede
  ser actualizado utilizando solicitudes `PUT` y `POST`.

Se enviarán solicitudes CoAP a una instancia de RIOT native desde la Máquina
Virtual (o la PC) utilizando los clientes del paquete de Python
[aiocaop](https://aiocoap.readthedocs.io/en/latest/).

### Ejercicio

1. Posicionarse en el directorio de la aplicación
`~/riot-course/exercises/riot-networking/gcoap`

2. Agregar los módulos `gcoap` y `fmt` a las dependencias de la aplicación en el
   `Makefile`:
  ```mk
  USEMODULE += gcoap
  USEMODULE += fmt
  ```
3. Agregar la lista de recursos en la forma {`<ruta>`, `<método>`, `<función controladora>`, `<contexto>`}:
  ```c
    { "/riot/board", COAP_GET, _riot_board_handler, NULL },
    { "/riot/cpu", COAP_GET, _riot_cpu_handler, NULL },
    { "/value", COAP_GET | COAP_PUT | COAP_POST, _value_handler, NULL },
  ```
4. Declarar el 'listener' de gcoap:
  ```c
  static gcoap_listener_t _listener = {
      &_resources[0],
      sizeof(_resources) / sizeof(_resources[0]),
      NULL
  };
  ```
5. Registrar el 'listener' de gcoap
  ```c
  gcoap_register_listener(&_listener);
  ```
6. Implementar el controlador para `GET /riot/board`:
  - Iniciar la respuesta a la solicitud:
  ```c
  gcoap_resp_init(pdu, buf, len, COAP_CODE_CONTENT);
  coap_opt_add_format(pdu, COAP_FORMAT_TEXT);
  size_t resp_len = coap_opt_finish(pdu, COAP_OPT_FINISH_PAYLOAD);
  ```
  - Enviar la respuesta con el valor del tipo de placa (contenido en la macro
    `RIOT_BOARD`):
  ```c
  /* se escribe el nombre de la placa en el buffer de la respuesta */
  if (pdu->payload_len >= strlen(RIOT_BOARD)) {
      memcpy(pdu->payload, RIOT_BOARD, strlen(RIOT_BOARD));
      return resp_len + strlen(RIOT_BOARD);
  }
  else {
      puts("gcoap: msg buffer too small");
      return gcoap_response(pdu, buf, len, COAP_CODE_INTERNAL_SERVER_ERROR);
  }
  ```
7. Implementar el controlador para `GET /riot/cpu` con el mismo principio que
   `/riot/board`: utilizar la macro `RIOT_CPU` en lugar de `RIOT_BOARD`.
8. Implementar el controlador de `GET, PUT/POST /value`:
  - Leer la bandera del método de coap incluída en paquete:
  ```c
  unsigned method_flag = coap_method2flag(coap_get_code_detail(pdu));
  ```
  - Implementar la respuesta al GET:
  ```c
  gcoap_resp_init(pdu, buf, len, COAP_CODE_CONTENT);
  coap_opt_add_format(pdu, COAP_FORMAT_TEXT);
  size_t resp_len = coap_opt_finish(pdu, COAP_OPT_FINISH_PAYLOAD);

  /* se escribe el buffer de respuesta con el valor de la cuenta */
  resp_len += fmt_u16_dec((char *)pdu->payload, _value);
  return resp_len;
  ```
  - Implementar la respuestas a las solicitudes PUT/POST:
  ```c
  if (pdu->payload_len <= 5) {
      char payload[6] = { 0 };
      memcpy(payload, (char *)pdu->payload, pdu->payload_len);
      _value = (uint16_t)strtoul(payload, NULL, 10);
      return gcoap_response(pdu, buf, len, COAP_CODE_CHANGED);
  }
  else {
      return gcoap_response(pdu, buf, len, COAP_CODE_BAD_REQUEST);
  }
  ```
9. Compilar e iniciar la aplicación del servidor CoAP en `tap0`. Fijarse en la
  dirección link-local del servidor:
  ```sh
  $ make -C ~/riot-course/exercises/riot-networking/gcoap
  $ make PORT=tap0 -C ~/riot-course/exercises/riot-networking/gcoap term
  Configured network interfaces:
  Iface  5  HWaddr: F2:9D:F4:58:14:D4
          L2-PDU:1500 MTU:1500  HL:64  Source address length: 6
          Link type: wired
          inet6 addr: fe80::f09d:f4ff:fe58:14d4  scope: local  TNT[1]
          inet6 group: ff02::1
          inet6 group: ff02::1:ff58:14d4
  ```
  Aquí la dirección link-local del servidor CoAP es **fe80::f09d:f4ff:fe58:14d4**.
  Mantener la terminar abierta y la aplicación del servidor corriendo.
10. Desde la máquina virtual (o la PC), utilizar `aiocoap-client` para enviar
  solicitudes al servidor CoAP (notar que se agrega `%tapbr0` al final de la
  dirección link-local del servidor):
  - Obtener los recursos `/riot/board` and `/riot/cpu`:
  ```
  $ aiocoap-client -m get coap://[fe80::f09d:f4ff:fe58:14d4%tapbr0]/riot/board
  native
  $ aiocoap-client -m get coap://[fe80::f09d:f4ff:fe58:14d4%tapbr0]/riot/cpu
  native
  ```
  - Verificar que el valor (`/value`) puede ser solicitado y cambiado utilizando
    CoAP:
  ```
  $ aiocoap-client -m get coap://[fe80::f09d:f4ff:fe58:14d4%tapbr0]/value
  0
  $ aiocoap-client -m put coap://[fe80::f09d:f4ff:fe58:14d4%tapbr0]/value --payload 42
  $ aiocoap-client -m get coap://[fe80::f09d:f4ff:fe58:14d4%tapbr0]/value
  42
  $ aiocoap-client -m post coap://[fe80::f09d:f4ff:fe58:14d4%tapbr0]/value --payload 10
  $ aiocoap-client -m get coap://[fe80::f09d:f4ff:fe58:14d4%tapbr0]/value
  10
  ```

Ahora sigamos con las
[diapositivas de red](https://riot-os.github.io/riot-course/slides/04-networking-in-riot/ES/#25).
