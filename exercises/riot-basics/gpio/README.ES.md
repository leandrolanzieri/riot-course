## GPIO

### Introducción

En este ejercicio proponemos utilizar la API de GPIO de RIOT para generar una
interrupción cuando el botón de la placa sea presionado. La sub-rutina de la
interrupción es utilizada para invertir el estado del LED presente en la placa.

### Ejercicio

- Ir a `~/riot-course/exercises/riot-basics/gpio`

#### Escribir la aplicación

**Nota:** Utilizar las macros predefinidas `BTN0_PIN` y `LED0_PIN` e incluir
`board.h`.

- Agregar los 'features' de GPIO a los requerimientos de la aplicación (en el
  Makefile):

```mk
FEATURES_REQUIRED += periph_gpio
FEATURES_REQUIRED += periph_gpio_irq
```

- Agregar el header de GPIO en la aplicación:

```c
#include "periph/gpio.h"
```

- Escribir una aplicación con un thread que espera mensajes entrantes.

- Por cada mensaje, el thread invierte el estado del LED0 en la placa.

- El botón de usuario B1 y el LED0 son iniciados de la siguiente manera en
  `main()`:
  - LED0 es iniciado en modo `GPIO_OUT`
  - B1 is iniciado en modo `GPIO_IN` con una función de callback `gpio_cb`. Se
    genera una interrupción cada vez que el botón es presionado.

- Por cada interrupción, un mensaje es enviado desde la ISR al thread que
  controla el LED &#x21d2; invierte el estado de LED0.

_Nota: por ahora se ignorará el problema del rebote del botón_
