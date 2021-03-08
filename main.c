#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define FILAS 3
#define COLUMNAS 3
#define TAMANIO_MATRIZ FILAS *COLUMNAS
#define JUGADOR_X 'X'
#define JUGADOR_O 'O'
#define JUGADOR_CPU_X JUGADOR_X
#define JUGADOR_CPU_O JUGADOR_O
#define ESPACIO_VACIO ' '
#define CONTEO_PARA_GANAR 3
// Modos de juego
#define JUGADOR_JUGADOR 1
#define JUGADOR_CPU 2
#define CPU_CPU 3
#define SALIR 4
void clonarMatriz(char tableroOriginal[FILAS][COLUMNAS], char destino[FILAS][COLUMNAS])
{
    memcpy(destino, tableroOriginal, TAMANIO_MATRIZ);
}

void limpiarTablero(char tablero[FILAS][COLUMNAS])
{
    int y;
    for (y = 0; y < FILAS; y++)
    {
        int x;
        for (x = 0; x < COLUMNAS; x++)
        {
            tablero[y][x] = ESPACIO_VACIO;
        }
    }
}

void imprimirTablero(char tablero[FILAS][COLUMNAS])
{
    printf("\n");
    int y;
    int x;
    // Imprimir encabezado
    printf("| ");
    for (x = 0; x < COLUMNAS; x++)
    {
        printf("|%d", x + 1);
    }
    printf("|\n");
    for (y = 0; y < FILAS; y++)
    {
        printf("|%d", y + 1);
        for (x = 0; x < COLUMNAS; x++)
        {
            printf("|%c", tablero[y][x]);
        }
        printf("|\n");
    }
}
int coordenadasVacias(int y, int x, char tablero[FILAS][COLUMNAS])
{
    return tablero[y][x] == ESPACIO_VACIO;
}
void colocarPieza(int y, int x, char pieza, char tablero[FILAS][COLUMNAS])
{
    if (y < 0 || y >= FILAS)
    {
        printf("Fila incorrecta");
        return;
    }

    if (x < 0 || x >= COLUMNAS)
    {
        printf("Columna incorrecta");
        return;
    }
    if (pieza != JUGADOR_O && pieza != JUGADOR_X)
    {
        printf("La pieza debe ser %c o %c", JUGADOR_O, JUGADOR_X);
        return;
    }
    if (!coordenadasVacias(y, x, tablero))
    {
        printf("Coordenadas ya ocupadas");
        return;
    }
    tablero[y][x] = pieza;
}
int contarHaciaArriba(int x, int y, char jugador, char tablero[FILAS][COLUMNAS])
{
    int yInicio = (y - CONTEO_PARA_GANAR >= 0) ? y - CONTEO_PARA_GANAR + 1 : 0;
    int contador = 0;
    for (; yInicio <= y; yInicio++)
    {
        if (tablero[yInicio][x] == jugador)
        {
            contador++;
        }
        else
        {
            contador = 0;
        }
    }
    return contador;
}

int contarHaciaDerecha(int x, int y, char jugador, char tablero[FILAS][COLUMNAS])
{
    int xFin = (x + CONTEO_PARA_GANAR < COLUMNAS) ? x + CONTEO_PARA_GANAR - 1 : COLUMNAS - 1;
    int contador = 0;
    for (; x <= xFin; x++)
    {
        if (tablero[y][x] == jugador)
        {
            contador++;
        }
        else
        {
            contador = 0;
        }
    }
    return contador;
}

int contarHaciaArribaDerecha(int x, int y, char jugador, char tablero[FILAS][COLUMNAS])
{
    int xFin = (x + CONTEO_PARA_GANAR < COLUMNAS) ? x + CONTEO_PARA_GANAR - 1 : COLUMNAS - 1;
    int yInicio = (y - CONTEO_PARA_GANAR >= 0) ? y - CONTEO_PARA_GANAR + 1 : 0;
    int contador = 0;
    while (x <= xFin && yInicio <= y)
    {
        if (tablero[y][x] == jugador)
        {
            contador++;
        }
        else
        {
            contador = 0;
        }
        x++;
        y--;
    }
    return contador;
}

