#include <stdio.h>
#include <stdlib.h>






__global__ void gridRealThreshold_v1(float** out, float** in, float threshold, const vect2& sz, int offset)
{

    // it should be done out-of-place
    if (in[0] == out[0]) {
        return; // Exit Program
    }

    float pp = 0;
    float pm = 0;
    float mp = 0;
    float mm = 0;
    float areamm = 0.0;
    float areamp = 0.0;
    float areapm = 0.0;
    float areapp = 0.0;

    // first set output to 0
    memset(&(out[0][0]), '\0', sz.xe * (offset + 1) * sizeof(float));
    float* inRow1, * inRow2, * outRow1, * outRow2;
    int ul = sz.ye - offset;

    int iy = threadIdx.y + blockDim.y * blockIdx.y;
    int ix = threadIdx.x + blockDim.x * blockIdx.x;

    if (iy < offset || iy >= ul)
    {
        return;
    }

    // start with internal pixels

        inRow1 = in[iy];
        inRow2 = in[iy + 1];
        outRow1 = out[iy];
        outRow2 = out[iy + 1];
        memset(&(out[iy + 1][0]), '\0', sz.xe * sizeof(float));

            // corner values
            mm = inRow1[ix];
            pm = inRow1[ix + 1];
            mp = inRow2[ix];
            pp = inRow2[ix + 1];

            if ((pp >= threshold) && (pm >= threshold) && (mp >= threshold) && (mm >= threshold)) return;

            if ((pp <= threshold) && (pm <= threshold) && (mp <= threshold) && (mm <= threshold)) {       // the whole pixel is <= threshold

                outRow1[ix] += (0.25);
                outRow1[ix + 1] += (0.25);
                outRow2[ix] += (0.25);
                outRow2[ix + 1] += (0.25);
            }
            else {

                // find the areas
                areasFromCornerValues(pp, pm, mp, mm, threshold, areapp, areapm, areamp, areamm);

                outRow1[ix] += (0.25 - areamm);
                outRow1[ix + 1] += (0.25 - areapm);
                outRow2[ix] += (0.25 - areamp);
                outRow2[ix + 1] += (0.25 - areapp);
            }
                  // finished with internal pixels

    if (offset > 1)
        memset(&(out[ul + 1][0]), '\0', sz.xe * (offset - 1) * sizeof(float));

}






