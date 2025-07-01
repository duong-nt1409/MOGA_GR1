#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N_DIM 2
#define POP_SIZE 150
#define GENERATIONS 100
#define RATE 0.4
#define MIN -4
#define MAX 4
#define RADIUS 0.01

double x[500][N_DIM]; 

double f1(double *x) {
    double sum = 0;
    for (int i = 0; i < N_DIM; i++)
        sum += pow(x[i] - 1.0 / sqrt(N_DIM), 2);
    return 1.0 - exp(-sum);
}

double f2(double *x) {
    double sum = 0;
    for (int i = 0; i < N_DIM; i++)
        sum += pow(x[i] + 1.0 / sqrt(N_DIM), 2);
    return 1.0 - exp(-sum);
}

int Dominance(double *a, double *b) {
    double f1a = f1(a), f2a = f2(a);
    double f1b = f1(b), f2b = f2(b);
    return ((f1a <= f1b && f2a <= f2b) && (f1a < f1b || f2a < f2b));
}

double euclidean(double f1a, double f2a, double f1b, double f2b) {
    return sqrt((f1a - f1b)*(f1a - f1b) + (f2a - f2b)*(f2a - f2b));
}

double rand_double(double min, double max) {
    return min + (max - min) * ((double)rand() / RAND_MAX);
}

int main() {
    srand(time(NULL));
    int i, j, h, p1, p2, t;
    double ap, SUMR, t1, t2, r, s[500], p[500], y[500];

    for (i = 0; i < POP_SIZE; i++)
        for (j = 0; j < N_DIM; j++)
            x[i][j] = rand_double(MIN, MAX);

    SUMR = (MAX - MIN) / 50.0;
    r = RATE;

    for (h = 0; h < GENERATIONS; h++) {
        for (i = 0; i < POP_SIZE; i++) {
            t = POP_SIZE + i;
            p1 = rand() % POP_SIZE;
            p2 = rand() % POP_SIZE;
            ap = rand_double(0, 1);
            for (j = 0; j < N_DIM; j++) {
                x[t][j] = x[p1][j] * ap + x[p2][j] * (1 - ap);
                if (rand_double(0, 1) <= r)
                    x[t][j] += rand_double(-SUMR, SUMR);
                if (x[t][j] > MAX) x[t][j] = MAX;
                if (x[t][j] < MIN) x[t][j] = MIN;
            }
        }

        for (i = 0; i < POP_SIZE * 2; i++) { y[i] = 0; s[i] = 0; }

        for (i = 0; i < POP_SIZE * 2; i++) {
            for (j = 0; j < POP_SIZE * 2; j++) {
                if (Dominance(x[j], x[i])) y[i]++;
                if (euclidean(f1(x[i]), f2(x[i]), f1(x[j]), f2(x[j])) <= RADIUS)
                    s[i]++;
            }
            p[i] = 1.0 / (y[i] + 1) + 1.0 / (s[i] + 1);
        }

        for (i = 0; i < POP_SIZE * 2; i++) {
            for (j = i + 1; j < POP_SIZE * 2; j++) {
                if (p[i] < p[j]) {
                    double temp_p = p[i]; p[i] = p[j]; p[j] = temp_p;
                    double temp_y[2];
                    for (int k = 0; k < N_DIM; k++) {
                        temp_y[k] = x[i][k];
                        x[i][k] = x[j][k];
                        x[j][k] = temp_y[k];
                    }
                }
            }
        }
    }

    for (i = 0; i < POP_SIZE; i++) {
        printf("%f %f\n", f1(x[i]), f2(x[i]));
    }

    return 0;
}
