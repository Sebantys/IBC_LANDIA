#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Visitante {

    int idVisitante;
    char nombre[50];

    int edad;

    /* altura en metros */
    float altura;

    /* peso en kilogramos */
    float peso;

    /* siguiente visitante en la fila */
    struct Visitante* sig;
};

struct FilaEspera {

    /* primer visitante en entrar */
    struct Visitante* frente;

    /* ultimo visitante agregado */
    struct Visitante* final;
};

struct Atraccion {

    int codigoAtraccion;
    char nombre[50];

    /* estados:
       0 = operativa
       1 = mantenimiento
       2 = fuera de servicio
       3 = cerrada por horario */
    int estado;

    /* cantidad maxima de personas por ciclo */
    int capacidadCiclo;

    /* duracion del recorrido en minutos */
    int duracionMinutos;

    /* restricciones de acceso */
    int edadMinima;
    float alturaMinima;
    float pesoMaximo;

    /* fila de espera asociada a la atraccion */
    struct FilaEspera fila;
};

struct ZonaTematica {

    int codigoZona;
    char nombre[50];
    char tematica[50];
    char horario[30];

    /* cantidad maxima de visitantes permitidos */
    int capacidad;

    /* arreglo estatico de punteros a atracciones */
    struct Atraccion* atracciones[10];

    /* cantidad actual de atracciones registradas */
    int cantidadAtracciones;
};

struct NodoZona {

    /* datos reales de la zona */
    struct ZonaTematica* datosZona;

    /* siguiente nodo de la lista simple */
    struct NodoZona* sig;
};

struct NodoEntrada {

    int codigoEntrada;

    /* general, vip, familiar, etc */
    char tipo[30];

    /* 0 = disponible
       1 = usada */
    int usada;

    /* ramas del ABB */
    struct NodoEntrada* izquierda;
    struct NodoEntrada* derecha;
};

struct Parque {

    char nombre[50];

    /* inicio de la lista de zonas tematicas */
    struct NodoZona* listaZonas;

    /* raiz del arbol binario de entradas */
    struct NodoEntrada* arbolEntradas;
};

void agregarZonaTematica(struct Parque* parque) {
    struct ZonaTematica* zona;
    struct NodoZona* nuevoNodo;
    struct NodoZona* aux;

    zona = (struct ZonaTematica*) malloc(sizeof(struct ZonaTematica));
    nuevoNodo = (struct NodoZona*) malloc(sizeof(struct NodoZona));

    printf("\n########## AGREGAR ZONA ##########\n");
    printf("Ingrese código de zona: ");
    scanf("%d", &zona->codigoZona);

    printf("Ingrese nombre de zona: ");
    scanf(" %49[^\n]", zona->nombre);

    printf("Ingrese temática: ");
    scanf(" %49[^\n]", zona->tematica);

    printf("Ingrese horario: ");
    scanf(" %29[^\n]", zona->horario);

    printf("Ingrese capacidad máxima: ");
    scanf("%d", &zona->capacidad);

    zona->cantidadAtracciones = 0;

    nuevoNodo->datosZona = zona;
    nuevoNodo->sig = NULL;

    if (parque->listaZonas == NULL) parque->listaZonas = nuevoNodo;
    else {
        aux = parque->listaZonas;
        while (aux->sig != NULL)
            aux = aux->sig;
        aux->sig = nuevoNodo;
    }

    printf("Zona agregada correctamente.\n");
}

