#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define POP_SIZE 100
#define GENERATIONS 50
#define RATE 0.4
#define MIN -5
#define MAX 5
#define RADIUS 0.01

double x[500][3], y[500], s[500], p[500];

double f1(double xi[3]) {
    double sum = 0.0;
    for (int i = 0; i < 2; i++) {
        double a = xi[i]*xi[i] + xi[i+1]*xi[i+1];
        sum += -10.0 * exp(-0.2 * sqrt(a));
    }
    return sum;
}

double f2(double xi[3]) {
    double sum = 0.0;
    for (int i = 0; i < 3; i++) {
        sum += pow(fabs(xi[i]), 0.8) + 5.0 * sin(pow(xi[i], 3));
    }
    return sum;
}

int Dominance(double a[3], double b[3]) {
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
        for (j = 0; j < 3; j++)
            x[i][j] = rand_double(MIN, MAX);

    for (h = 0; h < GENERATIONS; h++) {
        for (i = 0; i < POP_SIZE; i++) {
            t = POP_SIZE + i;
            p1 = rand() % POP_SIZE;
            p2 = rand() % POP_SIZE;
            ap = rand_double(0, 1);
            for (j = 0; j < 3; j++) {
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
                    double temp_x[3];
                    for (int k = 0; k < 3; k++) {
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
