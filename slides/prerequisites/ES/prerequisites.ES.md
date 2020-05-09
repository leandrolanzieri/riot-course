class: center, middle

# Pre-requisitos

---

## Tu ambiente de trabajo

- En todas las secciones se asume que el curso está ubicado en **~/riot-course**

- Los ejercicios están ubicados en: **~/riot-course/exercises**

- El código de RIOT está ubicado en: **~/riot-course/RIOT**

- El testbed [FIT IoT-LAB](https://www.iot-lab.info) puede utilizarse para
  los ejercicios que requieran hardware (especialmente en la sección
  [Redes en RIOT](https://riot-os.github.io/riot-course/slides/04-networking-in-riot))

- Para simplificar, todos los ejercicios pueden realizarse utilizando una
  máquina virtual (VM) pre-configurada. Para correr la VM instalar alguna de
  las siguientes opciones:
  - [Virtualbox](https://www.virtualbox.org/) (>= 5.2) y la versión
    correspondiente de
    [Virtualbox extention pack](https://www.virtualbox.org/wiki/Downloads)
  - [VMWare player](https://www.vmware.com/products/workstation-player.html)

---

## Configurar la VM

Hay dos maneras de configurar e iniciar la VM:

1. Descargar la VM desde [aquí](http://demo-fit.saclay.inria.fr/vms/RIOT-VM.ova)
   e importarla como un nuevo dispositivo (_appliance_) en Virtualbox o VMWare.
   Luego iniciar la VM.

2. Usar [vagrant](https://www.vagrantup.com/):
  ```sh
  $ git clone https://github.com/riot-os/riot-course
  $ cd riot-course
  $ vagrant up
  $ vagrant ssh
  ```

**Nota:** Por defecto la distribución del teclado se encuentra en `en`. Para
cambiar la distribución, por ejemplo a `es`:
- Desde la línea de comandos:
```sh
$ setxkbmap es
```
- Desde la interfaz de GNOME, hacer click en `en` en la esquina superior derecha
de la barra de menú y cambiar a `es`

---

## Obtener la última versión del curso

1. En la VM, abrir una terminal (o utilizar `vagrant ssh`)

2. Actualizar los archivos fuente del riot-course:
```sh
$ cd ~/riot-course
$ git pull --recurse-submodules
```

---

## Configurar el acceso a FIT IoT-LAB (opcional)

- Primero se necesita crear una cuenta en FIT IoT-LAB en
  <a href=https://www.iot-lab.info/testbed/#/signup>
  https://www.iot-lab.info/testbed/#/signup</a>

- En la VM, configurar el acceso SSH a FIT IoT-LAB siguiendo las instrucciones
  de [este tutorial](https://www.iot-lab.info/tutorials/ssh-access/) (sólo en
  inglés)

- En la VM, registrarse con el usuario y contraseña de FIT IoT-LAB localmente
  usando:
  ```sh
  $ iotlab-auth -u <tu_usuario>
  ```

Se recomienda jugar un poco con los
[tutoriales online de FIT IoT-LAB](https://www.iot-lab.info/tutorials/) primero.

---

class: center, middle

[Volver al curso](https://github.com/riot-os/riot-course#content-of-the-course)