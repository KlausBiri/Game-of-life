#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#ifdef _WIN32
    #include <windows.h>
#include <conio.h>

#endif

#include "debugmalloc.h"

#define DELAY 1000


typedef struct Eletter{
    char** sejtek;
    int sizeX;
    int sizeY;
} Eletter;

void clearScreen() {
    system("cls");
}

// Megjelen�ti az �letteret a consolon
void Kiiratas(Eletter *Tablazat) {
    for(int i = 0; i < Tablazat->sizeX; i++) {
        for(int j = 0; j < Tablazat->sizeY; j++) {
            putchar(Tablazat->sejtek[i][j]);
        }
        putchar('\n');
    }
}

// Maga az �letj�t�k�t szimul�lja
void Szimulator(Eletter *Tablazat) {
    Eletter Ujter;
    Ujter.sizeX = Tablazat->sizeX;
    Ujter.sizeY = Tablazat->sizeY;
    Ujter.sejtek = (char**)malloc(Ujter.sizeX * sizeof(char*));

    for(int i = 0; i < Ujter.sizeX; i++) {
        Ujter.sejtek[i] = (char*)malloc(Ujter.sizeY * sizeof(char));
    }

    for(int x = 0; x < Tablazat->sizeX; x++) {
        for(int y = 0; y < Tablazat->sizeY; y++) {
            int liveNeighbors = 0;
            int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
            int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

            for(int i = 0; i < 8; i++) {
                int newX = x + dx[i];
                int newY = y + dy[i];
                if(newX >= 0 && newX < Tablazat->sizeX && newY >= 0 && newY < Tablazat->sizeY) {
                    if(Tablazat->sejtek[newX][newY] == 'x') {
                        liveNeighbors++;
                    }
                }
            }

            if(Tablazat->sejtek[x][y] == 'x') {
                if(liveNeighbors < 2 || liveNeighbors > 3) {
                    Ujter.sejtek[x][y] = '-';
                }else{
                    Ujter.sejtek[x][y] = 'x';
                }
            }else{
                if(liveNeighbors == 3) {
                    Ujter.sejtek[x][y] = 'x';
                }else{
                    Ujter.sejtek[x][y] = '-';
                }
            }
        }
    }
    for(int i = 0; i < Tablazat->sizeX; i++) {
        free(Tablazat->sejtek[i]);
    }
    free(Tablazat->sejtek);
    Tablazat->sizeX = Ujter.sizeX;
    Tablazat->sizeY = Ujter.sizeY;
    Tablazat->sejtek = Ujter.sejtek;
}

// �lett�r beolvas�sa beolvas�sa f�jlb�l
bool loadFromFile(Eletter *Tablazat, char *filename) {
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

// A f�gv�ny felszabad�tja a sejteket
void Freeeles(Eletter *Tablazat) {
    for(int i = 0; i < Tablazat->sizeX; i++) {
        free(Tablazat->sejtek[i]);
    }
    free(Tablazat->sejtek);
}

// �lett�r random sejt �rt�k felt�lt�se
void Randomfeltolt(Eletter *Tablazat) {
    for(int x = 0; x < Tablazat->sizeX; x++) {
        for(int y = 0; y < Tablazat->sizeY; y++) {
            Tablazat->sejtek[x][y] = (rand() % 2 == 0) ? '-' : 'x';
        }
    }
}


int main() {


    #ifdef _WIN32
    SetConsoleCP(1250);
    SetConsoleOutputCP(1250);
    #endif
    srand(time(NULL));

    Eletter Tablazat;
    Tablazat.sizeX = 0;
    Tablazat.sizeY = 0;
    Tablazat.sejtek = NULL;

    int Menupont;
    bool Eletter_nemures = false;
    char filename[100];

    loadFromFile(&Tablazat, "alap.txt");

    while(1){
        clearScreen();
        if(Eletter_nemures) {
            printf("\nAz aktu�lis t�bl�zat:\n");
            Kiiratas(&Tablazat);
        }else{
            printf("\n");
        }
        printf("Conway f�le �let j�t�ka\n");
        printf("1. �lett�r m�ret�nek be�ll�t�sa (Jelenleg: %d %d)\n", Tablazat.sizeX, Tablazat.sizeY);
        printf("2. �lett�r bet�lt�se f�jlb�l\n");
        printf("3. �rt�k randomiz�l�s\n");
        printf("4. Indit�s\n");
        printf("5. Quit\n");
        printf("Melyik men�pont: ");
        scanf("%d", &Menupont);
        while(getchar() != '\n');

        switch(Menupont) {
            case 1:
                printf("Adja meg a kiv�nt m�retet (X Y): ");
                scanf("%d %d", &Tablazat.sizeX, &Tablazat.sizeY);
                while(getchar() != '\n');
                if(Tablazat.sejtek != NULL) {
                    Freeeles(&Tablazat);
                    Eletter_nemures = false;
                }
                Tablazat.sejtek = (char**)malloc(Tablazat.sizeX * sizeof(char*));

                for(int i = 0; i < Tablazat.sizeX; i++) {
                    Tablazat.sejtek[i] = (char*)malloc(Tablazat.sizeY * sizeof(char));
                }
                for(int x = 0; x < Tablazat.sizeX; x++) {
                                   for (int y = 0; y < Tablazat.sizeY; y++) {
                    Tablazat.sejtek[x][y] = '-';
                }
            }
            Eletter_nemures = true;
            break;

        case 2:
            printf("Adja meg a f�jl nev�t: ");
            scanf("%s", filename);
            while(getchar() != '\n');
            if(Tablazat.sejtek != NULL) {
                Freeeles(&Tablazat);
                Eletter_nemures = false;
            }

            if(loadFromFile(&Tablazat, filename)) {
                Eletter_nemures = true;
            }else{
                printf("Sikertelen f�jl bet�lt�s.\n");
            }
            break;

        case 3:
            if(Tablazat.sizeX == 0 || Tablazat.sizeY == 0){
                printf("�llits be m�retet el�bb!");
            }else{
            Randomfeltolt(&Tablazat);
            }
            Eletter_nemures = true;
            break;

        case 4:
            if(Eletter_nemures) {
                while(1) {
                    clearScreen();
                    Kiiratas(&Tablazat);
                    Szimulator(&Tablazat);
                    Sleep(DELAY);
                    if(_kbhit()) {
                        char c = _getch();
                        if(c == 'f' || c == 'F') {
                            break;
                        }
                    }
                }
            }else{
                printf("Nincs �lett�r.\n");
            }
            break;

        case 5:
            if(Tablazat.sejtek != NULL) {
                Freeeles(&Tablazat);
            }
            return 0;
        default:
            printf("Nincs ilyen men�pont.\n");
        }
    }

    return 0;
}
