/* USER CODE BEGIN Header */
/*******************************************************************************
 * EE 329 A2 KEYPAD INTERFACE
 *******************************************************************************
 * @file           : main.c
 * @brief          : Takes the returned value from the keypad.c and outputs it
 * 		         	 to the LEDs. Only changes the LEDs if a button is pressed
 * project         : EE 329 S'25 Assignment 2
 * authors         : Nathan Heil Benjamin Tavares
 * * version       : 0.3
 * date            : 4/16/25
 * compiler        : STM32CubeIDE v.1.12.0 Build: 14980_20230301_1550 (UTC)
 * target          : NUCLEO-L4A6ZG
 * clocks          : 4 MHz MSI to AHB2
 * @attention      : (c) 2023 STMicroelectronics.  All rights reserved.
 *******************************************************************************
 * main.c PLAN :
    * Configure Port C For Leds
    * Clear Mode For Pins 0,1,2,3
    * Set Mode OUTPUT for pins 0,1,2,3
    *
    * 		While {}
    * 			Check for keypress
    * 			If Keypress:
    * 				output_key = What Key Is Pressed:
    * 			Clear BSRR
    * 		Set BSRR to What Key Is Pressed

 *******************************************************************************
 * KEYPAD WIRING 4 ROWS 4 COLS (pinout NUCLEO-L4A6ZG = L496ZG)
 *      peripheral – Nucleo I/O
 * keypad Pin 1 -> Row 0 -> PD0 In
 * keypad Pin 2 -> ROW 1 -> PD1 In
 * keypad Pin 3 -> Row 2 -> PD2 In
 * keypad Pin 4 -> Row 3 -> PD3 In
 * keypad Pin 5 -> Col 0 -> PD4 Out
 * keypad Pin 6 -> Col 1 -> PD5 Out
 * keypad Pin 7 -> Col 2 -> PD6 Out
 * keypad Pin 8 -> Col 3 -> PD7 Out
 *******************************************************************************
 * REVISION HISTORY
 * 0.1 230318 bfd  created, wires in breadboard, no keypad
 * 0.2 230410 bfd  added keypad, debounce code, tested operational
 * 0.3 230413 bfd  refactored to share in lab manual, tested op’l
 *******************************************************************************
 * TODO
 * convert to separable module for portability
 *******************************************************************************
 * 45678-1-2345678-2-2345678-3-2345678-4-2345678-5-2345678-6-2345678-7-234567 */
/* USER CODE END Header */

#include "main.h"
#include "keypad.h"


void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void Keypad_Config(void);
int Keypad_IsKeyPressed(void);


int main(void)
{

  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  Keypad_Config();

  /* Configure Port C for LEDS*/
  RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOCEN);
  /*Clear Then Mode Pins, Then Set all Mode Pins To Outputs*/
  GPIOC->MODER &= ~(GPIO_MODER_MODE0 |
		  	  	  	GPIO_MODER_MODE1 |
					GPIO_MODER_MODE2 |
					GPIO_MODER_MODE3);
  GPIOC->MODER |= (GPIO_MODER_MODE0_0 |
		  	       GPIO_MODER_MODE1_0 |
				   GPIO_MODER_MODE2_0 |
				   GPIO_MODER_MODE3_0);
  // Set output type for all pins to Push Pull by making output type pins equal to zero*/
  GPIOC->OTYPER &= ~ (GPIO_OTYPER_OT0 |
		  	  	  	  GPIO_OTYPER_OT1 |
					  GPIO_OTYPER_OT2 |
					  GPIO_OTYPER_OT3  );
  GPIOC->OSPEEDR |= ((2<<GPIO_OSPEEDR_OSPEED0_Pos) |
		  	  	  	 (2<<GPIO_OSPEEDR_OSPEED1_Pos) |
					 (2<<GPIO_OSPEEDR_OSPEED2_Pos) |
					 (2<<GPIO_OSPEEDR_OSPEED3_Pos));
  /*Preset Port C Pins to Zero*/
  GPIOC->BRR = (GPIO_PIN_0 |
		  	    GPIO_PIN_1 |
				GPIO_PIN_2 |
				GPIO_PIN_3);


  int32_t key_value;
  while (1)
  {
	  key_value = Keypad_IsKeyPressed();
	  if (key_value > -1){
		  GPIOC->BRR = (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
		  GPIOC->BSRR = key_value;

	  }
	  else {
		  continue;
	  }
  }

}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }


  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}


static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  HAL_PWREx_EnableVddIO2();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_OverCurrent_Pin */
  GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : STLK_RX_Pin STLK_TX_Pin */
  GPIO_InitStruct.Pin = STLK_RX_Pin|STLK_TX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF8_LPUART1;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : USB_SOF_Pin USB_ID_Pin USB_DM_Pin USB_DP_Pin */
  GPIO_InitStruct.Pin = USB_SOF_Pin|USB_ID_Pin|USB_DM_Pin|USB_DP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


}


void Error_Handler(void)
{

  __disable_irq();
  while (1)
  {
  }

}

#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line)
{

}
#endif
