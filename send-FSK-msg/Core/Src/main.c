/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
DAC_HandleTypeDef hdac;
DMA_HandleTypeDef hdma_dac1;

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

#define NS  128

uint16_t Wave_LUT[NS] = {
		1300, 1350, 1401, 1451, 1501, 1550, 1599, 1647, 1694, 1740, 1786, 1830,
		1872, 1914, 1954, 1992, 2028, 2063, 2096, 2126, 2156, 2182, 2206, 2229,
		2249, 2267, 2282, 2296, 2306, 2314, 2320, 2323, 2324, 2322, 2317, 2310,
		2301, 2289, 2275, 2258, 2240, 2218, 2194, 2169, 2141, 2112, 2080, 2046,
		2010, 1973, 1934, 1894, 1852, 1808, 1764, 1718, 1671, 1624, 1575, 1526,
		1476, 1426, 1376, 1325, 1274, 1224, 1174, 1124, 1074, 1024, 976, 928,
		882, 836, 792, 748, 706, 666, 626, 590, 554, 520, 488, 458, 430, 405,
		382, 360, 341, 324, 310, 298, 289, 282, 278, 276, 276, 280, 286, 294,
		304, 317, 332, 350, 370, 393, 418, 444, 473, 504, 536, 572, 608, 646,
		686, 727, 770, 814, 859, 905, 952, 1000, 1049, 1098, 1148, 1198, 1249, 1300
};

// generated using "msk144code" binary from wsjtx repo
// represents "CQ KK7LHY CN87"


uint16_t MSK_CQ_LUT[144] = {
		1266, 1266, 733, 733, 733, 733, 1266, 1266, 733, 1266, 733, 1266, 733, 1266,
		733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266,
		733, 1266, 733, 1266, 733, 733, 1266, 1266, 1266, 733, 733, 733, 1266, 733,
		733, 1266, 733, 1266, 1266, 1266, 733, 733, 1266, 733, 733, 733, 1266, 733,
		1266, 1266, 733, 733, 733, 733, 1266, 733, 1266, 1266, 1266, 733, 1266, 733,
		1266, 733, 733, 1266, 733, 1266, 1266, 733, 733, 733, 733, 733, 1266, 1266,
		733, 1266, 1266, 733, 733, 733, 733, 733, 733, 1266, 1266, 1266, 733, 733,
		1266, 733, 733, 1266, 1266, 733, 733, 1266, 733, 733, 733, 1266, 1266, 733,
		733, 1266, 733, 733, 733, 1266, 733, 733, 1266, 1266, 733, 733, 1266, 1266,
		733, 1266, 1266, 1266, 1266, 733, 1266, 733, 733, 1266, 733, 733, 1266, 1266,
		733, 1266, 1266, 733
};
// 80mhz system clock / 40,000 clock div = 2,000 trigger freq
// 2,000 trigger freq / 2 samples per wavecycle = 1,000 repeats per second
// 1,000 repeats * 2 tones per repeat = 2000 baud

uint16_t MSK_CQ_INV[144] = {
		733, 733, 1266, 1266, 1266, 1266, 733, 733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266, 1266, 733, 733, 733, 1266, 1266, 1266, 733, 1266, 1266, 733, 1266, 733, 733, 733, 1266, 1266, 733, 1266, 1266, 1266, 733, 1266, 733, 733, 1266, 1266, 1266, 1266, 733, 1266, 733, 733, 733, 1266, 733, 1266, 733, 1266, 1266, 733, 1266, 733, 733, 1266, 1266, 1266, 1266, 1266, 733, 733, 1266, 733, 733, 1266, 1266, 1266, 1266, 1266, 1266, 733, 733, 733, 1266, 1266, 733, 1266, 1266, 733, 733, 1266, 1266, 733, 1266, 1266, 1266, 733, 733, 1266, 1266, 733, 1266, 1266, 1266, 733, 1266, 1266, 733, 733, 1266, 1266, 733, 733, 1266, 733, 733, 733, 733, 1266, 733, 1266, 1266, 733, 1266, 1266, 733, 733, 1266, 733, 733, 1266
};

