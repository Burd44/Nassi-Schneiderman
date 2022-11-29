#include <iostream>
#include "graphics.h"
#include "winbgim.h"
#include <Windows.h>
#include <cstring>
#pragma comment(lib,"graphics.lib")

using namespace std;

///////////// idea

struct ifOrElseNod {
    int maxLineLenght;
    char identity;     // i (pt if) sau e (pt else)
};
struct ifElseTrees {
    ifOrElseNod tree[100];  // un arbore pentru o amestecatura de if - uri si else - uri
    int lght = 0;      // lungimea vectorului care reprezinta arborele
    int arbLine;       // linia la care incepe arborele
}trees[50];

int treesCnt = 0;

/////////////



char Path[300];
int maxH = getmaxheight() / 1.15, maxW = getmaxwidth() / 1.15;

void afisare() 
{
    FILE* fptr = fopen(Path, "r");
    cleardevice();
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
        cleardevice();
        int r = 1;
        /*if (GetAsyncKeyState(VK_UP))
            ypoz+=5;
        if (GetAsyncKeyState(VK_DOWN))
            ypoz-=5;
        if (GetAsyncKeyState(VK_LEFT))
            xpoz += 5;
        if (GetAsyncKeyState(VK_RIGHT))
            xpoz -= 5;*/
        rewind(fptr);
        rectangle(maxW - 0.1 * maxW, 0.05 * maxH, maxW - 0.05 * maxW, 0.1 * maxH);
        line(maxW - 0.075 * maxW, 0.05 * maxH, maxW - 0.1 * maxW, 0.1 * maxH);
        line(maxW - 0.075 * maxW, 0.05 * maxH, maxW - 0.05 * maxW, 0.1 * maxH);
        rectangle(maxW - 0.1 * maxW, maxH - 0.05 * maxH, maxW - 0.05 * maxW, maxH - 0.1 * maxH);
        line(maxW - 0.075 * maxW, maxH - 0.05 * maxH, maxW - 0.1 * maxW, maxH - 0.1 * maxH);
        line(maxW - 0.075 * maxW, maxH - 0.05 * maxH, maxW - 0.05 * maxW, maxH - 0.1 * maxH);
        xt = mousex();
        yt = mousey();
        if (GetAsyncKeyState(VK_LBUTTON))
        {
            if (xt >= (maxW - 0.1 * maxW) && xt <= (maxW - 0.05 * maxW) && yt >= (0.05 * maxH) && yt <= (0.1 * maxH))
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
    }
}

void ecranEroare() 
{
    cleardevice();
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
    initwindow(maxW, maxH, "Generator de Diagrame Nassi-Schneiderman");
    settextstyle(9, HORIZ_DIR, 0);
    setusercharsize(1, 0.1*maxW, 1, 0.1*maxH);
    start();
    getch();
    closegraph();
    return 0;
}