int contarHaciaAbajoDerecha(int x, int y, char jugador, char tablero[FILAS][COLUMNAS])
{
    int xFin = (x + CONTEO_PARA_GANAR < COLUMNAS) ? x + CONTEO_PARA_GANAR - 1 : COLUMNAS - 1;
    int yFin = (y + CONTEO_PARA_GANAR < FILAS) ? y + CONTEO_PARA_GANAR - 1 : FILAS - 1;
    int contador = 0;
    while (x <= xFin && y <= yFin)
    {
        if (tablero[y][x] == jugador)
        {
            contador++;
        }
        else
        {
            contador = 0;
        }
        x++;
        y++;
    }
    return contador;
}
int comprobarSiGana(char jugador, char tablero[FILAS][COLUMNAS])
{
    int y;
    for (y = 0; y < FILAS; y++)
    {
        int x;
        for (x = 0; x < COLUMNAS; x++)
        {
            if (
                contarHaciaArriba(x, y, jugador, tablero) >= CONTEO_PARA_GANAR ||
                contarHaciaDerecha(x, y, jugador, tablero) >= CONTEO_PARA_GANAR ||
                contarHaciaArribaDerecha(x, y, jugador, tablero) >= CONTEO_PARA_GANAR ||
                contarHaciaAbajoDerecha(x, y, jugador, tablero) >= CONTEO_PARA_GANAR)
            {
                return 1;
            }
        }
    }
    // Terminamos de recorrer y no conectó
    return 0;
}
char oponenteDe(char jugador)
{
    if (jugador == JUGADOR_O)
    {
        return JUGADOR_X;
    }
    else
    {
        return JUGADOR_O;
    }
}
void hablar(char *mensaje, char jugador)
{
    printf("\nCPU (%c) dice: %s\n\n", jugador, mensaje);
}
// Debería llamarse después de verificar si alguien gana
int empate(char tableroOriginal[FILAS][COLUMNAS])
{
    int y;
    for (y = 0; y < FILAS; y++)
    {
        int x;
        for (x = 0; x < COLUMNAS; x++)
        {
            // Si hay al menos un espacio vacío se dice que no hay empate
            if (tableroOriginal[y][x] == ESPACIO_VACIO)
            {
                return 0;
            }
        }
    }
    return 1;
}
int aleatorio_en_rango(int minimo, int maximo)
{
    return minimo + rand() / (RAND_MAX / (maximo - minimo + 1) + 1);
}
void obtenerCoordenadasAleatorias(char jugador, char tableroOriginal[FILAS][COLUMNAS], int *yDestino, int *xDestino)
{
    int x, y;
    do
    {
        x = aleatorio_en_rango(0, COLUMNAS - 1);
        y = aleatorio_en_rango(0, FILAS - 1);
    } while (!coordenadasVacias(y, x, tableroOriginal));
    *yDestino = y;
    *xDestino = x;
}

void coordenadasParaGanar(char jugador, char tableroOriginal[FILAS][COLUMNAS], int *yDestino, int *xDestino)
{
    char copiaTablero[FILAS][COLUMNAS];
    int y, x;
    for (y = 0; y < FILAS; y++)
    {
        for (x = 0; x < COLUMNAS; x++)
        {
            clonarMatriz(tableroOriginal, copiaTablero);
            if (coordenadasVacias(y, x, copiaTablero))
            {
                colocarPieza(y, x, jugador, copiaTablero);
                if (comprobarSiGana(jugador, copiaTablero))
                {
                    *yDestino = y;
                    *xDestino = x;
                    return;
                }
            }
        }
    }
    *yDestino = *xDestino = -1;
}
/*
Esta función cuenta y te dice el mayor puntaje, pero no te dice en cuál X ni cuál Y. Está pensada
para ser llamada desde otra función que lleva cuenta de X e Y
*/
int contarSinSaberCoordenadas(char jugador, char copiaTablero[FILAS][COLUMNAS])
{
    int conteoMayor = 0;
    int x, y;
    for (y = 0; y < FILAS; y++)
    {
        for (x = 0; x < COLUMNAS; x++)
        {
            // Colocamos y contamos el puntaje
            int conteoTemporal;
            conteoTemporal = contarHaciaArriba(x, y, jugador, copiaTablero);
            if (conteoTemporal > conteoMayor)
            {
                conteoMayor = conteoTemporal;
            }
            conteoTemporal = contarHaciaArribaDerecha(x, y, jugador, copiaTablero);
            if (conteoTemporal > conteoMayor)
            {
                conteoMayor = conteoTemporal;
            }

            conteoTemporal = contarHaciaDerecha(x, y, jugador, copiaTablero);
            if (conteoTemporal > conteoMayor)
            {
                conteoMayor = conteoTemporal;
            }

            conteoTemporal = contarHaciaAbajoDerecha(x, y, jugador, copiaTablero);
            if (conteoTemporal > conteoMayor)
            {
                conteoMayor = conteoTemporal;
            }
        }
    }
    return conteoMayor;
}
/*
Esta función complementa a contarSinSaberCoordenadas. Te dice en qué X e Y el jugador [jugador]
obtendrá el mayor puntaje si pone ahí su pieza
*/
void coordenadasParaMayorPuntaje(char jugador, char tableroOriginal[FILAS][COLUMNAS], int *yDestino, int *xDestino, int *conteo)
{

    char copiaTablero[FILAS][COLUMNAS];
    int y, x;
    int conteoMayor = 0,
        xConteoMayor = -1,
        yConteoMayor = -1;
    for (y = 0; y < FILAS; y++)
    {
        for (x = 0; x < COLUMNAS; x++)
        {
            clonarMatriz(tableroOriginal, copiaTablero);
            if (!coordenadasVacias(y, x, copiaTablero))
            {
                continue;
            }
            // Colocamos y contamos el puntaje
            colocarPieza(y, x, jugador, copiaTablero);
            int conteoTemporal = contarSinSaberCoordenadas(jugador, copiaTablero);
            if (conteoTemporal > conteoMayor)
            {
                conteoMayor = conteoTemporal;
                yConteoMayor = y;
                xConteoMayor = x;
            }
        }
    }
    *conteo = conteoMayor;
    *xDestino = xConteoMayor;
    *yDestino = yConteoMayor;
}

