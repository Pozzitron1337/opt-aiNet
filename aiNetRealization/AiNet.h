#ifndef AINET_H
#define AINET_H
#define _USE_MATH_DEFINES

#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include "cell.h"

//Artificial immune Network
class AiNet{
public:
    AiNet(double, double, double, double);
    ~AiNet();
    cell* max();
    cell* min();
    double value(int i){
        return function(population[i].get_x1(), population[i].get_x2());
    }

    //Derivative
    void der(int i){
        cell x = population[i];
        cout <<"dx ";
        cout << (function(x.get_x1() + delta, x.get_x2()) - function(x.get_x1(), x.get_x2()))/delta<<endl;
        cout <<"dy ";
        cout << (function(x.get_x1(), x.get_x2() + delta) - function(x.get_x1(), x.get_x2()))/delta<<endl;
    }
private:
    //к-ть клітин в популяції
    int num_population = N;

    //к-ть клонів для кожної клітини
    int num_clone = NC;

    //область пошуку.Від х1 до х2 та від у1 до у2
    double x1;
    double x2;
    double y1;
    double y2;

    //середнє значення популяції для максимума
    double average_population_max = unfit_max;
    double copy_average_population_max = unfit_max;

    //середнє значення популяції для мінімума
    double average_population_min=unfit_min;
    double copy_average_population_min=unfit_min;

    //значення фітнес функції в population[]
    vector<double> fit_value;

    //значення фітнес функції в clone[]
    vector<std::vector<double>> fit_value_clone;

    //нормоване значення фітнес функції в population[]
    vector<double> n_fit_value;

    //записати в файл точки максимуму
    void writeToFileCurrentPopulation_max(cell* population){
        ofstream out;
        out.open("./maxValues.txt",std::ios::app);
        for(int i=8;i<N-1;i++){
            out<<"("<<population[i].get_x1()<<",";
            out<<population[i].get_x2()<<"), ";
        }
        out<<"("<<population[N-1].get_x1()<<",";
        out<<population[N-1].get_x2()<<");";
        out<<endl;
        out.close();
    }

    //записати в файл точки мінімуму
    void writeToFileCurrentPopulation_min(cell* population){
        ofstream out;
        out.open("./minValues.txt",std::ios::app);
        for(int i=8;i<N-1;i++){
            out<<"("<<population[i].get_x1()<<",";
            out<<population[i].get_x2()<<"), ";
        }
        out<<"("<<population[N-1].get_x1()<<",";
        out<<population[N-1].get_x2()<<");";
        out<<endl;
        out.close();
    }

    //ініціализація параметрів для пошуку максимуму
    void initialize_max();

    //ініціализація параметрів для пошуку мінімуму
    void initialize_min();

    //ФІТНЕС ФУНКЦІЯ
    double function (double, double);
    
    //сторення клонів для даної популяції
    void clone();
    
    //отримати вектор значеннь фітнес функції для клітин даної популяції
    void popVal();
    
    //отримати нормований вектор значень фітнес функції для даної популяції
    void normPopVal();
    
    //отримати матрицю значень фітнес функції для клонів даної популяції
    void cloneVal();

    //мутація клонів
    void mutation_clone();

    //сортуємо популяцію в порядку зростання значення фітнес функції
    void sort_max();

    //сортуємо популяцію в порядку спадання значення фітнес функції
    void sort_min();

    //вибір кращих представників серед клонів і популяції для максимумів
    void next_generate_max();

    //вибір кращих представників серед клонів і популяції для мінімумів
    void next_generate_min();

    //зробити копію терперішньої популяції в максимумі
    void copy_max();

    //зробити копію терперішньої популяції в мінімумі
    void copy_min();

    //average_max utility of the population
    //середня корисність популяції у максимізації
    void average_max();

    //середня корисність популяції у максимізації
    void average_min();

    //відновлюємо попереднє покоління в пошуці максимумів
    void restart_max();

    //відновлюємо попереднє покоління в пошуці мінімумів
    void restart_min();

    //стиснення популяції для максимума
    void contraction_max();

    //стиснення популяції для мінімума
    void contraction_min();

    //повертає евклідову норму між вказаними елементами
    double norm(int, int);

    //генерація рандомних значень популяції для максимума
    void random_generate_max();

    //генерація рандомних значень популяції для мінімума
    void random_generate_min();

    //вказівник на першого члена популяції
    cell *population;
    //копія теперішньої популяції
    cell *copy_population;
    //вказівник на перший індекс матриці клонів(двовимірна матриця)
    cell **p_clone;

};

//в конструктор подаєм значчення проміжків на яких шукаємо max
AiNet::AiNet( double x1, double x2, double y1, double y2)
{
    this->x1 = x1;
    this->x2 = x2;
    this->y1 = y1;
    this->y2 = y2;
}

AiNet::~AiNet()
{
    delete population;
    delete copy_population;
    for (int i = 0; i < num_population; i++)
        delete p_clone[i];

}

