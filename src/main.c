#include <snes.h>
#include <stdint.h>
#include "SuperFX.h"

// Palette numbers for tileset

#define PAL0 0
#define PAL1 1
#define PAL2 2
#define PAL3 3
#define PAL4 4
#define PAL5 5
#define PAL6 6
#define PAL7 7

#define transparentColor RGB5(0x1F, 0x00, 0x1F)

const u16 blackPalette[] = {
    RGB5(0x00, 0x00, 0x00),
    RGB5(0x00, 0x00, 0x00),
    RGB5(0x00, 0x00, 0x00),
    RGB5(0x00, 0x00, 0x00),
    RGB5(0x00, 0x00, 0x00),
    RGB5(0x00, 0x00, 0x00),
    RGB5(0x00, 0x00, 0x00),
    RGB5(0x00, 0x00, 0x00),
    RGB5(0x00, 0x00, 0x00),
    RGB5(0x00, 0x00, 0x00),
    RGB5(0x00, 0x00, 0x00),
    RGB5(0x00, 0x00, 0x00),
    RGB5(0x00, 0x00, 0x00),
    RGB5(0x00, 0x00, 0x00),
    RGB5(0x00, 0x00, 0x00),
    RGB5(0x00, 0x00, 0x00),
};

const u16 superfxPalette[] = {
    transparentColor, 
    RGB5(0x00, 0x00, 0x00),
    RGB5(0x00, 0x00, 0x0C), 
    RGB5(0x00, 0x00, 0x11), 
    RGB5(0x00, 0x00, 0x16), 
    RGB5(0x1F, 0x00, 0x00),
    RGB5(0x00, 0x00, 0x00),
    RGB5(0x00, 0x00, 0x00),
    RGB5(0x00, 0x00, 0x00),
    RGB5(0x00, 0x00, 0x00),
    RGB5(0x00, 0x00, 0x00),
    RGB5(0x00, 0x00, 0x00),
    RGB5(0x00, 0x00, 0x00),
    RGB5(0x00, 0x01, 0x00),
    RGB5(0x00, 0x00, 0x00),
    RGB5(0x00, 0x00, 0x00)
};

extern char tilfont, palfont;
u16 inc;
u8 xx;
u8 yy;
u8 ii;
u8 refresh;
u16 tileAddress;

void superNintendoVblank() {
    if (refresh == 1) {
        dmaCopyVram(0x704000, 0x0000, 4096);
        refresh = 0;
    }
}

int main() {
    consoleInit();

    refresh = 0;

    cleanSuperFX();
    SuperFXInit();
    cleanSuperFX_RAM();
    copySuperFXProgram();

    // Palette 0
    dmaCopyCGram(blackPalette, 0, 32);
    WaitForVBlank();
    
    // Palette 1
    dmaCopyCGram(superfxPalette, 16, 32);
    WaitForVBlank();

    // Init background
    bgSetGfxPtr(0, 0x0000);
    bgSetMapPtr(0, 0x2C00, SC_32x32);
    WaitForVBlank();

    // Now Put in 16 color mode and disable Bgs except current
    setMode(BG_MODE1, BG3_MODE1_PRIORITY_HIGH);
    bgSetEnable(0);
    bgSetDisable(1);
    bgSetDisable(2);
    WaitForVBlank();

    // Clean canvasTilesMap  - 0x300 is supposed to be an empty tile
    for (xx = 0; xx < 32; xx++) {
        for (yy = 0; yy < 32; yy++) {
            canvasTilesMap[(yy * 32) + xx] = 0x300 + (PAL1<<10);
        }
    }
    WaitForVBlank();

    // Set tile addresses
    yy = 2;
    tileAddress = 0x00;
    while (yy < 26) {
        for (xx = 2; xx < 30; xx++) {
            canvasTilesMap[xx + (yy*32)] = tileAddress + (PAL1<<10);
            tileAddress += 0x18;
        }
        yy += 1;
        tileAddress = 0x00 + (yy - 2);
    }
    WaitForVBlank();
    
    dmaCopyVram(canvasTilesMap, 0x2C00, 2048);
    setScreenOn();

    nmiSet(superNintendoVblank);

    while (1) {
        plotPixel_GSU(0, 0, 5);
        plotPixel_GSU(0, 8, 5);
        plotPixel_GSU(8, 0, 5);
        plotPixel_GSU(8, 8, 5);
        refresh = 1;

        WaitForVBlank();
    }
    return 0;
}
