#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define NPOINTS 2000
#define MAXITER 2000

struct complex
{
  double real;
  double imag;
};

int main()
{
  int numoutside = 0;
  double area, error, ztemp;
  struct complex z, c;

/*
 * Outer loops run over NPOINTS, initialize z = c
 * Inner loop has the iteration z = z*z + c, and threshold test
 */
#pragma omp parallel default(none) private(area, error, ztemp, z, c) reduction(task, + : numoutside)
  {
#pragma omp single

    for (int i = 0; i < NPOINTS; i++)
    {
#pragma omp task firstprivate(i) in_reduction(+ : numoutside)
      for (int j = 0; j < NPOINTS; j++)
      {
        c.real = -2.0 + 2.5 * (double)(i) / (double)(NPOINTS) + 1.0e-7;
        c.imag = 1.125 * (double)(j) / (double)(NPOINTS) + 1.0e-7;
        z = c;

        for (int iter = 0; iter < MAXITER; iter++)
        {
          ztemp = (z.real * z.real) - (z.imag * z.imag) + c.real;
          z.imag = 2 * z.real * z.imag + c.imag;
          z.real = ztemp;

          if ((z.real * z.real + z.imag * z.imag) > 4.0)
          {
            numoutside++;
            break;
          }
        }
      }
    }
  }
  /*
   * Calculate area and error and output the results
   */
  area = 2.0 * 2.5 * 1.125 * (double)(NPOINTS * NPOINTS - numoutside) / (double)(NPOINTS * NPOINTS);
  error = area / (double)NPOINTS;

  printf("Area of Mandelbrot set = %12.8f +/- %12.8f\n", area, error);

  return 0;
}
