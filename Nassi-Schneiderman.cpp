#include <iostream>
#include "graphics.h"
#include "winbgim.h"
#include <Windows.h>
#include <cstring>
#pragma comment(lib,"graphics.lib")

using namespace std;

char Path[300], img[100];
int maxH = getmaxheight() / 1.15, maxW = getmaxwidth() / 1.15;
int diagRowHeight = 0.055 * maxH, diagWidth = 0.8 * maxW, rowsInTotal=0;

char randuri[200][200];

void copiereRanduri() {
    int cnt = 0;
    FILE* fptr = fopen(Path, "r");
    while (fgets(randuri[cnt], 199, fptr)) {
        cnt++;
    }
    rowsInTotal = cnt;
}

int howManySpaces(char rand[]) {
    int spaces=0;
    while (rand[spaces] == 32)
        spaces++;
    return spaces;
}

int tipOperatie(char rand[]) {
    int spaces = howManySpaces(rand);
    char cuvInceput[10]{ 0 };
    strncpy(cuvInceput, rand + spaces, 3);
    cuvInceput[3] = 0;
    if (strcmp(cuvInceput, "if(") == 0) {
        return 1;
    }
    strncpy(cuvInceput, rand + spaces, 4);
    cuvInceput[4] = 0;
    if (strcmp(cuvInceput, "for(") == 0) {
        return 2;
    }
    strncpy(cuvInceput, rand + spaces, 6);
    cuvInceput[6] = 0;
    if (strcmp(cuvInceput, "while(") == 0) {
        return 3;
    }
    strncpy(cuvInceput, rand + spaces, 5);
    cuvInceput[5] = 0;
    if (strcmp(cuvInceput, "read(") == 0) {
        return 4;
    }
    strncpy(cuvInceput, rand + spaces, 6);
    cuvInceput[6] = 0;
    if (strcmp(cuvInceput, "print(") == 0) {
        return 5;
    }
    strncpy(cuvInceput, rand + spaces, 7);
    cuvInceput[7] = 0;
    if (strcmp(cuvInceput, "return(") == 0) {
        return 6;
    }
    strncpy(cuvInceput, rand + spaces, 2);
    cuvInceput[2] = 0;
    if (strstr(rand + spaces, "<-")!=NULL) {
        return 7;
    }
    strncpy(cuvInceput, rand + spaces, 4);
    cuvInceput[4] = 0;
    if (strcmp(cuvInceput, "else") == 0) {
        return 8;
    }
    return 0;  // something else
}

void printRow(int Left, int Right, int currLine, int rowNumber,int ypoz, int xpoz) {
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    int spaces = howManySpaces(randuri[rowNumber]);
    outtextxy((Right + Left) / 2 + xpoz, diagRowHeight * currLine + 0.7 * diagRowHeight + ypoz, randuri[rowNumber] + spaces);
}

void skipElseOrIf(int& row, int& linesToBracket, int &linesToDraw) {
    row++, linesToBracket++;
    int bracketsStack = 0;
    int linesCntElse = 0;
    int linesCntIf = 0;
    do {
        int tip = tipOperatie(randuri[row]);
        if (tip >= 2 && tip <= 7) {
            linesCntElse++;
        }
        else if (tip == 1) {
            linesCntElse += 2;
        }
        if (strstr(randuri[row], "{") != 0)
            bracketsStack++;
        else if (strstr(randuri[row], "}") != 0)
            bracketsStack--;
        row++, linesToBracket++;

    } while (bracketsStack > 0);
    // Right now it skips only else, if more operations are in else than in if, diagram will break
    // TODO    make this function skip either if or else, whichever has less operations to draw  (HINT: traverse both up and down directions)
}

void lastBracket(int row,int &linesToBracket, int &linesToDraw) {
    row++;
    int bracketsStack = 0;
    do {
        int tip = tipOperatie(randuri[row]);
        if (tip >= 2 && tip <= 7) {
            linesToDraw++;
        }
        else if (tip == 1) {
            linesToDraw += 2;
        }
        else if (tip == 8) {
            skipElseOrIf(row, linesToBracket, linesToDraw);
            tip = tipOperatie(randuri[row]);
            if (tip >= 2 && tip <= 7) {
                linesToDraw++;
            }
            else if (tip == 1) {
                linesToDraw += 2;
            }
        }

        if (strstr(randuri[row],"{")!=0)
            bracketsStack++;
        else if (strstr(randuri[row],"}")!=0)
            bracketsStack--;

        row++,linesToBracket++;
    } while (bracketsStack > 0);
}