void AiNet::initialize_max()
{
    //ініціалізація масивів
    fit_value.resize(num_population);
    n_fit_value.resize(num_population);
    fit_value_clone.resize(num_population);
    for (int i = 0; i < num_population; i++)
        fit_value_clone[i].resize(num_clone);

    population =  new cell[num_population];
    copy_population = new cell[num_population];

    //створення нового покоління
    srand(time(NULL));
    for (int i = 0; i < num_population; i++)
    {
        this->population[i].set(cell::uniform_random(x1, x2), cell::uniform_random(y1, y2));
    }
    this->copy_max();
    this->average_max();
    copy_average_population_max = average_population_max;

    //ініціалізація двовимірного масиву для клонів
    this->p_clone = new cell*[num_population];
    for (int i = 0; i < num_population; i++)
        p_clone[i] = new cell[num_clone];
}

void AiNet::initialize_min()
{
    //ініціалізація масивів
    fit_value.resize(num_population);
    n_fit_value.resize(num_population);
    fit_value_clone.resize(num_population);
    for (int i = 0; i < num_population; i++)
        fit_value_clone[i].resize(num_clone);

    population =  new cell[num_population];
    copy_population = new cell[num_population];

    //створення нового покоління
    srand(time(NULL));
    for (int i = 0; i < num_population; i++)
    {
        this->population[i].set(cell::uniform_random(x1, x2), cell::uniform_random(y1, y2));
    }
    this->copy_min();
    this->average_min();
    copy_average_population_min = average_population_min;

    //ініціалізація двовимірного масиву для клонів
    this->p_clone = new cell*[num_population];
    for (int i = 0; i < num_population; i++)
        p_clone[i] = new cell[num_clone];

}

double AiNet::function(double x, double y)
{
    return FUNCTION(x,y);
}

void AiNet::clone()
{
    for (int j = 0; j < num_population; j++)
    {
        for (int i = 0; i < num_clone; i++)
        {
            p_clone[j][i].set(population[j].get_x1(), population[j].get_x2());
        }
    }
}

void AiNet::popVal()
{
    for (int i = 0; i < num_population; i++)
        this->fit_value[i] = function(population[i].get_x1(), population[i].get_x2());
}

void AiNet::normPopVal()
{
    //шукаємо коефіцієнт нормування
    double norm=0;
    for (int i = 0; i < num_population; i++)
        norm += pow(fit_value[i], (double)2);
    norm = sqrt(norm);

    //нормуємо значення в точці
    for (int i = 0; i < num_population; i++)
        n_fit_value[i] = fit_value[i]/norm;

    double min = n_fit_value[0];
    double max = n_fit_value[0];
    for (int i = 1; i < num_population; i++){
        if (n_fit_value[i] < min) min = n_fit_value[i];
        if (n_fit_value[i] > max) max = n_fit_value[i];
    };

   //приводимо до проміжку [0;1]
    if(min < 0)
    {
        max += fabs(min);
        for (int i = 0; i < num_population; i++)
        {
            n_fit_value[i] += fabs(min);
            n_fit_value[i] /= max;
        }
    }
}

void AiNet::cloneVal()
{
    for (int i = 0; i < num_population; i++)
    {
        for(int j = 0; j < num_clone; j++)
        {
            fit_value_clone[i][j] = function(p_clone[i][j].get_x1(), p_clone[i][j].get_x2());
        }
    }
}


void AiNet::mutation_clone()
{
    for (int i = 0; i < num_population; i++)
    {
        for (int j = 0; j < num_clone; j++)
            p_clone[i][j].mutate(n_fit_value[i], this->x1, this->x2, this->y1, this->y2);
    }
}


void AiNet::next_generate_max()
{
    this->cloneVal();
    //шукаємо представників з найбільшим значенням фітнес функції
    for (int i = 0; i < num_population; i++)
    {
        for (int j = 0; j < num_clone; j++)
        {
            if(fit_value[i] < fit_value_clone[i][j])
            {
                fit_value[i] = fit_value_clone[i][j];
                population[i] = p_clone[i][j];
            }
        }
    }
}

void AiNet::next_generate_min()
{
    this->cloneVal();
    //шукаємо представників з найменшим значенням фітнес функції
    for (int i = 0; i < num_population; i++)
    {
        for (int j = 0; j < num_clone; j++)
        {
            if(fit_value[i] > fit_value_clone[i][j])
            {
                fit_value[i] = fit_value_clone[i][j];
                population[i] = p_clone[i][j];
            }
        }
    }
}

void AiNet::sort_max()
{
    for (int i = 0; i < num_population; i++)
        for (int j =  i+1; j < num_population; j++)
            if( fit_value[j] < fit_value[i]){
                swap(fit_value[j], fit_value[i]);
                swap(population[i], population[j]);
                swap(p_clone[i], p_clone[j]);
            }
}