void agregarAtraccion(struct Parque* parque) {
    int codigoZona;
    int i;
    struct NodoZona* auxZona;
    struct Atraccion* atraccion;

    /* verificar que existan zonas registradas */
    if (parque->listaZonas == NULL) {
        printf("No hay zonas registradas. Agregue una zona primero.\n");
        return;
    }

    printf("\n########## AGREGAR ATRACCIÓN ##########\n");
    printf("Ingrese código de zona: ");
    scanf("%d", &codigoZona);

    auxZona = parque->listaZonas;

    while (auxZona != NULL && auxZona->datosZona->codigoZona != codigoZona)
        auxZona = auxZona->sig;
    if (auxZona == NULL) {
        printf("Zona no encontrada.\n");
        return;
    }

    if (auxZona->datosZona->cantidadAtracciones >= 10) {
        printf("No se pueden agregar más atracciones.\n");
        return;
    }

    atraccion = (struct Atraccion*) malloc(sizeof(struct Atraccion));

    printf("Ingrese código de atracción: ");
    scanf("%d", &atraccion->codigoAtraccion);

    printf("Ingrese nombre de atracción: ");
    scanf(" %49[^\n]", atraccion->nombre);

    printf("Ingrese estado:\n");
    printf("0 = operativa\n");
    printf("1 = mantenimiento\n");
    printf("2 = fuera de servicio\n");
    printf("3 = cerrada por horario\n");

    scanf("%d", &atraccion->estado);

    printf("Ingrese capacidad por ciclo: ");
    scanf("%d", &atraccion->capacidadCiclo);

    printf("Ingrese duración en minutos: ");
    scanf("%d", &atraccion->duracionMinutos);

    printf("Ingrese edad mínima: ");
    scanf("%d", &atraccion->edadMinima);

    printf("Ingrese altura mínima: ");
    scanf("%f", &atraccion->alturaMinima);

    printf("Ingrese peso máximo: ");
    scanf("%f", &atraccion->pesoMaximo);

    atraccion->fila.frente = NULL;
    atraccion->fila.final = NULL;

    i = auxZona->datosZona->cantidadAtracciones;
    auxZona->datosZona->atracciones[i] = atraccion;
    auxZona->datosZona->cantidadAtracciones++;

    printf("Atracción agregada correctamente.\n");
}

void agregarVisitanteFila(struct Parque* parque) {
    int codigoZona;
    int codigoAtraccion;
    int i;
    struct NodoZona* auxZona;
    struct Atraccion* atraccion;
    struct Visitante* visitante;

    /* verificar que existan zonas registradas */
    if (parque->listaZonas == NULL) {
        printf("No hay zonas registradas. Agregue una zona primero.\n");
        return;
    }

    printf("\n########## AGREGAR VISITANTE ##########\n");
    printf("Ingrese código de zona: ");
    scanf("%d", &codigoZona);
    auxZona = parque->listaZonas;
    while (
        auxZona != NULL &&
        auxZona->datosZona->codigoZona != codigoZona
    )
        auxZona = auxZona->sig;

    if (auxZona == NULL) {
        printf("Zona no encontrada.\n");
        return;
    }

    /* verificar que la zona tenga atracciones */
    if (auxZona->datosZona->cantidadAtracciones == 0) {
        printf("La zona no tiene atracciones registradas.\n");
        return;
    }

    printf("Ingrese código de atracción: ");
    scanf("%d", &codigoAtraccion);

    atraccion = NULL;

    for (i = 0; i < auxZona->datosZona->cantidadAtracciones; i++)
        if (auxZona->datosZona->atracciones[i]->codigoAtraccion == codigoAtraccion) atraccion = auxZona->datosZona->atracciones[i];

    if (atraccion == NULL) {
        printf("Atracción no encontrada.\n");
        return;
    }
    visitante = (struct Visitante*) malloc(sizeof(struct Visitante));

    printf("Ingrese ID visitante: ");
    scanf("%d", &visitante->idVisitante);

    printf("Ingrese nombre visitante: ");
    scanf(" %49[^\n]", visitante->nombre);

    printf("Ingrese edad: ");
    scanf("%d", &visitante->edad);

    printf("Ingrese altura: ");
    scanf("%f", &visitante->altura);

    printf("Ingrese peso: ");
    scanf("%f", &visitante->peso);

    visitante->sig = NULL;

    if (atraccion->fila.frente == NULL) atraccion->fila.frente = visitante;
    if (atraccion->fila.final != NULL) atraccion->fila.final->sig = visitante;

    atraccion->fila.final = visitante;

    printf("Visitante agregado a la fila.\n");
}

