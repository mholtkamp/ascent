#ifndef FIXED_POINT_H
#define FIXED_POINT_H

#define FIXED_SHIFT 16
#define FIXED_SCALE 0x10000

// Referred to the Tonc library's fixed point primer for these functions.
// http://www.coranac.com/tonc/text/fixed.htm

// This utilizes 16.16 fixed point numbers
typedef int fixed;

// Conversions
static inline fixed int_to_fixed(int nInt)
{
    return nInt << FIXED_SHIFT;
}

static inline int fixed_to_int(fixed nFixed)
{
    return nFixed >> FIXED_SHIFT;
}

// Fixed point math operations
static inline fixed fixed_add(fixed fA, fixed fB)
{
    return fA + fB;
}

static inline fixed fixed_subtract(fixed fA, fixed fB)
{   
    return fA - fB;
}

static inline fixed fixed_multiply(fixed fA, fixed fB)
{   
    return (fA * fB) >> FIXED_SHIFT;
}

static inline fixed fixed_divide(fixed fA, fixed fB)
{
    return ((fA) * FIXED_SCALE)/(fB);
}


#endif