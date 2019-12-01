#include "AiNet.h"
#include "cell.h"
extern "C"{
    AiNet* AiNet_new(double x1,double x2,double y1,double y2){
     return new AiNet(x1,x2,y1,y2);
    }
    cell* AiNet_max(AiNet* aiNet){ return aiNet->max();}
    cell* AiNet_min(AiNet* aiNet){ return aiNet->min();}

}