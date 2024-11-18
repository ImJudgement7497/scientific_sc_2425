# PPQ 5

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


