#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <dirent.h>
#include <time.h>
#include "parser.h"
#include "funciones.h"
#define MAX_SIZE_BUFFER 126

/*
		1. mipwd()
*/
void mipwd(){
	//Variable que almacena el path actual.
	char buffer[MAX_SIZE_BUFFER];
	//Se le atribuye un valor al buffer con la ruta.
	getcwd(buffer, MAX_SIZE_BUFFER);
	//Imprimimos la ruta por pantalla.
	printf("Directorio actual: %s\n", buffer);
}
//Función auxiliar que imprime los detalles de cada archivo de un directorio.
void l(char * path){
	//Variable directorio donde se abrirá la carpeta que queremos mostrar los archivos.
	DIR *directorio;
	//Abrimos el directorio de la ruta que le pasamos por parámetro.
	directorio = opendir(path);
	//Si no se ha podido abrir el directorio se acaba la ejecución de la función y se muestra un error.
	if(directorio==NULL){
		printf("No existe el directorio, utilice la funcion correctamente: ls (-l) (directorio) \n");
		return;
	}
	//Estructura que representa la siguiente entrada del directorio que hemos abierto.
	struct dirent * entry;
	//Estructura que almacenará los datos de los archivos del directorio.
	struct stat arc;
	//Mientras que haya entradas en el directorio se podrán ver sus datos.
	while((entry = readdir(directorio)) != NULL){
		//Si la entrada que ha leído no es '.' o '..' se mostrarán.
		if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0){
			//Examina el fichero de entry y guarda sus datos en la estructura arc.			
			stat(entry->d_name,&arc);
			//Muestra el nombre del archivo.
			printf("%s\n", entry->d_name);
			//Identificador de usuario.
			printf("\tUID: %d", arc.st_uid);
			//Identificador de grupo.
			printf("\tGID: %d\n", arc.st_gid);
			//Permisos del archivo: mostrado en octal.
			printf("\tPermisos (octal): %o\n", arc.st_mode);
	
			//Muestra el tipo de archivo, para escribir el tipo usamos un switch dependiendo del valor de arc.st_mode
			//linux.die.net/man/2/stat
			printf("\tTipo de archivo: ");
			switch (arc.st_mode & S_IFMT) {
	 		   case S_IFBLK:  printf("\tblock device\n");            break;
	 		   case S_IFCHR:  printf("\tcharacter device\n");        break;
	 		   case S_IFDIR:  printf("\tdirectory\n");               break;
	 		   case S_IFIFO:  printf("\tFIFO/pipe\n");               break;
	 		   case S_IFLNK:  printf("\tsymlink\n");                 break;
	 		   case S_IFREG:  printf("\tregular file\n");            break;
	 		   case S_IFSOCK: printf("\tsocket\n");                  break;
	 		   default:       printf("\tunknown?\n");                break;
	 	   }		
			//Tamaño del archivo en bytes.
			printf("\tTamanyo: %ld bytes", arc.st_size);
			//Fecha de modificación. Pasa el valor de st_time (tipo time_t) a string.			
			printf("\tModificado en %s\n\n", ctime(&arc.st_mtime));
		}
	}
	//Cerramos el directorio una vez acabadas las entradas.
	closedir(directorio);
}

