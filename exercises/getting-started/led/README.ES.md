## Encendiendo y apagando LEDs con comandos de shell

### Introducción

RIOT define macros para interactuar con LEDs &#x21d2; **LEDi_ON**, **LEDi_OFF**,
**LEDi_TOGGLE** con i en {0, 1, ..., N}.

Estas macros están definidas en el archivo **board.h** de cada placa que provee
LEDs. Esto es así porque los LEDs y botones son específicos a cada placa. Ver
`~/riot-course/RIOT/boards/b-l072z-lrwan1/include/board.h` por ejemplo.

En este ejercicio proponemos escribir un comando de shell que invierte el estado
de un LED dado su número:
```sh
> toggle
Usage: toggle <led num>
> toggle 0
Toggle led 0
> toggle 1
Toggle led 1
```

### Ejercicio

1. Posicionarse en el directorio de la aplicación
   `~/riot-course/exercises/getting-started/led` y editar el archivo `main.c`.

2. Agregar el comando de shell 'toggle' que invierte el estado de los leds
   presentes en la placa. Se puede utilizar como base la aplicación de shell
   escrita previamente en `~/riot-course/exercises/getting-started/shell`.
   `board.h` también debe ser incluído.

   __Nota:__ Asegurarse de que el número de LED dado no excede el número de LEDs
   disponibles (*pista:* la B-L072Z-LRWAN1 tiene 4 LEDs y native tiene 2).

3. Compilar y correr la aplicación para la plataforma `native`:
```sh
$ make all term
```

3. Compilar y correr la aplicación para la placa B-L072Z-LRWAN1:

   _**Recordar:**_ si se utiliza un nodo en IoT-LAB, se necesita también
   especificar en el comando `make` la variable `IOTLAB_NODE` con el nombre del
   nodo.
   __Ejemplo:__ `IOTLAB_NODE=st-lrwan1-10.saclay.iot-lab.info`.

```sh
$ make BOARD=b-l072z-lrwan1 flash term
```

Sigamos ahora con el
[ejercicio del sensor](https://github.com/riot-os/riot-course/tree/master/exercises/getting-started/sensor).
