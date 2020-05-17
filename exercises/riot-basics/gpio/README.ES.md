## GPIO

### Introducción

En este ejercicio proponemos utilizar la API de GPIO de RIOT para generar una
interrupción cuando el botón de la placa sea presionado. La sub-rutina de la
interrupción es utilizada para invertir el estado del LED presente en la placa.

### Ejercicio

- Ir a `~/riot-course/exercises/riot-basics/gpio`

#### Escribir la aplicación

**Nota:** Utilizar las macros predefinidas `BTN_B1_PIN` y `LED1_PIN` e incluir
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

_Nota: no conectar la placa de extensión X-NUCLEO-IKS01A2 dado que el botón en
       la placa principal está conectado al traductor de nivel de la 
       X-NUCLEO-IKS01A2, el cual afecta el estado del nivel del botón.

Ahora, continuemos con las
[diapositivas de RIOT básico](https://riot-os.github.io/riot-course/slides/03-riot-basics/ES/#28).