/*
		2. mils()
*/
void mils(orden O){
	//Variable donde se guardará la ruta. En principio será la actual.
	char path[MAX_SIZE_BUFFER];
	//Atribuimos valor a la ruta.
	getcwd(path, MAX_SIZE_BUFFER);
	//Directorio que abriremos.
	DIR *directorio;
	//Estructura que representa la siguiente entrada del directorio que hemos abierto.
	struct dirent * entry;
	//Si solo se ha pasado un argumento (ls), se muestran solo los nombres de los archivos.
	if(O.argc == 1){
		//Abrimos el directorio actual.
		directorio = opendir(path);
		//Mientras haya entradas en el directorio las mostrará.
		while((entry = readdir(directorio)) != NULL){
			//Si la entrada es '.' o '..' no lo imprimirá por pantalla.
			if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0){			
				printf("%s\n", entry->d_name);
			}
		}
		//Cierra el directorio actual.
		closedir(directorio);
	}else{
		//Si se han introducido dos argumentos en la orden.
		if(O.argc == 2){
			//Si el segundo es '-l' llamaremos a la función auxiliar que imprime los detalles de los archivos de éste directorio.
			if(strcmp(O.argv[1], "-l") == 0){
				l(path);
			//Si no es '-l' es porque se ha pasado una carpeta de un directorio diferente.
			}else{
				//Abrimos el directorio que se introduce.
				directorio=opendir(O.argv[1]);
				//Si el segundo argumento no es un directorio o no se puede abrir se avisa por pantalla y finaliza la función.
				if(directorio==NULL){
					printf("No existe el directorio,utilice la funcion correctamente: ls (-l) directorio.\n");				
					return;
				}
				//Mientras haya entradas en el directorio las mostrará.
				while((entry = readdir(directorio)) != NULL){
					//Si la entrada es '.' o '..' no lo imprimirá por pantalla.
					if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0){			
						printf("%s\n", entry->d_name);
					}
				}
				//Cerramos el directorio que le hemos pasado.
				closedir(directorio);
			}
		//Si se han introducido 3 argumentos es porque se ha optado por: ls -l directorio
		}else{
			if(O.argc == 3){
				//Si el segundo argumento es '-l' llamamos a la función auxiliar que imprimirá los detalles pasándole la carpeta del tercer argumento.
				if(strcmp(O.argv[1], "-l") == 0){
					l(O.argv[2]);			
				//Si no se ha introducido '-l' como segundo argumento se ha llamado mal a la función.				
				}else{
					printf("Utilice la funcion correctamente: ls (-l) (directorio) \n");
					return;
				}
			//Si hay más de 3 argumentos se mostrará un error por pantalla.
			}else{
				printf("Utilice la funcion correctamente: ls (-l) (directorio) \n");
				return;
			}
		}
	}
}
/*
		3. mimkdir()
*/
void mimkdir(orden O){
	//Si no se ha pasado el nombre de la carpeta que queramos le atribuimos un nombre por defecto.
	if(O.argc == 1){
		//Los permisos, en octal 0700 indican que solo son de lectura, escritura y ejecución para el usuario.
		mkdir("Nueva carpeta", 0700);
		printf("Carpeta \"Nueva carpeta\" creada con exito.\n");
	}else{
		//Si se introducen dos argumentos se crea la carpeta. Se puede poner el nombre de una carpeta entre comillas.
		if(O.argc == 2){
			//Los permisos, en octal 0700 indican que solo son de lectura, escritura y ejecución para el usuario.
			mkdir(O.argv[1], 0700);
			printf("Carpeta \"%s\" creada con exito.\n", O.argv[1]);
		}else{
			//Si se han introducido más de dos argumentos es que la nueva carpeta tiene más de una palabra como nombre.
			printf("Ha ocurrido un problema al crear la carpeta.\nUso: mkdir \"nombre de directorio nuevo\"\n");
		}
	}
}
/*
		4. mirmdir()
*/

void mirmdir(orden O){
	//Si la orden tiene dos argumentos es porque la carpeta a eliminar tiene una sola palabra, es inmediato.
	if(O.argc == 2){
		if(rmdir(O.argv[1]) == 0){
			printf("Carpeta '%s' eliminada con exito.\n", O.argv[1]);
		}else{
			printf("Ha ocurrido un problema al eliminar la carpeta.\n");
		}
	}else{
		//Si hay más de dos argumentos es que se ha llamado mal a la función.		
		printf("Ha ocurrido un problema al eliminar la carpeta.\nUso: rmdir \"Nombre de la carpeta\"\n");
		
	}
}
/*
		5. micd()
*/

