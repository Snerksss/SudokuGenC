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


void ShowGameField(int field[9][9]) {
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

int IndexOf(char alphabet[9], char buchstabe) {
    for (int i = 0; i < 9; i++) {
        if (alphabet[i] == buchstabe) {
            return i;
        }
    }
    return 10;
}

int CheckLine(int line, int row, int zahl) {
    for (int i = 0; i < 9; i++) {
        if (gameArea[line][i] == zahl) {
            return 0;
        }
    }
    return 1;
}

int CheckRow(int line, int row, int zahl) {
    for (int i = 0; i < 9; i++) {
        if (gameArea[i][row] == zahl) {
            return 0;
        }
    }
    return 1;
}

int CheckBox(int line, int row, int zahl) {
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

int CheckRules(int line, int row, int zahl) {
    if (CheckRow(line, row, zahl) == 1 && CheckLine(line, row, zahl) == 1 && CheckBox(line, row, zahl) == 1) {
        return 1;
    }
    return 0;
}

int CheckGamePlay() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (gameArea[i][j] != kontrollArea[i][j]) {
                return 0;
            }
        }
    }
    return 1;
}

int TryToSetPlayerCord(char eingabe[30]) {
    int y, x, zahl;
    if(eingabe[0] >= 'a' && eingabe[0] <= 'i') {
        y = eingabe[0] - 'a';
        if(eingabe[1] >= '1' && eingabe[1] <= '9') {
            x = eingabe[1] - '0';
            x -= 1;
            if(eingabe[2] == ','){
                if(eingabe[3] >= '1' && eingabe[3] <= '9'){
                    zahl = eingabe[3] - '0';
                    return 1;
                }
            }
        }
    }
    return 0;
}

int ChangeRequest(char eingabe[30]) {
    if(strcmp(eingabe, "UNDO") == 0) {
        return 1;
    } else {
        if(strcmp(eingabe, "RULES_ON") == 0) {
            return 1;
        } else {
            if(strcmp(eingabe, "RULES_OFF") == 0) {
                return 1;
            } else {
                if(strcmp(eingabe, "SHOW_SOLUTION_ON") == 0) {
                    return 1;
                } else {
                    if(strcmp(eingabe, "SHOW_SOLUTION_OFF") == 0) {
                        return 1;
                    } else{
                        if(strcmp(eingabe, "STOP_GAME") == 0) {
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}

void PlayerInput() {
    ShowGameField(gameArea);
    int bool = 1;
    char alphabet[9] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i'};
    while (CheckGamePlay() == 0) {
        int y = 0;
        int x = 0;
        char buchstabe;
        int zahl;
        char eingabe[30];
        char eingabe2[30];
        for(int i = 0; i < sizeof(eingabe); i++) {
            eingabe[i] = '/';
        }
        printf("\n");
        printf("gebe bitte die Position und die passende Zahl ein\n");
        printf("Bsp.: a1,1\n");
        printf("\n");
        int testDurchlauf = 1;
        while (testDurchlauf == 1) {
            if(1 == scanf("%30s", &eingabe, &eingabe2)) {
                if(1 == ChangeRequest(eingabe)) {
                    testDurchlauf = 0;
                } else {
                    if(1 == TryToSetPlayerCord(eingabe)) {
                        testDurchlauf = 0;
                    }
                }
            }
        }

        for (int i = 0; i < 25; i++) {
            printf("\n");
        }
        x = IndexOf(alphabet, buchstabe);
        if (zahl > 0 && zahl < 10 && x >= 0 && x != 10)
            if (gameField[y - 1][x] == 0)
                gameArea[y - 1][x] = zahl;

        ShowGameField(gameField);
        printf("\n");
        printf("\n");
        printf("\n");
        ShowGameField(gameArea);
    }
}

//Befuellt ein 3x3 Feld abhaengig von der gegebenen StartLine und StartRow
void SquareMatrix(int startLine, int startRow) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int num;
            do {
                num = (rand() % 9) + 1;
            } while (CheckBox(startLine, startRow, num) == 0);
            gameArea[startLine + i][startRow + j] = num;
        }
    }

}

//Befuellt Rekursiv die fehlenden Felder
//braucht keinen Zufall, da dieser bereits durch die gegebenen 3x3 Felder gegeben ist, denn hier haengen alles Zahlen von den 3x3 Feldern ab
int RekursivFill(int line, int row) {

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
        if (CheckRules(line, row, testZahl) == 1) {
            gameArea[line][row] = testZahl;
            if (RekursivFill(line, row + 1) == 1) {
                //Uebergabe des Abbruchs bis zum ersten Aufruf
                return 1;
            }
            //wenn Rechner zu diesem Schritt kommt, bedeutet das, dass die genommene Zahl nicht funktioniert, Position wird wieder zurueckgenommen
            gameArea[line][row] = 0;

            //Test um zu sehen wie oft es nicht geklappt hat nicht relevant fuer die Berechnung
            //ShowGameField();

        }
    }
    return 0;
}

void ClearField(int numberOfClear) {
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

void CopyField() {
    for (int i = 0; i < numberLR; i++) {
        for (int j = 0; j < numberLR; j++) {
            kontrollArea[i][j] = gameArea[i][j];
        }
    }
}

void CopyPlayerField() {
    for (int i = 0; i < numberLR; i++) {
        for (int j = 0; j < numberLR; j++) {
            gameField[i][j] = gameArea[i][j];
        }
    }
}

void GenNew() {
    srand(time(NULL));
    for (int i = 0; i < numberLR; i = i + sqrLR) {
        //es werden die 3x3 Felder oben links, mitte und unten rechts befuellt
        //Die 3x3 Felder sind die einzigen die in Line und Row von nichts abhaengen
        //somit ist der Zufall des Sodokus hierdurch gegeben
        SquareMatrix(i, i);
    }

    RekursivFill(0, 0);

    CopyField();

    ClearField(50);

    CopyPlayerField();

}

void PrintfN(int number) {
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
    GenNew();
    ShowGameField(kontrollArea);


    PrintfN(10);
    printf("\n              ^");
    printf("\n              |");
    printf("\n           Loesung");
    PrintfN(2);
    PlayerInput();
    return (EXIT_SUCCESS);
}

