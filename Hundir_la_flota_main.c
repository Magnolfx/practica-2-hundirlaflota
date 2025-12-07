#include <stdio.h>

#define TAM 12

typedef char Tablero[TAM][TAM];

void mostrarMenu(void);
int leerOpcion(void);
void jugar(void);
void verScoreboard(void);

void inicializarTablero(Tablero t);
void mostrarTablero(const Tablero t);
void colocarBarcos(Tablero t);
int colocarBarco(Tablero t, int longitud, char simbolo, const char *nombre);

void limpiarBuffer(void);

int main(void) {
    int opcion;

    do {
        mostrarMenu();
        opcion = leerOpcion();

        switch (opcion) {
            case 1:
                jugar();
                break;
            case 2:
                verScoreboard();
                break;
            case 3:
                printf("Saliendo del juego...\n");
                break;
            default:
                break;
        }
    } while (opcion != 3);

    return 0;
}

/* Limpia el buffer de entrada hasta el salto de línea */
void limpiarBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* nada */
    }
}

/* Muestra el menu principal */
void mostrarMenu(void) {
    printf("==============================\n");
    printf("       HUNDIR LA FLOTA\n");
    printf("==============================\n");
    printf("1. Jugar\n");
    printf("2. Puntuaciones\n");
    printf("3. Salir\n");
    printf("------------------------------\n");
    printf("Elige una opcion (1-3): ");
}

/* Lee y valida la opcion del menu */
int leerOpcion(void) {
    int op;
    int leidos;

    do {
        leidos = scanf("%d", &op);
        if (leidos != 1) {
            printf("Entrada no valida. Intenta de nuevo: ");
            limpiarBuffer();
        } else if (op < 1 || op > 3) {
            printf("Opcion fuera de rango. Intenta de nuevo: ");
            limpiarBuffer();
            leidos = 0; /* forzar repetir */
        } else {
            limpiarBuffer(); /* limpiar el salto de linea */
        }
    } while (leidos != 1);

    return op;
}

/* Opcion Jugar */
void jugar(void) {
    Tablero tablero;

    printf("\n=== NUEVA PARTIDA ===\n");

    inicializarTablero(tablero);

    printf("Tablero vacio (agua):\n");
    mostrarTablero(tablero);

    colocarBarcos(tablero);

    printf("\nTablero final con los barcos colocados:\n");
    mostrarTablero(tablero);

    printf("\nPulsa ENTER para volver al menu...\n");
    limpiarBuffer();
    getchar();  /* espera un ENTER */
}

/* Opcion Puntuaciones */
void verScoreboard(void) {
    printf("\n=== PUNTUACIONES ===\n");
    printf("Esta parte no esta implementada todavia.\n");
    printf("Pulsa ENTER para volver al menu...\n");
    limpiarBuffer();
    getchar();
}

/* Rellena el tablero con agua (~) */
void inicializarTablero(Tablero t) {
    for (int fila = 0; fila < TAM; fila++) {
        for (int col = 0; col < TAM; col++) {
            t[fila][col] = '~';
        }
    }
}

/* Muestra el tablero con indices de fila y columna */
void mostrarTablero(const Tablero t) {
    printf("   ");
    for (int col = 0; col < TAM; col++) {
        printf("%2d ", col);
    }
    printf("\n");

    printf("   ");
    for (int col = 0; col < TAM; col++) {
        printf("---");
    }
    printf("\n");

    for (int fila = 0; fila < TAM; fila++) {
        printf("%2d|", fila);
        for (int col = 0; col < TAM; col++) {
            printf(" %c ", t[fila][col]);
        }
        printf("\n");
    }
}

/* Pide al jugador que coloque todos sus barcos.
   Ahora primero pregunta QUÉ BARCO quieres colocar,
   luego FILA, luego COLUMNA (y luego orientación). */
