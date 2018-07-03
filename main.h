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