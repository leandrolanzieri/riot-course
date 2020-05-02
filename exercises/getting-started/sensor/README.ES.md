## Leyendo e imprimiendo valores de un sensor con comandos de shell

### Introducción

En este ejercicio proponemos escribir 2 nuevos comandos de shell para leer la
temperatura y la humedad relativa de un
[sensor ST HTS221](https://www.st.com/en/mems-and-sensors/hts221.html).

Este sensor está disponible en el
[shield ST X-NUCLEO-IKS01A2](https://www.st.com/en/ecosystems/x-nucleo-iks01a2.html)
conectado a la placa
[B-L072Z-LRWAN1](https://www.st.com/en/evaluation-tools/b-l072z-lrwan1.html).

Este ejercicio sólo puede ser ejecutado en hardware.

El comportamiento esperado es el siguiente:
```sh
> temperature
Temperature: 18.3ºC
> humidity
Humidity: 75%
```

### Ejercicio

1. Posicionarse en el directorio de la aplicación
   `~/riot-course/exercises/getting-started/sensor`.

2. Agregar el módulo `hts221` como dependencia de la aplicación en el
   `Makefile`:
   ```mk
   USEMODULE += hts221
   ```

3. Darle una mirada al código de la
   [aplicación de prueba del hts221](https://github.com/RIOT-OS/RIOT/blob/master/tests/driver_hts221/main.c) en el repositorio de RIOT para ver cómo puede utilizarse.
   Para una descripción detallada de la API del driver, ver la
   [documentación online](http://doc.riot-os.org/group__drivers__hts221.html)
   (sólo disponible en inglés).

4. Agregar las inclusiones y declaraciones necesarias al comienzo del archivo
   `main.c` (luego del `include` ya existente).
```c
#include "hts221.h"
#include "hts221_params.h"

static hts221_t dev;
```

5. Agregar el código de iniciación del driver al comienzo de la función `main`
   (luego del mensaje existente):
```c
if (hts221_init(&dev, &hts221_params[0]) != HTS221_OK) {
    puts("Cannot initialize hts221 sensor");
    return -1;
}
if (hts221_power_on(&dev) != HTS221_OK) {
    puts("Failed to set hts221 power on");
    return -2;
}
if (hts221_set_rate(&dev, dev.p.rate) != HTS221_OK) {
    puts("Failed to set hts221 continuous mode");
    return -3;
}
```

El driver del sensor debe ser correctamente iniciado antes de iniciar el shell.

6. Agregar funciones que manejen los comandos de shell 'temperature' y
   'humidity'. Se puede utilizar como base la aplicación escrita previamente
   en `~/riot-course/exercises/getting-started/led` (en un directorio llamado
   `.solution` se encuentra un código funcional).

7. Compilar y correr la aplicación para la placa B-L072Z-LRWAN1.

   _**Recordar:**_ si se utiliza un nodo en IoT-LAB, se necesita también
   especificar en el comando `make` la variable `IOTLAB_NODE` con el nombre del
   nodo.
   __Ejemplo:__ `IOTLAB_NODE=st-lrwan1-10.saclay.iot-lab.info`.

```sh
$ make BOARD=b-l072z-lrwan1 flash term
```

Sigamos con
[las diapositivas](https://riot-os.github.io/riot-course/slides/02-getting-started/#18).
