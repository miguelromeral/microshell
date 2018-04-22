/*
    prueba.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "parser.h"
#include "funciones.h"
//Definimos el tamaño de la ruta donde se encuentra el ejecutable.
#define MAX_SIZE_NAME 255

void mostrar_orden (orden * O);

int main ()
{
//Orden que contendrá todos los comandos introducidos.
    orden O;
//Variable entera que determina errores en la lectura.
    int r;
	//Variable que guardará la ruta donde se encuentra el ejecutable.
	char pathEjecutable[MAX_SIZE_NAME];
	//Para ello llamamos a la función getcwd.
	getcwd(pathEjecutable, MAX_SIZE_NAME);
	//Imprimimos el mensaje inicial.
    printf ("Introduzca órdenes (teclee exit o pulse Ctrl-D para terminar)\n");

	//Leerá órdenes hasta encontrarse con Ctrl-D.
    do              // Leer órdenes y mostrarlas
    {
		//Libera la orden vaciando sus valores.
        inicializar_orden (&O);
		//Símbolo denotando el comienzo de la orden.
        printf ("$ ");
		//Función en parser.c que guardará la cadena de caracteres introducida y la organizará en argumentos.
        r = leer_orden (&O, stdin);
		//Si ha ocurrido un error en la lectuar se envía un mensaje por pantalla.
        if (r < 0)
            fprintf (stderr, "\nError %d: %s\n",
                             -r, mensajes_err[-r]);
        else{
			//No es necesario mostrar la orden como estaba al principio, la suprimimos.
            //mostrar_orden (&O);

			//A partir de aquí compararemos el primer argumento y el número total para llamar a la función correcta.

/* 1 - mipwd() */ 	if(O.argc == 1 && strcmp(O.argv[0],"pwd") == 0) mipwd();
/* 2 - mils() */	else if(strcmp(O.argv[0], "ls") == 0)			mils(O);
/* 3 - mimkdir() */	else if(strcmp(O.argv[0], "mkdir") == 0)		mimkdir(O);
/* 4 - mirmdir() */	else if(strcmp(O.argv[0], "rmdir") == 0)		mirmdir(O);
/* 5 - micd() */	else if(strcmp(O.argv[0], "cd") == 0 && O.argc > 1)	micd(O);
			else if(strcmp(O.argv[0], "cd") == 0 && O.argc == 1){   //cd al que no se le pasa nada, va al directorio del ejecutable.
				chdir(pathEjecutable);
				printf("Directorio cambiado con exito.\n");
			}
/* 6 - micat() */	else if(strcmp(O.argv[0], "cat") == 0)			micat(O);
/* 7 - micp() */	else if(strcmp(O.argv[0], "cp") == 0)			micp(O);
/* 8 - mirm() */	else if(strcmp(O.argv[0], "rm") == 0)			mirm(O);
/* 9 - miexit() */	else if(strcmp(O.argv[0], "exit") == 0 && O.argc == 1)	miexit();	
			else printf("Orden no reconocida.\nDebe ser una orden interna: (pwd, ls, mkdir, rmdir, cd, cat, cp, rm, exit).\n");
        		//Se vacía la orden para poder leer una nueva otra vez.
				liberar_orden (&O);
	}
    }
    while (r==0);   // Repetir hasta error o EOF

    return 0;
}

//Muestra la orden organizada por pantalla.
//Nosotros no la utilizaremos.
void mostrar_orden (orden * O)
{
    int i;

    printf ("\tOrden cruda: \"%s\"\n", O->orden_cruda);
    printf ("\tNúmero de argumentos: %d\n", O->argc);

    for (i=0; i<=O->argc; i++)
        if (O->argv[i] != NULL)
            printf ("\t\targv[%d]: \"%s\"\n", i, O->argv[i]);
        else
            printf ("\t\targv[%d]: NULL\n", i);

    if (O->entrada)
        printf ("\tEntrada: \"%s\"\n", O->entrada);

    if (O->salida)
        printf ("\tSalida: \"%s\"\n", O->salida);

    if (O->salida_err)
        printf ("\tSalida de err.: \"%s\"\n", O->salida_err);

    printf ("\tEjecutar en segundo plano: %s\n",
            O->segundo_plano ? "Sí" : "No");
}


