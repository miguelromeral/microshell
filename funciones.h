/*
    funciones.h
*/

#ifndef _FUNCIONES_H_
#define _FUNCIONES_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include "parser.h"

// 1	Imprime la ruta actual.
void mipwd();
// 2	Imprime un listado de los archivos que contiene y, si lo desea, los detalles de cada uno.
void mils(orden O);
// 3	Crea un nuevo directorio.
void mimkdir(orden O);
// 4	Elimina un directorio.
void mirmdir(orden O);
// 5 	Cambia de directorio.
void micd(orden O);
// 6	Imprime por pantalla el archivo que se le pasa como argumento.
void micat(orden O);
// 7	Copia el contenido de un archivo a otro, si éste no existe lo crea.
void micp(orden O);
// 8	Elimina, rompe los enlaces, de un archivo.
void mirm(orden O);
// 9	Acaba la ejecución de la microshell.
void miexit();

//	Función auxiliar que imprime los detalles de cada archivo de un directorio.
void l(char * path);

#endif // _FUNCIONES_H_