void colocarBarcos(Tablero t) {
    int portavionesColocados = 0;
    int crucerosColocados   = 0;
    int patrullerosColocados = 0;
    int opcionBarco;
    int longitud;
    char simbolo;
    char nombre[32];

    printf("\nVas a colocar tus barcos en el tablero.\n");
    printf("Coordenadas desde 0 hasta %d (filas y columnas).\n", TAM - 1);
    printf("Simbolos: A=Portaviones, C=Crucero, P=Patrullero\n\n");

    /* Mientras queden barcos por colocar */
    while (portavionesColocados < 1 ||
           crucerosColocados   < 2 ||
           patrullerosColocados < 3) {

        printf("\nBarcos restantes por colocar:\n");
        printf("  Portaviones (4 casillas): %d de 1\n", 1 - portavionesColocados);
        printf("  Cruceros   (3 casillas): %d de 2\n", 2 - crucerosColocados);
        printf("  Patrulleros(2 casillas): %d de 3\n", 3 - patrullerosColocados);

        printf("\nQue barco quieres colocar ahora?\n");
        printf("  1. Portaviones (4 casillas)\n");
        printf("  2. Crucero    (3 casillas)\n");
        printf("  3. Patrullero (2 casillas)\n");
        printf("Elige (1-3): ");

        int leidos = scanf("%d", &opcionBarco);
        if (leidos != 1) {
            printf("Entrada no valida.\n");
            limpiarBuffer();
            continue;
        }
        limpiarBuffer();

        /* Elegir en función del tipo y si quedan de ese tipo */
        if (opcionBarco == 1) {
            if (portavionesColocados >= 1) {
                printf("Ya has colocado el portaviones.\n");
                continue;
            }
            longitud = 4;
            simbolo = 'A';
            snprintf(nombre, sizeof(nombre), "PORTAVIONES");
        } else if (opcionBarco == 2) {
            if (crucerosColocados >= 2) {
                printf("Ya has colocado todos los cruceros.\n");
                continue;
            }
            longitud = 3;
            simbolo = 'C';
            snprintf(nombre, sizeof(nombre), "CRUCERO");
        } else if (opcionBarco == 3) {
            if (patrullerosColocados >= 3) {
                printf("Ya has colocado todos los patrulleros.\n");
                continue;
            }
            longitud = 2;
            simbolo = 'P';
            snprintf(nombre, sizeof(nombre), "PATRULLERO");
        } else {
            printf("Opcion de barco no valida.\n");
            continue;
        }

        /* Intentar colocar ese barco concreto.
           Dentro se pregunta primero FILA, luego COLUMNA y luego ORIENTACION. */
        if (colocarBarco(t, longitud, simbolo, nombre)) {
            if (simbolo == 'A') {
                portavionesColocados++;
            } else if (simbolo == 'C') {
                crucerosColocados++;
            } else if (simbolo == 'P') {
                patrullerosColocados++;
            }
            printf("\nBarco colocado correctamente. Tablero actual:\n");
            mostrarTablero(t);
        } else {
            printf("\nNo se pudo colocar el barco. Intenta de nuevo.\n");
        }
    }

    printf("\nTodos los barcos han sido colocados.\n");
}

/* Coloca un solo barco.
   Aquí se pregunta:
     1) fila
     2) columna
     3) orientación
   Devuelve 1 si se coloco bien, 0 si hubo error. */
int colocarBarco(Tablero t, int longitud, char simbolo, const char *nombre) {
    int fila, col;
    char orientacion;
    int leidos;

    printf("\nColocando %s (longitud %d).\n", nombre, longitud);

    /* Leer fila */
    printf("Introduce fila inicial: ");
    leidos = scanf("%d", &fila);
    if (leidos != 1) {
        printf("Entrada no valida para la fila.\n");
        limpiarBuffer();
        return 0;
    }

    /* Leer columna */
    printf("Introduce columna inicial: ");
    leidos = scanf("%d", &col);
    if (leidos != 1) {
        printf("Entrada no valida para la columna.\n");
        limpiarBuffer();
        return 0;
    }

    /* Leer orientacion */
    printf("Orientacion (H=horizontal, V=vertical): ");
    leidos = scanf(" %c", &orientacion);
    limpiarBuffer();  /* limpiamos el salto de linea */
    if (leidos != 1) {
        printf("Entrada no valida para la orientacion.\n");
        return 0;
    }

    /* Comprobaciones basicas de rango */
    if (fila < 0 || fila >= TAM || col < 0 || col >= TAM) {
        printf("Posicion inicial fuera del tablero.\n");
        return 0;
    }

    if (orientacion != 'H' && orientacion != 'h' &&
        orientacion != 'V' && orientacion != 'v') {
        printf("Orientacion no valida.\n");
        return 0;
    }

    /* Comprobar que cabe y no se solapa */
    if (orientacion == 'H' || orientacion == 'h') {
        if (col + longitud > TAM) {
            printf("El barco no cabe horizontalmente en esa posicion.\n");
            return 0;
        }
        for (int j = 0; j < longitud; j++) {
            if (t[fila][col + j] != '~') {
                printf("Las casillas se solapan con otro barco.\n");
                return 0;
            }
        }
        for (int j = 0; j < longitud; j++) {
            t[fila][col + j] = simbolo;
        }
    } else { /* vertical */
        if (fila + longitud > TAM) {
            printf("El barco no cabe verticalmente en esa posicion.\n");
            return 0;
        }
        for (int i = 0; i < longitud; i++) {
            if (t[fila + i][col] != '~') {
                printf("Las casillas se solapan con otro barco.\n");
                return 0;
            }
        }
        for (int i = 0; i < longitud; i++) {
            t[fila + i][col] = simbolo;
        }
    }

    printf("%s colocado correctamente.\n", nombre);
    return 1;
}
