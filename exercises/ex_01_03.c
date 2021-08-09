/*
Exercise 1-3. Modify the temperature conversion program to print a heading above the table.
 */
#include <stdio.h>

int main(void) {

    double fahr;
    double celsius;
    int    lower =   0;  // lower limit of temperature table
    int    upper = 300;  // upper limit
    int    step  =  20;  // step size

    printf("Fahrenheit\tCelsius\n");
    fahr = (double)lower;
    while (fahr <= (double)upper) {
        celsius = (5.0 / 9.0) * (fahr - 32.0);
        printf("%3.0f\t\t%6.1f\n", fahr, celsius);
        fahr = fahr + step;
    }

    return 0;
}
