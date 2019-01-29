#ifndef COMMON_H
#define COMMON_H

#include <genesis.h>

#define W 256
#define H 160

typedef struct {
    fix32 x,y;
} Vect2D_f32;

typedef struct {
    fix32 x,y,z;
} Vect3D_f32;


#define EYE_HEIGHT FIX32(10)
#define DUCK_HEIGHT FIX32(3.5)
#define HEAD_MARGIN FIX32(1)
#define KNEE_HEIGHT FIX32(2.5)

#define HFOV FIX32(0.73 * H/W) //FIX32(0.73 * H)
#define VFOV FIX32(.2f) //*H)



#define clamp(a, mi,ma)      min(max(a,mi),ma)         // clamp: Clamp value into set range.


#define SAFEMUL32(x,y) (((x)>>FIX32_FRAC_BITS/2) * ((y)>>FIX32_FRAC_BITS/2))

#define SAVEDIV32(x,y) (((x)<<FIX32_FRAC_BITS/2) / ((x)<<FIX32_FRAC_BITS/2))

// vxs: Vector cross product
#define vxs16(x0,y0, x1,y1)    (fix16Mul((x0), (y1)) - fix16Mul((x1), (y0)))
//#define vxs32(x0,y0, x1,y1)    (fix32Mul((x0), (y1)) - fix32Mul((x1), (y0)))
fix32 vxs32(fix32 x0, fix32 y0, fix32 x1, fix32 y1) {
    return fix32Mul(x0, y1) - fix32Mul(x1, y0);
}

//#define vxs32(x0,y0, x1,y1)    (SAFEMUL32((x0), (y1)) - SAFEMUL32((x1), (y0)))
// Overlap:  Determine whether the two number ranges overlap.
#define Overlap(a0,a1,b0,b1) (min(a0,a1) <= max(b0,b1) && min(b0,b1) <= max(a0,a1))
// IntersectBox: Determine whether two 2D-boxes intersect.
#define IntersectBox(x0,y0, x1,y1, x2,y2, x3,y3) (Overlap(x0,x1,x2,x3) && Overlap(y0,y1,y2,y3))
// PointSide: Determine which side of a line the point is on. Return value: <0, =0 or >0.
#define PointSide16(px,py, x0,y0, x1,y1) vxs16((x1)-(x0), (y1)-(y0), (px)-(x0), (py)-(y0))
#define PointSide32(px,py, x0,y0, x1,y1) vxs32((x1)-(x0), (y1)-(y0), (px)-(x0), (py)-(y0))
// Intersect: Calculate the point of intersection between two lines.
#define Intersect16(x1,y1, x2,y2, x3,y3, x4,y4) ((Vect2D_f16) {		\
      fix16Div(vxs16(vxs16(x1,y1, x2,y2), (x1)-(x2), vxs16(x3,y3, x4,y4), (x3)-(x4)), vxs16((x1)-(x2), (y1)-(y2), (x3)-(x4), (y3)-(y4))), \
	fix16Div(vxs16(vxs16(x1,y1, x2,y2), (y1)-(y2), vxs16(x3,y3, x4,y4), (y3)-(y4)), vxs16((x1)-(x2), (y1)-(y2), (x3)-(x4), (y3)-(y4))) })

//#define Intersect32(x1,y1, x2,y2, x3,y3, x4,y4) ((Vect2D_f32) {		\
//      fix32Div(vxs32(vxs32(x1,y1, x2,y2), (x1)-(x2), vxs32(x3,y3, x4,y4), (x3)-(x4)), vxs32((x1)-(x2), (y1)-(y2), (x3)-(x4), (y3)-(y4))), \
//	fix32Div(vxs32(vxs32(x1,y1, x2,y2), (y1)-(y2), vxs32(x3,y3, x4,y4), (y3)-(y4)), vxs32((x1)-(x2), (y1)-(y2), (x3)-(x4), (y3)-(y4))) })

Vect2D_f32 Intersect32(fix32 x1, fix32 y1, fix32 x2, fix32 y2, fix32 x3, fix32 y3, fix32 x4, fix32 y4) {
    fix32 x1minusx2 = x1-x2;
    fix32 x3minusx4 = x3-x4;
    fix32 y1minusy2 = y1-y2;
    fix32 y3minusy4 = y3-y4;
    fix32 x3y3crossx4y4 = vxs32(x3,y3, x4,y4);
    fix32 x1y1crossx2y2 = vxs32(x1,y1, x2,y2);

    fix32 rightside = vxs32(x1minusx2, y1minusy2, x3minusx4, y3minusy4);
    fix32 x = fix32Div(vxs32(x1y1crossx2y2, x1minusx2, x3y3crossx4y4, x3minusx4), rightside);

    fix32 y = fix32Div(vxs32(x1y1crossx2y2, y1minusy2, x3y3crossx4y4, y3minusy4), rightside);
    return (Vect2D_f32){x, y};
}



#endif