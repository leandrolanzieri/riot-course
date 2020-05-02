## Escribiendo una aplicación de shell básica

### Introducción

En este ejercicio proponemos descubrir las maneras en las que RIOT propone
agregar funcionalidades de shell a una aplicación.
Primero se agregarán comandos de shell existentes agregando los módulos
requeridos a una aplicación. Luego, se escribirá un comando de shell propio.

### Ejercicio

#### Compilar y correr una aplicación básica con shell

1. Posicionarse en el directorio de la aplicación
   `~/riot-course/exercises/getting-started/shell` y editar el `Makefile`.

2. Agregar el módulo **shell** al `Makefile`:

```mk
USEMODULE += shell
```

3. Modifica el archivo `main.c`:

```c
#include "shell.h"
```

```c
/* in main */
char line_buf[SHELL_DEFAULT_BUFSIZE];
shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);
return 0;
```

Compila y corre la aplicación:
```sh
$ make all term
> help
help
Command              Description
---------------------------------------
> 
```

La aplicación corre en _native_ con un shell vacío: no hay comandos disponibles.

#### Agregar comandos al shell

Para agregar comandos al shell hay 3 posibilidades:

- se puede incluir el módulo `shell_commands` &#x21d2; esto agrega comandos por
   defecto (ej. `reboot`) o comandos provistos por otros módulos también
   incluídos.

- se pueden incluir módulos extra con comandos pre-definidos: `ps`, `random`.

- se puede definir un controlador de comando propio en el archivo `main.c`:
  ```c
  static int cmd_handler(int argc, char **argv)
  {
      /* Tu código */
  }
  ```
  En este caso, `shell_run` debe ser llamada se la siguiente manera (se utiliza
  `shell_commands` en lugar de `NULL` para el primer argumento):
  ```c
  shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
  ```

Ahora, agreguemos nuevos comandos:

1. Agregar los módulos `shell_commands` y `ps` al `Makefile` de la aplicación:
```mk
USEMODULE += shell_commands
USEMODULE += ps
```

2. Compilar y correr la aplicación para verificar que el nuevo comando `ps` fue
   agregado:
```sh
$ make all term
```

3. Escribir un controlador de comando que requiere 2 argumentos e imprimirlos.
   El controlador imprime un mensaje que informa cómo debe ser utilizado, en
   caso de recibir una lista de argumentos no válida.

4. Registrar este controlador a la lista de comandos de shell.

5. Compilar y correr la aplicación. Verificar que el nuevo controlador se
   comporta como debe.

6. Ahora volver a compilar la misma aplicación para la placa `b-l072z-lrwan1` y
   correrla.
   _**Recordar:**_ si se utiliza un nodo en IoT-LAB, se necesita también
   especificar en el comando `make` la variable `IOTLAB_NODE` con el nombre del
   nodo.
   __Ejemplo:__ `IOTLAB_NODE=st-lrwan1-10.saclay.iot-lab.info`.

El comando se comporta de la misma manera en el hardware sin modificaciones :)

Sigamos con
[las diapositivas](https://riot-os.github.io/riot-course/slides/02-getting-started/#14).