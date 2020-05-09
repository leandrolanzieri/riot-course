## Leyendo e imprimiendo valores de un sensor con comandos de shell

### Introducción

En este ejercicio proponemos escribir 2 nuevos comandos de shell para leer la
temperatura y la presión atmosférica de un
[sensor NXP MPL3115A2](https://www.nxp.com/products/sensors/pressure-sensors/barometric-pressure-15-to-115-kpa/20-to-110-kpa-absolute-digital-pressure-sensor:MPL3115A2).

Este sensor está disponible en la placa
[phyWAVE-KW22](http://doc.riot-os.org/group__boards__pba-d-01-kw2x.html).

Este ejercicio sólo puede ser ejecutado en hardware.

El comportamiento esperado es el siguiente:
```sh
> temperature
Temperature: 18.3ºC
> pressure
Pressure: 101515 Pa
```

### Ejercicio

1. Posicionarse en el directorio de la aplicación
   `~/riot-course/exercises/getting-started/sensor`.

2. Agregar el módulo `mpl3115a2` como dependencia de la aplicación en el
   `Makefile`:
   ```mk
   USEMODULE += mpl3115a2
   ```

3. Darle una mirada al código de la
   [aplicación de prueba del mpl3115a2](https://github.com/RIOT-OS/RIOT/blob/master/tests/driver_mpl3115a2/main.c) en el repositorio de RIOT para ver cómo puede utilizarse.
   Para una descripción detallada de la API del driver, ver la
   [documentación online](http://doc.riot-os.org/group__drivers__mpl3115a2.html)
   (sólo disponible en inglés).

4. Agregar las inclusiones y declaraciones necesarias al comienzo del archivo
   `main.c` (luego del `include` ya existente).
```c
#include "mpl3115a2.h"
#include "mpl3115a2_params.h"

static mpl3115a2_t dev;
```

5. Agregar el código de iniciación del driver al comienzo de la función `main`
   (luego del mensaje existente):
```c
if (mpl3115a2_init(&dev, &mpl3115a2_params[0]) != MPL3115A2_OK) {
    puts("Cannot initialize mpl3115a2 sensor");
    return -1;
}
if (mpl3115a2_set_active(&dev) != MPL3115A2_OK) {
   puts("[FAILED] activate measurement!");
   return -2;
}
```

El driver del sensor debe ser correctamente iniciado antes de iniciar el shell.

6. Agregar funciones que manejen los comandos de shell 'temperature' y
   'pressure'. Se puede utilizar como base la aplicación escrita previamente
   en `~/riot-course/exercises/getting-started/led` (en un directorio llamado
   `.solution` se encuentra un código funcional).

7. Compilar y correr la aplicación para la placa phyWAVE-KW22.

   _**Recordar:**_ si se utiliza un nodo en IoT-LAB, se necesita también
   especificar en el comando `make` la variable `IOTLAB_NODE` con el nombre del
   nodo.
   __Ejemplo:__ `IOTLAB_NODE=st-lrwan1-10.saclay.iot-lab.info`.

```sh
$ make BOARD=pba-d-01-kw2x flash term
```

Sigamos con
[las diapositivas](https://riot-course.lanzieri.dev/slides/02-getting-started/ES/#18).