uint16_t MSK_QUADCQ_LUT[144] = {
		1266, 1266, 1266, 1266, 1266, 1266, 1266, 1266, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 1266, 1266, 1266, 1266, 1266, 1266, 1266, 1266, 733, 733, 733, 733, 1266, 1266, 1266, 1266, 733, 733, 733, 733, 1266, 1266, 1266, 1266, 733, 733, 733, 733, 1266, 1266, 1266, 1266, 733, 733, 733, 733, 1266, 1266, 1266, 1266, 733, 733, 733, 733, 1266, 1266, 1266, 1266, 733, 733, 733, 733, 1266, 1266, 1266, 1266, 733, 733, 733, 733, 1266, 1266, 1266, 1266, 733, 733, 733, 733, 1266, 1266, 1266, 1266, 733, 733, 733, 733, 1266, 1266, 1266, 1266, 733, 733, 733, 733, 1266, 1266, 1266, 1266, 733, 733, 733, 733, 1266, 1266, 1266, 1266, 733, 733, 733, 733, 1266, 1266, 1266, 1266, 733, 733, 733, 733, 733, 733, 733, 733, 1266, 1266, 1266, 1266, 1266, 1266, 1266, 1266
};

uint16_t MSK_FAKE_LUT[144] = {
		733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266,
		733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266,
		733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266,
		733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266,
		733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266,
		733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266,
		733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266,
		733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266,
		733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266, 733, 1266
};

uint16_t MSK_QUADFAKE_LUT[144] = {
		733, 733, 733, 733, 1266, 1266, 1266, 1266, 733, 733, 733, 733, 1266, 1266, 1266, 1266,
		733, 733, 733, 733, 1266, 1266, 1266, 1266, 733, 733, 733, 733, 1266, 1266, 1266, 1266,
		733, 733, 733, 733, 1266, 1266, 1266, 1266, 733, 733, 733, 733, 1266, 1266, 1266, 1266,
		733, 733, 733, 733, 1266, 1266, 1266, 1266, 733, 733, 733, 733, 1266, 1266, 1266, 1266,
		733, 733, 733, 733, 1266, 1266, 1266, 1266, 733, 733, 733, 733, 1266, 1266, 1266, 1266,
		733, 733, 733, 733, 1266, 1266, 1266, 1266, 733, 733, 733, 733, 1266, 1266, 1266, 1266,
		733, 733, 733, 733, 1266, 1266, 1266, 1266, 733, 733, 733, 733, 1266, 1266, 1266, 1266,
		733, 733, 733, 733, 1266, 1266, 1266, 1266, 733, 733, 733, 733, 1266, 1266, 1266, 1266,
		733, 733, 733, 733, 1266, 1266, 1266, 1266, 733, 733, 733, 733, 1266, 1266, 1266, 1266
};
// 80mhz system clock / 10,000 clock div = 8,000 trigger freq
// 8,000 trigger freq / 4 samples per symbol = 2000 baud

uint16_t MSK_NARFAKE_LUT[144] = {
		866, 1133, 866, 1133, 866, 1133, 866, 1133, 866, 1133, 866, 1133, 866, 1133, 866, 1133,
		866, 1133, 866, 1133, 866, 1133, 866, 1133, 866, 1133, 866, 1133, 866, 1133, 866, 1133,
		866, 1133, 866, 1133, 866, 1133, 866, 1133, 866, 1133, 866, 1133, 866, 1133, 866, 1133,
		866, 1133, 866, 1133, 866, 1133, 866, 1133, 866, 1133, 866, 1133, 866, 1133, 866, 1133,
		866, 1133, 866, 1133, 866, 1133, 866, 1133, 866, 1133, 866, 1133, 866, 1133, 866, 1133,
		866, 1133, 866, 1133, 866, 1133, 866, 1133, 866, 1133, 866, 1133, 866, 1133, 866, 1133,
		866, 1133, 866, 1133, 866, 1133, 866, 1133, 866, 1133, 866, 1133, 866, 1133, 866, 1133,
		866, 1133, 866, 1133, 866, 1133, 866, 1133, 866, 1133, 866, 1133, 866, 1133, 866, 1133,
		866, 1133, 866, 1133, 866, 1133, 866, 1133, 866, 1133, 866, 1133, 866, 1133, 866, 1133
};

