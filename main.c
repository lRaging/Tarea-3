#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"
#include "heap.h"
#include "list.h"
#include "stack.h"


typedef struct {
  char *nombre;
  int prioridad;
  List *tareasPrecedentes;
  int contTareas;  
} tipoTarea;

typedef struct
{
  List *listatareasPrecedentes;
  List * listaTareasDondeAparece;
  int accion; // 0 = Agregar tarea ; 1 = Establecer tarea precedente ; 2 = Eliminar tarea
  char *nombreTareaAEliminarPrecedenica;
  char *nombreTarea;
  int prioridad1;

} tipoAccion;

/*void imprimir_mapa_inverso(HashMap *mapaInverso) {
  printf("Contenido del mapa inverso:\n");

  Pair *pair = firstMap(mapaInverso);
  while (pair != NULL) {
    char *tareaPrecedente = pair->key;
    List *tareas = pair->value;

    printf("Tarea Precedente: %s\n", tareaPrecedente);
    printf("Tareas en las que aparece:\n");

    char *current = firstList(tareas);

    while (current != NULL) {
      char *tarea = current;
      printf("- %s\n", tarea);
      current = nextList(tareas);
    }

    printf("\n");

    pair = nextMap(mapaInverso);
  }
}*/

void Agregar_tarea(HashMap *mapaTarea, Stack *acciones) {
  printf("Ingrese el nombre de la tarea y su prioridad, separada por una coma\n");
  int prioridad;
  char *nombre;
  tipoTarea *tareaLocal = malloc(sizeof(tipoTarea));
  
  // Leer el nombre y la prioridad de la tarea
  scanf(" %m[^,],%d", &nombre, &prioridad);
  getchar();
  
  // Crear una nueva tarea y asignar los valores correspondientes
  tareaLocal->nombre = strdup(nombre);
  tareaLocal->prioridad = prioridad;
  tareaLocal->tareasPrecedentes = createList();
  tareaLocal->contTareas=0;
  // Crear una instancia de tipoAccion para la acción de agregar tarea
  tipoAccion *accionTarea=  malloc(sizeof(tipoAccion));
  accionTarea->accion=0;
  accionTarea->nombreTarea=tareaLocal->nombre;
  // Insertar la acción en la pila
  stack_push(acciones, accionTarea);
  // Insertar la tarea en el mapa de tareas
  insertMap(mapaTarea, tareaLocal->nombre, tareaLocal);
  printf("\n——————————————————————————————————————————————————————————————————————————\n");
  printf("Tarea agregada con éxito.\n");
  printf("\n——————————————————————————————————————————————————————————————————————————\n");
}

