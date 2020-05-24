## RTC

### Introducción

En este ejercicio, proponemos experimentar con la API del RTC.

### Ejercicio

1. Ir a `~/riot-course/exercises/riot-basics/rtc`

2. Escribir una aplicación que obtenga el tiempo actual del RTC y lo imprima en
   stdout.
   (No olvidar agregar `periph_rtc` a la lista de 'features' requeridas e
   incluirlo en la aplicación).

3. Iniciar un thread llamado `blink_thread`, que espere mensajes entrantes. Por
   cada mensaje, el thread enciende el LED1 durante 1 segundo y luego lo apaga.

4. Luego de apagar el LED, el thread obtiene el tiempo actual y lo imprime.

5. Finalmente, setea una alarma de RTC para dentro de 5 segundos.

6. En el callback de la alarma de RTC, enviar un mensaje al thread
   `blink_thread`.

Ahora, sigamos con las
[diapositivas de RIOT básico](https://riot-os.github.io/riot-course/slides/03-riot-basics/ES/#32).
