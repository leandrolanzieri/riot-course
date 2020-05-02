class: center, middle

# Introducción

---

## ¿Qué es RIOT?

- **Sistema operativo** para microcontroladores

 - **arquitectura de microkernel** &#x21d2; requiere muy pocos recursos
 - **tiempo real** y **multi-threaded**
 - incluye **stack de red propios**

--

<br>

- **código abierto**: <a href="https://github.com/RIOT-OS/RIOT">https://github.com/RIOT-OS/RIOT</a>

 - plataforma de software libre
 - **comunidad global** de desarrolladores

--

<br>

- **fácil de utilizar** y _reutilizar_

 - Programación estándar en C
 - Herramientas estándares
 - **La API es independiente** del hardware

<br><br>

---

### RIOT en el mundo de IoT

.center[<img src="../images/iot-overview.png" alt="" style="width:600px;"/>
<br/><br/>
<br/><br/>
&#x21d2; RIOT está diseñado para dispositivos 'low-end'
<br>(kB RAM, MHz, mW)
]

---

## Historia del proyecto

- 2013: Inria, FU Berlin y HAW Hamburg fundaron RIOT

 - parte de un proyecto de investigación entre Francia y Alemania
 - el kernel evolucionó del FireKernel

--

- La comunidad hoy:

  - Hasta ahora, **+200** contribuidores en la rama master
  - Académicos: Berkeley, UCLA, MIT, AIT, TZI, etc
  - Industriales: Cisco, Samsung, ImgTec, Fujitsu, Thalès
  - PyMEs: Zolertia, OTAKeys, Mesotic, Eistec, We-sens
  - Miembro de la iniciativa EdgeXFoundry

--

- RIOT Summit anual: https://summit.riot-os.org

.center[
<img src="../images/riot-summit.png" alt="" style="width:220px;"/>&nbsp;
<img src="../images/summit-2018.jpg" alt="" style="width:220px;"/>
]

---

## Competidores

.center[
    <img src="../images/os-timeline.png" alt="" style="width:600px;"/>
]
<div style="position: absolute;right: 100px;">
<span style="font-style: italic;font-size:12px;text-align:right">
Referencia: O. Hahm et al. "Operating Systems for Low-End Devices<br>
in the Internet of Things: A survey," IEEE Internet of ThingsJournal, 2016.
</span>
</div>
<br>
<br>

 - requiere menos memoria y se adapta a un mayor rango de arquitecturas

 - promueve una filosofía de código abierto similar a Linux

 - provee más funcionalidades de alto nivel integradas


---

## La filosofía y la comunidad de RIOT

- RIOT es software libre, licenciado bajo la **LGPLv2.1**

- La comunidad se inspira en Linux

<img src="../images/linux.jpg" alt="" style="width:150px;position:fixed;top:120px;right:100px;"/>

--

- Utilización de estándares cuando sea posible <br>
  (C-ANSI, herramientas estándares, protocolos estándares, procedimientos
   estándares)

- Se siguen los estándares **POSIX**

--

- Evitar duplicación de código. Facilidad para programar. Incrementar la
  **portabilidad** y modularidad

- **Independencia** del fabricante y la tecnología

--

- Las decisiones y orientaciones son tomadas por una **comunidad de base**

.center[
    <img src="../images/riot-contributors.png" alt="" style="width:350px"/>
]

---

## Ecosistema y procesos de la comunidad

- Herramientas y sistema de compilación estándares: **Makefiles**, **OpenOCD**,
  **GDB**

--

- CI distribuido y rápido, Murdock: https://ci.riot-os.org

    &#x21d2; **Compilar y correr** todas las aplicaciones de ejemplo y tests<br><br>
    &#x21d2; **Chequeos estáticos** (Cppcheck, Coccinelle, etc)<br>

.center[
<img src="../images/murdock.png" alt="" style="width:400px"/>
]
--

- **Documentación online** &#x21d2; https://doc.riot-os.org (sólo en inglés)

--

- **Revisiones de código** profundas

- **Release estable cada 3 meses**: &lt;año&gt;.&lt;mes&gt; (ej: 2018.07, 2018.10, etc.)

---

class: center, middle

# Visión técnica general

<br/>
La historia corta: publicación en IEEE Internet of Things Journal<br/>
Disponible en: http://riot-os.org/files/2018-IEEE-IoT-Journal-RIOT-Paper.pdf

---

## Características del OS

- Arquitectura basada en **micro-kernel**: diseñado modularmente

<img src="../images/riot-architecture.png" alt="" style="width:250px;position:fixed;right:50px;top:120px"/>

--

- Binario de pequeño tamaño <br> &#x21d2; **2.8kB** RAM, **3.2kB** ROM on
  32-bit Cortex-M

