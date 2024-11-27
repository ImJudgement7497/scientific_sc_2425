# PPQ 5

All results can be found in the `./results_*` directories, submitted as part of the `Q5_bmjm501.tar`.

Below is the data table for part a-c on `teaching0`, noting the following information:

```txt

GCC Version: GNU Fortran (GCC) 12.3.0
Intel Version: ifort (IFORT) 2021.7.1 20221019
CPU: 
```


| Compiler Flags  	| Actions to source (letter refers to part and result directory) 	| No of inaccurate values 	|
|-----------------	|----------------------------------------------------------------	|-------------------------	|
| gfortran -O0    	| No changes (a)                                                 	| 135                     	|
| ifort -O0       	| No changes (a)                                                 	| 135                     	|
| gfortran -O0    	| Changed single to double precision (b)                         	| 82                      	|
| ifort -O0       	| Changed single to double precision (b)                         	| 82                      	|
| gfortran -O0    	| Removed print statement, using double precision (c1)           	| 82                      	|
| ifort -O0       	| Removed print statement, using double precision (c1)           	| 82                      	|
| gfortran -O3    	| Removed print statement, using double precision (c2)           	| 82                      	|
| ifort -O3       	| Removed print statement, using double precision (c2)           	| 0                       	|
| gfortran -Ofast 	| Removed print statement, using double precision (c3)           	| 82                      	|
| ifort -Ofast    	| Removed print statement, using double precision (c3)           	| 0                       	|
| gfortran -O0    	| Removed print statement, using single precision (c4)           	| 135                     	|
| ifort -O0       	| Removed print statement, using single precision (c4)           	| 135                     	|
| gfortran -O3    	| Removed print statement, using single precision (c5)           	| 135                     	|
| ifort -O3       	| Removed print statement, using single precision (c5)           	| 629                     	|
| gfortran -Ofast 	| Removed print statement, using single precision (c6)           	| 629                     	|
| ifort -Ofast    	| Removed print statement, using single precision (c6)           	| 629                     	|

Below is the data table for part a-c on `VIKING`, noting the following information:

```txt

GCC Version: GNU Fortran (GCC) 13.2.0
Intel Version: ifort (IFORT) 2021.9.0 20230302
CPU: AMD EPYC3 7643
```

| Compiler Flags  	| Actions to source (letter refers to part and result directory) 	| No of inaccurate values 	|
|-----------------	|----------------------------------------------------------------	|-------------------------	|
| gfortran -O0    	| No changes (a)                                                 	| 135                     	|
| ifort -O0       	| No changes (a)                                                 	| 135                     	|
| gfortran -O0    	| Changed single to double precision (b)                         	| 82                      	|
| ifort -O0       	| Changed single to double precision (b)                         	| 82                      	|
| gfortran -O0    	| Removed print statement, using double precision (c1)           	| 82                      	|
| ifort -O0       	| Removed print statement, using double precision (c1)           	| 82                      	|
| gfortran -O3    	| Removed print statement, using double precision (c2)           	| 82                      	|
| ifort -O3       	| Removed print statement, using double precision (c2)           	| 0                       	|
| gfortran -Ofast 	| Removed print statement, using double precision (c3)           	| 82                      	|
| ifort -Ofast    	| Removed print statement, using double precision (c3)           	| 0                       	|
| gfortran -O0    	| Removed print statement, using single precision (c4)           	| 135                     	|
| ifort -O0       	| Removed print statement, using single precision (c4)           	| 135                     	|
| gfortran -O3    	| Removed print statement, using single precision (c5)           	| 135                     	|
| ifort -O3       	| Removed print statement, using single precision (c5)           	| 310                     	|
| gfortran -Ofast 	| Removed print statement, using single precision (c6)           	| 310                     	|
| ifort -Ofast    	| Removed print statement, using single precision (c6)           	| 310                     	|


The tables are exactly the same until the last 3 rows.

The number of inaccurate values changes with different compilers and different flags due to the nature of compiler optimisation. 
https://www.tablesgenerator.com/markdown_tables#

## Number of digits of precision

The results I got from `./main`, compiled on an AMD Ryzen 5 3500U with `g++ -O0 Q5_bmjm501.cpp -o main` were:
```txt

Float digits of precision: 8. Double digits of precision: 16 

```

This falls within the IEEE 754 standard. The explanation of the code can be found below, or in source `Q5_bmjm501.cpp`

```c++

#include <iostream>

int main()
{
    /* Need a way of determining accuracy i.e. comparing
    to a number to itself plus a small change, while counting number of digits
    i.e. dividing by 10
    */

    float flt = 1.0f; // Specify 'f' to make sure it is float
    double dbl = 1.0;

    float delta_f = 1.0f; // Inital small change
    double delta_d = 1.0;

    int digits_f = 0, digits_d = 0;

    // When flt+delta_f == flt, we have lost accuracy
    // A number plus a really small number algebraically never is the same number
    // Thus is we break this condition, the computer thinks the numbers are the same
    while (flt + delta_f != flt)
    {
        delta_f /= 10.0; // Divide by 10 as we are checking digits
        digits_f++;
    }

    // When dbl+delta_d == dbl, we have lost accuracy
    while (dbl + delta_d != dbl)
    {
        delta_d /= 10.0; // Divide by 10 as we are checking digits
        digits_d++;
    }

    printf("Float digits of precision: %d. Double digits of precision: %d \n", digits_f, digits_d);

    return 0;
}

```