## Threads

### Introducción

En este ejercicio proponemos descubrir el mecanismo básico propuesto por RIOT
para configurar, iniciar y monitoread threads.

Se utilizará el módulo `ps` con comandos de `shell` para monitorear los threads
en ejecución y para jugar con sus prioridades para asegurar que la aplicación
se ejecuta como se espera.

Para comenzar se ejecutará la aplicación con threads en `native`, y al final del
ejercicio proponemos ejecutar esa misma aplicación, sin cambios, en hardware
real.

### Ejercicio

Ir a `~/riot-course/exercises/riot-basics/first-thread`

#### Listar los threads en ejecución usando el shell

- Agregar los módulos `shell`, `shell_commands` y `ps` al Makefile de la
  aplicación:
  ```mk
  USEMODULE += shell
  USEMODULE += shell_commands
  USEMODULE += ps
  ```

- En `main.c`, agregar el código para iniciar el shell con los comandos por
  defecto:
  ```c
  #include "shell.h"
  ```
  And in the `main` function:
  ```c
  char line_buf[SHELL_DEFAULT_BUFSIZE];
  shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);
  ```

- Compilar y correr la aplicación. Listar los threads en ejecución:
  ```sh
  $ make -C ~/riot-course/exercices/riot-basics/first-thread all term
  > ps
  pid | name                 | state    Q | pri | stack  ( used) | base addr  | current     
    - | isr_stack            | -        - |   - |   8192 (   -1) | 0x56617380 | 0x56617380
    1 | idle                 | pending  Q |  15 |   8192 (  436) | 0x566150a0 | 0x56616f00 
    2 | main                 | running  Q |   7 |  12288 ( 3020) | 0x566120a0 | 0x56614f00 
      | SUM                  |            |     |  28672 ( 3456)
  ```

#### Iniciar un nuevo thread

- Iniciar un nuevo thread the sólo imprime "Hello from thread"
```c
#include "thread.h"
static char stack[THREAD_STACKSIZE_MAIN];
void *thread_handler(void *arg)
{
        /* thread code */
        return NULL;
}
kernel_pid_t pid = thread_create(stack,
                                 sizeof(stack),
                                 THREAD_PRIORITY_MAIN - 1,
                                 0,
                                 thread_handler,
                                 NULL,
                                 "thread name");
```

- Compilar, correr la aplicación y verificar que el mensaje se muestre
  correctamente (utilizar `native` y la placa).
  Intentar mostrar la lista de threads nuevamente. ¿Cuál es el problema?

- Ahora agregar un loop infinito a la función del thread, justo antes del 
  `return`:
```c
while (1) {}
```

- Compilar y correr la aplicación nuevamente. Intentar listar los threads
  disponibles. ¿Qué ocurre ahora?

- Cambiar la prioridad del thread a algo menor (ej. aumentar el valor). Ahora
  debería ser posible utilizar el shell para ver el valor del thread con el
  comando `ps`.

#### Ejecutar en hardware

- Probar la aplicación en la placa:
```c
$ make BOARD=pba-d-01-kw2x -C ~/riot-course/exercices/riot-basics/first-thread flash term
```
