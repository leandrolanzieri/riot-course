## Un chat simple sobre CoAP

### Introducción

En este ejercicio proponemos armar una pequeña aplicación de chat sobre el
protocolo CoAP. Para esto, se deberá armar un servidor CoAP que exponga un 
returso accesible via solicitudes POST. La ruta al recurso debe ser `/chat`.

Al recibir un mensaje la función controladora del recurso deberá imprimirlo en
consola.

A su vez, la aplicación debe exponer un shell por el puerto serie con un comando
`chat <contacto> <msj>` que permite enviar un mensaje a un contacto.

### Opcional

Opcionalmente se pueden agregar dos recursos más para exponer funcionalidades de
hardware del microcontrolador:
  - `/led/<num>`: Recibe solicitudes del tipo GET y POST. Debe responder a los
    GET con 1 o 0 dependiendo del estado del LED `num`. Las solicitudes POST
    deben enviar 1 o 0 para establecer el estado del LED.
  - `/temp`: Recibe solicitudes del tipo GET. Debe responder con la temperatura
    medida desde el sensor en ºC.

### Ejercicio

1. Posicionarse en el directorio de la aplicación
`~/riot-course/exercises/riot-networking/coap-chat`

2. Agregar el módulo `gcoap` a las dependencias de la aplicación en el
   `Makefile`:
  ```mk
  USEMODULE += gcoap
  ```
3. Agregar la lista de recursos en la forma {`<ruta>`, `<método>`, `<función controladora>`, `<contexto>`}:
  ```c
    { "/chat", COAP_POST, chat_handler, NULL },
  ```
4. Declarar el 'listener' de gcoap:
  ```c
  static gcoap_listener_t _listener = {
      &_resources[0],
      sizeof(_resources) / sizeof(_resources[0]),
      NULL
  };
  ```
5. Registrar el 'listener' de gcoap:
  ```c
  gcoap_register_listener(&_listener);
  ```
6. Implementar el controlador para `POST /chat`:
  - Obtener el método utilizado para verificar que es correcto:
  ```c
  unsigned method_flag = coap_method2flag(coap_get_code_detail(pdu));
  if (method_flag != COAP_POST) {
      puts("Invalid method in the chat request");
      goto bad_req;
  }
  ```
  - Imprimir el mensaje y enviar respuesta:
  ```c
      if (pdu->payload_len > 0) {
          printf("\n[ CHAT ] %.*s\n\n", pdu->payload_len, (char *)pdu->payload);
          return gcoap_response(pdu, buf, len, COAP_CODE_CHANGED);
      }

  bad_req:
      return gcoap_response(pdu, buf, len, COAP_CODE_BAD_REQUEST);
  ```
7. Implementar el controlador el comando de shell `chat`:
  - Buscar el contacto solicitado:
  ```c
    chat_contact_t *dst = chat_contacts_get_by_nickname(argv[1]);

    if (!dst) {
        puts("Contact not found");
        return 1;
    }
  ```
  - Iniciar la consulta CoAP y copiar el mensaje al buffer de salida:
  ```c
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_POST, "/chat");

    memcpy(pdu.payload, argv[2], strlen(argv[2]));

    len = gcoap_finish(&pdu, strlen(argv[2]), COAP_FORMAT_TEXT);
  ```
  - Iniciar el endpoint UDP de destino y parsear la IPv6 de string a bytes:
  ```c
    remote.family = AF_INET6;
    remote.port = 5683;
    remote.netif = (uint16_t)gnrc_netif_iter(NULL)->pid;

    /* parse destination address */
    if (ipv6_addr_from_str(&addr, dst->addr) == NULL) {
        puts("[CoAP] can't parse IPv6 address");
        return 0;
    }

    memcpy(&remote.addr.ipv6[0], &addr.u8[0], sizeof(addr.u8));
  ```
  - Enviar la consulta CoAP:
  ```c
    return gcoap_req_send(buf, len, &remote, NULL, NULL);
  ```
