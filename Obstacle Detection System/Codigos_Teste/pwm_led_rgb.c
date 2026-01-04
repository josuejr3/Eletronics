#include "main.h"

static void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM5_Init(void);

TIM_HandleTypeDef hTim5;

int main(void){

  // Inicia a Hal e Configura os clocks
  HAL_Init();                                             
  SystemClock_Config();    
  
  // Configurações de Pinos
  MX_GPIO_Init();
  MX_TIM5_Init();

  // Começa a geração de sinal PWM
  HAL_TIM_PWM_Start(&hTim5, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&hTim5, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&hTim5, TIM_CHANNEL_1);


  while (1){

	  for(uint8_t i = 0; i <= 99; i++){
	      __HAL_TIM_SET_COMPARE(&hTim5, TIM_CHANNEL_1, i);         // Red aumenta
	      __HAL_TIM_SET_COMPARE(&hTim5, TIM_CHANNEL_2, 99 - i);    // Green diminui
	      __HAL_TIM_SET_COMPARE(&hTim5, TIM_CHANNEL_3, (i/2));     // Blue aumenta mais devagar
	      HAL_Delay(20);                                           // fade suave
	  }

  }

}

void Error_Handler(void)
{
    __disable_irq();   // Desliga interrupções

    while (1)
    {}
}

static void SystemClock_Config(void){

  // Evita lixo de memória nas structs
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  // Tratmento do PWR e Voltage Scaling
  __HAL_RCC_PWR_CLK_ENABLE();                                       // Habilita o Clock da Interface de Energia
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);    // Regulador de Tensão no Modo 2

  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSI;

  RCC_OscInitStruct.PLL.PLLM  = 8;                                  // << depende do cristal
  RCC_OscInitStruct.PLL.PLLN  = 168;
  RCC_OscInitStruct.PLL.PLLP  = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ  = 4;

  // Aplica configurações no oscilador
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  // Configuração dos barramentos e clock do sistema
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  // Aplica configurações do clock 
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }

}


// Função para configuração de GPIOs
static void MX_GPIO_Init(void){


  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_0 | GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM5;  

  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}


static void MX_TIM5_Init(void){

  TIM_OC_InitTypeDef configTim5CH3 = {0};
  TIM_OC_InitTypeDef configTim5CH2 = {0};
  TIM_OC_InitTypeDef configTim5CH1 = {0};

  __HAL_RCC_TIM5_CLK_ENABLE();

  hTim5.Instance = TIM5;
  hTim5.Init.Prescaler = 419;
  hTim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  hTim5.Init.Period = 99;
  hTim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  hTim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

  HAL_TIM_PWM_Init(&hTim5);

  configTim5CH3.OCMode = TIM_OCMODE_PWM1;
  configTim5CH3.Pulse = 30; // 30% de duty cycle;
  configTim5CH3.OCPolarity = TIM_OCPOLARITY_HIGH;
  configTim5CH3.OCFastMode = TIM_OCFAST_DISABLE;

  configTim5CH2.OCMode = TIM_OCMODE_PWM1;
  configTim5CH2.Pulse = 20; // 30% de duty cycle;
  configTim5CH2.OCPolarity = TIM_OCPOLARITY_HIGH;
  configTim5CH2.OCFastMode = TIM_OCFAST_DISABLE;

  configTim5CH1.OCMode = TIM_OCMODE_PWM1;
  configTim5CH1.Pulse = 10; // 30% de duty cycle;
  configTim5CH1.OCPolarity = TIM_OCPOLARITY_HIGH;
  configTim5CH1.OCFastMode = TIM_OCFAST_DISABLE;

  HAL_TIM_PWM_ConfigChannel(&hTim5, &configTim5CH3, TIM_CHANNEL_3);
  HAL_TIM_PWM_ConfigChannel(&hTim5, &configTim5CH1, TIM_CHANNEL_1);
  HAL_TIM_PWM_ConfigChannel(&hTim5, &configTim5CH2, TIM_CHANNEL_2);

}

// https://chatgpt.com/share/6954b373-d268-8008-a737-1e3ec2b70576 - Como ativar IntelliSense STM32 VSCode