void micd(orden O){
	//Si se introducen dos argumentos es porque el directorio al que queremos acceder tiene solo una palabra, por tanto vuelve a ser inmediato.
	if(O.argc == 2){
		//Si se puede cambiar el directorio se avisa por pantalla.
		if(chdir(O.argv[1]) == 0){
			printf("Directorio cambiado con exito a \"%s\".\n", O.argv[1]);
		//Si el segundo argumento no coincide con ningún directorio se muestra un mensaje de error.
		}else{
			printf("No se ha podido cambiar el directorio.\n");
		}
	//Si hay más de dos argumentos es que se ha llamado mal a la función.
	}else{
		if(O.argc > 2){
			printf("Error en la orden cd.\nUso: cd \"Nombre directorio\"\n");
		}
	}
}
/*
		6. micat()
*/
void micat(orden O){
	//Abrimos el archivo que le pasamos por parámetro como solo lectura.
	int fd = open(O.argv[1],O_RDONLY);
	//Si el descriptor es diferente de -1, existe, lo mostraremos.
	if(fd != -1){
		//Indicamos el tamaño en bytes del archivo poniendo el puntero al final de éste.
		int tamanyoArchivo = lseek(fd, 0, SEEK_END);
		//Array de caracteres dinámico donde se alojarán los caracteres del archivo.
		char *map;
		//Proyectamos en memoria, en map, el archivo que le pasamos por argumento con el tamaño indicado antes.
		map = mmap(0, tamanyoArchivo, PROT_READ, MAP_SHARED, fd, 0);
		//Si la proyección en memoria ha fallado se cierra el archivo y se finaliza la ejecución de la función.
		if(map == MAP_FAILED){
			close(fd);
			printf("Error al proyectar el archivo en memoria\n");
			return;
		}
		
		int i=0;
		printf("--------Mostrando el archivo %s--------\n\n", O.argv[1]);
		//Mientras que i sea menor que el tamaño del archivo que imprima caracter a caracter.
		//Podemos hacer i++ porque el tamaño de un caracter es de 1 byte.
		while(i < tamanyoArchivo){
			printf("%c", map[i]);
			i++;
		}
		//Se elimina la proyección en memoria.
		if(munmap(map, tamanyoArchivo) == -1){
			printf("Error al liberar el archivo de memoria\n");
			return;
		}
	//Si el descriptor del archivo es -1 significa que no lo ha podido abrir.
	}else{
		printf("No se puede abrir el archivo '%s'. Es posible que no exista.\n", O.argv[1]);
	}
}
/*
		7. micp()
*/
void micp(orden O){
	//Si el número de argumentos es tres, se procede.
	if(O.argc == 3){
		//Abrimos el archivo origen solo como lectura.
		int fdOrigen = open(O.argv[1], O_RDONLY);
		//Abrimos el archivo destino como lectura y escritura.
		int fdDestino = open(O.argv[2], O_RDWR);
		//Si el archivo origen existe, descriptor diferente de -1.
		if(fdOrigen != -1){
			//Si no hemos podido abrir el archivo destino será porque no existe, entonces lo crearemos.
			if(fdDestino == -1){
				//Abrimos el archivo pero O_CREAT para crearlo, con modo escritura solo.
				fdDestino = open(O.argv[2], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
				//Avisamos de que hemos creado el archivo destino.
				printf("Se ha creado el archivo %s\n", O.argv[2]);
			}
			//Buffer que copiará caracter a caracter del archivo origen al destino.
			char buffer;
			//Mientras que la lectura del buffer en el origen sea distina de nula, aún haya caracteres que copiar.
			while(read(fdOrigen, &buffer, sizeof(buffer)) != 0){
				//Escribimos el contenido del buffer en el archivo destino.
				write(fdDestino, &buffer, sizeof(buffer));
			}
			printf("Copiado el contenido de %s en %s con exito.\n", O.argv[1], O.argv[2]);
			//Cerramos los dos archivos.			
			if(close(fdOrigen) == -1){
				printf("Error al cerrar el archivo origen.\n");
			}
			if(close(fdDestino) == -1){
				printf("Error al cerrar el archivo destino.\n");
			}
		//Si no podemos abrir el origen no habrá copia alguna.
		}else{
			printf("No se ha podido abrir el archivo origen.\n");
		}
	}else{
		printf("Error al llamar a cp.\nUso: cp archivo_origen archivo_destino\n");
	}
}
/*
		8. mirm()
*/
void mirm(orden O){
	//Debe haber dos argumentos.
	if(O.argc == 2){
		//Si la llamada a unlink ha sido correcta avisamos por pantalla.
		if(unlink(O.argv[1]) != -1){
			printf("Archivo eliminado con exito.\n");
		}else{
			//Al igual que si hubiera ocurrido un error.
			printf("Hubo un problema al eliminar el archivo.\n");
		}
	}else{
		printf("Ha ocurrido un problema al borrar el archivo.\n");
	}
}
/*
		9. miexit()
*/
void miexit(){
	//Imprimimos un mensaje cuco y llamamos a la función exit.
	printf("Hasta pronto :)\n\n");
	exit(0);
}
