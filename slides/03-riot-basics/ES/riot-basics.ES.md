class: center, middle

# RIOT básico

---

## Vista general del sistema

.center[
    <img src="../images/riot-apis.png" alt="" style="width:500px;"/>
]

---

## Organización del código fuente

- **boards:** definiciones específicas de las placas, modelo de CPU, relojes,
  periféricos, configuraciones, documentación, configuración del puerto serie y
  programador (dist)

- **core:** inicio del kernel, threads, IPC

- **cpu:** soporte para microcontroladores (cmsis, definiciones, periféricos,
  drivers), punto de entrada (_reset\_handler\_default_)

- **dist:** herramientas de administración y utilidades (script, ci,
  verificaciones estáticas, etc)

- **doc:** documentación de Doxygen

- **drivers:** drivers de dispositivos de alto nivel (sensores, actuadores,
  radios), HAL API

- **examples:** aplicaciones de ejemplo

- **makefiles:** Makefiles del sistema de compilación y administración

- **pkg:** paquetes externos

- **sys:** bibliotecas de sistema,network, shell, xtimer, etc

- **tests:** tests unitarios, aplicaciones de test (pueden utilizarse como
  ejemplos)

---

## El sistema de compilación

- Un conjunto de `Makefiles` escritos a mano &#x21d2; más control

- Correr **make** desde el directorio de una aplicación:
```
$ cd <application_dir> && make all
$ make -C <application_dir> all
```

- _RIOTBASE_ variable que apunta al código fuente de RIOT

- _$(RIOTBASE)/Makefile.include_ define todos los objetivos (targets de make)

- Módulos:
  - `nombre del módulo = nombre del directorio`

  - Incluir _$(RIOTBASE)/Makefile.base_ en el _Makefile_ del módulo

  - por defecto, todos los `.c` son agregados al módulo

- en `drivers`, `sys`, `pkg`
  - _Makefile.include_: agrega los directorios específicos al sistema de
    compilación

  - _Makefile.dep_: maneja las dependencias entre módulos

---

### El sistema de compilación

- Targets útiles de alto nivel (aparte de _all_, _flash_, _term_, _debug_):

  - **info-build:** provee información útil sobre la compilación (placas
  soportadas, CPU, módulos incluidos, etc)

  - **flash-only:** no re-compila antes de programar (_flash_ depende de _all_)

  - **list-ttys:** retorna una lista de puertos serie disponibles con la
  información de la placa conectada (útil cuando se trabaja con múltiples
  placas)

  - usar `<tab>` (autocompletado) para obtener una lista completa

- Por cada módulo **_xx_** importado, una macro **MODULE_XX** constante es
definida:
```c
#ifdef MODULE_XX
/* código condicional cuando el módulo XX es incluido */
#endif
```
&#x21d2; muy util para optimizar el tamaño del código

---

## El sistema de compilación: depurando una aplicación

- Utilizar el target **debug**. Por ejemplo con OpenOCD:

   - Inicia un servidor GDB

   - Inicia un cliente GDB, se conecta al servidor GDB y carga la aplicación<br><br>
   &#x21d2; Listo para depurar

- **DEVELHELP:** variable que habilita verificaciones de seguridad (assert,
obtener los nombres de los threads, etc)

- Opciones de `CFLAGS` útiles:
  - -DLOG_LEVEL=LOG_DEBUG : habilita la salida de depuración de manera global

  - -DDEBUG_ASSERT_VERBOSE : atrapar  errores del tipo `FAILED ASSERTION` y dar
   el archivo y número de línea donde se produjo

---

## El kernel de RIOT

- Vista general de la secuencia de inicio (boot):

.center[
    <img src="../images/riot-boot.png" alt="" style="width:600px;"/>
]

- `board_init()` es implementada en el archivo `boards/<board name>/board.c`

- `cpu_unit()` es implementada en el archivo `cpu/<cpu model>/cpu.c`

- `kernel_init()` es implementada en el archivo `core/kernel_init.c`

