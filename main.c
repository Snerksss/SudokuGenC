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
#include <string.h>
#include "stack.h"

#define KGRN  "\x1B[32m"   // green

int gameArea[9][9];
int gameField[9][9];
int kontrollArea[9][9];
int rules;
int solution;
int stop;
int numberLR;
int sqrLR;
myStack_t *playerstack;

//zeichnet ein 9x9 Feld
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

void ShowSimilarNumbersInColour(int field[9][9], int compareField[9][9]) {
    printf("     A B C   D E F   G H I \n");
    printf("     -----   -----   ----- \n");
    for (int i = 0; i < 9; i++) {
        printf(" %d | ", i + 1);
        for (int j = 0; j < 9; j++) {
            if (j == 2 || j == 5 || j == 8) {
                int tmp = field[i][j];
                if (tmp != 0) {
                    if (compareField[i][j] == tmp) {
                        printf("%s%d | ", KGRN, tmp);

                    } else {
                        printf("%d | ", tmp);

                    }
                } else {
                    printf("  | ");
                }
            } else {
                int tmp = field[i][j];
                if (tmp != 0) {
                    if (compareField[i][j] == tmp) {
                        printf("%s%d ", KGRN, tmp);

                    } else {
                        printf("%d ", tmp);

                    }
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

void PrintfN(int number) {
    for (int i = 0; i < number; i++) {
        printf("\n");
    }
}

//checkt eine Spalte
int CheckLine(int line, int row, int zahl) {
    for (int i = 0; i < 9; i++) {
        if (gameArea[line][i] == zahl) {
            return 0;
        }
    }
    return 1;
}

//checkt die ganze Reihe
int CheckRow(int line, int row, int zahl) {
    for (int i = 0; i < 9; i++) {
        if (gameArea[i][row] == zahl) {
            return 0;
        }
    }
    return 1;
}

//checkt eine 3x3 Box
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

//checkt Regeln zum setzen einzelner Zahlen
int CheckRules(int line, int row, int zahl) {
    if (CheckRow(line, row, zahl) == 1 && CheckLine(line, row, zahl) == 1 && CheckBox(line, row, zahl) == 1) {
        return 1;
    }
    return 0;
}

//checkt ob alle Regeln erfuellt sind;
//wird nur ausgefuehrt, wenn alle Felder belegt sind.
int CheckRulesForPlayer() {
    for (int i = 1; i <= 9; i++) {
        int count = 0;
        for (int j = 0; j < 9; j++) {
            for (int k = 0; k < 9; k++) {
                if (gameArea[j][k] == i) {
                    count++;
                }
            }
            if (count != 1) {
                return 0;
            } else {
                count = 0;
            }
        }
        for (int j = 0; j < 9; j++) {
            for (int k = 0; k < 9; k++) {
                if (gameArea[k][j] == i) {
                    count++;
                }
            }
            if (count != 1) {
                return 0;
            } else {
                count = 0;
            }
        }
        for (int j = 0; j < 9; j += 3) {
            for (int k = 0; k < 9; k += 3) {
                for (int l = 0; l < 3; l++) {
                    for (int m = 0; m < 3; m++) {
                        if (gameArea[j + l][k + m] == i) {
                            count++;
                        }
                    }
                }
                if (count != 1) {
                    return 0;
                } else {
                    count = 0;
                }
            }
        }
    }
    return 1;
}

//checkt, ob der Spieler fertig ist, spiel stoppt dann, passiert nur, wenn alle Felder belegt sind und zudem die Regeln erfuellt sind
int CheckGamePlay() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (gameArea[i][j] == 0) {
                return 0;
            }
        }
    }
    if (CheckRulesForPlayer() == 0) {
        return 0;
    }
    return 1;
}

//checkt ob der Spieler versucht auf ein vorgegebenes Feld eine Zahl zu setzen
int CheckGivenNumber(int line, int row) {
    if (gameField[line][row] != 0) {
        return 0;
    }
    return 1;
}

//versucht die eingegebene Koordinate zu verarbeitn
int TryToSetPlayerCord(char eingabe[30]) {
    int y, x, zahl;
    if (eingabe[0] >= 'a' && eingabe[0] <= 'i') {
        y = eingabe[0] - 'a';
        if (eingabe[1] >= '1' && eingabe[1] <= '9') {
            x = eingabe[1] - '0';
            x -= 1;
            if (eingabe[2] == ',') {
                if (eingabe[3] >= '1' && eingabe[3] <= '9') {
                    zahl = eingabe[3] - '0';
                    if (CheckGivenNumber(x, y)) {
                        if (rules == 1) {
                            if (CheckRules(x, y, zahl)) {
                                gameArea[x][y] = zahl;
                                position set = {x, y};
                                Push(playerstack, &set);
                            } else {
                                printf("\nDie Regelpruefung ist fehlgeschlagen");
                                printf("\n");
                                return 0;
                            }
                        } else {
                            gameArea[x][y] = zahl;
                            position set = {x, y};
                            Push(playerstack, &set);
                        }
                        return 1;
                    } else {
                        printf("\nDas versuchte Feld ist ein vorgegebenes Feld");
                        printf("\n");
                    }
                }
            }
        }
    }
    return 0;
}


//laesst den Spieler einen Schritt zurueck gehen
void Undo() {
    if (IsStackEmpty(playerstack) == 0) {
        position temp;
        Pop(playerstack, &temp);
        gameArea[temp.line][temp.row] = 0;
    }
}


//aktiviert/deaktiviert die Regeln
void RulesSwitch() {
    if (rules == 1) {
        rules = 0;
    } else {
        rules = 1;
    }
}


//Zeigt die Loesung an oder zeigt sie nicht mehr an
void SolutionSwitch() {
    if (solution == 1) {
        solution = 0;
    } else {
        solution = 1;
    }
}


//Stoppt das Spiel
void Stop() {
    stop = 1;
}

//wird abgefragt, ob eins der Keywords aufgerufen wurde
int ChangeRequest(char eingabe[30]) {
    if (strcmp(eingabe, "UNDO") == 0) {
        Undo();
        return 1;
    } else {
        if (strcmp(eingabe, "RULES") == 0) {
            RulesSwitch();
            return 1;
        } else {
            if (strcmp(eingabe, "SOLUTION") == 0) {
                SolutionSwitch();
                return 1;
            } else {
                if (strcmp(eingabe, "STOP_GAME") == 0) {
                    Stop();
                    return 1;
                }
            }
        }
    }
    return 0;
}

//Player Funktion, steuert die Funktionen des Spielers
void PlayerInput() {
    playerstack = StackNew(sizeof(position), 81);
    ShowSimilarNumbersInColour(gameArea, gameField);
    solution = 0;
    rules = 0;
    stop = 0;
    while (CheckGamePlay() == 0 && stop == 0) {
        char eingabe[30];
        printf("\n");
        printf("gebe bitte die Position und die passende Zahl ein;   Bsp.: a1,1\n");
        printf("oder nutze einen der folgenden Befehlen\n");
        printf("UNDO, RULES, SOLUTION, STOP_GAME\n");
        printf("\n");
        int testDurchlauf = 1;
        while (testDurchlauf == 1) {
            if (1 == scanf("%30s", &eingabe)) {
                if (ChangeRequest(eingabe)) {
                    testDurchlauf = 0;
                } else {
                    if (TryToSetPlayerCord(eingabe)) {
                        testDurchlauf = 0;
                    }
                }
            }
        }
        if (stop != 1) {
            PrintfN(25);
            ShowGameField(gameArea);
        }
    }
    if (CheckRulesForPlayer()) {
        PrintfN(5);
        printf("\nHERZLICHEN GLUECKWUNSCH ZUM BEZWINGEN DES SUDOKUS");
        PrintfN(5);
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


    const int start_val = rand() % numberLR;
    for (int testZahl = start_val; testZahl < start_val + numberLR; testZahl++) {
        if (CheckRules(line, row, (testZahl % 9) + 1) == 1) {
            gameArea[line][row] = (testZahl % 9) + 1;
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


    int numberOfStand = rand() % (36 - 17 + 1) + 17;
    //ClearField(81 - numberOfStand);
    ClearField(1);

    CopyPlayerField();

}


int main(int argc, char **argv) {
    setbuf(stdout, 0);
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