void establecer_tarea_precedentes(HashMap *mapaTarea,HashMap *mapaInverso,Stack *acciones) {
  char input[100];
  printf("Ingrese el nombre de las dos tareas separadas por una coma: ");
  fgets(input, 100, stdin);

  char *nombreTarea1 = strtok(input, ",");
  char *nombreTarea2 = strtok(NULL, ",\n");

  // Verificar si existen las tareas
  if (nombreTarea1 == NULL || nombreTarea2 == NULL) {
    printf("Error leyendo las tareas\n");
    return;
  }
  
 // Buscar en el mapa para verificar la existencia de las tareas
  Pair *node1 = searchMap(mapaTarea, nombreTarea1);
  Pair *node2 = searchMap(mapaTarea, nombreTarea2);

  if (node1 == NULL) {
    printf("La tarea %s no se encuentra en la lista\n", nombreTarea1);
    return;
  }

  if (node2 == NULL) {
    printf("La tarea %s no se encuentra en la lista\n", nombreTarea2);
    return;
  }
  tipoTarea *tarea1 = node1->value;
  tipoTarea *tarea2 = node2->value;

  // Verificar si la tarea 2 ya está en la lista de tareas precedentes de la tarea 1
  for(char* auxiliar=firstList(tarea1->tareasPrecedentes);auxiliar!=NULL;auxiliar=nextList(tarea1->tareasPrecedentes)){
    printf("\n——————————————————————————————————————————————————————————————————————————\n");
    printf("La tarea %s ya se encuentra en la lista de tareas de la tarea %s\n",tarea2->nombre,tarea1->nombre);
    printf("\n——————————————————————————————————————————————————————————————————————————\n");
    if(strcmp(strdup(auxiliar), strdup(tarea2->nombre))==0) return;
    
  }
  
  // Agregar la tarea 2 a la lista de tareas precedentes de la tarea 1
  pushBack(tarea1->tareasPrecedentes, tarea2->nombre);
  tarea1->contTareas++;
  List* tareasPredececesoras =createList();
  
  // Actualizar el mapa inverso
    if (searchMap(mapaInverso, tarea2->nombre) == NULL) {
      List *tareasPredecesoras = createList();
      pushBack(tareasPredecesoras, tarea1->nombre);
      insertMap(mapaInverso, strdup(tarea2->nombre), tareasPredecesoras);
    } else {
      List *tareasPredecesoras = searchMap(mapaInverso, tarea2->nombre)->value;
      pushBack(tareasPredecesoras, tarea1->nombre);
    }

  // Crear y agregar la acción a la pila de acciones
  tipoAccion *accionTarea= malloc(sizeof(tipoAccion));
  accionTarea->accion=1;
  accionTarea->nombreTarea=tarea1->nombre;
  accionTarea->nombreTareaAEliminarPrecedenica=tarea2->nombre;
  stack_push(acciones, accionTarea);
  
  printf("La tarea %s debe realizarse antes que la tarea %s.\n", tarea1->nombre,tarea2->nombre);
}

void mostrar_tareas_por_hacer(HashMap *mapaTarea) {
  printf("Tareas por hacer, ordenadas por prioridad y precedencia:\n");

  // Crear un nuevo heap auxiliar para ordenar las tareas
  Heap *tareasOrdenadas = createHeap();

  // Insertar todas las tareas pendientes en el heap auxiliar
  Pair *pair = firstMap(mapaTarea);
  while (pair != NULL) {
    tipoTarea *tarea = pair->value;
    heap_push(tareasOrdenadas, tarea, -(tarea->contTareas * 1000 + tarea->prioridad));
    pair = nextMap(mapaTarea);
  }

  // Mostrar las tareas por hacer en el orden correcto
  int contador = 1;
  while (!isEmptyHeap(tareasOrdenadas)) {
    tipoTarea *tarea = heap_top(tareasOrdenadas);
    heap_pop(tareasOrdenadas);

    printf("%d. %s (Prioridad: %d)", contador, tarea->nombre, tarea->prioridad);
    
    if (!isEmptyList(tarea->tareasPrecedentes)) {
      printf(" - Precedente(s): ");
      char *node = firstList(tarea->tareasPrecedentes);
      while (node != NULL) {
        char *precedente = node;
        printf("%s ", precedente);
        node = nextList(tarea->tareasPrecedentes);
      }
    }
    printf("\n");
    contador++;
  }
  // Liberar memoria
  free(tareasOrdenadas);
}

