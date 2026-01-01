#include "main.h"

static void SystemClock_Config(void);

int main(void){

  // Inicia a Hal e Configura os clocks
  HAL_Init();                                             
  SystemClock_Config();                                   

  // Habilita o Clock de GPIOA
  __HAL_RCC_GPIOA_CLK_ENABLE();                           

  GPIO_InitTypeDef GPIO_InitStructPA3 = {0};              // Define a Struct do pino com as configs
  GPIO_InitStructPA3.Pin = GPIO_PIN_3;                    // Número do Pino
  GPIO_InitStructPA3.Mode = GPIO_MODE_INPUT;              // Tipo da saída PushPull
  GPIO_InitStructPA3.Pull = GPIO_NOPULL;                  // Desativa Resistor Interno
  GPIO_InitStructPA3.Speed = GPIO_SPEED_FAST;             


  GPIO_InitTypeDef GPIO_InitStructPA2 = {0};
  GPIO_InitStructPA2.Pin = GPIO_PIN_2;
  GPIO_InitStructPA2.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructPA2.Pull = GPIO_NOPULL;
  GPIO_InitStructPA2.Speed = GPIO_SPEED_FAST;


  GPIO_InitTypeDef GPIO_InitStructPA4 = {0};              
  GPIO_InitStructPA4.Pin = GPIO_PIN_4;                    
  GPIO_InitStructPA4.Mode = GPIO_MODE_INPUT;              
  GPIO_InitStructPA4.Pull = GPIO_NOPULL;                  
  GPIO_InitStructPA4.Speed = GPIO_SPEED_FAST;             


  GPIO_InitTypeDef GPIO_InitStructPA5 = {0};
  GPIO_InitStructPA5.Pin = GPIO_PIN_5;
  GPIO_InitStructPA5.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructPA5.Pull = GPIO_NOPULL;
  GPIO_InitStructPA5.Speed = GPIO_SPEED_FAST;

  // Ativa os Pinos
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructPA3);
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructPA2);
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructPA4);
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructPA5);

  while (1){

    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3)){
    	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
    }else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4)){
    	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
    }else {
    	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
    	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

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

// https://chatgpt.com/share/6954b373-d268-8008-a737-1e3ec2b70576 - Como ativar IntelliSense STM32 VSCode
