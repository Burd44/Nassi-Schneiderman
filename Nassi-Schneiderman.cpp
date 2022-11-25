#include <iostream>
#include "graphics.h"
#include "winbgim.h"
#include <Windows.h>
#include <cstring>
#pragma comment(lib,"graphics.lib")

using namespace std;

char Path[300];
int maxH = getmaxheight() / 1.25, maxW = getmaxwidth() / 1.25;

void afisare() 
{
    settextstyle(DEFAULT_FONT, 0, 140);
    FILE* fptr = fopen(Path, "r");
    cleardevice();
    int nrRanduri = 0, maxRand = 0;
    char rand[300], maxRandSir[300];
    while (fgets(rand, sizeof(rand), fptr)) {
        nrRanduri++;
        if (strlen(rand) > maxRand) {
            maxRand = strlen(rand);
            strcpy(maxRandSir, rand);
        }
    }
    int page = 0, ypoz = 0;
    while (1) {
        setvisualpage(page);
        setactivepage(1 - page);
        cleardevice();
        int r = 1;
        if (GetAsyncKeyState(VK_UP))
            ypoz-=5;
        if (GetAsyncKeyState(VK_DOWN))
            ypoz+=5;
        rewind(fptr);
        rectangle(maxW/2-(textwidth(maxRandSir)/2)-maxW*0.01, maxH*0.1+ypoz, maxW / 2 + (textwidth(maxRandSir) / 2) + maxW * 0.01, maxH*0.1+(25*(nrRanduri+1))+ypoz);
        while (fgets(rand, sizeof(rand), fptr)) {
            outtextxy(maxW / 2 - (textwidth(maxRandSir) / 2), maxH*0.1+(25*r)+ypoz, rand);
            r++;
        }
        page = 1 - page;
    }
}

void ecranEroare() 
{
    cleardevice();
    setcolor(4);
    char eroare[50] = "Eroare la deschiderea fisierului";
    char second[100] = "Apasati orice tasta pentru a inchide aplicatia";
    outtextxy(maxW / 2 - 0.15 * maxW, maxH / 2 + 0.03 * maxH, eroare);
    outtextxy(maxW / 2 - 0.2 * maxW, maxH / 2 + 0.1 * maxH, second);
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
    if (GetOpenFileNameA(&ofn) == 0) {
        ecranEroare();
    }
    afisare();
}

void start() 
{
    int x = 0, y = 0;
    char word[20] = "Apasa";
    char word2[20] = "Iesire";
    rectangle(maxW / 2 - 0.1 * maxW, maxH / 2, maxW / 2 + 0.1 * maxW, maxH / 2 + 0.1 * maxH);
    rectangle(maxW / 2 - 0.1 * maxW, maxH / 2 + 0.2 * maxH, maxW / 2 + 0.1 * maxW, maxH / 2 + 0.3 * maxH);
    settextstyle(DEFAULT_FONT, 0, 140);
    outtextxy(maxW / 2 - 0.025 * maxW, maxH / 2 + 0.03 * maxH, word);
    outtextxy(maxW / 2 - 0.025 * maxW, maxH / 2 + 0.23 * maxH, word2);
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
    initwindow(getmaxwidth() / 1.25, getmaxheight() / 1.25, "Generator de Diagrame Nassi-Schneiderman");
    start();
    getch();
    closegraph();
    return 0;
}