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
[phyWAVE-KW22](http://doc.riot-os.org/group__boards__pba-d-01-kw2x.html).

El nombre clave de esta placa en RIOT es **pba-d-01-kw2x**.

1. Utilizar la variable `BOARD` para seleccionar la plataforma en tiempo de
   compilación.

```sh
$ make BOARD=pba-d-01-kw2x
Building application "first-app" for "pba-d-01-kw2x" with MCU "kinetis".

"make" -C /home/leandro/experiments/riot-course/RIOT/boards/pba-d-01-kw2x
"make" -C /home/leandro/experiments/riot-course/RIOT/core
"make" -C /home/leandro/experiments/riot-course/RIOT/cpu/kinetis
"make" -C /home/leandro/experiments/riot-course/RIOT/cpu/cortexm_common
"make" -C /home/leandro/experiments/riot-course/RIOT/cpu/cortexm_common/periph
"make" -C /home/leandro/experiments/riot-course/RIOT/cpu/kinetis/periph
"make" -C /home/leandro/experiments/riot-course/RIOT/drivers
"make" -C /home/leandro/experiments/riot-course/RIOT/drivers/periph_common
"make" -C /home/leandro/experiments/riot-course/RIOT/sys
"make" -C /home/leandro/experiments/riot-course/RIOT/sys/auto_init
"make" -C /home/leandro/experiments/riot-course/RIOT/sys/newlib_syscalls_default
"make" -C /home/leandro/experiments/riot-course/RIOT/sys/stdio_uart
   text	  data	   bss	   dec	   hex	filename
   8192	   108	  2552	 10852	  2a64	..getting-started/first-app/bin/pba-d-01-kw2x/first-app.elf
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
  $ make BOARD=pba-d-01-kw2x term
  ```
  **term** utiliza la herramienta `pyterm`por defecto para abrir la terminal
  serie. En el caso de IoT-LAB se abre un túnel SSH que redirige el puerto serie
  desde allí.
  **Mantiene la terminal serie abierta.**

  - **En otra terminal** utilizar el objetivo **flash** para programar la placa:
  ```sh
  $ make BOARD=pba-d-01-kw2x flash
  ```
  Ahora observar el mensaje que se muestra la aplicación de RIOT en la terminal
  serie.

  **flash** invoca a la herramienta de programación de manera automática
  (OpenOCD o iotlab-node en el caso de IoT-LAB).

```sh
$ make BOARD=pba-d-01-kw2x flash term
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
[las diapositivas](https://riot-course.lanzieri.dev/slides/02-getting-started/ES/#11).
