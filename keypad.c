

/* USER CODE BEGIN Header */
/*******************************************************************************
* EE 329 A2 KEYPAD INTERFACE
*******************************************************************************
* @file           : keypad.c
* @brief          : Identifies Keypresses on 4x4 Mechanical Keypad; Returns
* 					 Keymapped Value Corresponding to Keypad.
* 					 -includes keypad.h
* project         : EE 329 S'25 Assignment 2
* authors         : Nathan Heil Benjamin Tavares
* version         : 0.3
* date            : 4/16/25
* compiler        : STM32CubeIDE v.1.12.0 Build: 14980_20230301_1550 (UTC)
* target          : NUCLEO-L4A6ZG
* clocks          : 4 MHz MSI to AHB2
* @attention      : (c) 2023 STMicroelectronics.  All rights reserved.
*******************************************************************************
* KEYPAD WIRING 4 ROWS 4 COLS (pinout NUCLEO-L4A6ZG = L496ZG)
*      peripheral – Nucleo I/O
* --> Keypad Pins 0 and 9 are unused
* Keypad Pin1 -> Row 0 -> PD0
* Keypad Pin2 -> Row 1 -> PD1
* Keypad Pin3 -> Row 2 -> PD2
* Keypad Pin4 -> Row 3 -> PD3
* Keypad Pin5 -> Col 0 -> PD4
* Keypad Pin6 -> Col 1 -> PD5
* Keypad Pin7 -> Col 2 -> PD6
* Keypad Pin8 -> Col 3 -> PD7


 *******************************************************************************
 * REVISION HISTORY
 * 0.1 230318 bfd  created, wires in breadboard, no keypad
 * 0.2 230410 bfd  added keypad, debounce code, tested operational
 * 0.3 230413 bfd  refactored to share in lab manual, tested op’l

*******************************************************************************
keypad.c Plan
********************************************************************************
* Configure Port D For Keypad
* KEYMAP (FROM CHAT)
*Clear Mode For Portd Pins 0,1,2,3,4,5,6,7
*Set Mode 0,1,2,3 (input) Rows
*Set Mode 4,5,6,7 (Output) Columns
*
*int keypad_IsAnyKeyPressed(Void)
*	Drive Columns High:
*	Delay
*	If Any Row is High
*		Return True
*	Else:
*		Exit
*
*int Keypad_WhichKeyIsPressed?
*	For Each Column:
*		Set High
*		For Each Row:
*			If Row Is High
*				Return Column, Row; --> Define all of the different keys
*******************************************************************************
*/
/* USER CODE END Header */

#include "keypad.h"
#include "stm32l4xx_hal.h"
/*Configuration*/
void Keypad_Config(void) {
	keyport_clk_en();
	keyport->MODER &= ~(GPIO_MODER_MODE0 |
						GPIO_MODER_MODE1 |
						GPIO_MODER_MODE2 |
						GPIO_MODER_MODE3 |
						GPIO_MODER_MODE4 |
						GPIO_MODER_MODE5 |
						GPIO_MODER_MODE6 |
						GPIO_MODER_MODE7);

	/*Set Pins 4,5,6,7 to Outputs, no need to set them as inputs*/
	keyport->MODER |= (GPIO_MODER_MODE4_0 |
					   GPIO_MODER_MODE5_0 |
					   GPIO_MODER_MODE6_0 |
					   GPIO_MODER_MODE7_0);

	/*Set Output type. there are two types: 0:PULL Up Pull Down 1:Open Drain*/
	keyport->OTYPER &= ~(GPIO_OTYPER_OT0 |
						 GPIO_OTYPER_OT1 |
						 GPIO_OTYPER_OT2 |
						 GPIO_OTYPER_OT3 |
						 GPIO_OTYPER_OT4 |
						 GPIO_OTYPER_OT5 |
						 GPIO_OTYPER_OT6 |
						 GPIO_OTYPER_OT7);

	keyport->OSPEEDR |= ((3 << GPIO_OSPEEDR_OSPEED1_Pos) |
						 (3 << GPIO_OSPEEDR_OSPEED2_Pos) |
						 (3 << GPIO_OSPEEDR_OSPEED3_Pos) |
						 (3 << GPIO_OSPEEDR_OSPEED4_Pos) |
						 (3 << GPIO_OSPEEDR_OSPEED5_Pos) |
						 (3 << GPIO_OSPEEDR_OSPEED6_Pos) |
						 (3 << GPIO_OSPEEDR_OSPEED7_Pos));

	keyport->PUPDR &= ~(GPIO_PUPDR_PUPD0 | //clear all bits of pupdr being used
						GPIO_PUPDR_PUPD1 |
						GPIO_PUPDR_PUPD2 |
						GPIO_PUPDR_PUPD3);
	keyport->PUPDR |=  (GPIO_PUPDR_PUPD0_1 | // set used bits of pupdr to 1 (pull down)
					    GPIO_PUPDR_PUPD1_1 |
						GPIO_PUPDR_PUPD2_1 |
						GPIO_PUPDR_PUPD3_1);
}

/*Define A Key Map To map The [Column] and [Row] and Return the mapped integer*/
const int keymap[4][4] = {
    {1, 2, 3, 10},   // 10 = 'A'
    {4, 5, 6, 11},   // 11 = 'B'
    {7, 8, 9, 12},   // 12 = 'C'
    {14, 0, 15, 13}  // 13 = 'D', 14 = '*', 15 = '#'
};

/*
 * Check row by row for a button press,
 *  if  a button is pressed, wait, and check again for debounce
*/
int Keypad_IsKeyPressed(void) {
	uint8_t idx_r;
	uint8_t idx_c;
	uint8_t delay;

	for (idx_c = 0; idx_c < 4; idx_c++) {
		keyport->BRR = all_columns;
		keyport->BSRR = (1 << (idx_c + 4)); //left shift 4 because the row pins come first
		for (delay = 0; delay < 200; delay++){} // delay for stabilization

		for (idx_r = 0; idx_r < 4; idx_r++) {
			if ((keyport->IDR) & (1 << idx_r)) {  // check IDR to see if pins are high
				for (delay = 0; delay < 200; delay++){} // delay to confirm btn.press
				if ((keyport->IDR) & (1 << idx_r)) { // check 2 confirm btn. press
					return keymap[idx_c][idx_r]; // keymap based on col/row
				}
			}
		}
	}
	return -1; // Button not pressed, return -1
}