void eliminar_tarea(HashMap *mapaTarea, HashMap *mapaInverso,Stack *acciones) {
  char *nombre;
  tipoTarea *tarea;

  printf("Ingrese el nombre de la tarea: ");
  scanf(" %m[^\n]", &nombre);
  getchar();

  // Buscar la tarea en el mapa de tareas
  Pair *pair = searchMap(mapaTarea, nombre);

  if (pair == NULL) {
    printf("La tarea no existe en el mapa.\n");
    free(nombre);
    return;
  }

  tarea = pair->value;

  // Si la tarea tiene tareas precedentes, mostrar una advertencia
  if (tarea->contTareas > 0) {
    printf("La tarea tiene tareas precedentes. ¿Está seguro que desea eliminarla? (s/n): ");
    char confirmacion;
    scanf("%c", &confirmacion);
    if (confirmacion != 's') {
    free(nombre);
    return;
    }
  }
  tipoAccion *accionTarea= malloc(sizeof(tipoAccion));
  accionTarea->accion=2;
  accionTarea->nombreTarea=strdup(nombre);
  accionTarea->prioridad1=tarea->prioridad;
  accionTarea->listatareasPrecedentes=createList();
  // Si la tarea no está en el mapa inverso, se elimina enseguida del mapaTareas y se elimina la tarea de todas las listas de tareas del mapaInverso
  if (searchMap(mapaInverso, strdup(nombre)) == NULL) {
    char * tareaEliminar=firstList(tarea->tareasPrecedentes);
    while(tareaEliminar!= NULL){
      pushBack(accionTarea->listatareasPrecedentes,tareaEliminar);
      List * Q = searchMap(mapaInverso, tareaEliminar)->value;
      for(char* tarea1=firstList(Q);tarea1!=NULL;tarea1=nextList(Q)){
        if(strcmp(strdup(tarea1), strdup(tarea->nombre))==0) popCurrent(Q); // Eliminar la tarea de la lista de tareas donde aparece
      }
      
      if(firstList(Q)==NULL) eraseMap(mapaInverso, tareaEliminar);
      tareaEliminar=nextList(tarea->tareasPrecedentes);
    }
    
    // Insertar la acción en la pila de acciones
    stack_push(acciones, accionTarea);
    // Eliminar la tarea del mapaTarea y limpiar su lista de tareas precedentes
    eraseMap(mapaTarea, nombre);  
    cleanList(tarea->tareasPrecedentes);
    free(nombre);
    printf("\n——————————————————————————————————————————————————————————————————————————\n");
    printf("La tarea fue eliminada con exito");
    printf("\n——————————————————————————————————————————————————————————————————————————\n");
    return;
  }
  // Si la tarea está en el mapa inverso, también se elimina del mapaTarea y de todas las listas donde aparece
  if (searchMap(mapaInverso, strdup(nombre)) != NULL) {
    
    char * tareaEliminar2=firstList(tarea->tareasPrecedentes);
    accionTarea->listaTareasDondeAparece=createList(); // Crear una lista para almacenar las tareas donde aparece la tarea eliminada
    
    while(tareaEliminar2!= NULL){
      
      pushBack(accionTarea->listatareasPrecedentes,tareaEliminar2); // Agregar tarea a la lista de tareas precedentes de la acción
      List * Q = searchMap(mapaInverso, tareaEliminar2)->value; // Obtener la lista de tareas donde aparece la tarea a eliminar
      for(char* tarea1=firstList(Q);tarea1!=NULL;tarea1=nextList(Q)){
        if(strcmp(strdup(tarea1), strdup(tarea->nombre))==0) popCurrent(Q); // Eliminar la tarea de la lista de tareas donde aparece
      }
      if(firstList(Q)==NULL) eraseMap(mapaInverso, tareaEliminar2); // Si la lista de tareas donde aparece la tarea está vacía, eliminarla del mapa inverso
      tareaEliminar2=nextList(tarea->tareasPrecedentes);
    }
    
    List * ListadeP = searchMap(mapaInverso, tarea->nombre)->value;// Obtener la lista de tareas donde aparece la tarea eliminada
    char * tareaEliminar=firstList(ListadeP);
    while(tareaEliminar!=NULL){
      pushBack(accionTarea->listaTareasDondeAparece, tareaEliminar); // Agregar tarea a la lista de tareas donde aparece la tarea eliminada
      tipoTarea *  tarea2 = searchMap(mapaTarea, tareaEliminar)->value; // Obtener la tarea donde aparece la tarea eliminada
      for(char* tarea1=firstList(tarea2->tareasPrecedentes);tarea1!=NULL;tarea1=nextList(tarea2->tareasPrecedentes)){
        
        if(strcmp(strdup(tarea1), strdup(tarea->nombre))==0) {
          
          popCurrent(tarea2->tareasPrecedentes);// Eliminar la tarea de la lista de tareas precedentes de la tarea donde aparece
          tarea2->contTareas--;
        }

      }
      tareaEliminar=nextList(ListadeP);
    }

  // Eliminar la tarea del mapaInverso y del mapaTarea
   eraseMap(mapaInverso, tarea->nombre);
   eraseMap(mapaTarea, nombre);
    
   stack_push(acciones, accionTarea);
  }
  free(nombre);
  printf("\n——————————————————————————————————————————————————————————————————————————\n");
  printf("La tarea fue eliminada con exito");
  printf("\n——————————————————————————————————————————————————————————————————————————\n");
}

