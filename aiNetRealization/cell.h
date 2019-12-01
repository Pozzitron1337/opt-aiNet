#ifndef CELL_H
#define CELL_H
#define _USE_MATH_DEFINES

#include "resources.h"
#include <cmath>
#include <ctime>
#include <iostream>
using namespace std;

class cell {

public:
    cell(){x1 = x2 = 0;};
    cell(double, double);
    void set(double x1, double x2)
    {
        this->x1 = x1;
        this->x2 = x2;
    }
    double get_x1() {return x1;}
    double get_x2() {return x2;}
    void out(){ printf("(%g, %g)\n", x1, x2);}
    void mutate(double, double, double, double, double);
    static double uniform_random(double, double);

private:
    double x1;
    double x2;
};

cell::cell(double x1, double x2)
{
    this->x1 = x1;
    this->x2 = x2;
}

double cell::uniform_random(double v1, double v2) {
    return rand()%(int)(v2-v1)+v1;
}

void cell::mutate(double coefficient, double x_1, double x_2, double y_1, double y_2)
{
    //TODO переглянути в яких рамках краще генерувати числа для випадкової величини
    double mutation_member = b * exp(0.5- coefficient) * exp( - uniform_random(3125, 10125)/1000) / sqrt(2*M_PI);
    int operation = rand() %2;
    switch(operation){
        case 0 : if((x1 + mutation_member) <= x_2) x1 += mutation_member;
                    else x1 -= mutation_member;
                    break;
        case 1 : if((x1 - mutation_member) >= x_1) x1 -= mutation_member;
                    else x1 += mutation_member;
                    break;
    }

    mutation_member = b * exp(0.5 - coefficient) * exp( - uniform_random(3125, 10125)/1000) / sqrt(2*M_PI);
    operation = rand() %2;
    switch(operation){
        case 0 : if((x2 + mutation_member) <= y_2) x2 += mutation_member;
                    else x2 -= mutation_member;
                    break;
        case 1 : if((x2 - mutation_member) >= y_1) x2 -= mutation_member;
                    else x2 += mutation_member;
                    break;
    }
}

#endif