struct NodoEntrada* insertarEntradaABB(struct NodoEntrada* raiz, struct NodoEntrada* nuevaEntrada) {
    if (raiz == NULL) return nuevaEntrada;

    if (nuevaEntrada->codigoEntrada < raiz->codigoEntrada) raiz->izquierda = insertarEntradaABB(raiz->izquierda, nuevaEntrada);
    if (nuevaEntrada->codigoEntrada > raiz->codigoEntrada) raiz->derecha = insertarEntradaABB(raiz->derecha, nuevaEntrada);

    return raiz;
}

void agregarEntrada(struct Parque* parque) {
    struct NodoEntrada* entrada;
    entrada = (struct NodoEntrada*) malloc(sizeof(struct NodoEntrada));

    printf("\n########## AGREGAR ENTRADA ##########\n");
    printf("Ingrese código entrada: ");
    scanf("%d", &entrada->codigoEntrada);

    printf("Ingrese tipo de entrada: ");
    scanf(" %29[^\n]", entrada->tipo);

    printf("Ingrese estado:\n");
    printf("0 = disponible\n");
    printf("1 = usada\n");
    scanf("%d", &entrada->usada);

    entrada->izquierda = NULL;
    entrada->derecha = NULL;

    parque->arbolEntradas = insertarEntradaABB(parque->arbolEntradas, entrada);

    printf("Entrada agregada correctamente al ABB.\n");
}

void menuAgregar(struct Parque* parque) {
    int opcion;
    do {
        printf("\n########## AGREGAR DATOS ##########\n");
        printf("1) Agregar zona temática.\n");
        printf("2) Agregar atracción.\n");
        printf("3) Agregar visitante a fila.\n");
        printf("4) Agregar entrada.\n");
        printf("0) Volver.\n");
        printf("Seleccione opción: ");

        scanf("%d", &opcion);
        if (opcion == 1) agregarZonaTematica(parque);
        if (opcion == 2) agregarAtraccion(parque);
        if (opcion == 3) agregarVisitanteFila(parque);
        if (opcion == 4) agregarEntrada(parque);
        if (opcion == 0) printf("Volviendo...\n");
        if (opcion < 0 || opcion > 4) printf("Opción inválida.\n");
    } while(opcion != 0);
}

void menuOperar() {
    int opcion;
    do {
        printf("\n########## OPERAR DATOS ##########\n");
        printf("1) Buscar datos.\n");
        printf("2) Modificar datos.\n");
        printf("3) Eliminar datos.\n");
        printf("4) Listar datos.\n");
        printf("5) Funciones extra.\n");
        printf("0) Volver.\n");
        printf("Seleccione opción: ");

        scanf("%d", &opcion);
        if (opcion == 1) printf("Buscar datos.\n");
        if (opcion == 2) printf("Modificar datos.\n");
        if (opcion == 3) printf("Eliminar datos.\n");
        if (opcion == 4) printf("Listar datos.\n");
        if (opcion == 5) printf("Funciones extra.\n");
        if (opcion == 0) printf("Volviendo...\n");
        if (opcion < 0 || opcion > 5) printf("Opción inválida.\n");
    } while(opcion != 0);
}

int main() {
    int opcion;
    struct Parque* parque;
    parque = (struct Parque*) malloc(sizeof(struct Parque));
    parque->listaZonas = NULL;
    parque->arbolEntradas = NULL;

    printf("Ingrese nombre del parque: ");
    scanf(" %49[^\n]", parque->nombre);
    do {
        printf("\n########## MENÚ IBCLANDIA ##########\n");
        printf("1) Agregar datos.\n");
        printf("2) Operar con los datos.\n");
        printf("0) Salir.\n");
        printf("Seleccione acción a realizar: ");

        scanf("%d", &opcion);
        if (opcion == 1) menuAgregar(parque);
        if (opcion == 2) menuOperar();
        if (opcion == 0) printf("Saliendo del sistema...\n");
        if (opcion < 0 || opcion > 2) printf("Opción inválida.\n");
    } while(opcion != 0);

    return 0;
}