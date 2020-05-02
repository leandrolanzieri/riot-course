class: center, middle

# Comenzando

---

class: center, middle

Si aún no lo has hecho, asegúrate de haber seguido los
**[pre-requisitos](https://riot-os.github.io/riot-course/slides/prerequisites)**

<br><br><br><br>

.right[Sino, sigue a la siguiente diapositiva]

---

## Estructura de una aplicación de RIOT

Una aplicación de RIOT mínima consiste de:

- Un `Makefile`

```mk
APPLICATION = example

BOARD ?= native

RIOTBASE ?= $(CURDIR)/../../../RIOT

DEVELHELP ?= 1

include $(RIOTBASE)/Makefile.include
```

- Un archivo C que contiene la función `main`

```c
#include <stdio.h>

int main(void)
{
    puts("Mi primera aplicacion de RIOT");
    return 0;
}
```

---

## Compilar una aplicación de RIOT

- El sistema de compilación de RIOT está basado en la herramienta de compilación
  **make**

--

- Para compilar una aplicación, **make** puede ser invocado de 2 maneras:

  - Desde el directorio de la aplicación
    ```sh
  $ cd <directorio_de_aplicacion>
  $ make
  ```

  - Desde cualquier lugar, utilizando `-C` para especificar el directorio de la
    aplicación:
  ```sh
  $ make -C <directorio_de_aplicacion>
  ```

--

- Usa la variable **BOARD** para especificar el objetivo (la placa) en tiempo de
  compilación
```sh
$ make BOARD=<objetivo> -C <directorio_de_aplicacion>
```
`BOARD` puede ser cualquier placa soportada por RIOT<br/>
&#x21d2; ver el directorio **RIOT/boards** para la lista completa

--

- Usa la variable **RIOTBASE** para especificar el directorio base de RIOT

---

## Correr una aplicación de RIOT

Esto depende de la placa objetivo:

- Correr en **native**: la aplicación de RIOT es ejecutada como un proceso de
  Linux
```sh
$ make BOARD=native -C <directorio_de_aplicacion>
$ <directorio_de_aplicacion>/bin/native/aplicacion.elf
```

- Correr en **hardware**: la aplicación de RIOT debe ser *programada* primer en
  la placa

--

&#x21d2; usa los objectivos de make **flash** y **term**
  - **flash**: compila y escribe el firmware en la memoria flash del MCU

  - **term**: abre un cliente de terminal conectado al puerto serie de la placa

Todo esto puede hacerse en un solo comando:

```sh
$ make BOARD=<objetivo> -C <directorio_de_aplicacion> flash term
```

*Nota:* el comando anterior también puede utilizarse en **native**.

---

## Ejercicio: tu primera aplicación de RIOT

Compilemos y corramos nuestra primera aplicación de RIOT! <br/><br/>
Sólo debes seguir las instrucciones en este
[README](https://github.com/riot-os/riot-course/tree/master/exercises/getting-started/first-app/README.ES.md)

```sh
$ cd ~/riot-course/exercises/getting-started/first-app
$ make
Building application "example" for "native" with MCU "native".

"make" -C /home/user/RIOT/boards/native
"make" -C /home/user/RIOT/boards/native/drivers
"make" -C /home/user/RIOT/core
"make" -C /home/user/RIOT/cpu/native
"make" -C /home/user/RIOT/cpu/native/periph
"make" -C /home/user/RIOT/cpu/native/vfs
"make" -C /home/user/RIOT/drivers
"make" -C /home/user/RIOT/drivers/periph_common
"make" -C /home/user/RIOT/sys
"make" -C /home/user/RIOT/sys/auto_init
 text   data  bss    dec    hex   filename
 20206  568   47652  68426  10b4a .../getting-started/first-app/bin/native/example.elf
```

---

## ¿Cómo extender la aplicación?

&#x21d2; agregando módulos al `Makefile` de la aplicación, o desde la línea de comandos:

- Agregar módulos extra con **USEMODULE**<br/>
     &#x21d2; `xtimer`, `fmt`, `shell`, `ps`, etc.

- Incluir paquetes externos con **USEPKG**<br/>
    &#x21d2; `lwip`, `semtech-loramac`, etc.

- Utilizar drivers de periféricos del MCU con **FEATURES_REQUIRED**<br/>
    &#x21d2; `periph_gpio`, `periph_uart`, `periph_spi`, `periph_i2c`

--

Ejemplo en un `Makefile`:
```mk
USEMODULE += xtimer shell

USEPKG += semtech-loramac

FEATURES_REQUIRED += periph_gpio
```

Ejemplo desde la línea de comandos:
```sh
$ USEMODULE=xtimer make BOARD=b-l072z-lrwan1
```

---

## Ejercicio: escribe una aplicación con un shell

Sigue las instrucciones en el
[README](https://github.com/riot-os/riot-course/tree/master/exercises/getting-started/shell/README.ES.md)
del ejercicio de shell.

---

## Interacción con el hardware

La interacción con el hardware puede realizarse en 3 niveles:

- A nivel de placa, utilizando macros pre-definidas para controlar LEDs y botones

  &#x21d2; sólo hay que incluir `board.h` para utilizarlas

--

- A nivel de CPU, utilizando las APIs de los drivers para periféricos del MCU

  &#x21d2; periph_gpio,periph_i2c, periph_uart, etc.

  Este nivel es considerado como la **HAL** (capa de abstracción de hardware -
  _hardware abstraction layer_) de RIOT, ya que provee una API común para todos
  los tipos/arquitecturas de CPU

--

- A nivel de driver utilizando APIs de alto nivel para sensores, actuadores o
  radios específicos

  &#x21d2; bmp180, hts221, sx1276, etc.

---

## Ejercicio: interacción con el hardware

- Sigue las instrucciones del
  [README](https://github.com/riot-os/riot-course/tree/master/exercises/getting-started/led)
  del ejercicio de LEDs, para cambiar el estado de los mismos por comandos de
  shell.

- Sigue las instrucciones del
  [README](https://github.com/riot-os/riot-course/tree/master/exercises/getting-started/sensor)
  del ejercicio de sensor, para leer valores de un sensor con comandos de shell.

---

## Próximos pasos: leer el código de aplicaciones existentes

El directorio de RIOT contiene aplicaciones que pueden ser utilizadas como
ejemplos para la mayoría de las características que provee.

- Ve aplicaciones en el directorio `examples`

- Prueba las aplicaciones en el directorio `tests`, que también provee buenos
  ejemplos para comenzar en RIOT.

---

## Resumen

- Compilar y correr tu primera aplicación de RIOT, en native y en hardware

- ¿Cómo extender una aplicación?, el `shell`

- Interacción básica con el hardware

- Leer valores de sensores

---

class: center, middle

[Volver al curso](https://github.com/riot-os/riot-course#content-of-the-course)