--

- Scheduler de **tiempo real**
  - &#x21d2; preempción con prioridades fijas y operaciones O(1)
  - &#x21d2; scheduler tickless

--

- **Multi-thread** e IPC:
  - Contextos separados de threads con stacks<br/> de memoria separados
  - Bloque de Control de Thread (TCB) mínimo
  - Sincronización de threads utilizando mutex,<br/> semáforos y mensajes.

<img src="../images/riot-application.png" alt="" style="width:300px;position:fixed;right:50px;top:350px"/>

---

## Múltiples threads

2 threads por defecto:

- el thread `main`: ejecuta la funció `main`

- el thread `idle`:

  - mínima prioridad <br/>&#x21d2; thread que corre cuando las demás están bloqueadas o terminaron
  - pasa el sistema a modo de bajo consumo

El contexto de interrupción (ISR) controla los eventos externos y notifica a los
threads utilizando mensajes IPC (Inter Process Communication).

.center[
    <img src="../images/riot-application.png" alt="" style="width:300px;"/>
]

---

## Capa de abstracción de hardware

- Dividida en 3 bloques: boards, cpus, drivers

- Los CPUs están organizados de la siguiente manera:
**arquitectura** (ARM) > **familia** (stm32) > **tipo** (stm32l4) > **modelo** (stm32l476rg)

- API genérica para los periféricos del CPU (gpio, uart, spi, pwm, etc.)

    &#x21d2; la misma API para todas las arquitecturas

- Sólo basadas en los headers del fabricante (CMSIS) &#x21d2; implementación
  desde cero

    &#x21d2; menos duplicación de código, más eficiencia, más trabajo

- Una aplicación &#x21d2; una placa &#x21d2; un modelo de CPU

.center[
    <img src="../images/riot-architecture.png" alt="" style="width:300px;"/>
]

---

## Soporte de hardware

- **Capa de abstracción de hardware:** soporte para 8/16/32 bit, ARM, AVR, MIPS

- Fabricantes soportados: Microchip, NXP, STMicroelectronics, Nordic, TI, ESP, RISC-V, etc.

- **Gran lista de sensores y actuadores** soportados (e.g. drivers)

- Placa _native_: **corre RIOT como un proceso en tu PC**

- **+100 placas soportadas**

.center[
    <img src="../images/riot-boards.png" alt="" style="width:620px;"/>
]

---

## Un OS modular

Las funcionalidades son provistas como módulos &#x21d2; **sólo se compila lo que se requiere**

- Bibliotecas de sistema: **xtimer**, **shell**, crypto, etc.

--

- Sensores y actuadores

- Drivers de pantallas, sistemas de archivos, etc.

<img src="../images/riot-ucglib.jpg" alt="" style="width:200px;position:fixed;right:100px;top:250px"/>

--

- Intérpretes embebidos: JavaScript, MicroPython, LUA

--

- Protolos de red de alto nivel: CoAP, MQTT-SN, etc.

--

- Paquetes externos

.center[
<img src="../images/packages.png" alt="" style="width:400px"/>
]

---

## Bibliotecas de sistema útiles

- **xtimer**

  - Subsistema de timer de alto nivel. Provee una abstracción completa del hardware del timer

  - Puede configurar callbacks, enviar un thread a dormir, etc.

- **shell**

  - provee una interfaz por línea de comando interactiva

  - útil para depuración interactiva o ejemplos

- **Otras:** crypto, fmt, math, etc.

---

## Paquetes externos

- RIOT puede ser extendido con paquetes externos

- Integrados (y eventualmente modificados) mientras se compila la aplicación

- Fáciles de agregar: sólo requieren 2 `Makefiles`

- Ejemplos de paquietes: lwIP, OpenThread, lvgl, loramac, etc.

<br/><br/>

.center[
    <img src="../images/packages.png" alt="" style="width:400px;"/>
]

---

## Stacks de red

**Stacks orientados a IP** &#x21d2; diseñados para redes Ethernet, WiFi, 802.15.4

- **GNRC**: el stack 802.15.4/6LoWPAN/IPv6 propio de RIOT

--

- **Thread**: Stack 802.15.4 IPv6 provisto por tl ThreadGroup

.center[
    <img src="../images/openthread-logo.png" alt="" style="width:200px"/>
]

--

- **OpenWSN** (experimental): una capa MAC determinística que implementa el
   protocolo IEEE 802.15.4e TSCH

.center[
    <img src="../images/Openwsn_logo.png" alt="" style="width:100px"/>
]

--

- Otros stacks IPv6

  - **lwIP**: Un stack de red completo diseñado para bajo consumo de memoria

  - **emb6**: Un fork del stack de red de Contiki que puede ser utilizado sin proto-threads

---

