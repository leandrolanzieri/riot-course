## Timers

### Introducción

En este ejercicio, proponemos controlad 3 LEDs desde 3 threads diferentes.

Cada LED parpadea de manera diferente:
- _thread 1_ invierte el estado del LED0 cada 500 ms
- _thread 2_ invierte el estado del LED1 cada 1 s
- _thread 3_ invierte el estado del LED2 cada 2 s

Por defecto, la aplicación puede correrse en la placa ST B-L072Z-LRWAN1, pero
cualquier otra placa con por lo menos 3 LEDs podría utilizarse.

### Ejercicio

- Ir a `~/riot-course/exercises/riot-basics/timers`

#### Escribir la aplicación

- Implementar el cambio de estado del LED0 en el thread1. Utilizar
  `xtimer_usleep` y la macro `LED0_TOGGLE`.

- Repetir para LED1 y LED2.

- Compilar y programar la aplicación, ¡y ver como parpadean los LEDs!

Ahora, continuemos con las
[diapositivas de RIOT básico](https://riot-os.github.io/riot-course/slides/03-riot-basics/ES/#22).

