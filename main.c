/* 
 * File:   main.c
 * Author: Benjamin
 *
 * Created on 2018. május 21., 11:31
 */
#include <main.h>

#define USB_IDLE    0
#define USB_SENDING 1
#define USB_READING 2
#define USB_DATA_IN_INPUT_BUFFER    3
#define USB_MAX_TRANSFER    63
#define USB_INPUT_BUFFER_SIZE   USB_MAX_TRANSFER
#define USB_MESSAGE_HEADER_SIZE 4
#define BUFFER_SIZE 8192
uint8 UsbState=0;
uint8 UsbInputBuffer[USB_INPUT_BUFFER_SIZE];
uint8 *UsbOutputArrayPointer;
uint16 UsbOutputSize=0;
uint16 UsbIterator=0;

union
{
    uint8 Bytes[USB_MESSAGE_HEADER_SIZE+BUFFER_SIZE];
    struct
    {
        struct
        {
            uint8 Start;
            uint8 Sign;
            uint8 Size1;
            uint8 Size2;
        } Header;
        uint32 Data[2048];
    };
}Message;
    
uint32 ChangeEndiannes32Bit(uint32 Data)
{
#asm
    MOV Data, W1
    MOV Data+2, W2
    SWAP W1
    SWAP W2
    EXCH W1, W2
    MOV W2, Data+2
    MOV W1,Data
#endasm
    return Data;
}

void ruUsb()
{
    uint16 looper=0;
    switch(UsbState)
    {
        case USB_DATA_IN_INPUT_BUFFER:
        case USB_IDLE:
            if(usb_cdc_kbhit())
            {
                UsbState=USB_READING;
            }
            break;
        case USB_SENDING:
            if(usb_cdc_put_buffer_free())
            {
                while(looper<USB_MAX_TRANSFER && UsbIterator < UsbOutputSize)
                {
                    _usb_cdc_putc_fast_noflush(Message.Bytes[UsbIterator]);
                    UsbIterator++;
                    looper++;
                }
                usb_cdc_flush_tx_buffer();
                if(UsbIterator == UsbOutputSize)
                {
                    UsbIterator=0;
                    UsbState=USB_IDLE;
                }
            }
            break;
        case USB_READING:
            while(looper<USB_MAX_TRANSFER && usb_cdc_kbhit())
            {
                UsbInputBuffer[looper]=usb_cdc_getc();
                looper++;
            }
            if(!usb_cdc_kbhit())
            {
                UsbState=USB_DATA_IN_INPUT_BUFFER;
                
            }
            break;
    }
}

void ruUsbInputProcess(void)
{
    uint8 looper;
    if(UsbState == USB_DATA_IN_INPUT_BUFFER)
    {
        if(UsbInputBuffer[0] == 'S')
        {
            switch(UsbInputBuffer[1])
            {
                case 1:
                    output_low(PIN_B1);
                    //UsbOutputArrayPointer=&array[0];
                    UsbOutputSize=8196;
                    UsbState=USB_SENDING;
                    output_toggle(PIN_B0);
                    break;
                case 2:
                    
                    break;
            }
        }
    }
    
}


int main(int argc, char** argv) 
{
    output_high(PIN_F3);
    output_high(PIN_B1);
    usb_cdc_init();
    usb_init();
    output_toggle(PIN_B0);
    
    unsigned long looper;
    unsigned long looper2=0;
    unsigned long Size=8192;
    for(looper=0;looper<Size/4;looper++)
    {
        Message.Data[looper]=ChangeEndiannes32Bit(0x11000000|looper);
    }
    Size=8192;
    Message.Header.Start='S';
    Message.Header.Sign=0x02;
    Message.Header.Size1=Size>>8;
    Message.Header.Size2=Size;
    looper=0;
    
    unsigned int InputMessageSignal=0;
    unsigned long long looper3=0x11000000;
    unsigned long long PrevLooper=0x10ffffff;
    unsigned int ByteSign=0;
    while(1)
    {
        ruUsb();
        ruUsbInputProcess();
    }
    return (1);
}