#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define DIM 30
#define POP_SIZE 100
#define GENERATIONS 50
#define RATE 0.4
#define MIN 0
#define MAX 1
#define RADIUS 0.01

double x[500][DIM], y[500], s[500], p[500];

double f1(double xi[DIM]) {
    return xi[0];
}

double g(double xi[DIM]) {
    double sum = 0.0;
    for (int i = 1; i < DIM; i++) {
        sum += xi[i];
    }
    return 1.0 + 9.0 * sum / (DIM - 1);
}

double h(double f1x, double gx) {
    return 1.0 - sqrt(f1x / gx) - (f1x / gx) * sin(10.0 * M_PI * f1x);
}

double f2(double xi[DIM]) {
    double f1x = f1(xi);
    double gx = g(xi);
    return gx * h(f1x, gx);
}

int Dominance(double a[DIM], double b[DIM]) {
    double fa1 = f1(a), fa2 = f2(a);
    double fb1 = f1(b), fb2 = f2(b);
    return (fa1 <= fb1 && fa2 <= fb2 && (fa1 < fb1 || fa2 < fb2));
}

double euclidean(double a1, double a2, double b1, double b2) {
    return sqrt((a1 - b1)*(a1 - b1) + (a2 - b2)*(a2 - b2));
}

double rand_double(double min, double max) {
    return min + (max - min) * ((double)rand() / RAND_MAX);
}

int main() {
    srand(time(NULL));
    int i, j, h, p1, p2, t;
    double ap, SUMR = (MAX - MIN) / 30.0;
    double r = RATE;

    for (i = 0; i < POP_SIZE; i++)
        for (j = 0; j < DIM; j++)
            x[i][j] = rand_double(MIN, MAX);

    for (h = 0; h < GENERATIONS; h++) {
        for (i = 0; i < POP_SIZE; i++) {
            t = POP_SIZE + i;
            p1 = rand() % POP_SIZE;
            p2 = rand() % POP_SIZE;
            ap = rand_double(0, 1);
            for (j = 0; j < DIM; j++) {
                x[t][j] = x[p1][j]*ap + x[p2][j]*(1-ap);
                if (rand_double(0,1) <= r)
                    x[t][j] += rand_double(-SUMR, SUMR);
                if (x[t][j] > MAX) x[t][j] = MAX;
                if (x[t][j] < MIN) x[t][j] = MIN;
            }
        }

        for (i = 0; i < POP_SIZE * 2; i++) {
            y[i] = 0; s[i] = 0;
        }
        for (i = 0; i < POP_SIZE * 2; i++) {
            for (j = 0; j < POP_SIZE * 2; j++) {
                if (Dominance(x[j], x[i])) y[i]++;
                if (euclidean(f1(x[i]), f2(x[i]), f1(x[j]), f2(x[j])) <= RADIUS) s[i]++;
            }
            p[i] = 1.0 / (y[i] + 1) + 1.0 / (s[i] + 1);
        }

        for (i = 0; i < POP_SIZE * 2; i++) {
            for (j = i+1; j < POP_SIZE * 2; j++) {
                if (p[i] < p[j]) {
                    double temp_p = p[i]; p[i] = p[j]; p[j] = temp_p;
                    double temp_x[DIM];
                    for (int k = 0; k < DIM; k++) {
                        temp_x[k] = x[i][k];
                        x[i][k] = x[j][k];
                        x[j][k] = temp_x[k];
                    }
                }
            }
        }
    }


    for (i = 0; i < POP_SIZE; i++)
        printf("%f %f\n", f1(x[i]), f2(x[i]));

    return 0;
}
