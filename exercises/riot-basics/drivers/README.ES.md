## Drivers

### Introducción

En este ejercicio proponemos leer e imprimir los valores de 2 sensores en 2
threads diferentes.

Este ejercicio, tal cual se presenta, sólo funciona en placas que provean los
sensores HTS221 y LSM6DSL, pero puede adaptarse fácilmente a otros sensores
soportados por RIOT.

### Ejercicio

- Ir a `~/riot-course/exercises/riot-basics/drivers`

- Escribir una aplicación que inicia 2 threads
  - `thread1` lee los valores del sensor HTS221 (temperatura y humedad) cada 2
    segundos y los imprime en stdout
  - `thread2` lee los valores del acelerómetro LSM6DSL cada 100 ms y los imprime
    en stdout

- Los drivers son inicializados en la función `main` **antes** que los threads

- No olvidar cargar los módulos de los drivers en el Makefile de la aplicación

_Pista_: La documentación de los drivers está disponible online:

  - HTS221: http://doc.riot-os.org/group__drivers__hts221.html

  - LSM6DSL: http://doc.riot-os.org/group__drivers__lsm6dsl.html

  - Las aplicaciones de prueba también pueden ayudar: <br>
    &#x21d2; `~/RIOT/tests/drivers_hts221`<br>
    &#x21d2; `~/RIOT/tests/drivers_lsm6dsl`

Ahora, continuemos con las
[diapositivas de RIOT básico](https://riot-os.github.io/riot-course/slides/03-riot-basics/ES/#37).
