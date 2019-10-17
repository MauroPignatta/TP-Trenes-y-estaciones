# Ejemplo - Uso de biblioteca ncurses

## Pre-requisitos

1. Tener instalado el comando make
1. Tener instalada la biblioteca ncurses
1. Dentro del directorio del proyecto crear los directorios "bin" y "bin/lib"

## Compilación e Instalación

* Dentro del directorio del proyecto ejecutar:
  * Compilar las bibliotecas: `make libs`
  * Compilar el ejecutable: `make all`
  * Instalar el ejecutable: `make install`
* Exportar la variable de entorno LD_LIBRARY_PATH con path absoluto al directorio bin/lib del proyecto. 
  * Ejemplo: export LD_LIBRARY_PATH=/home/usuario/testNcurses/bin/lib

## Uso

`./bin/server`

----

**Para más información ver**

* [NCURSES Programming HOWTO](http://www.tldp.org/HOWTO/NCURSES-Programming-HOWTO/index.html)