- Ejemplo para ARM Cortex-M:

 - el punto de entrada es `reset_handler_default`

 - implementada en el archivo `cpu/cortexm_common/vectors_cortexm.c`

---

## El scheduler y los threads

- Política de scheduler sin tick (tick-less, `O(1)`)

- Los threads de mayor prioridad corren hasta que finalizan o se bloquean:
   - 16 niveles de prioridad
   - cuanto más bajo es el nivel, más alta es la prioridad
   - el thread Idle tiene prioridad 15
   - el thread Main tiene prioridad 7

- Una interrupción puede tomar control del CPU en cualquier momento

- Si todas los threads están bloqueados:
   - Se ejecuta el thread especial IDLE
   - Se pasa al modo de menor gasto de energía posible

- Un thread is una función con la firma:
```c
void *thread_handler(void *arg);
```

- Los threads tienen su propia pila (stack) de memoria

---

## Utilizando threads

- Los threads son creados utilizando la función `thread_create()` de `thread.h`:
```c
kernel_pid_t pid;
pid = thread_create(stack,  /* puntero al array del stack */
                    sizeof(stack), /* tamaño del stack */
                    THREAD_PRIORITY_MAIN - 1, /* prioridad del thread */
                    flag, /* bandera de configuración del thread */
                    thread_handler, /* función que implementa el thread */
                    NULL, /* argumento para la función que implementa el thread */
                    "nombre del thread");
```

- Por defecto, el thread se inicia inmediatamente

- `stack` es un array de bytes global y estático:
```c
static char stack[THREAD_STACKSIZE_MAIN];
```

- En este ejemplo, la prioridad es mayor que la prioridad del thread `main` 
  (`THREAD_PRIORITY_MAIN - 1`)

- Util: `thread_getpid()` retorna el ID de proceso (pid) del thread actual

--

.center[&#x21d2; más ejemplos de uso de threads en las aplicaciones en
        `tests/thread_*`]

---

## Ejercicio: Thread

