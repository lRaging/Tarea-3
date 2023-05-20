# Tarea-Estructura 3

En la solución del problema planteado para la tarea 3, se decidió utilizar estructuras de datos como mapa, lista, pila y montículo, debido a que son herramientas eficientes y adecuadas para las operaciones que se requerían en el problema.

(1)Como compilar y ejecutar el codigo:

 Para que el código funcione, se debe tener un compilador de C, además, se deben tener en la misma carpeta de directorio los siguientes archivos:
-main.c, es el código ejecutable.
-hashmap.c y hashmap.h
-list.c y list.h
-stack.h
-heap.c y heap.h
-El archivo tareas.cvs , dado que es el archivo a analizar principalmente, por lo que si no está incluido en el directorio, el código tiene error de segmentación y falla.

(2) Las opciones que funcionan correctamente y las que no lo hacen indicando posibles causas;

Para el codigo primero se guardan las tareas en el mapa principal con todos sus datos y una lista con sus tareas precedentes, a la vez las tareas se guardan en un mapa inverso, pero las claves son las tareas precedentes y el dato es una lista de las tareas en donde aparece.(Si quiere ver lo que guarda este mapa esta la funcion imprimir_mapa_inverso en notas).

-Agregar tarea: Permite al usuario ingresar el nombre y la prioridad de una tarea, crea una nueva instancia de tipoTarea con los valores proporcionados, crea una instancia de tipoAccion para la acción de agregar tarea y la inserta en la pila acciones. Además, inserta la tarea en el mapa de tareas utilizando insertMap.

-Establecer tarea precedentes:La función permite establecer una relación de precedencia entre dos tareas existentes. Verifica si las tareas ingresadas existen en el mapa de tareas, agrega la segunda tarea a la lista de tareas precedentes de la primera, actualiza el mapa inverso y registra esta acción en la pila de acciones.

-Mostrar tareas por hacer: La función mostrar_tareas_por_hacer muestra las tareas pendientes ordenadas por prioridad y precedencia. Utiliza un heap auxiliar para ordenar las tareas en base a un valor que considera tanto la cantidad de tareas precedentes como la prioridad. Luego, recorre el heap para imprimir cada tarea junto con su prioridad y, si tiene, sus tareas precedentes.

-Eliminar tarea: La función permite eliminar una tarea del mapa de tareas y a la vez actualizar el mapa inverso. Primero, solicita al usuario el nombre de la tarea a eliminar. Si la tarea existe, verifica si tiene tareas precedentes y solicita confirmación para eliminarla. Luego, crea una acción de eliminación y actualiza las listas de tareas precedentes en el mapa inverso. Finalmente, elimina la tarea de los mapas correspondientes.

-Deshacer última acción: La función permite deshacer la última acción realizada en el sistema de gestión de tareas. Primero, verifica si hay acciones para deshacer en la pila. Luego, dependiendo del tipo de acción, revierte los cambios realizados. Si la acción es eliminar una tarea, la restaura en el mapa de tareas y mapa inverso. Si la acción es agregar una tarea precedente a otra, la elimina de la lista de tareas precedentes y del mapa inverso. Si la acción es agregar una nueva tarea, la elimina del mapa de tareas.

-Cargar datos de tareas desde un archivo de texto: Esta funcion hace lo  mismo que el main, pero tiene un problema serio,  el archivo ingresado NO puede tener tareas repetidas del archivo original (tareas.csv), ya que genera errores en el MapaInverso, se duplican los datos en mapa inverso.


(3) coevaluación del trabajo en equipo (usando rúbrica).

 Nuestra pareja hizo la tarea a través de la plataforma Repl.it para realizar todas nuestras tareas de manera eficiente. Ambos compartimos un objetivo común y trabajamos juntos para alcanzarlo. Nos comunicamos de manera fluida y clara, lo que nos mantiene en atento a cada problema en todo momento. Cada uno de nosotros aporta habilidades y conocimientos únicos que se complementan mutuamente, lo que nos permite abordar cualquier tarea con éxito. La confianza fue importante en nuestra pareja, lo que nos permite trabajar con eficacia y obtener resultados de alta calidad. Estamos comprometidos en apoyarnos en el compañero y en superar cualquier problema presentado en las funciones, logrando asi un trabajo conjunto efectivo y exitoso.
 
╔══════════════════════════════╦═══════════════════════════════════╦══════════════════════════╦══════════════╦══════════════════╦═════════════╦═══════════╗
║    Nombre                    ║ Participación activa y equitativa ║ Resolución de conflictos ║ Colaboración ║  Responsabilidad ║ Comunicación║ Pje Total ║
╠══════════════════════════════╬═══════════════════════════════════╬══════════════════════════╬══════════════╬══════════════════╬═════════════╬═══════════╣
║   Overfore(Parte Perfecta)   ║               20                  ║            20            ║      20      ║        20        ║      20     ║    100    ║
╚══════════════════════════════╩═══════════════════════════════════╩══════════════════════════╩══════════════╩══════════════════╩═════════════╩═══════════╝

(4) coevaluación del aporte individual de cada integrante (usando rúbrica).

╔══════════════════╦═══════════════╦══════════════════╦══════════════╦════════════════════╦═══════════════════╦═══════════╗
║    Nombre        ║ Participación ║  Responsabilidad ║ Comunicacion ║ Calidad de Trabajo ║ Trabajo en Equipo ║ Pje Total ║
╠══════════════════╬═══════════════╬══════════════════╬══════════════╬════════════════════╬═══════════════════╬═══════════╣
║  Lucas Zamora    ║      20       ║        20        ║      20      ║         20         ║         20        ║    100    ║
╠══════════════════╬═══════════════╬══════════════════╬══════════════╬════════════════════╬═══════════════════╬═══════════╣
║  Marcelo Flores  ║      20       ║        20        ║      20      ║         20         ║         20        ║    100    ║
╚══════════════════╩═══════════════╩══════════════════╩══════════════╩════════════════════╩═══════════════════════════════╝

Lucas Zamora   : Demostro un alto nivel de compromiso y responsabilidad en el trabajo, manteniendose enfocado en todo momento. Desde el inicio del proyecto, me esforcé por comprender claramente mis tareas y metas, y trabajé de manera eficiente tratando de tener una buena idea y luego ejecutarla. Mi dedicación y compromiso fueron evidentes a lo largo del proceso, ya que me mantuve enfocado en alcanzar los objetivos establecidos y lograr bueno resultados.

Marcelo Flores : Su habilidad para comunicarme de manera efectiva. Tenía la capacidad de comprender las ideas y explicarlas de forma clara y concisa. Además, cuando surgían problemas, podía pensar en algo para poder resolverlo. Esta habilidad me permitía trabajar de manera eficiente y lograr los objetivos de manera efectiva. Lo que me ayudaba a mantenerme enfocado y lograr resultados de alta calidad en mis tareas.