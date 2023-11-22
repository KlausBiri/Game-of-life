#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
/*
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>*/
#include "Fugvenyek.h"


#ifdef _WIN32
    #include <windows.h>
#endif

#include "debugmalloc.h"

#define DELAY 500


/*
typedef struct Eletter{
    int sizeX;
    int sizeY;
    char** sejtek;
} Eletter;

*/


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

    while(1){
        clearScreen();
        if(Eletter_nemures){
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

        switch(Menupont){
            case 1:
                printf("Adja meg a kiv�nt m�retet (X Y): ");
                scanf("%d %d", &Tablazat.sizeX, &Tablazat.sizeY);
                while(getchar() != '\n');
                if(Tablazat.sejtek != NULL){
                    Freeeles(&Tablazat);
                    Eletter_nemures = false;
                }
                Tablazat.sejtek = (char**)malloc(Tablazat.sizeX * sizeof(char*));

                for(int i = 0; i < Tablazat.sizeX; i++){
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
            if(Tablazat.sejtek != NULL){
                Freeeles(&Tablazat);
                Eletter_nemures = false;
            }

            if(Loadfromfile(&Tablazat, filename)){
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
            if(Eletter_nemures){
                while(1){
                    clearScreen();
                    Kiiratas(&Tablazat);
                    Szimulator(&Tablazat);
                    Sleep(DELAY);
                    if(_kbhit()){
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
            if(Tablazat.sejtek != NULL){
                Freeeles(&Tablazat);
            }
            return 0;
        default:
            printf("Nincs ilyen men�pont.\n");
        }
    }

    return 0;
}
