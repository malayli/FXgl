#ifndef SUPERFX_H
#define SUPERFX_H

#define SFX_ScreenWidth 191
#define SFX_ScreenHeight 159

// Define the SuperFX registers
#define GSU_R15u (*(u8 *)0x301e)
#define GSU_R15l (*(u8 *)0x301f)
#define GSU_CBR (*(u8 *)0x3000)
#define GSU_PBR (*(u8 *)0x3001)
#define GSU_PBR1 (*(u8 *)0x3002)
#define GSU_ROMBR (*(u8 *)0x3036)

#define GSU_CFGR    (*(u8 *)0x3037)
#define GSU_CFBR    (*(u8 *)0x3038)
#define GSU_CLSR    (*(u8 *)0x3039)

#define GSU_PBR     (*(u8 *)0x3034)
#define GSU_SCMR    (*(u8 *)0x303A)
#define GSU_CACHE   (*(u8 *)0x3100)

#define GSU_ACTIVE_FL (*(u8 *)0x3FFF)

#define GSU_COLOR (*(vuint8 *)0x701002)
#define GSU_X (*(u8 *)0x701003)
#define GSU_Y (*(u8 *)0x701004)

u16 c1;
u8 c2;

u16 canvasTilesMap[2048];

extern void SuperFXInit();
extern void copySuperFXProgram();
extern void executeSuperFX_asm();
extern void cleanSuperFX_RAM();

void cleanSuperFX() {
    int i = 0;
    for (i = 0; i < 0xA001; i++) {
        (*(u8 *)(0x701000 + i)) = 0; 
    }
}

// Keep these functions in here.
void setSuperFX_Function(u16 functionCall) {
    (*(u8 *)0x703F00) = (functionCall);
}

void executeSuperFX() {
    executeSuperFX_asm();
    while (GSU_ACTIVE_FL == 1);
}

void plotPixel_GSU(u8 x, u8 y, u8 color) {
    /*u8 isOverX = (x > SFX_ScreenWidth);
    u8 isOverY = (y > SFX_ScreenHeight);

    if (isOverX) {
        x = SFX_ScreenWidth;
    }
    if (isOverY) {
        y = SFX_ScreenHeight;
    }*/

    //if (!(isOverX || isOverY)) {
        GSU_COLOR = color;
        GSU_X = x;
        GSU_Y = y;
        setSuperFX_Function(1);
        executeSuperFX();
    //}
}

void plotBox_GSU(u8 x, u8 y, u8 w, u8 h, u8 color)
{
    u8 isOverX = (x > SFX_ScreenWidth);
    u8 isOverY = (y > SFX_ScreenHeight);

    if (isOverX) {
        x = SFX_ScreenWidth;
    }
    if (isOverY) {
        y = SFX_ScreenHeight;
    }

    u8 isOverXW = ((x+w) > SFX_ScreenWidth);
    u8 isOverYH = ((y+h) > SFX_ScreenHeight);

    if (isOverXW) {
        w = (SFX_ScreenWidth - x);
    }
    if (isOverYH) {
        h = (SFX_ScreenHeight- y);
    }

    w++;
    h++;

    if (!(isOverX || isOverY) && !(isOverXW || isOverYH)) {
        GSU_COLOR = color;
        GSU_X = x;
        GSU_Y = y;
        (*(u8 *)0x701005) = w;
        (*(u8 *)0x701006) = h;
        setSuperFX_Function(2);
        executeSuperFX();
    }
}

void plotTriangle_GSU(u8 x, u8 y, u8 x1, u8 y1, u8 x2, u8 y2, u8 color)
{
    u8 isOverX = (x > SFX_ScreenWidth);
    u8 isOverY = (y > SFX_ScreenHeight);

    if (isOverX) {
        x = SFX_ScreenWidth;
    }
    if (isOverY) {
        y = SFX_ScreenHeight;
    }

    u8 isOverX2 = x1;
    u8 isOverY2 = y1;

    if (isOverX2) {
        x1 = SFX_ScreenWidth;
    }
    if (isOverY2) {
        y1 = SFX_ScreenHeight;
    }

    u8 isOverX3 = x1;
    u8 isOverY3 = y1;

    if (isOverX3) {
        x2 = SFX_ScreenWidth;
    }
    if (isOverY3) {
        y2 = SFX_ScreenHeight;
    }

    if (!(isOverX || isOverY) && !(isOverX2 || isOverY2) && !(isOverX3 || isOverY3)) {
        GSU_COLOR = color;
        GSU_X = x;
        GSU_Y = y;
        (*(u8 *)0x701005) = x1;
        (*(u8 *)0x701006) = y1;
        (*(u8 *)0x701007) = x2;
        (*(u8 *)0x701008) = y2;
        setSuperFX_Function(3);
        executeSuperFX();
    }
}

#endif