void deshacerUltimaAccion(HashMap *mapaTarea, HashMap *mapaInverso,Stack *acciones) {
  // Verificar si hay acciones para deshacer
  if(stack_top(acciones)==NULL){
    printf("No hay acciones para deshacer\n");
    return;
  } 
  // Obtener la última acción realizada
  tipoAccion * accionTarea =  stack_top(acciones);

  // Deshacer la acción según el tipo
  if(accionTarea->accion==0){
    tipoTarea  * tarea = searchMap(mapaTarea, accionTarea->nombreTarea)->value;
    eraseMap(mapaTarea,tarea->nombre);
    printf("\n——————————————————————————————————————————————————————————————————————————\n");
    printf("Se deshizo la ultima accion\n");
    printf("\n——————————————————————————————————————————————————————————————————————————\n");
    stack_pop(acciones);
  }  
  if(accionTarea->accion == 1){
    
    tipoTarea *tarea= searchMap(mapaTarea, accionTarea->nombreTarea)->value;
    char *nombreTareaPrecedente = accionTarea->nombreTarea;
    
    for(char* tarea1=firstList(tarea->tareasPrecedentes) ; tarea1!=NULL; tarea1=nextList(tarea->tareasPrecedentes)){
      if(strcmp(strdup(tarea1), strdup(accionTarea->nombreTareaAEliminarPrecedenica))==0) popCurrent(tarea->tareasPrecedentes); // Eliminar la tarea precedente de la lista de tareas precedente
    }

    // Eliminar la tarea actual de la lista de tareas donde aparece la tarea precedente
    List * ListaDeTareasDondeAparece = searchMap(mapaInverso, accionTarea->nombreTareaAEliminarPrecedenica)->value;
    for(char* tarea2=firstList(ListaDeTareasDondeAparece) ; tarea2!=NULL; tarea2=nextList(ListaDeTareasDondeAparece)){
      if(strcmp(strdup(tarea2), strdup(tarea->nombre))==0) popCurrent(ListaDeTareasDondeAparece);
    }
     // Si la lista de tareas donde aparece la tarea precedente está vacía, eliminarla del mapa inverso
    if(firstList(ListaDeTareasDondeAparece)==NULL) eraseMap(mapaInverso, accionTarea->nombreTareaAEliminarPrecedenica);     
    tarea->contTareas--;
    stack_pop(acciones);
    printf("\n——————————————————————————————————————————————————————————————————————————\n");
    printf("Se deshizo la ultima accion\n");
    printf("\n——————————————————————————————————————————————————————————————————————————\n");
  } 
  if (accionTarea->accion == 2) {
    
    tipoTarea * tarea =malloc(sizeof(tipoTarea));
    tarea->nombre=accionTarea->nombreTarea;
    tarea->prioridad=accionTarea->prioridad1;
    tarea->tareasPrecedentes=createList();
    tarea->contTareas=0;

     // Comprobar si hay tareas donde aparece la nueva tarea a deshacer
    if(firstList(accionTarea->listaTareasDondeAparece)!=NULL){
      char * tareaDondeAparece = firstList(accionTarea->listaTareasDondeAparece);

      // Recorrer la lista de tareas donde aparece la nueva tarea
      while(tareaDondeAparece!=NULL){
        puts(tareaDondeAparece);

        // Verificar si la tarea actual no existe en el mapa inverso y agregarla si es necesario
        if(searchMap(mapaInverso,tarea->nombre)==NULL){
          List * listaTareasAuxiliar = createList();
          pushBack(listaTareasAuxiliar, tareaDondeAparece);
          insertMap(mapaInverso, tarea->nombre, listaTareasAuxiliar);
        } else {
          // Agregar la tarea actual al mapa inverso si ya existe
        if(searchMap(mapaInverso,tarea->nombre)!=NULL){
          pushBack(searchMap(mapaInverso,tarea->nombre)->value, tareaDondeAparece);
          }
        }
        // Obtener la tarea actual y agregarla a la lista de tareas precedentes
        tipoTarea *tarea3= searchMap(mapaTarea, tareaDondeAparece)->value;
        pushBack(tarea3->tareasPrecedentes, tareaDondeAparece);
        tarea3->contTareas++;
        tareaDondeAparece = nextList(accionTarea->listaTareasDondeAparece);
      }
      
    }
     // Comprobar si hay tareas precedentes para la nueva tarea a deshacer
    if(firstList(accionTarea->listatareasPrecedentes)!=NULL){
      char * tareaAuxiliar = firstList(accionTarea->listatareasPrecedentes);
       // Recorrer la lista de tareas precedentes
      while(tareaAuxiliar!=NULL){
        // Agregar la tarea precedente a la lista de tareas precedentes de la nueva tarea
        pushBack(tarea->tareasPrecedentes, tareaAuxiliar);
        tarea->contTareas++;
        
        // Verificar si la tarea precedente no existe en el mapa inverso y agregarla si es necesario
        if(searchMap(mapaInverso,tareaAuxiliar)==NULL){
          List * listaTareasAuxiliar = createList();
          pushBack(listaTareasAuxiliar, tarea->nombre);
          insertMap(mapaInverso, tareaAuxiliar, listaTareasAuxiliar);
        } else {
           // Agregar la tarea precedente al mapa inverso si ya existe
        if(searchMap(mapaInverso,tareaAuxiliar)!=NULL){
          pushBack(searchMap(mapaInverso,tareaAuxiliar)->value, tarea->nombre);
          }
        }

        tareaAuxiliar=nextList(accionTarea->listatareasPrecedentes);
      }
    }
    stack_pop(acciones);
    insertMap(mapaTarea, tarea->nombre, tarea);
    
    
    printf("\n——————————————————————————————————————————————————————————————————————————\n");
    printf("Se deshizo la ultima accion\n");
    printf("\n——————————————————————————————————————————————————————————————————————————\n");
  }
}