void ifAndElseBracket(int row, int &linesToDrawFB, int &linesToFB,int &linesToDrawSecondB,int &linesInElseBrackets, bool &existaElse) {
    row++;
    int bracketsStack = 0;
    do {
        int tip = tipOperatie(randuri[row]);
        if (tip >= 2 && tip <= 7) {
            linesToDrawFB++;
        }
        else if (tip == 1) {
            linesToDrawFB += 2;
        }
        else if (tip == 8) {
            skipElseOrIf(row, linesToFB, linesToDrawFB);
            tip = tipOperatie(randuri[row]);
            if (tip >= 2 && tip <= 7) {
                linesToDrawFB++;
            }
            else if (tip == 1) {
                linesToDrawFB += 2;
            }
        }

        if (strstr(randuri[row], "{") != 0)
            bracketsStack++;
        else if (strstr(randuri[row], "}") != 0)
            bracketsStack--;
        row++, linesToFB++;

    } while (bracketsStack > 0);

    if (tipOperatie(randuri[row]) == 8)
        existaElse = TRUE;

    if (existaElse) {
        row++;
        bracketsStack = 0;
        do {
            int tip = tipOperatie(randuri[row]);
            if (tip >= 2 && tip <= 7) {
                linesToDrawSecondB++;
            }
            else if (tip == 1) {
                linesToDrawSecondB += 2;
            }
            else if (tip == 8) {
                skipElseOrIf(row, linesInElseBrackets, linesToDrawSecondB);
                tip = tipOperatie(randuri[row]);
                if (tip >= 2 && tip <= 7) {
                    linesToDrawSecondB++;
                }
                else if (tip == 1) {
                    linesToDrawSecondB += 2;
                }
            }

            if (strstr(randuri[row], "{") != 0)
                bracketsStack++;
            else if (strstr(randuri[row], "}") != 0)
                bracketsStack--;
            row++, linesInElseBrackets++;

        } while (bracketsStack > 0);
    }
}

void drawInstructions(int currLeft, int currRight, int &row,int &currLine,int &xpoz,int &ypoz,int rowLimFromPrev) {
    int rowLimit = 9999, rowLimitIf = 9999, rowLimitElse = 9999;

    for (; row < rowsInTotal; row++) {
        int tip = tipOperatie(randuri[row]);


        if (tip >= 4 && tip <= 7) {
            rectangle(currLeft + xpoz, diagRowHeight * currLine + ypoz, currRight + xpoz, diagRowHeight * (currLine + 1) + ypoz);
            printRow(currLeft, currRight, currLine, row, ypoz, xpoz);
            currLine++;
        }
        else if (tip == 2 || tip == 3) {
            // find last bracket
            int linesToBracket = 0;  //lines from current row to bracket (the closing bracket)
            int linesToDraw = 0;
            lastBracket(row, linesToBracket, linesToDraw);  //lines to draw from first bracket to last
            rowLimit = row + linesToBracket;

            rectangle(currLeft + xpoz, diagRowHeight * currLine + ypoz, currRight + xpoz, diagRowHeight * (currLine + linesToDraw + 1) + ypoz);
            printRow(currLeft, currRight, currLine, row, ypoz, xpoz);
            row++,currLine++;

            // paint instructionss with this left this right until last bracket (that is what rowLimit is for)
            drawInstructions(currLeft + maxW * 0.06, currRight, row, currLine, xpoz, ypoz, rowLimit);
        }
        else if (tip == 1) {
            // Find first bracket limit
            // find if else exists
            // if else exists find else brackets and limits
            int linesToDrawFirstB = 0;
            int linesToFirstB = 0;
            int linesToDrawSecondB = 0;
            int linesInElseBrackets = 0;
            bool existaElse = 0;
            ifAndElseBracket(row, linesToDrawFirstB, linesToFirstB, linesToDrawSecondB, linesInElseBrackets, existaElse);
            rowLimitIf = row + linesToFirstB;
            rowLimitElse = row + linesToFirstB + linesInElseBrackets;

            //draw the if else box
            char tru[] = "TRUE";
            char fals[] = "FALSE";
            int linesInRectangle = max(linesToDrawFirstB, linesToDrawSecondB);
            rectangle(currLeft + xpoz, diagRowHeight * currLine + ypoz, currRight + xpoz, diagRowHeight * (currLine + linesInRectangle + 2) + ypoz);
            line(currLeft + xpoz, diagRowHeight * (currLine + 2) + ypoz, currRight + xpoz, diagRowHeight * (currLine + 2) + ypoz);
            line(currLeft + xpoz, diagRowHeight * currLine + ypoz, (currRight + currLeft) / 2 + xpoz, diagRowHeight * (currLine + 2) + ypoz);
            line((currRight + currLeft) / 2 + xpoz, diagRowHeight * (currLine + 2) + ypoz, currRight + xpoz, diagRowHeight * currLine + ypoz);
            printRow(currLeft, currRight, currLine, row, ypoz, xpoz);
            settextjustify(BOTTOM_TEXT, LEFT_TEXT);
            outtextxy(currLeft + 0.01 * maxW + xpoz, diagRowHeight * (currLine + 2) - 0.01 * maxH + ypoz, tru);
            settextjustify(RIGHT_TEXT, BOTTOM_TEXT);
            outtextxy(currRight - 0.01 * maxW + xpoz, diagRowHeight * (currLine + 2) - 0.01 * maxH + ypoz, fals);


            // 
            // drawInstructions recursively for every if and else
            row++; currLine += 2;
            int currLineElse = currLine;
            drawInstructions(currLeft, (currRight + currLeft) / 2, row, currLine, xpoz, ypoz, rowLimitIf);
            drawInstructions((currRight + currLeft) / 2, currRight, row, currLineElse, xpoz, ypoz, rowLimitElse);
        }
        if (row >= rowLimFromPrev) break;
    }
}

