## UART

### Introducción

En este ejercicio, proponemos utilizar la API de la UART junto con threads para
imprimir todos los caracteres recibidos en stdio.

### Ejercicio

- Ir a `~/riot-course/exercises/riot-basics/uart`

#### Escribir la aplicación

**Nota:** Utilizar el primer UART, ej. `UART_DEV(0)`

- Agregar el 'feature' UART a los requerimientos de la aplicación (en el
`Makefile`):
```mk
FEATURES_REQUIRED += periph_uart
```

- Agregar el archivo header del UART a la aplicación:
```c
#include "periph/uart.h"
```

- Escribir una aplicación con un thread, llamado `printer_thread`, que espera
  mensajes entrantes.

- Por cada mensaje, el thread imprimirá "received &lt;c&gt;", donde &lt;c&gt; es
  el contenido del mensaje como carácter.

- Iniciar `UART_DEV(0)` a 115200 bauds, con `uart_cb` como la función de
  callback.

- Por cada carácter recibido en el UART (presionar una tecla y luego 'Enter' en
  el teclado), enviar un mensaje conteniendo el carácter al thread que imprime.

- Experimentar con cadenas de caracteres largas &#x21d2; se necesitará una cola
  de mensajes o un buffer externo.

Ahora, continuemos con las
[diapositivas de RIOT básico](https://riot-os.github.io/riot-course/slides/03-riot-basics/ES/#30).
