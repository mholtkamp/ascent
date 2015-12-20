#ifndef FIXED_POINT_H
#define FIXED_POINT_H

#define FIXED_SHIFT 16
#define FIXED_SCALE 0x10000

// This utilizes 16.16 fixed point numbers
typedef int fixed

static inline fixed int_to_fixed(int nInt)
{
    return nInt << FIXED_SHIFT;
}

static inline int fixed_to_int(fixed nFixed)
{
    return nFixed >> FIXED_SHIFT;
}


#endif