void diagram() {
    int page = 0, ypoz = 0, xpoz = 0, xt = 0, yt = 0;   
    while (1) { 
        setvisualpage(page);
        setactivepage(1 - page);
        clearviewport();
        int currLine = 1;                                                        // currLine reprezinta nr liniei orizontale la care am ajuns sa desenam
        int currLeft = maxW/2-diagWidth/2, currRight = currLeft+diagWidth;       // row reprezinta randul din vectorul cu pseudocodul
        int rowLimit = 9999, rowLimitIf = 9999, rowLimitElse = 9999;

        for (int row = 0; row < rowsInTotal; row++) {
            int tip = tipOperatie(randuri[row]);

            
            if (tip >= 4 && tip <= 7) {
                rectangle(currLeft + xpoz, diagRowHeight * currLine + ypoz, currRight + xpoz, diagRowHeight * (currLine + 1) + ypoz);
                printRow(currLeft, currRight, currLine, row, ypoz, xpoz);
                currLine++;
            }
            else if (tip == 2 || tip == 3) {
                int linesToBracket =0;                          //lines from current row to bracket (the closing bracket)
                int linesToDraw = 0;                            //horizontal lines to draw in while/for
                lastBracket(row, linesToBracket, linesToDraw);
                rowLimit = row + linesToBracket;

                rectangle(currLeft + xpoz, diagRowHeight * currLine + ypoz, currRight + xpoz, diagRowHeight * (currLine + linesToDraw + 1) + ypoz);
                printRow(currLeft, currRight, currLine, row, ypoz, xpoz);
                row++,currLine++;

                // paint instructionss this with left this right until last bracket (that is what rowLimit is for)
                drawInstructions(currLeft + maxW * 0.06, currRight, row, currLine,xpoz,ypoz, rowLimit);
            }
            else if (tip == 1) {
                // Find first bracket limit
                // find if else exists
                // if else exists find else brackets and row limit for current if else
                int linesToDrawFirstB = 0;               // "B" stands for Bracket/Brackets
                int linesToFirstB = 0;
                int linesToDrawSecondB = 0;
                int linesInElseBrackets = 0;
                bool existaElse = 0;
                ifAndElseBracket(row, linesToDrawFirstB, linesToFirstB, linesToDrawSecondB, linesInElseBrackets, existaElse);
                rowLimitIf = row + linesToFirstB;
                rowLimitElse = row + linesToFirstB + linesInElseBrackets;
                 
                //draw the if else box
                char tru[] = "TRUE";
                char fals[] = "FALSE";
                int linesInRectangle = max(linesToDrawFirstB, linesToDrawSecondB);
                rectangle(currLeft + xpoz, diagRowHeight * currLine + ypoz, currRight + xpoz, diagRowHeight * (currLine + linesInRectangle + 2) + ypoz);
                line(currLeft + xpoz, diagRowHeight * (currLine + 2) + ypoz, currRight + xpoz, diagRowHeight * (currLine + 2) + ypoz);
                line(currLeft + xpoz, diagRowHeight * currLine + ypoz, (currRight+currLeft)/2+xpoz, diagRowHeight * (currLine + 2) + ypoz);
                line((currRight + currLeft) / 2 + xpoz, diagRowHeight * (currLine + 2) + ypoz, currRight + xpoz, diagRowHeight * currLine + ypoz);
                printRow(currLeft, currRight, currLine, row, ypoz, xpoz);
                settextjustify(BOTTOM_TEXT, LEFT_TEXT);
                outtextxy(currLeft + 0.01 * maxW + xpoz, diagRowHeight * (currLine + 2) - 0.01 * maxH + ypoz, tru);
                settextjustify(RIGHT_TEXT, BOTTOM_TEXT);
                outtextxy(currRight - 0.01 * maxW + xpoz, diagRowHeight * (currLine + 2) - 0.01 * maxH + ypoz, fals);


                // drawInstructions recursively for every if and else
                row++; currLine += 2;
                int currLineElse = currLine;   // because if and else start from same height
                drawInstructions(currLeft, (currRight+currLeft) / 2, row, currLine, xpoz, ypoz, rowLimitIf);
                drawInstructions((currRight + currLeft) / 2, currRight, row, currLineElse, xpoz, ypoz, rowLimitElse);
            }
        }

        setfillstyle(BKSLASH_FILL, 1);
        bar(maxW - 0.1 * maxW, 0.05 * maxH - 0.045 * maxH, maxW - 0.05 * maxW, 0.1 * maxH - 0.045 * maxH);
        setfillstyle(SLASH_FILL, 1);
        bar(maxW - 0.1 * maxW, maxH - 0.05 * maxH - 0.123 * maxH, maxW - 0.05 * maxW, maxH - 0.1 * maxH - 0.123 * maxH);
        xt = mousex();
        yt = mousey();
        if (GetAsyncKeyState(VK_UP))
            ypoz -= 5;
        if (GetAsyncKeyState(VK_DOWN))
            ypoz += 5;
        if (GetAsyncKeyState(VK_LEFT))
            xpoz -= 5;
        if (GetAsyncKeyState(VK_RIGHT))
            xpoz += 5;
        if (GetAsyncKeyState(VK_RBUTTON))
        {
            if (xt >= (maxW - 0.1 * maxW) && xt <= (maxW - 0.05 * maxW) && yt >= (0.05 * maxH + 0.077 * maxH) && yt <= (0.1 * maxH + 0.077 * maxH))
                diagWidth -= 1;
            else if (xt >= (maxW - 0.1 * maxW) && xt <= (maxW - 0.05 * maxW) && yt >= (maxH - 0.1 * maxH) && yt <= (maxH - 0.05 * maxH))
            {
                diagRowHeight -= 1;
            }
        }
        if (GetAsyncKeyState(VK_LBUTTON))
        {
            if (xt >= (maxW - 0.1 * maxW) && xt <= (maxW - 0.05 * maxW) && yt >= (0.05 * maxH + 0.077 * maxH) && yt <= (0.1 * maxH + 0.077 * maxH))
                diagWidth += 1;
            else if (xt >= (maxW - 0.1 * maxW) && xt <= (maxW - 0.05 * maxW) && yt >= (maxH - 0.1 * maxH) && yt <= (maxH - 0.05 * maxH))
                diagRowHeight += 1;
        }

        page = 1 - page;
    }
}

