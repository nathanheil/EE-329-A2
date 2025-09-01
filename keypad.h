//keypad.h
//------------------------------------------------------------------------------------------------------------------------------------------------------------------
//prototype Keypad_Is_KeyPressed
//prototype Keypad_WhichKeyPressed
//prototype Keypad_Config
//# Define: Row0, Row1, Row2, Row3
//# Define: Column0, Column1, Column2, Column3


#ifndef __KEYPAD_H
#define __KEYPAD_H

/*Write Function Prototypes*/
void Keypad_Config(void);
int Keypad_IsKeyPressed(void);
// int Keypad_WhichKeyIsPressed(void);

/*Define Port Used*/
/*When Changing Ports, Changes Must Be Made To These Lines Of Code*/
#define keyport GPIOD
#define keyport_clk_en() (RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN)


/*Use #define to symbolically name "Rows" and "Columns"*/

/*Rows*/
#define Row0 (1 << 0) // (32bit 1) --> Keypad Pin0
#define Row1 (1 << 1) // = (1 shifted by 1) --> Keypad Pin1
#define Row2 (1 << 2) // --> Keypad Pin2
#define Row3 (1 << 3) // --> Keypad Pin3
#define all_rows (Row0 | Row1 | Row2 | Row3) //  bitmask all rows

/*Columns*/
#define Column0 (1 << 4) // --> Keypad Pin4
#define Column1 (1 << 5) // --> Keypad Pin5
#define Column2 (1 << 6) // --> Keypad Pin6
#define Column3 (1 << 7) // --> Keypad Pin7
#define all_columns (Column0 | Column1 | Column2 | Column3) // bitmask all columns


#endif

