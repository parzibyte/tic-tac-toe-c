#include <stdio.h>
#include <string.h>
#define FILAS 3
#define COLUMNAS 3
#define TAMANIO_MATRIZ FILAS *COLUMNAS
#define JUGADOR_X 'X'
#define JUGADOR_O 'O'
#define ESPACIO_VACIO ' '
#define CONTEO_PARA_GANAR 3
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
void elegirCoordenadasCpu(char jugador, char tablero[FILAS][COLUMNAS], int *y, int *x)
{
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

int main(int argc, char const *argv[])
{
    char tablero[FILAS][COLUMNAS];
    limpiarTablero(tablero);
    imprimirTablero(tablero);

    colocarPieza(2, 0, JUGADOR_X, tablero);
    colocarPieza(1, 1, JUGADOR_X, tablero);
    imprimirTablero(tablero);
    printf("Gana %c? %d\n", JUGADOR_O, comprobarSiGana(JUGADOR_O, tablero));
    printf("Gana %c? %d\n", JUGADOR_X, comprobarSiGana(JUGADOR_X, tablero));
    int x, y, conteo;
    coordenadasParaMayorPuntaje(JUGADOR_X, tablero, &y, &x, &conteo);
    printf("Coordenadas con mayor puntaje para %c son x en %d, y en %d con un conteo de %d", JUGADOR_X, x, y, conteo);
    return 0;
}