#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "Fugvenyek.h"

#define DELAY 500 // Delay in milliseconds (2 seconds)

void clearScreen(){
    system("cls");
}
// A fögvény felszabadítja a sejteket
void Freeeles(Eletter *Tablazat){
    for(int i = 0; i < Tablazat->sizeX; i++) {
        free(Tablazat->sejtek[i]);
    }
    free(Tablazat->sejtek);
}

// Megjeleníti az életteret a consolon
void Kiiratas(Eletter *Tablazat){
    for(int i = 0; i < Tablazat->sizeX; i++) {
        for(int j = 0; j < Tablazat->sizeY; j++) {
            putchar(Tablazat->sejtek[i][j]);
        }
        putchar('\n');
    }
}

// Maga az életjátékát szimulálja
void Szimulator(Eletter *Tablazat){
    Eletter Ujter;
    Ujter.sizeX = Tablazat->sizeX;
    Ujter.sizeY = Tablazat->sizeY;
    Ujter.sejtek = (char**)malloc(Ujter.sizeX * sizeof(char*));

    for(int i = 0; i < Ujter.sizeX; i++){
        Ujter.sejtek[i] = (char*)malloc(Ujter.sizeY * sizeof(char));
    }

    for(int x = 0; x < Tablazat->sizeX; x++){
        for(int y = 0; y < Tablazat->sizeY; y++) {
            int liveNeighbors = 0;
            int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
            int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

            for(int i = 0; i < 8; i++){
                int newX = x + dx[i];
                int newY = y + dy[i];
                if(newX >= 0 && newX < Tablazat->sizeX && newY >= 0 && newY < Tablazat->sizeY) {
                    if(Tablazat->sejtek[newX][newY] == 'x'){
                        liveNeighbors++;
                    }
                }
            }

            if(Tablazat->sejtek[x][y] == 'x'){
                if(liveNeighbors < 2 || liveNeighbors > 3){
                    Ujter.sejtek[x][y] = '-';
                }else{
                    Ujter.sejtek[x][y] = 'x';
                }
            }else{
                if(liveNeighbors == 3){
                    Ujter.sejtek[x][y] = 'x';
                }else{
                    Ujter.sejtek[x][y] = '-';
                }
            }
        }
    }
    Freeeles(Tablazat);
    /*for(int i = 0; i < Tablazat->sizeX; i++){
        free(Tablazat->sejtek[i]);
    }
    free(Tablazat->sejtek);*/
    Tablazat->sizeX = Ujter.sizeX;
    Tablazat->sizeY = Ujter.sizeY;
    Tablazat->sejtek = Ujter.sejtek;
}

// Élettér beolvasása beolvasása fájlból
bool Loadfromfile(Eletter *Tablazat, char *filename){
    FILE* file = fopen(filename, "r");
    if(file == NULL) {
        return false;
    }

    int ujxmeret = 1;
    int ujymeret = 1;
    char c;

    while((c = fgetc(file)) != EOF){
        if(c == '\n') {
            ujxmeret++;
            printf("\n");
            ujymeret=0;
        }else{
            ujymeret++;
            printf("%c",c);
        }
    }
    Tablazat->sizeX = ujxmeret;
    Tablazat->sizeY = ujymeret;
    Tablazat->sejtek = (char**)malloc(Tablazat->sizeX * sizeof(char*));

    for(int i = 0; i < Tablazat->sizeX; i++){
        Tablazat->sejtek[i] = (char*)malloc(Tablazat->sizeY * sizeof(char));
    }
    rewind(file);

    int x = 0, y = 0;
    while((c = fgetc(file)) != EOF){
        if(c == '\n'){
            x++;
            y = 0;
        }else{
            Tablazat->sejtek[x][y] = c;
            y++;

        }
    }

    fclose(file);
    return true;
}



// Élettér random sejt érték feltöltése
void Randomfeltolt(Eletter *Tablazat){
    for(int x = 0; x < Tablazat->sizeX; x++) {
        for(int y = 0; y < Tablazat->sizeY; y++) {
            Tablazat->sejtek[x][y] = (rand() % 2 == 0) ? '-' : 'x';
        }
    }
}