## Otros soportes de red

- Controller Area Network (**CAN**) propio

--

- Soporte para stack **BLE**: <a href="https://github.com/apache/mynewt-nimble">NimBLE</a>
<img src="../images/ble.png" alt="" style="width:180px;position:fixed;top:150px;right:100px"/>
<br><br>

--

- **LoRaWAN** stack
&#x21d2; Compatible con LoRaWAN 1.0.2
<img src="../images/lorawan.png" alt="" style="width:200px;position:fixed;top:235px;right:100px"/>
<br><br>

--

- Soporte de **SigFox** para los módulos ATA8520e
<img src="../images/sigfox.png" alt="" style="width:100px;position:fixed;top:300px;right:125px"/>

---

## Otras características importantes

- Stack USB completo (CDC-ACM, CDC-ECM, etc.)

<img src="../images/usb-logo.png" alt="" style="width:180px;position:fixed;top:80px;right:100px"/>

<br/><br/>

--

- Actualización estándar y segura de software

.center[https://datatracker.ietf.org/wg/suit/about/]

<br/>

--

## Próximamente

- Soporte para NB-IoT

<img src="../images/nb_iot_logo.jpg" alt="" style="width:180px;position:fixed;top:450px;right:100px"/>
<br/>

- Configuración más avanzada con Kconfig

<img src="../images/kconfig.jpeg" alt="" style="width:180px;position:fixed;top:525px;right:100px"/>

---

class: center, middle

# RIOT en acción

---

## Despliegues IoT utilizando RIOT

.center[
    **Telefónica Chile: dispositivos LoRa en una mina**<br><br>
    <img src="../images/drop-watcher.png" alt="" style="width:630px"/><br/>
    <a href="http://riot-os.org/files/RIOT-Summit-2017-slides/6-2-Network-Session-DropWatcher.pdf">Más información</a>
]

---

## Despliegues IoT utilizando RIOT

.center[
**Algramo: Dispenser automático de cereal**<br><br>
<img src="../images/almagro-feeder.png" alt="" style="width:200px"/><br/>
<a href="https://www.indiegogo.com/projects/the-venture-algramo-chile#/">Más información</a>
]

---

## Despliegues IoT utilizando RIOT

.center[
**Automatización de hogares con KNX**<br><br>
<img src="../images/knx-element.png" alt="" style="width:400px"/><br>
<img src="../images/knx-element2.png" alt="" style="width:200px"/>
<img src="../images/knx-board.png" alt="" style="width:150px"/><br><br>
<a href="http://summit.riot-os.org/2018/wp-content/uploads/sites/10/2018/09/2_2-Bas-Stottelaar-KNX.pdf">Más información</a>
]

---

## Productos IoT y servicios utilizando RIOT

- Monitoreo ambiental: Hamilton IoT (EE.UU.), Unwired Devices (Rusia)

.center[
    <img src="../images/hamilton-board.png" alt="" style="width:200px"/>
    <img src="../images/unwired-one-fingers.jpg" alt="" style="width:200px"/>
]

- Diagnóstico a bordo para autos conectados: OTAKeys (Continental)

- Design Office: Eistec (Suecia), Mesotic (Francia)

---

## RIOT en el testbed de gran escala FIT/IoT-LAB

.center[<a href=https://www.iot-lab.info>https://www.iot-lab.info</a><br><br>
IoT-LAB es un **testbed de experimentación de gran escala**]

  - Puede utilizarse para **ensayos de redes de comunicación inalámbrica** en **pequeños dispositivos**

  - Puede utilizarse para **aprender** programación de IoT y **protocolos de comunicación**

  - Puede utilizarse para ensayos de plataformas de software en **hardware heterogéneo**

.center[
      <img src="../images/iotlab-nodes.png" alt="" style="width:250px;"/>&nbsp;
      <img src="../images/iotlab-sites.png" alt="" style="width:250px;"/>
  ]

---

## Aprender RIOT

- Tutoriales de RIOT (sólo en inglés)

.center[<a href="https://github.com/RIOT-OS/Tutorials">https://github.com/RIOT-OS/Tutorials</a>]

<br/>

- Curso online RIOT (parcialmente traducido)

.center[<a href="https://github.com/riot-os/riot-course">https://github.com/riot-os/riot-course</a>]

<br/>

---

## Resumen

- Generalidades de RIOT: historia, comunidad, usuarios

- Una visión técnica general

  - Características del OS

  - soporte de hardware

  - bibliotecas

  - stack de red

  - importar bibliotecas externas por medio de paquetes

- El ecosistema de RIOT: herramientas estándares, CI, documentación

- Compañías utilizando RIOT

---

class: center, middle

[Volver al curso](https://github.com/riot-os/riot-course#content-of-the-course)