Seguir las instrucciones en el
[README del primer ejercicio de thread](https://github.com/riot-os/riot-course/tree/master/exercises/riot-basics/first-thread)

---

## Manejo de concurrencia de threads

- Los threads tienen su propio stack

- Pero los threads pueden a su vez acceder a memoria global compartida de la
  aplicación

   &#x21d2; se requieren mecanismos de protección y sincronización

- RIOT provee:

  - Mutexes en `mutex.h`:
```c
mutex_t lock;
mutex_lock(&lock);
mutex_unlock(&lock);
```

   - Semáforos (semaphores) en `sema.h` (también hay 'POSIX sepmaphores' en
     `semaphores.h`)

--

<br>

.center[&#x21d2; Más ejemplos en las aplicaciones en `tests/mutex_*`]

---

## Ejercicio: Concurrencia de threads

Seguir las instrucciones en el
[README del ejercicio de concurrencia de threads](https://github.com/riot-os/riot-course/tree/master/exercises/riot-basics/thread-concurrency)

---

## IPC

- Pueden intercambiarse mensajes IPC entre threads o entre interrupciones y
  threads

- Los IPC son **sincrónicos** por defecto, o **asincrónicos**

- La API de mensajes está definida en `msg.h` (en `core`):
   - El tipo de mensaje es `msg_t`
   - A cada mensaje se le asigna un tipo (`type`) y un contenido (`content`)

```c
msg_t msg;
msg.type = MSG_TYPE;
msg.content.value = 42; /* el contenido puede ser un valor */
msg.content.ptr = array; /* o puede ser un puntero */
```

   - Enviando un mensaje:

```c
msg_send(&msg, pid); /* se bloquea excepto cuando se llama desde interrupción */
msg_try_send(&msg, pid); /* envío no bloqueante */
msg_send_receive(&msg, &msg_reply, pid); /* bloquear hasta recibir respuesta */
msg_reply(&msg, &msg_reply); /* responder a un mensaje */
```

---

## IPC

- Recibiendo mensajes:

```c
msg_receive(&msg); /* bloquear hasta que se recibe un mensaje */
msg_try_receive(&msg); /* intentar recibir un mensaje sin bloquear */
```

- Caso de uso típico: un thread espera mensajes de otro thread o ISR

```c
void *thread_handler(void *arg)
{
    /* bucle infinito */
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Message received: %s\n", (char *)msg.content.ptr);
    }
    return NULL;
}
```

- Cuando se utilizan mensajes asincrónicos (típicamente el caso de una ISR),
  siempre iniciar una cola de mensajes del thread en la función del thread:
  
```c
msg_t msg_queue[8];
msg_init_queue(msg_queue, 8);
```
---

## Ejercicio: IPC

Seguir las instrucciones del
[README del ejercicio thread IPC](https://github.com/riot-os/riot-course/tree/master/exercises/riot-basics/thread-ipc)

---

## Administración de la energía

**Principio:**<br>
> _cuando todos los threads están bloqueados o terminaron, el scheduler cambia_
> _al thread idle._<br>
> _El thread idle pasa al modo de menor gasto de energía posible._

<table>
<tr>
  <td>
    <ul>
      <li>el modo de bajo consumo debe estar desbloqueado<br><br></li>
      <li>se selecciona el modo de menor consumo de energía ("Cascada")<br><br></li>
      <li>La API del módulo de sistema`pm_layered` está definida en `pm_layered.h`</li>
    </ul>
  </td>
  <td>
.center[
    <img src="../images/riot-application.png" alt="" style="width:250px;"/>
]
  </td>
</tr>
</table>

**Important:**
- El microcontrolador de la placa debe importar el módulo `pm_layered`
- Este módulo aún es un trabajo en proceso, su diseño está sujeto a cambios

---

## Timers

- Un timer de alto nivel es provisto por el módulo `xtimer`

- `xtimer` multiplexa los timers de hardware

- exactitud de microsegundos

- API sencilla:

   - obtener el tiempo actual del sistema en microsegundos

```c
xtimer_ticks32_t now = xtimer_now();
```

   - agregar un delay de `sec` segundos

```c
xtimer_sleep(sec);
```

   - agregar un delay de `microsec` microsegundos

```c
xtimer_usleep(microsec);
```

---

## Timers (continuación)

- Uso de xtimer para despertarse periódicamente:

```c
void xtimer_periodic_wakeup(xtimer_ticks32_t *last_wakeup, uint32_t period);
```

- Uso de una variable `xtimer_t`para enviar un mensaje en un momento dado:

```c
void xtimer_set_msg(xtimer_t *timer, uint32_t offset, msg_t *msg,
                    kernel_pid_t target_pid);
```

- Uso de una variable `xtimer_t` para disparar una función de callback en un
  momento dado:

```c
xtimer_set(xtimer_t *timer, uint32_t offset);
```

- `timex.h` provee macros útiles para convertir de segundos a us, ms a us, etc.:
```c
US_PER_SEC  /* number of microseconds per seconds */
US_PER_MS   /* number of milliseconds per seconds */
```

---

## Ejercicio: Timers

Seguir las instrucciones del
[README del ejercicio de timers](https://github.com/riot-os/riot-course/tree/master/exercises/riot-basics/timers)

---

## La capa de abstracción de hardware (HAL)

- La capa de abstracción de hardware se basa en 3 bloques: **cpus**, **placas**
  y **drivers**

.center[
    <img src="../images/riot-architecture.png" alt="" style="width:400px;"/>
]

- Refleja los dispositivos IoT a los que apunta RIOT<br><br>
   &#x21d2; un microcontrolador con dispositivos conectados en una placa

- Una aplicación se compila para una placa con un CPU y, potencialmente,
  múltiples dispositivos (sensores, actuadores, radios)

---

## Abstracción del CPU

La clasificación de CPUs sigue un order jerárquico:

- **arquitectura** ej. ARM, AVR

- **familia** ej. stm32, sam

- **tipo** ej. stm32l0, stm32l1, sam0, sam3

- **modelo** ej. stm32l072cz, samd21g18a

<table>
<tr>
  <td>
    <ul>
      <li>Esta organización no es obligatoria<br><br></li>
      <li>`native` está formado por un solo módulo<br><br></li>
    </ul>
  </td>
  <td>
.center[
    <img src="../images/riot-cpus.png" alt="" style="width:400px;"/>
]
  </td>
</tr>
</table>

---

## Abstracción de placa

- Cada directorio en `boards` creaa un módulo para una placa<br>
  &#x21d2; `BOARD=<nombre del módulo de la placa>`

- El modelo y familia del MCU utilizado son definidos en `Makefile.features`

- La lista de 'features' (características o funcionalidades) provistas (ej.
  periférico del MCU) se define en `Makefile.features`

- Las configuraciones de los relojes y periféricos del MCU se definen en
  `periph_conf.h`

- `board.h` provee macros específicas de la placa para:

   - Configuraciones para el módulo `xtimer`

   - Pines de LEDs y botones presentes en la placa

   - Drivers de alto nivel de dispositivos presentes en la placa (sensores,
     actuadores, radios)

---

## APIs de los periféricos del MCU

- APIs uniformes sobre hardware heterogéneo

- El mismo código puede correr potencialmente en todos los hardwares

- Las APIs están definidas en `drivers/include/periph` e implementadas en el
  subdirectorio `periph` de cada CPU

- Es incluye un módulo de periférico explícitamente en el Makefile a través de
  la variable **FEATURES_REQUIRED**

- Los drivers de periféricos son implementados desde cero. Esto garantiza:

   - Eficiencia de memoria

   - Menos duplicación de código

   - Implementaciones independientes del fabricante

- Ver las aplicaciones en `tests/periph_*` para ejemplos de uso

- Ver la documentación de las APIs en
  http://doc.riot-os.org/group__drivers__periph.html (sólo inglés)

---

## API del periférico GPIO

- El nombre del módulo es **periph_gpio**. Se incluye desde **periph/gpio.h**

- Utilizar la macro `GPIO_PIN(PORT, PIN)` para obtener un pi de gpio a partir
  de un puerto y pin

- Todos los modos usuales de GPIO pueden utilizarse, pero depende del hardware
  en el que se ejecuta

- El manejo de interrupciones se habilita con el módulo **perip_gpio_irq**

- Utilizar `gpio_init_int()` para configurar un gpio como interrupción externa:

```c
static void gpio_cb(void *arg)
{
    (void) arg;
    /* se hace algo en la interrupción */
}

int main()
{
    gpio_init_int(GPIO_PIN(PA, 0), GPIO_IN, GPIO_RISING, gpio_cb, NULL);
}
```

---

## Ejercicio: GPIO

Seguir las instrucciones en el
[README del ejercicio de gpio](https://github.com/riot-os/riot-course/tree/master/exercises/riot-basics/gpio)

---

## API del periférico UART

- Permite enviar y recibir mensajes asincrónicamente desde un UART

- El nombre del módulo es **periph_uart**. Se incluye desde **periph/uart.h**

- El peripférico UART dispara una interrupción por cada carácter recibido

```c
static void uart_cb(void *arg, uint8_t data)
{
    (void) arg;
    /* se hace algo en la interrupción */
}

int main()
{
    uart_init(UART_DEV(0), BAUDRATE, uart_cb, NULL);
}
```

---

## Ejercicio: UART

Seguir las instrucciones en el
[README del ejerciio de UART](https://github.com/riot-os/riot-course/tree/master/exercises/riot-basics/uart)

---

## API del periférico RTC

- Provee acceso preciso y de bajo consumo a temporizaciones

- Sigue corriendo aún cuando el CPU está en modo de bajo consumo o dormido

- Utiliza la estructura `tm` de la biblioteca estándar `time.h`

- API sencilla:

```c
/* Establecer el tiempo actual del RTC */
rtc_set_time(struct tm *time);

/* Obtener el tiempo actual */
rtc_get_time(struct tm *time);
```

- Puede utilizarse para configurar una alarma:

```c
static void rtc_alarm_cb(void *)
{
    /* hacer algo cuando se activa la alarma */
}

[...]
struct tm alarm_time;
rtc_set_alarm(&alarm_time, rtc_alarm_cb, NULL);
```

---

## Ejercicio: RTC

Seguir las instrucciones en el
[README del ejercicio de RTC](https://github.com/riot-os/riot-course/tree/master/exercises/riot-basics/rtc)

---

## Otros drivers de periférico disponibles

- **Timer**: módulo `periph_timer`. Se incluye desde `periph/timer.h`

- **I2C**: módulo `periph_i2c`. Se incluye desde `periph/i2c.h`

- **SPI**: módulo `periph_spi`. Se incluye desde `periph/spi.h`

- **PWM**: módulo `periph_pwm`. Se incluye desde `periph/pwm.h`

- **ADC**: módulo `periph_adc`. Se incluye desde `periph/adc.h`

- **DAC**: módulo `periph_dac`. Se incluye desde `periph/dac.h`

- **RTT**: real-time ticker. Se incluye desde `periph/rtt.h`

.center[etc]

--

&#x21d2; se pueden encontrar ejemplos de aplicaciones en `tests/periph_*`

---

## Drivers de alto nivel

.center[
    <img src="../images/riot-architecture.png" alt="" style="width:400px;"/>
]

- Escritos sobre las APIs de drivers de periféricos &#x21d2;  asegura postabilidad

- El diseño permite utilizar múltiples dispositivos del mismo tipo<br>

   &#x21d2; Concepto de **descriptores de dispositivos**, que contienen el
            estado de ejecución

- Los drivers están definidos en `drivers/include/<nombre del driver>` e
  implementados en `drivers/<nombre del driver>`

- **SAUL** (Sensor Actuator Uber Layer) &#x21d2; abstracción de alto nivel sobre
  los drivers

---

## Drivers de alto nivel: iniciación

- Las implementaciones de los drivers proveen parámetros por defecto de
  iniciación `drivers/<nombre del driver>/include/<nombre del driver>_params.h`

- Los parámetros por defecto pueden redefinir:

   - in el código de la aplicación: agregando un archivo `<nombre del driver>_params.h`

   - en la configuración de la placa en `board.h`: redefiniendo las macros necesarias

- La iniciación típica de un driver es:

```c
#include "nombre_driver.h"
#include "nombre_driver_params.h"

static nombre_driver_t dev;

[...]

/* en la función main */
nombre_driver_init(&dev, &nombre_driver_params[0]);
```

---

## Ejercicio: Uso de drivers de alto nivel

Seguir las instrucciones del
[README del ejercicio de drivers](https://github.com/riot-os/riot-course/tree/master/exercises/riot-basics/drivers)

---

## Paquetes

**Principio:** Integrara una biblioteca externa desde el código fuente al
firmware final

**Flujo de trabajo:**

1. Se obtiene el código fuente desde un directorio local o usando `git`

2. Se aplican los parches al código (opcional)

3. Se copia el Makefile de RIOT a los archivos fuente: crea uno o más módulos

**Sencillo de agregar:**

- Los paqete se descargan en el directorio `bin` de la aplicación

- Sólo se requieren mínimamente 3 archivos:

   - `Makefile` define la URL, version y comandos para compilar el paquete
   - `Makefile.include` define el directorio de inclusión
   - `Makefile.<nombre_paquete>` es el makefile de RIOT que se copia a los archivos fuente obtenidos

---

## Resumen

- Organización del código

- El sistema de compilación

- El kernel de RIOT: secuencia de inicio, threads, IPC

- Timers

- Administración de la energía

- Capa de abstracción de hardware: placa, cpu, periféricos

- Drivers

- Paquetes

---

class: center, middle

[Volver al curso](https://github.com/riot-os/riot-course#content-of-the-course)