void afisare() 
{
    FILE* fptr = fopen(Path, "r");
    
    setviewport(0, 0.123*maxH, maxW, maxH-0.045*maxH, 1);
    clearviewport();
    int nrRanduri = 0, maxRand = 0;
    char rand[300], maxRandSir[300];
    while (fgets(rand, sizeof(rand), fptr)) 
    {
        nrRanduri++;
        if (strlen(rand) > maxRand) 
        {
            maxRand = strlen(rand);
            strcpy(maxRandSir, rand);
        }
    }
    int page = 0, ypoz = 0, xpoz = 0, xt = 0, yt = 0;
    int maxWidthRow = textwidth(maxRandSir);
    int textHeight = textheight(rand);
    while (1) 
    {
        setvisualpage(page);
        setactivepage(1 - page);
        clearviewport();
        int r = 1;
        rewind(fptr);
        rectangle(maxW - 0.1 * maxW, 0.05 * maxH - 0.045 * maxH, maxW - 0.05 * maxW, 0.1 * maxH - 0.045 * maxH);
        line(maxW - 0.075 * maxW, 0.05 * maxH - 0.045 * maxH, maxW - 0.1 * maxW, 0.1 * maxH - 0.045 * maxH);
        line(maxW - 0.075 * maxW, 0.05 * maxH - 0.045 * maxH, maxW - 0.05 * maxW, 0.1 * maxH - 0.045 * maxH);
        rectangle(maxW - 0.1 * maxW, maxH - 0.05 * maxH - 0.123 * maxH, maxW - 0.05 * maxW, maxH - 0.1 * maxH - 0.123 * maxH);
        line(maxW - 0.075 * maxW, maxH - 0.05 * maxH - 0.123 * maxH, maxW - 0.1 * maxW, maxH - 0.1 * maxH - 0.123 * maxH);
        line(maxW - 0.075 * maxW, maxH - 0.05 * maxH - 0.123 * maxH, maxW - 0.05 * maxW, maxH - 0.1 * maxH - 0.123 * maxH);
        xt = mousex();
        yt = mousey();
        if (GetAsyncKeyState(VK_LBUTTON))
        {
            if (xt >= (maxW - 0.1 * maxW) && xt <= (maxW - 0.05 * maxW) && yt >= (0.05 * maxH + 0.077 * maxH) && yt <= (0.1 * maxH + 0.077 * maxH))
                ypoz += 5;
            else if (xt >= (maxW - 0.1 * maxW) && xt <= (maxW - 0.05 * maxW) && yt >= (maxH - 0.1 * maxH) && yt <= (maxH - 0.05 * maxH))
                ypoz -= 5;
        }
        rectangle(maxW/2-(maxWidthRow /2)-maxW*0.01 + xpoz, maxH*0.1+ypoz, maxW / 2 + (maxWidthRow / 2) + maxW * 0.01 + xpoz, maxH*0.1+(textHeight *(nrRanduri+1))+ypoz);
        while (fgets(rand, sizeof(rand), fptr)) 
        {
            outtextxy(maxW / 2 - (maxWidthRow / 2) + xpoz, maxH*0.1+ (textHeight * r) + ypoz, rand);
            r++;
        }
        page = 1 - page;
        if (GetAsyncKeyState(VK_RETURN)) {
            copiereRanduri();
            diagram();
        }
    }
}

