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