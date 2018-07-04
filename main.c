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
#define USB_HEADER_SENDING 4
#define USB_DATA_IN_INPUT_BUFFER    3
#define USB_MAX_TRANSFER    63
#define USB_INPUT_BUFFER_SIZE   USB_MAX_TRANSFER
#define USB_MESSAGE_HEADER_SIZE 4
#define BUFFER_SIZE 8196
uint8 UsbState=0;
uint8 UsbInputBuffer[USB_INPUT_BUFFER_SIZE];
uint8 *UsbOutputArrayPointer;
uint16 UsbOutputSize=0;
uint16 UsbIterator=0;
uint16 BufferIterator=0;
uint16 BufferElementAmount=0;
uint16 SavedBufferElementAmount=0;
uint16 Iter=0;

  
    struct
    {
        struct
        {
            uint8 Start;
            uint8 Sign;
            uint8 Size1;
            uint8 Size2;
        } Header;
        union
        {
            uint8 Bytes[BUFFER_SIZE];
            uint32 Data[BUFFER_SIZE/4];
        } Data;
        
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
        case USB_HEADER_SENDING:
            if(usb_cdc_put_buffer_free())
            {
                _usb_cdc_putc_fast_noflush(Message.Header.Start);
                _usb_cdc_putc_fast_noflush(Message.Header.Sign);
                _usb_cdc_putc_fast_noflush(Message.Header.Size1);
                _usb_cdc_putc_fast_noflush(Message.Header.Size2);
                UsbState=USB_SENDING;
                looper=4;
            }
            else
            {
                break;
            }
        case USB_SENDING:
            if(usb_cdc_put_buffer_free())
            {
                while(looper<USB_MAX_TRANSFER && Iter < SavedBufferElementAmount)
                {
                    _usb_cdc_putc_fast_noflush(Message.Data.Bytes[UsbIterator]);
                    UsbIterator++;
                    looper++;
                    Iter++;
                    if(UsbIterator == BUFFER_SIZE-4) UsbIterator = 0; 
                }
                usb_cdc_flush_tx_buffer();
                if(Iter == UsbOutputSize)
                {
                    Iter=0;
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
uint16 test=0;
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
                    //UsbOutputSize=8192;
                    UsbState=USB_HEADER_SENDING;
                    BufferElementAmount=4;
                    /*if(test==0)
                    {
                        BufferElementAmount=4;
                        test=1;
                    }
                    else BufferElementAmount=8192;*/
                    UsbOutputSize=BufferElementAmount;
                    //UsbIterator = 0x1FFF & (BufferIterator-BufferElementAmount);
                    SavedBufferElementAmount=BufferElementAmount;
                    UsbState=USB_SENDING;
                    break;
                case 2:
                    
                    break;
            }
        }
    }
    
}

int32 AdasSpiReadWrite(int32 command)
{
    int32 ret=0;
    output_low(SS); //Slave select láb szoftveresen vezérelve
    ret = (spi_xfer(ADAS1,command>>16))<<16;
    ret += spi_xfer(ADAS1,(int16)command);
    if((ret>>24) == 0xf) printf(usb_cdc_putc,"1.\n");//debug jelzés
    output_high(SS);//Slave select láb szoftveresen vezérelve
    return (ret);
}

void cdc_uart(uint32 d)
{
    usb_cdc_putc(d>>24);
    usb_cdc_putc(d>>16);
    usb_cdc_putc(d>>8);
    usb_cdc_putc(d>>0);
    usb_cdc_putc('\n');
}

void AdasInit()
{
    output_high(PWD);
    output_high(RESET);
    delay_ms(20);
    unsigned char looper;
    long answer=0;
    long conf[5] =
                    {
                        0x0A07FF00,//Frame control register
                        0x05E06003,//Common mode register
                        0x08800000,//Test tone register: utolsó bájt: 15: négyszög; 05: 10Hz szinusz
                        0x0B000000,//Filter control register: a defaul megfelel?
                        0x01F804AE//ECG control register
                    };
    for(looper=0;looper<3;looper++)
    {
        
    AdasSpiReadWrite(conf[0]|0x80000000);
    AdasSpiReadWrite(conf[0]);
    answer=AdasSpiReadWrite(conf[1]|0x80000000);
    cdc_uart(answer);
    AdasSpiReadWrite(conf[1]);
    answer=AdasSpiReadWrite(conf[2]|0x80000000);
    cdc_uart(answer);
    AdasSpiReadWrite(conf[2]);
    answer=AdasSpiReadWrite(conf[3]|0x80000000);
    cdc_uart(answer);
    AdasSpiReadWrite(conf[3]);
    answer=AdasSpiReadWrite(conf[4]|0x80000000);
    cdc_uart(answer);
    AdasSpiReadWrite(conf[4]);//
    answer=AdasSpiReadWrite(0);
    cdc_uart(answer);//*/
    }
    AdasSpiReadWrite(0x40000000);//*/
    usb_cdc_putc('1');
}

void ruAdasHandler(void)
{
    if(0 == input(DRDY))
    {
        output_low(PIN_B0);
        uint16 looper;
        for(looper = 0; looper<6; looper++, BufferIterator++)
        {
            Message.Data.Data[BufferIterator] = AdasSpiReadWrite(0x00000000);
            if(BufferIterator == BUFFER_SIZE-1)
            {
                BufferIterator = -1;
            }
        }
        BufferElementAmount+=4*6;
        if(BufferElementAmount >8192) BufferElementAmount=8192;
    }
}

int main(int argc, char** argv) 
{
    output_high(PIN_F3);
    output_high(PIN_B1);
    usb_cdc_init();
    usb_init();
    output_high(PIN_B0);
    AdasInit();
    
    unsigned long looper;
    unsigned long looper2=0;
    unsigned long Size=8192;
    for(looper=0;looper<BUFFER_SIZE/4;looper++, looper2++)
    {
        switch(looper2)
        {
            case 0:
                Message.Data.Data[looper]=ChangeEndiannes32Bit(0x11000000|looper);
                break;
            case 1:
                Message.Data.Data[looper]=ChangeEndiannes32Bit(0x12000000|looper);
                break;
            case 2:
                Message.Data.Data[looper]=ChangeEndiannes32Bit(0x13000000|looper);
                break;
            case 3:
                Message.Data.Data[looper]=ChangeEndiannes32Bit(0x14000000|looper);
                break;
            case 4:
                Message.Data.Data[looper]=ChangeEndiannes32Bit(0x15000000|looper);
                looper2=-1;
                break;
            default:
                looper2=-1;
        }
        
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
        ruAdasHandler();
        //if(UsbState == USB_SENDING) output_toggle(PIN_B0);
    }
    return (1);
}