void Cargar_datos_de_tareas_desde_un_archivo_de_texto(char* nombre_archivo, HashMap* mapaTarea,HashMap* mapaInverso){
  char caracter[100];
  FILE *archivoCsv = fopen(nombre_archivo, "r");
  if (archivoCsv == NULL) {
    printf("\n——————————————————————————————————————————————————————————————————————————\n");
    printf("El archivo %s no existe en el directorio actual o esta mal escrito.\n", nombre_archivo);
    printf("——————————————————————————————————————————————————————————————————————————\n\n");
    return;
  }
  fgets(caracter, 99, archivoCsv);
  int prioridad;
  char *nombre, *tareasPrecedente;
  while (fscanf(archivoCsv, " %m[^,], %d,%m[^\n]\n", &nombre, &prioridad, &tareasPrecedente) != EOF) {
    
    tipoTarea *tarea = malloc(sizeof(tipoTarea));
    tarea->nombre = strdup(nombre);
    tarea->prioridad = prioridad;
    tarea->tareasPrecedentes = createList();
    tarea->contTareas=0;
    if (tareasPrecedente != NULL){
      
      char *token = strtok(tareasPrecedente, " ");
      if (strlen(tareasPrecedente) > 0 && strcmp(tareasPrecedente, " ") != 0) {
        
        while (token != NULL) {
          
          if (strcmp(token, " ") != 0) {
            if (strlen(token) > 0) pushBack(tarea->tareasPrecedentes, strdup(token));

            List* tareasPredececesoras =createList();
            if (searchMap(mapaInverso, token) == NULL) {
              List *tareasPredecesoras = createList();
              pushBack(tareasPredecesoras, tarea->nombre);
              insertMap(mapaInverso, strdup(token), tareasPredecesoras);
            } else {
              List *tareasPredecesoras = searchMap(mapaInverso, token)->value;
              pushBack(tareasPredecesoras, tarea->nombre);
            }  
            tarea->contTareas++;
            }  
          token = strtok(NULL, " ");
          }
        }
      }
    
    insertMap(mapaTarea, tarea->nombre, tarea);
    
  }
}

