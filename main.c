/*
* To change this license header, choose License Headers in Project Properties.
* To change this template file, choose Tools | Templates
        * and open the template in the editor.
*/

/* 
 * File:   main.c
 * Author: simon
 *
 * Created on 4. Januar 2022, 08:31
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


int gameArea[9][9];
int gameField[9][9];
int kontrollArea[9][9];
int numberLR;
int sqrLR;


void showGameField(int field[9][9]) {
    printf("     A B C   D E F   G H I \n");
    printf("     -----   -----   ----- \n");
    for (int i = 0; i < 9; i++) {
        printf(" %d | ", i + 1);
        for (int j = 0; j < 9; j++) {
            if (j == 2 || j == 5 || j == 8) {
                int tmp = field[i][j];
                if (tmp != 0) {
                    printf("%d | ", tmp);

                } else {
                    printf("  | ");
                }
            } else {
                int tmp = field[i][j];
                if (tmp != 0) {
                    printf("%d ", tmp);
                } else {
                    printf("  ");
                }
            }
        }
        if (i == 2 || i == 5 || i == 8) {
            printf("\n");
            printf("     -----   -----   ----- \n");
        } else {
            printf("\n");
        }
    }
}

int indexOf(char alphabet[9], char buchstabe) {
    for (int i = 0; i < 9; i++) {
        if (alphabet[i] == buchstabe) {
            return i;
        }
    }
    return 10;
}

int checkLine(int line, int row, int zahl) {
    for (int i = 0; i < 9; i++) {
        if (gameArea[line][i] == zahl) {
            return 0;
        }
    }
    return 1;
}

int checkRow(int line, int row, int zahl) {
    for (int i = 0; i < 9; i++) {
        if (gameArea[i][row] == zahl) {
            return 0;
        }
    }
    return 1;
}

int checkBox(int line, int row, int zahl) {
    int tmpLine = line / 3;
    int tmpRow = row / 3;
    tmpRow *= 3;
    tmpLine *= 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (gameArea[tmpLine + i][tmpRow + j] == zahl) {
                return 0;
            }
        }
    }
    return 1;
}

int checkRules(int line, int row, int zahl) {
    if (checkRow(line, row, zahl) == 1 && checkLine(line, row, zahl) == 1 && checkBox(line, row, zahl) == 1) {
        return 1;
    }
    return 0;
}

int checkGamePlay() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (gameArea[i][j] != kontrollArea[i][j]) {
                return 0;
            }
        }
    }
    return 1;
}

void play() {
    showGameField(gameArea);
    int bool = 1;
    char alphabet[9] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i'};
    while (checkGamePlay() == 0) {
        int y = 0;
        int x = 0;
        char buchstabe;
        int zahl;
        do {
            printf("\n");
            printf("gebe bitte die Position und die passende Zahl ein\n");
            printf("Bsp.: a1, 1\n");
            printf("\n");
        } while (0 == scanf("%1s%1d, %1d", &buchstabe, &y, &zahl));
        for (int i = 0; i < 25; i++) {
            printf("\n");
        }
        x = indexOf(alphabet, buchstabe);
        if (zahl > 0 && zahl < 10 && x >= 0 && x != 10)
            if (gameField[y - 1][x] == 0)
                gameArea[y - 1][x] = zahl;

        showGameField(gameField);
        printf("\n");
        printf("\n");
        printf("\n");
        showGameField(gameArea);
    }
}

//Befuellt ein 3x3 Feld abhaengig von der gegebenen StartLine und StartRow
void squareMatrix(int startLine, int startRow) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int num;
            do {
                num = (rand() % 9) + 1;
            } while (checkBox(startLine, startRow, num) == 0);
            gameArea[startLine + i][startRow + j] = num;
        }
    }

}

//Befuellt Rekursiv die fehlenden Felder
//braucht keinen Zufall, da dieser bereits durch die gegebenen 3x3 Felder gegeben ist, denn hier haengen alles Zahlen von den 3x3 Feldern ab
int rekursivFill(int line, int row) {

    // Abbruchkriterium, alles ist dann befuellt
    if (row >= 9 && line >= 9)
        return 1;


    if (row >= 9) {
        //eine Zeile ist durchgegangen
        if (line < 9 - 1) {
            //eine Spalte weiter, Reihe wieder an Anfang
            line++;
            row = 0;
        }
    }

    if (line >= numberLR - sqrLR && row >= numberLR - sqrLR) {
        line++;
        row = 0;
        if (line >= 9) {
            //Abbruchkriterium, fertig.
            return 1;
        }

    }

    if (sqrLR > line < numberLR - sqrLR && row == (int) (line / sqrLR) * sqrLR)
        row = row + sqrLR;

    if (line < sqrLR && row < sqrLR)
        row = sqrLR;


    for (int testZahl = 1; testZahl <= numberLR; testZahl++) {
        if (checkRules(line, row, testZahl) == 1) {
            gameArea[line][row] = testZahl;
            if (rekursivFill(line, row + 1) == 1) {
                //Uebergabe des Abbruchs bis zum ersten Aufruf
                return 1;
            }
            //wenn Rechner zu diesem Schritt kommt, bedeutet das, dass die genommene Zahl nicht funktioniert, Position wird wieder zurueckgenommen
            gameArea[line][row] = 0;

            //Test um zu sehen wie oft es nicht geklappt hat nicht relevant fuer die Berechnung
            //showGameField();

        }
    }
    return 0;
}

void clearField(int numberOfClear) {
    for (int i = 0; i < numberOfClear; i++) {
        int line = rand() % 10;
        int row = rand() % 10;
        if (gameArea[line][row] != 0) {
            gameArea[line][row] = 0;
        } else {
            i--;
        }
    }
}

void copyField() {
    for (int i = 0; i < numberLR; i++) {
        for (int j = 0; j < numberLR; j++) {
            kontrollArea[i][j] = gameArea[i][j];
        }
    }
}

void copyPlayerField() {
    for (int i = 0; i < numberLR; i++) {
        for (int j = 0; j < numberLR; j++) {
            gameField[i][j] = gameArea[i][j];
        }
    }
}

void genNew() {
    srand(time(NULL));
    for (int i = 0; i < numberLR; i = i + sqrLR) {
        //es werden die 3x3 Felder oben links, mitte und unten rechts befuellt
        //Die 3x3 Felder sind die einzigen die in Line und Row von nichts abhaengen
        //somit ist der Zufall des Sodokus hierdurch gegeben
        squareMatrix(i, i);
    }

    rekursivFill(0, 0);

    copyField();

    clearField(50);

    copyPlayerField();

}

void printfN(int number) {
    for (int i = 0; i < number; i++) {
        printf("\n");
    }
}

int main(int argc, char **argv) {
    for (int i = 0; i < 9; i++) {
        //Definition eines Arrays mit in diesem Fall auf jedem Feld mit der Zahl 0
        //0 steht fuer ein nicht belegtes Feld
        for (int j = 0; j < 9; j++) {
            gameArea[i][j] = 0;
        }
    }
    /*
    char name[30];
    do {
        printf("Geben sie einen korrekten Namen ein\n");
    } while (0 == scanf("%s", &name));
    for (int i = 0; i < 25; i++) {
        printf("\n");
    }
    printf("%s\n", name);
    printf("\n");
    */
    numberLR = 9;
    sqrLR = (int) sqrt(numberLR);
    genNew();
    showGameField(kontrollArea);


    printfN(10);
    printf("\n              ^");
    printf("\n              |");
    printf("\n           Loesung");
    printfN(2);
    play();
    return (EXIT_SUCCESS);
}

