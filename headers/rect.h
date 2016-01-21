#ifndef RECT_H
#define RECT_H

#include "fixedpoint.h"

#define DEFAULT_RECT_WIDTH 8
#define DEFAULT_RECT_HEIGHT 8

typedef struct Rect
{
    fixed fX;
    fixed fY;
    fixed fWidth;
    fixed fHeight;
} Rect;

void rect_initialize(Rect* pRect);

int rect_move_with_bg_collision(Rect* pRect,
                                fixed fDX,
                                fixed fDY);

int rect_overlap(Rect* pOne, Rect* pTwo);

#endif