#ifndef FUGVENYEK_H_INCLUDED
#define FUGVENYEK_H_INCLUDED

typedef struct {
    char** sejtek;
    int sizeX;
    int sizeY;
} Eletter;


void clearScreen();
void Freeeles(Eletter *Tablazat);
void Kiiratas(Eletter *Tablazat);
void Szimulator(Eletter *Tablazat);
bool loadFromFile(Eletter *Tablazat, char *filename);
void Randomfeltolt(Eletter *Tablazat);

#endif // FUGVENYEK_H_INCLUDED
