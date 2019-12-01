#ifndef DEF_H
//цей файл є конфігураційним.
#define DEF_H

#include <cmath>

#define K   500     //мах к-ть ітерацій
#define N   20      //початкова кількість клітин 
#define NC  10      //к-ть клонів для кожної клітини
#define d   0.4     //коефіцієнт клітин з найгіршим приспособленням які будуть замінені
#define b   100     //параметр операції клонування
#define r    0.2    //параметр мутації
#define unfit_max -9223372036854775808.0    //мінімальне значення для максимуму
#define unfit_min 9223372036854775808.0     //максимальне значення для мінумуму
#define delta 0.00001   //для lim delta->0 (f(x+delta)-f(x))/delta

#define FUNCTION(x, y) (pow(x, 2.0) + pow(y,2.0) - 10*( cos(2*M_PI*x) + cos(2*M_PI*y))) //функція Растригіна як приклад
#endif