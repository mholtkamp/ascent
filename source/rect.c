#include "rect.h"

void rect_initialize(Rect* pRect)
{
    if (pRect != 0)
    {
        pRect->fX = 0;
        pRect->fY = 0;
        pRect->fWidth = int_to_fixed(DEFAULT_RECT_WIDTH);
        pRect->fHeight = int_to_fixed(DEFAULT_RECT_HEIGHT);
    }
}