void elegirCoordenadasCpu(char jugador, char tablero[FILAS][COLUMNAS], int *yDestino, int *xDestino)
{
    hablar("Estoy pensando...", jugador);
    /*
    El orden en el que el CPU infiere las coordenadas que toma es:
    1. Ganar si se puede
    2. Hacer perder al oponente si está a punto de ganar
    3. Tomar el mejor movimiento del oponente (en donde obtiene el mayor puntaje)
    4. Tomar mi mejor movimiento (en donde obtengo mayor puntaje)
    5. Elegir la de la esquina superior izquierda (0,0)
    6. Columna aleatoria
    */
    int y, x, conteoJugador, conteoOponente;
    char oponente = oponenteDe(jugador);
    // 1
    coordenadasParaGanar(jugador, tablero, &y, &x);
    if (y != -1 && x != -1)
    {
        hablar("Ganar", jugador);
        *yDestino = y;
        *xDestino = x;
        return;
    }
    // 2
    coordenadasParaGanar(oponente, tablero, &y, &x);
    if (y != -1 && x != -1)
    {
        hablar("Tomar victoria de oponente", jugador);
        *yDestino = y;
        *xDestino = x;
        return;
    }
    // 3
    coordenadasParaMayorPuntaje(jugador, tablero, &y, &x, &conteoJugador);
    coordenadasParaMayorPuntaje(oponente, tablero, &y, &x, &conteoOponente);
    if (conteoOponente > conteoJugador)
    {
        hablar("Tomar puntaje mayor del oponente", jugador);
        *yDestino = y;
        *xDestino = x;
        return;
    }
    else
    {
        hablar("Tomar mi mayor puntaje", jugador);
        *yDestino = y;
        *xDestino = x;
        return;
    }
    // 4
    if (coordenadasVacias(0, 0, tablero))
    {
        hablar("Tomar columna superior izquierda", jugador);
        *yDestino = 0;
        *xDestino = 0;
        return;
    }
    // 5
    hablar("Coordenadas aleatorias", jugador);
    obtenerCoordenadasAleatorias(jugador, tablero, yDestino, xDestino);
}
char jugadorAleatorio()
{
    if (aleatorio_en_rango(0, 1) == 0)
    {
        return JUGADOR_O;
    }
    else
    {
        return JUGADOR_X;
    }
}
void iniciarJuego(int modo)
{
    if (modo != JUGADOR_JUGADOR && modo != JUGADOR_CPU && modo != CPU_CPU)
    {
        printf("Modo de juego no permitido");
        return;
    }

    srand(getpid());
    char tablero[FILAS][COLUMNAS];
    limpiarTablero(tablero);
    char jugadorActual = jugadorAleatorio();
    printf("El jugador que inicia es: %c\n", jugadorActual);
    int x, y;
    while (1)
    {
        imprimirTablero(tablero);
        if (modo == JUGADOR_JUGADOR || (modo == JUGADOR_CPU && jugadorActual == JUGADOR_X))
        {
            printf("Jugador %c. Ingresa coordenadas (x,y) para colocar la pieza separadas por una coma. Por ejemplo: 5,5\n", jugadorActual);
            scanf("%d,%d", &x, &y);
            // Restamos 1 en ambos casos porque empezamos a contar desde 0 internamente
            x--;
            y--;
        }
        else if (modo == CPU_CPU || (modo == JUGADOR_CPU && jugadorActual == JUGADOR_CPU_O))
        {
            elegirCoordenadasCpu(jugadorActual, tablero, &y, &x);
        }
        colocarPieza(y, x, jugadorActual, tablero);
        if (comprobarSiGana(jugadorActual, tablero))
        {
            imprimirTablero(tablero);
            printf("El jugador %c gana\n", jugadorActual);
            return;
        }
        else if (empate(tablero))
        {
            imprimirTablero(tablero);
            printf("Empate");
            return;
        }
        jugadorActual = oponenteDe(jugadorActual);
    }
}
int main(int argc, char const *argv[])
{
    printf("================================================\n");
    printf("|   Tres en línea en ANSI C - By Parzibyte     |\n");
    printf("|           https://parzibyte.me/blog          |\n");
    printf("| (también conocido como tic tac toe o gatitos)|\n");
    printf("================================================\n");
    sleep(1);
    int modo;
    printf("1. Humano contra humano\n2. Humano contra CPU (El CPU juega como %c)\n3. CPU contra CPU\n4. Salir\nElige: ", JUGADOR_CPU_O);
    scanf("%d", &modo);
    if (modo == SALIR)
    {
        return 0;
    }
    iniciarJuego(modo);
    return 0;
}