int main() {
  HashMap *mapaTarea = createMap((long)100);
  HashMap *mapaInverso = createMap((long)100);
  Stack *acciones = stack_create();
  int opcion, prioridad;
  char caracter[100], *nombre, *tareasPrecedente,*archivoCargado;
  FILE *archivoCsv = fopen("tareas.csv", "r");
  fgets(caracter, 99, archivoCsv);

 while (fscanf(archivoCsv, " %m[^,], %d,%m[^\n]\n", &nombre, &prioridad, &tareasPrecedente) != EOF) {
    
    tipoTarea *tarea = malloc(sizeof(tipoTarea));
    tarea->nombre = strdup(nombre);
    tarea->prioridad = prioridad;
    tarea->tareasPrecedentes = createList();
    tarea->contTareas=0;
    if (tareasPrecedente != NULL){
      
      char *token = strtok(tareasPrecedente, " ");
      if (strlen(tareasPrecedente) > 0 && strcmp(tareasPrecedente, " ") != 0) {
        
        while (token != NULL) {
          
          if (strcmp(token, " ") != 0) {
            if (strlen(token) > 0) pushBack(tarea->tareasPrecedentes, strdup(token));

            List* tareasPredececesoras =createList();
            if (searchMap(mapaInverso, token) == NULL) {
              List *tareasPredecesoras = createList();
              pushBack(tareasPredecesoras, tarea->nombre);
              insertMap(mapaInverso, strdup(token), tareasPredecesoras);
            } else {
              List *tareasPredecesoras = searchMap(mapaInverso, token)->value;
              pushBack(tareasPredecesoras, tarea->nombre);
            }  
            tarea->contTareas++;
            }  
          token = strtok(NULL, " ");
          }
        }
      }
    
    insertMap(mapaTarea, tarea->nombre, tarea);
    
  }
  fclose(archivoCsv);

  while (true) {
    
    printf("Elija opcion\n");

    printf("1.- Agregar tarea \n");
    printf("2.- Establecer precedencia entre tareas \n");
    printf("3.- Mostrar tareas por hacer \n");
    printf("4.- Marcar tarea como completada \n");
    printf("5.- Deshacer última acción\n");
    printf("6.- Cargar datos de tareas desde un archivo de texto \n");
    scanf("%d", &opcion);
    getchar();

    switch (opcion) {

    case 1:
      Agregar_tarea(mapaTarea,acciones);
      break;
    case 2:
      establecer_tarea_precedentes(mapaTarea,mapaInverso,acciones);
      break;
    case 3:
      mostrar_tareas_por_hacer(mapaTarea);
      break;
    case 4:
       eliminar_tarea(mapaTarea,mapaInverso,acciones);
      break;
    case 5:
       deshacerUltimaAccion(mapaTarea,mapaInverso,acciones);
      
      break;
    case 6:
        printf("\n————————————————————————————————————————————————————————————\n");
      printf("Ingrese el nombre del archivo, introduzca el formato (.csv)\n");
      scanf("%m[^\n]",&archivoCargado);
      getchar();
      if (strstr(archivoCargado,".csv")==0)
      {
        printf("\nEl formato del archivo '%s' es incorrecto.\n", archivoCargado);
        printf("————————————————————————————————————————————————————————————\n\n");
        break;
      }
      Cargar_datos_de_tareas_desde_un_archivo_de_texto(archivoCargado, mapaTarea, mapaInverso);
      printf("————————————————————————————————————————————————————————————\n\n");
        break;
    default:
      return 0;
    }
  }
  return 0;
}