## Tu primera aplicación con RIOT

### Compilar y correr la aplicación en **native**

1. Abrir una linea de comandos y moverse con `cd` a
  `~/riot-course/exercises/getting-started/first-app`

2. Compilar la aplicación para **native** usando el comando **make**:
  ```sh
  $ make
  ```
Notar que por defecto es seleccionada la plataforma `native` en el `Makefile` de
la aplicación.

La plataforma **native** compila una aplicación de RIOT que es simplemente un
**Proceso de Linux**.

El sistema de compilación imprime la ubicación del archivo de la aplicación. En
este caso
`~/riot-course/exercises/getting-started/first-app/bin/native/first-app.elf`.

El proceso de compilación imprime además información sobre diferentes tamaños y
estructuras binarias de la aplicación.

3. Verificar el tipo del archivo .elf generado, utilizando el comando `file`.
   Notar que es un proceso común de Linux.

4. Correr la aplicación:
  - ejecutando el binario directamente
  - utilizando el objetivo **term** de `make`
```sh
$ make term
.../getting-started/first-app/bin/native/example.elf
RIOT native interrupts/signals initialized.
LED_RED_OFF
LED_GREEN_ON
RIOT native board initialized.
RIOT native hardware initialization complete.

main(): This is RIOT! (Version: vm-riot)
My first RIOT application
```

### Compilar y correr la aplicación en un hardware específico

En esta sección se compilará la misma aplicación, pero esta vez para la placa
[ST B-L072Z-LRWAN1](https://www.st.com/en/evaluation-tools/b-l072z-lrwan1.html).

El nombre clave de esta placa en RIOT es **b-l072z-lrwan1**.

1. Utilizar la variable `BOARD` para seleccionar la plataforma en tiempo de
   compilación.

```sh
$ make BOARD=b-l072z-lrwan1
Building application "first-app" for "b-l072z-lrwan1" with MCU "stm32l0".

"make" -C /home/user/RIOT/boards/b-l072z-lrwan1
"make" -C /home/user/RIOT/core
"make" -C /home/user/RIOT/cpu/stm32l0
"make" -C /home/user/RIOT/cpu/cortexm_common
"make" -C /home/user/RIOT/cpu/cortexm_common/periph
"make" -C /home/user/RIOT/cpu/stm32_common
"make" -C /home/user/RIOT/cpu/stm32_common/periph
"make" -C /home/user/RIOT/cpu/stm32l0/periph
"make" -C /home/user/RIOT/drivers
"make" -C /home/user/RIOT/drivers/periph_common
"make" -C /home/user/RIOT/sys
"make" -C /home/user/RIOT/sys/auto_init
"make" -C /home/user/RIOT/sys/isrpipe
"make" -C /home/user/RIOT/sys/newlib_syscalls_default
"make" -C /home/user/RIOT/sys/pm_layered
"make" -C /home/user/RIOT/sys/tsrb
"make" -C /home/user/RIOT/sys/uart_stdio
 text   data    bss    dec    hex filename
 7596    140   2740  10476   28ec .../getting-started/first-app/bin/b-l072z-lrwan1/first-app.elf
```

2. Verificar el tipo del binario generado. Es una aplicación binaria compilada
   para la arquitectura ARM.

3. Correr la aplicación en el hardware.

  _**Notas importantes:**_
    - si se utiliza un nodo en IoT-LAB, se necesitará también especificar en el
      comando `make` la variable `IOTLAB_NODE` con el nombre del nodo.
      __Ejemplo:__ `IOTLAB_NODE=st-lrwan1-10.saclay.iot-lab.info`.

    - si se tiene la placa en el escritorio, conectarla por USB a la PC.

  - Utilizar el objetivo **term** para abrir la terminal serie en la placa:
  ```sh
  $ make BOARD=b-l072z-lrwan1 term
  ```
  **term** utiliza la herramienta `pyterm`por defecto para abrir la terminal
  serie. En el caso de IoT-LAB se abre un túnel SSH que redirige el puerto serie
  desde allí.
  **Mantiene la terminal serie abierta.**

  - **En otra terminal** utilizar el objetivo **flash** para programar la placa:
  ```sh
  $ make BOARD=b-l072z-lrwan1 flash
  ```
  Ahora observar el mensaje que se muestra la aplicación de RIOT en la terminal
  serie.

  **flash** invoca a la herramienta de programación de manera automática
  (OpenOCD o iotlab-node en el caso de IoT-LAB).

```sh
$ make BOARD=b-l072z-lrwan1 flash term
[...]
### Flashing Target ###
Open On-Chip Debugger 0.10.0+dev-00290-g5a98ff78 (2018-01-31-14:50)
[...]
INFO # Connect to serial port /dev/ttyACM0
Welcome to pyterm!
Type '/exit' to exit.
INFO # main(): This is RIOT! (Version: vm-riot)
INFO # My first RIOT application
```

Ahora, sigamos con
[las diapositivas](https://riot-os.github.io/riot-course/slides/02-getting-started/#11).
