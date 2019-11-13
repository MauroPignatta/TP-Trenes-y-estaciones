# Programa Trenes y Estaciones

## Instalación y Ejecución del Programa Para Plataformas Linux:

1-	Abrir  una Terminal 

2-	Crear el directorio “Trenes y Estaciones” donde Ud quiera

3-	Ingresar al directorio o carpeta creada

4-	Clonar este repositorio con el siguiente comando: 
-	git clone https://github.com/MauroPignatta/TP-Trenes-y-estaciones

5-Instalar Make y GCC con los siguientes comandos:
- sudo apt install make
- sudo apt install gcc

6-	Instalar NCurses con el siguiente comando:
-	sudo apt-get install libncurses5-dev
-	sudo apt-get install libncursesw5-dev

7-  Crear el subdirectorio "bin" del de la carpeta donde se alojará el programa.

8-  Ejecutar el programa make dentro de la carpeta contenedora de los archivos del codigo ".c".
(Ej: "/TP-Trenes-y-estaciones")

9-	Ingresar al sub-directorio “bin” ubicado en la carpeta contenedora donde se aloja el programa. 
(Ej: "/TP-Trenes-y-estaciones/bin")

10-	Ya puede ejecutar el programa de Estaciones con el siguiente comando:
-	Para la Estación: ./estación <nombre del archivo de configuración de estaciones> .conf

11-	Abrir otra Terminal para ejecutar el programa de Tren con el siguiente comando:
-	Para cada Tren: ejecutar la aplicacion del programa "tren" y a continuacion el nombre del archivo de configuracion de trenes seguido del el nombre de la estacion a la cual desea conectarse.
./tren <nombre del archivo de configuración de trenes>.conf <nombre estacion>

12-	Por cada Tren que desee agregar, deberá repetir el punto 8 completo.