uint16_t MSK_WIDFAKE_LUT[144] = {
		466, 1533, 466, 1533, 466, 1533, 466, 1533, 466, 1533, 466, 1533, 466, 1533, 466, 1533,
		466, 1533, 466, 1533, 466, 1533, 466, 1533, 466, 1533, 466, 1533, 466, 1533, 466, 1533,
		466, 1533, 466, 1533, 466, 1533, 466, 1533, 466, 1533, 466, 1533, 466, 1533, 466, 1533,
		466, 1533, 466, 1533, 466, 1533, 466, 1533, 466, 1533, 466, 1533, 466, 1533, 466, 1533,
		466, 1533, 466, 1533, 466, 1533, 466, 1533, 466, 1533, 466, 1533, 466, 1533, 466, 1533,
		466, 1533, 466, 1533, 466, 1533, 466, 1533, 466, 1533, 466, 1533, 466, 1533, 466, 1533,
		466, 1533, 466, 1533, 466, 1533, 466, 1533, 466, 1533, 466, 1533, 466, 1533, 466, 1533,
		466, 1533, 466, 1533, 466, 1533, 466, 1533, 466, 1533, 466, 1533, 466, 1533, 466, 1533,
		466, 1533, 466, 1533, 466, 1533, 466, 1533, 466, 1533, 466, 1533, 466, 1533, 466, 1533
};


uint16_t MSK_SMOOTH_LUT[160] = {
		733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733,
		733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733,
		740, 750, 770, 790, 820, 860, 910, 960, 1040, 1090, 1140, 1180, 1210, 1230, 1250, 1260,
		1266, 1266, 1266, 1266, 1266, 1266, 1266, 1266, 1266, 1266, 1266, 1266, 1266, 1266, 1266, 1266,
		1266, 1266, 1266, 1266, 1266, 1266, 1266, 1266, 1266, 1266, 1266, 1266, 1266, 1266, 1266, 1266,
		1266, 1266, 1266, 1266, 1266, 1266, 1266, 1266, 1266, 1266, 1266, 1266, 1266, 1266, 1266, 1266,
		1266, 1266, 1266, 1266, 1266, 1266, 1266, 1266, 1266, 1266, 1266, 1266, 1266, 1266, 1266, 1266,
		1260, 1250, 1230, 1210, 1180, 1140, 1090, 1040, 960, 910, 860, 820, 790, 770, 750, 740,
		733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733,
		733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733, 733
};
// 80mhz system clock / 500 clock div = 160,000 trigger freq
// 160,000 trigger freq / 160 samples per wavecycle = 1,000 repeats per second
// 1,000 repeats * 2 tones per repeat = 2000 baud

uint16_t MSK_SMOOTHWIDE_LUT[160] = {
		466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 480, 500, 540, 580, 640, 720, 820, 920, 1080, 1180, 1280, 1360, 1420, 1460, 1500, 1520, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1532, 1520, 1500, 1460, 1420, 1360, 1280, 1180, 1080, 920, 820, 720, 640, 580, 540, 500, 480, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466, 466
};


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_DAC_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_DAC_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  //uint8_t is_set = 0;

  HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint16_t*)MSK_QUADCQ_LUT, 144, DAC_ALIGN_12B_R);
  HAL_TIM_Base_Start(&htim2);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, is_set);
	//is_set = (is_set + 1) % 2;
	//HAL_Delay(1000);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 80;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief DAC Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC_Init(void)
{

  /* USER CODE BEGIN DAC_Init 0 */

  /* USER CODE END DAC_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC_Init 1 */

  /* USER CODE END DAC_Init 1 */

  /** DAC Initialization
  */
  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT1 config
  */
  sConfig.DAC_Trigger = DAC_TRIGGER_T2_TRGO;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC_Init 2 */

  /* USER CODE END DAC_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 10000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GreenLED_GPIO_Port, GreenLED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : GreenLED_Pin */
  GPIO_InitStruct.Pin = GreenLED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GreenLED_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
