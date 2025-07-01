#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define POP_SIZE 100
#define GENERATIONS 50
#define RATE 0.4
#define MIN -5
#define MAX 10
#define RADIUS 0.01

double x[500],y[500];

double f1(double x)
{
    return x*x;
}

double f2(double x)
{
    return (x-2)*(x-2);
}

int Dominance(double x1, double x2)
{
    if ((f1(x1)<=f1(x2) && f2(x1)<=f2(x2)) && (f1(x1)<f1(x2) || f2(x1)<f2(x2)))
        return 1;
    return 0;
}

double euclidean(double a1, double a2, double b1, double b2) {
    return sqrt((a1 - b1)*(a1 - b1) + (a2 - b2)*(a2 - b2));
}

double rand_double(double min, double max) {
    return min + (max - min) * ((double)rand() / RAND_MAX);
}

int main()
{
    srand(time(NULL));
    int i,j,h,p1,p2,t;
    double ap,SUMR,t1,t2,r,s[500],p[500];
    for (i=0;i<POP_SIZE;i++)
        {
            x[i]=rand_double(MIN,MAX);
        }
    SUMR=(MAX-MIN)/30.0;
    r=RATE;
    for (h=0;h<GENERATIONS;h++){ 
        for (i=0;i<POP_SIZE;i++)
            {
                t=POP_SIZE+i;
                p1=rand()%POP_SIZE;
                p2=rand()%POP_SIZE;
                ap=rand_double(0,1);
                x[t]=x[p1]*ap+x[p2]*(1-ap); 
                ap=rand_double(0,1);
                r=(GENERATIONS*2.0-h)/(GENERATIONS*2.0)*r;
                if (ap<=r) x[t]=x[t]+rand_double(-SUMR,SUMR);
                if (x[t]>MAX) x[t]=MAX;
                if (x[t]<MIN) x[t]=MIN;
             }
        for (i = 0; i < POP_SIZE * 2; i++) {y[i] = 0;s[i]=0;}    
        for (i=0;i<POP_SIZE*2;i++)
            {
                for (j=0;j<POP_SIZE*2;j++) {
                    if (Dominance(x[j],x[i])) y[i]++;
                    if (euclidean(f1(x[i]),f2(x[i]),f1(x[j]),f2(x[j]))<=RADIUS) s[i]++;}
                p[i]=1.0/(y[i]+1)+1.0/s[i];
            }
        for (i=0;i<POP_SIZE*2;i++)
            for (j=i+1;j<POP_SIZE*2;j++)
                if (p[i]<p[j])
                        {
                            t1=p[i];t2=x[i];
                            p[i]=p[j];x[i]=x[j];
                            p[j]=t1;x[j]=t2;
                        }
        }
        for (i=0;i<POP_SIZE;i++) printf("%f %f\n",f1(x[i]),f2(x[i]));
}