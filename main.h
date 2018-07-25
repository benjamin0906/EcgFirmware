#include <33EP256MU806.h>
#device ICSP=1
//#use delay(clock=140000000,crystal=16000000,USB_FULL,AUX:clock=48000000)
#use delay(clock=140MHz,crystal=16MHz, USB_FULL, AUX:clock=48MHz/*,crystal=16MHz*/)

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES CKSFSM                   //Clock Switching is enabled, fail Safe clock monitor is enabled
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOJTAG                   //JTAG disabled
#FUSES ICSP1

#define PIC_16BIT
#include <MyTypes.h>
#include <usb_cdc.h>

#define SS      PIN_D7  //4
#define SCK     PIN_D4  //1
#define SDI     PIN_D3  //2
#define SDO     PIN_D5  //3
#define RESET   PIN_D1  //8
#define PWD     PIN_D2  //6
#define DRDY    PIN_D6  //5

#define USB_IDLE    0
#define USB_SENDING 1
#define USB_READING 2
#define USB_HEADER_SENDING 4
#define USB_DATA_IN_INPUT_BUFFER    3
#define USB_MAX_TRANSFER    63
#define USB_INPUT_BUFFER_SIZE   USB_MAX_TRANSFER
#define USB_MESSAGE_HEADER_SIZE 4
#define BUFFER_SIZE 8192
#define FILTER_ORDER 497
#define ASM_FILTER_ORDER #FILTER_ORDER

#pin_select SCK1OUT=SCK
#pin_select SDI1=SDI
#pin_select SDO1=SDO
#use SPI(MASTER,MODE=3, BITS=16, BAUD=3500000,SPI1, FORCE_HW, XFER16,STREAM=ADAS1)

#define SIMULATOR