void AiNet::sort_min() {
    for (int i = 0; i < num_population; i++)
        for (int j =  i+1; j < num_population; j++)
            if( fit_value[j] > fit_value[i]){
                swap(fit_value[j], fit_value[i]);
                swap(population[i], population[j]);
                swap(p_clone[i], p_clone[j]);
            }
}


void AiNet::copy_max()
{
    for(int i = 0; i < num_population; i++)
        copy_population[i] = population[i];
    this->average_max();
    copy_average_population_max = average_population_max;

}

void AiNet::copy_min(){
    for(int i = 0; i < num_population; i++)
        copy_population[i] = population[i];
    this->average_min();
    copy_average_population_min = average_population_min;
}

void AiNet::average_max()
{
    this->popVal();
    average_population_max = 0;
    for (int i = 0 ; i < num_population; i++)
        average_population_max += fit_value[i];
    average_population_max /= num_population;
}

void AiNet::average_min()
{
    this->popVal();
    average_population_min = 0;
    for (int i = 0 ; i < num_population; i++)
        average_population_min += fit_value[i];
    average_population_min /= num_population;
}

void AiNet::restart_max()
{
    //відновлюємо попереднє покоління
    for(int i = 0; i < num_population; i++)
        population[i] = copy_population[i];
    this->average_max();

}

void AiNet::restart_min()
{
    for(int i = 0; i < num_population; i++)
        population[i] = copy_population[i];
    this->average_min();
}

double AiNet::norm(int i, int j)
{
    return sqrt(pow(population[i].get_x1() - population[j].get_x1(), 2.0) + pow(population[i].get_x2() - population[j].get_x2(), 2.0));
}

void AiNet::contraction_max()
{
    for (int i = num_population -1; i >= 0; i--)
    {
        if (fit_value[i] == unfit_max) continue;
        for (int j = i - 1; j >= 0; j--)
        {
            if (fit_value[j] == unfit_max) continue;
            if ( this->norm(i,j) < r)
            {
                if ( fit_value[i] >= fit_value[j]) fit_value[j] = unfit_max;
                else 
                {
                    fit_value[i] = unfit_max;
                    break;
                }
            }
        }
    }
}

void AiNet::contraction_min()
{
    for (int i = num_population -1; i >= 0; i--)
    {
        if (fit_value[i] == unfit_min) continue;
        for (int j = i - 1; j >= 0; j--)
        {
            if (fit_value[j] == unfit_min) continue;
            if ( this->norm(i,j) < r)
            {
                if ( fit_value[i] <= fit_value[j]) fit_value[j] = unfit_min;
                else
                {
                    fit_value[i] = unfit_min;
                    break;
                }
            }
        }
    }
}

void AiNet::random_generate_max()
{
    for (int i = 0; i < int(d*num_population); i++){
        population[i].set(cell::uniform_random(x1, x2), cell::uniform_random(y1, y2));
    }
    for(int i = int(d*num_population); i < num_population; i++){
        if(fit_value[i] == unfit_max) {
            population[i].set(cell::uniform_random(x1, x2), cell::uniform_random(y1, y2));
        }
    }
}

void AiNet::random_generate_min()
{
    for (int i = 0; i < int(d*num_population); i++){
        population[i].set(cell::uniform_random(x1, x2), cell::uniform_random(y1, y2));
    }
    for(int i = int(d*num_population); i < num_population; i++){
        if(fit_value[i] == unfit_min) {
            population[i].set(cell::uniform_random(x1, x2),cell::uniform_random(y1, y2));
        }
    }
}

cell* AiNet::max()
{
    this->initialize_max();
    for (int i = 0; i < K; i++)
    {
        this->popVal();
        this->normPopVal();
        this->clone();
        this->mutation_clone();
        this->cloneVal();
        this->next_generate_max();
        this->average_max();
        this->sort_max();
        if(average_population_max < copy_average_population_max)
        {
            this->restart_max();
            i--;
            continue;
        }
        else
        {
            this->sort_max();
            this->contraction_max();
            this->sort_max();
            this->random_generate_max();
            this->copy_max();
        }
        this->writeToFileCurrentPopulation_max(population);
    }
    this->popVal();
    this->sort_max();
    for (int i = num_population - 1; i >7; i--){
        population[i].out();
        cout<<endl;
    }
    return population;
}

cell* AiNet::min()
{
    this->initialize_min();
    for (int i = 0; i < K; i++)
    {
        this->popVal();
        this->normPopVal();
        this->clone();
        this->mutation_clone();
        this->cloneVal();
        this->next_generate_min();
        this->average_min();
        this->sort_min();
        if(average_population_min > copy_average_population_min)
        {
            this->restart_min();
            i--;
        }
        else
        {
            this->sort_min();
            this->contraction_min();
            this->sort_min();
            this->random_generate_min();
            this->copy_min();
        }
        this->writeToFileCurrentPopulation_min(population);

    }
    this->popVal();
    this->sort_min();
    for (int i = num_population - 1; i >7; i--){
        population[i].out();
        cout<<endl;
    }
    return population;
}

#endif