## IPC

### Introducción

En este ejercicio, proponemos intercambiar mensajes entre threads utilizando la
API de IPC.

Se escribirá un comando de shell que envía una cadena de caracteres a un thread.
El thread que recibe el mensaje lo imprimirá.

Al principio se correrá la aplicación en `native` y al final del ejercicio se
correrá la misma aplicación en hardware real.

### Ejercicio

- Ir a `~/riot-course/exercises/riot-basics/thread-ipc`

#### Escribir la aplicación

- En la función controladora del comando send, utilizar `msg_send` y agregar
  la cadena de caracteres a enviar al puntero de contenido de un mensaje
  (`msg_t`).

- En la función del thread que recibe los mensajes, esperar mensajes entrantes
  utilizando `msg_receive` en un bucle.

- Probar la aplicación:
```sh
$ make -C ~/riot-course/exercices/riot-basics/thread-ipc all term
> send "Hello World"
Message received: Hello World
```

#### Correr la aplicación en hardware

- Probar la aplicación en la placa ST `b-l072z-lrwan1`:
```c
$ make BOARD=b-l072z-lrwan1 -C ~/riot-course/exercices/riot-basics/thread-ipc flash term
```

_Note:_ Si se utiliza hardware en FIT IoT-LAB, agregar `IOTLAB_NODE=auto-ssh` a
        `make`.

Ahora, continuemos con las
[diapositivas de RIOT básico](https://riot-os.github.io/riot-course/slides/03-riot-basics/ES/#18).