void ecranEroare() 
{
    clearviewport();
    setcolor(4);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    char eroare[50] = "Eroare la deschiderea fisierului";
    char second[100] = "Apasati orice tasta pentru a inchide aplicatia";
    outtextxy(maxW / 2, maxH / 2 + 0.03 * maxH, eroare);
    outtextxy(maxW / 2, maxH / 2 + 0.1 * maxH, second);
    getch();
    exit(1);
}

void alegeFisier() 
{
    OPENFILENAMEA ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = Path;
    *(ofn.lpstrFile) = NULL;
    ofn.nMaxFile = sizeof(Path);
    if (GetOpenFileNameA(&ofn) == 0) 
    {
        ecranEroare();
    }
    afisare();
}

void start() 
{
    int x = 0, y = 0;
    char word[20] = "Alege fisier text";
    char word2[20] = "Iesire";
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    rectangle(maxW / 2 - 0.1 * maxW, maxH / 2, maxW / 2 + 0.1 * maxW, maxH / 2 + 0.1 * maxH);
    rectangle(maxW / 2 - 0.1 * maxW, maxH / 2 + 0.2 * maxH, maxW / 2 + 0.1 * maxW, maxH / 2 + 0.3 * maxH);
    outtextxy(maxW / 2, maxH / 2 + 0.06 * maxH, word);
    outtextxy(maxW / 2, maxH / 2 + 0.26 * maxH, word2);
    settextjustify(LEFT_TEXT, TOP_TEXT);
    while (1) 
    {
        getmouseclick(WM_LBUTTONUP, x, y);
        if (x >= (maxW / 2 - 0.1 * maxW + 1) && x <= (maxW / 2 + 0.1 * maxW - 1) && y >= (maxH / 2 + 1) && y <= (maxH / 2 + 0.1 * maxH - 1)) 
        {
            alegeFisier();
        }
        else if (x >= (maxW / 2 - 0.1 * maxW + 1) && x <= (maxW / 2 + 0.1 * maxW - 1) && y >= (maxH / 2 + 0.2 * maxH + 1) && y <= (maxH / 2 + 0.3 * maxH - 1)) 
        {
            exit(1);
        }
    }
}

int main()
{
    int page = 0;
    initwindow(maxW, maxH, "Generator de Diagrame Nassi-Schneiderman");
    setactivepage(page);
    readimagefile("image.bmp", 0, 0, maxW, maxH);
    setactivepage(1 - page);
    readimagefile("image.bmp", 0, 0, maxW, maxH);
    setvisualpage(1-page);
    setcolor(1);
    setbkcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 0);
    setusercharsize(1, 0.1*maxW, 1, 0.1*maxH);
    setlinestyle(SOLID_LINE, 1, 3);
    start();
    getch();
    closegraph();
    return 0;
}