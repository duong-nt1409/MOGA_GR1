/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : MOGA trên STM32F429 - phiên bản 2
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* UART handle */
UART_HandleTypeDef huart1;

/* MOGA defines */
#define POP_SIZE 100
#define GENERATIONS 20
#define RATE 0.4
#define MIN -5
#define MAX 10
#define RADIUS 0.01

/* Global variables */
double x[500], y[500], s[500], p[500];

/* Function declarations */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);

double f1(double x) {
    if (x <= 1)
        return -x;
    else if (x <= 3)
        return x - 2;
    else if (x <= 4)
        return 4 - x;
    else
        return x - 4;
}

double f2(double x) {
    return (x - 5) * (x - 5);
}

int Dominance(double x1, double x2) {
    if ((f1(x1) <= f1(x2) && f2(x1) <= f2(x2)) &&
        (f1(x1) < f1(x2) || f2(x1) < f2(x2)))
        return 1;
    return 0;
}

double euclidean(double a1, double a2, double b1, double b2) {
    return sqrt((a1 - b1)*(a1 - b1) + (a2 - b2)*(a2 - b2));
}

double rand_double(double min, double max) {
    return min + (max - min) * ((double)rand() / RAND_MAX);
}

/* Entry point */
int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART1_UART_Init();

  printf("Khoi dong MOGA v2!\r\n");

  srand(HAL_GetTick());  // dùng thời gian hệ thống STM32 để tạo seed

  int i, j, h, p1, p2, t;
  double ap, SUMR, t1, t2, r;

  for (i = 0; i < POP_SIZE; i++) {
    x[i] = rand_double(MIN, MAX);
  }

  SUMR = (MAX - MIN) / 30.0;
  r = RATE;

  for (h = 0; h < GENERATIONS; h++) {
    for (i = 0; i < POP_SIZE; i++) {
      t = POP_SIZE + i;
      p1 = rand() % POP_SIZE;
      p2 = rand() % POP_SIZE;
      ap = rand_double(0, 1);
      x[t] = x[p1] * ap + x[p2] * (1 - ap);
      ap = rand_double(0, 1);
      r = (GENERATIONS * 2.0 - h) / (GENERATIONS * 2.0) * r;
      if (ap <= r) x[t] += rand_double(-SUMR, SUMR);
      if (x[t] > MAX) x[t] = MAX;
      if (x[t] < MIN) x[t] = MIN;
    }

    for (i = 0; i < POP_SIZE * 2; i++) {
      y[i] = 0;
      s[i] = 0;
    }

    for (i = 0; i < POP_SIZE * 2; i++) {
      for (j = 0; j < POP_SIZE * 2; j++) {
        if (Dominance(x[j], x[i])) y[i]++;
        if (euclidean(f1(x[i]), f2(x[i]), f1(x[j]), f2(x[j])) <= RADIUS)
          s[i]++;
      }
      p[i] = 1.0 / (y[i] + 1) + 1.0 / s[i];
    }

    for (i = 0; i < POP_SIZE * 2; i++) {
      for (j = i + 1; j < POP_SIZE * 2; j++) {
        if (p[i] < p[j]) {
          t1 = p[i]; t2 = x[i];
          p[i] = p[j]; x[i] = x[j];
          p[j] = t1; x[j] = t2;
        }
      }
    }

    printf("The he %d:\r\n", h + 1);
    for (i = 0; i < POP_SIZE; i++) {
      printf("%f %f\r\n", f1(x[i]), f2(x[i]));
    }

    HAL_Delay(400);
  }

  printf("Hoan thanh %d the he.\r\n", GENERATIONS);

  while (1) {
    HAL_Delay(1000);
  }
}

/* Redirect printf to UART1 */
int __io_putchar(int ch)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  return ch;
}

/* Clock Configuration */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 90;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) Error_Handler();

  if (HAL_PWREx_EnableOverDrive() != HAL_OK) Error_Handler();

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|
                                RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    Error_Handler();
}

/* UART1 */
static void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;

  if (HAL_UART_Init(&huart1) != HAL_OK)
    Error_Handler();
}

/* GPIO */
static void MX_GPIO_Init(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
}

/* Error Handler */
void Error_Handler(void)
{
  __disable_irq();
  while (1) {
    printf("Loi he thong!\r\n");
    HAL_Delay(1000);
  }
}



















































































































































































































