## Concurrencia de Threads

### Introducción

En este ejercicio, proponemos escribir y leer en un buffer global compartido en
una manera 'thread-safe'.

Se iniciarán 2 threads:
- un primer thread escribe contenido en un buffer compartido cada 100 ms. Cada
  escritura toma 200 ms en completarse.
- un segundo thread lee el contenido del buffer compartido y lo imprime cada
  200 ms.

Se comenzará corriendo la aplicación en `native` y al final del ejercicio se
correrá la misma aplicación en hardware real.

### Ejercicio

- Ir a `~/riot-course/exercises/riot-basics/thread-concurrency`

#### Escribir una aplicación que no es 'thread-safe'

- Implementar la lógica del thread que imprime: cada 200 ms imprimirá el
  contenido del atributo `buffer` de la estructura `data`. Utilizar la función
  `xtimer_usleep` para agregar un delay de 200 ms. La macro `US_PER_MS` es útil
  para convertir ms en us.

- Implementar la lógica del thread que escribe: modificará el contenido del
  buffer y esperará 100 ms antes de volver a hacerlo. Aquí se quiere simular una
  acción que toma tiempo, así que se debe realizar en 3 pasos:
  - Escribir una parte del contenido en el buffer (utilizar `sprintf` o
    equivalente)
  - Esperar 200 ms
  - Escribir el resto del contenido en el buffer
  ```c
  size_t p = sprintf(data.buffer, "start: %"PRIu32"", xtimer_now().ticks32);
  xtimer_usleep(200 * US_PER_MS);
  p += sprintf(&data.buffer[p], " - end: %"PRIu32"", xtimer_now().ticks32);
  ```

- Compilar y correr la aplicación: se debería observar que los mensajes impresos
  son a menudo inválidos. Esto se debe a que el buffer no está completamente
  escrito cuando el thread que imprime lo lee: se necesita un mecanismo de
  sincronización.
  ```sh
  start: 24
  start: 24 - end: 200172
  start: 300267
  start: 300267 - end: 500409
  start: 600555
  start: 900836
  start: 900836 - end: 1100944
  start: 1201012
  start: 1501294
  start: 1501294 - end: 1701402
  start: 1801475
  start: 2101777
  start: 2101777 - end: 2301924
  start: 2401998
  start: 2702242
  start: 2702242 - end: 2902380
  ```

#### Hacer que la aplicación sea 'thread-safe'

Para hacer la aplicación 'thread-safe', proponemos utilizar mutexes. La variable
`data` ya provee un atributo `mutex` que se utilizará para sincronizar los
threads.

- En el thread que escribe, proteger las escrituras al buffer con llamadas a las
  funciones `mutex_lock`/`mutex_unlock`:
  ```c
  mutex_lock(&data.lock);
  [...]
  mutex_unlock(&data.lock);
  ```
- Hacer lo mismo para el acceso al buffer en el thread que imprime.

- Compilar y correr la aplicación: ¡ahora el contenido del buffer es escrito
  correctamente siempre!
  ```sh
  start: 30 - end: 200193
  start: 300285 - end: 500418
  start: 600567 - end: 800697
  start: 900809 - end: 1100906
  start: 1201024 - end: 1401154
  start: 1501293 - end: 1701448
  start: 1801585 - end: 2001705
  start: 2101908 - end: 2302040
  ```

#### Corriendo la aplicación en hardware

- Probar la aplicación en la placa ST `b-l072z-lrwan1`:
```c
$ make BOARD=b-l072z-lrwan1 -C ~/riot-course/exercices/riot-basics/thread-concurrency flash term
```

_Nota:_ Si se utiliza hardware en FIT IoT-LAB, agregar `IOTLAB_NODE=auto-ssh`
        a `make`

Ahora, sigamos con las
[diapositivas de RIOT básico](https://riot-os.github.io/riot-course/slides/03-riot-basics/ES/#15).
