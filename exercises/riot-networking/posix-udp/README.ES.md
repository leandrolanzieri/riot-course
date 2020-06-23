## UDP utilizando al API Sock

### Introducción

En este ejercicio, proponemos implementar 2 aplicaciones UDP utilizando la API compatible con Posix de RIOT: un cliente periódicamente enviará el mensaje "Hello Server", el servidor imprimirá los mensajes recibidos.

El servidor escucha en el puerto 8888. El cliente envía sus mensajes a la dirección IPv6 local del servidor.

Ambas aplicaciones van a correr en instancias de RIOT `native`.

### Ejercicio

1. Configurar 2 interfaces tap para utilizar en las 2 instancias native. El
   servidor correrá en `tap0` y el cliente en `tap1`. Ejecutar

  ```sh
  $ sudo ~/riot-course/RIOT/dist/tools/tapsetup/tapsetup -c 2
  ```

2. Implementar la aplicación del servidor

  - Ir al directorio de la aplicación `~/riot-course/exercises/riot-networking/posix-udp/server`
  - Agregar la dependencia con el módulo de Posix Sockets al `Makefile`de la aplicación:
    ```mk
    USEMODULE += posix_sockets
    ```
  - En el archivo `main.c`, crear el socket del servidor:
    ```c
    int server_socket = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    if (server_socket < 0) {
        puts("error initializing socket");
        server_socket = 0;
        return 1;
    }
    ```
  - Instanciar y configurar la dirección del servidor:
  ```c
  struct sockaddr_in6 server_addr;
  server_addr.sin6_family = AF_INET6;
  memset(&server_addr.sin6_addr, 0, sizeof(server_addr.sin6_addr));
  server_addr.sin6_port = htons(SERVER_PORT);
  ```
  - Vincular el socket a la dirección del servidor:
  ```
  if (bind(server_socket, (struct sockaddr *)&server_addr,
          sizeof(server_addr)) < 0) {
      server_socket = -1;
      puts("error binding socket");
      return 1;
  }
  ```
  - Comenzar a escuchar paquetes entrantes:
    ```c
    int res;
    struct sockaddr_in6 src;
    socklen_t src_len = sizeof(struct sockaddr_in6);
    if ((res = recvfrom(server_socket, server_buffer, sizeof(server_buffer), 0,
                        (struct sockaddr *)&src, &src_len)) < 0) {
        puts("Error on receive");
    }
    else if (res == 0) {
        puts("Peer did shut down");
    }
    else {
        printf("Message received: ");
        puts(server_buffer);
        sendto(server_socket, SERVER_MESSAGE, sizeof(SERVER_MESSAGE), 0,
               (struct sockaddr *)&src, sizeof(src));
    }
    ```

3. Implementar la aplicación del cliente

  - Ir al directorio de la aplicación `~/riot-course/exercises/riot-networking/posix-udp/client`
  - Agregar la dependencia con el módulo de Posix Sockets al `Makefile`de la aplicación:
    ```mk
    USEMODULE += posix_sockets
    ```
  - En el archivo `main.c`, crear el socket:
    ```c
    int s = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    if (s < 0) {
        puts("error initializing socket");
        return 1;
    }
    ```
 - Configurar la dirección de destino (basarse en las macros `SERVER_ADDR` y `SERVER_PORT`)
    ```c
    /* configure the destination address */
    struct sockaddr_in6 dst;
    dst.sin6_family = AF_INET6;

    /* parse destination address */
    ipv6_addr_from_str((ipv6_addr_t *)&dst.sin6_addr, SERVER_ADDR);

    /* parse port */
    dst.sin6_port = htons(SERVER_PORT);
    ```
  - Enviar mensajes al servidor:
    ```c
    sendto(s, CLIENT_MESSAGE, strlen(CLIENT_MESSAGE), 0,
              (struct sockaddr *)&dst, sizeof(dst));
    ```

4. Compilar e iniciar la aplicación del servidor en `tap0`. Notar la dirección IPv6 local del servidor:
  ```sh
  $ make -C ~/riot-course/exercises/riot-networking/posix-udp/server
  $ make PORT=tap0 -C ~/riot-course/exercises/riot-networking/posix-udp/server term
  Configured network interfaces:
  Iface  5  HWaddr: F2:9D:F4:58:14:D4
          L2-PDU:1500 MTU:1500  HL:64  Source address length: 6
          Link type: wired
          inet6 addr: fe80::f09d:f4ff:fe58:14d4  scope: local  TNT[1]
          inet6 group: ff02::1
          inet6 group: ff02::1:ff58:14d4
  ```
  Aquí la dirección local del servidor es **fe80::f09d:f4ff:fe58:14d4**.
  Mantener la terminal abierta y la aplicación corriendo.

5. Compilar e iniciar la aplicación del cliente en `tap1`. Utilizar la macro `SERVER_ADDR` para especificar la dirección del servidor a utilizar:
  ```sh
  $ make SERVER_ADDR=fe80::f09d:f4ff:fe58:14d4 -C ~/riot-course/exercises/riot-networking/posix-udp/client
  $ make PORT=tap1 -C ~/riot-course/exercises/riot-networking/posix-udp/client term

  On the server terminal, you should also see the following messages:
  ```
  Message received: Hello Server
  Message received: Hello Server
  Message received: Hello Server
  Message received: Hello Server
  Message received: Hello Server
  ```

Ahora, continuemos con las
[diapositivas de red](https://riot-os.github.io/riot-course/slides/04-networking-in-riot/ES/#19).