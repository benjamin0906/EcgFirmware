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
#define BUFFER_SIZE 8192
uint8 UsbState=0;
uint8 UsbInputBuffer[USB_INPUT_BUFFER_SIZE];
uint16 BufferReadPosition=0;
uint16 BufferIterator=0;
uint16 BufferElementAmount=0;
uint16 SavedBufferElementAmount=0;
uint16 BufferReadCounter=0;

sint32 BufferForWeight[211];
#banky //This preprocessor instruction ensure that the following data will be pressed into the Y Ram space.
uint16 Weight[211]=
{
0xFFC1,
0xFFC3,
0xFFC4,
0xFFC7,
0xFFC9,
0xFFCB,
0xFFCE,
0xFFD1,
0xFFD5,
0xFFD8,
0xFFDC,
0xFFE0,
0xFFE5,
0xFFE9,
0xFFEE,
0xFFF3,
0xFFF9,
0xFFFF,
0x0004,
0x000A,
0x0010,
0x0017,
0x001E,
0x0025,
0x002D,
0x0034,
0x003C,
0x0044,
0x004D,
0x0055,
0x005E,
0x0067,
0x0070,
0x0079,
0x0082,
0x008C,
0x0096,
0x009F,
0x00A9,
0x00B3,
0x00BD,
0x00C7,
0x00D1,
0x00DB,
0x00E5,
0x00F0,
0x00FA,
0x0104,
0x010E,
0x0118,
0x0122,
0x012C,
0x0136,
0x013F,
0x0149,
0x0152,
0x015B,
0x0164,
0x016D,
0x0176,
0x017E,
0x0186,
0x018E,
0x0196,
0x019E,
0x01A5,
0x01AC,
0x01B2,
0x01B8,
0x01BE,
0x01C4,
0x01C9,
0x01CE,
0x01D3,
0x01D7,
0x01DB,
0x01DE,
0x01E1,
0x01E4,
0x01E6,
0x01E8,
0x01E9,
0x01EA,
0x01EB,
0x01EB,
0x01EB,
0x01EA,
0x01E9,
0x01E8,
0x01E6,
0x01E4,
0x01E1,
0x01DE,
0x01DB,
0x01D7,
0x01D3,
0x01CE,
0x01C9,
0x01C4,
0x01BE,
0x01B8,
0x01B2,
0x01AC,
0x01A5,
0x019E,
0x0196,
0x018E,
0x0186,
0x017E,
0x0176,
0x016D,
0x0164,
0x015B,
0x0152,
0x0149,
0x013F,
0x0136,
0x012C,
0x0122,
0x0118,
0x010E,
0x0104,
0x00FA,
0x00F0,
0x00E5,
0x00DB,
0x00D1,
0x00C7,
0x00BD,
0x00B3,
0x00A9,
0x009F,
0x0096,
0x008C,
0x0082,
0x0079,
0x0070,
0x0067,
0x005E,
0x0055,
0x004D,
0x0044,
0x003C,
0x0034,
0x002D,
0x0025,
0x001E,
0x0017,
0x0010,
0x000A,
0x0004,
0xFFFF,
0xFFF9,
0xFFF3,
0xFFEE,
0xFFE9,
0xFFE5,
0xFFE0,
0xFFDC,
0xFFD8,
0xFFD5,
0xFFD1,
0xFFCE,
0xFFCB,
0xFFC9,
0xFFC7,
0xFFC4,
0xFFC3,
0xFFC1,
};

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

/* This function changes the endianness in double-words. */
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
                BufferReadPosition = (0x7FF & (BufferIterator-BufferElementAmount))*4;
                SavedBufferElementAmount=BufferElementAmount;
                BufferElementAmount=0;
                Message.Header.Size1=SavedBufferElementAmount*4>>8;
                Message.Header.Size2=SavedBufferElementAmount*4;
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
                while((looper<USB_MAX_TRANSFER) && (BufferReadCounter < (SavedBufferElementAmount*4)))
                {
                    _usb_cdc_putc_fast_noflush(Message.Data.Bytes[BufferReadPosition]);
                    BufferReadPosition++;
                    looper++;
                    BufferReadCounter++;
                    if(BufferReadPosition == BUFFER_SIZE) BufferReadPosition = 0; 
                }
                usb_cdc_flush_tx_buffer();
                if(BufferReadCounter == (SavedBufferElementAmount*4))
                {
                    BufferReadCounter=0;
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
    if(UsbState == USB_DATA_IN_INPUT_BUFFER)
    {
        if(UsbInputBuffer[0] == 'S')
        {
            switch(UsbInputBuffer[1])
            {
                case 1:
                    UsbState=USB_HEADER_SENDING;
                    output_toggle(PIN_B1);
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

/* This function implements the initialization of the ADAS1000 front-end IC via SPI */
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
                        0x08800005,//Test tone register: utolsó bájt: 15: négyszög; 05: 10Hz szinusz
                        0x0B000000,//Filter control register: a defaul megfelel?
                        //0x01F804AE//ECG control register digital lead mode
                        0x01F800AE//ECG control register analog lead mode
                    };
    for(looper=0;looper<3;looper++)
    {
        AdasSpiReadWrite(conf[0]|0x80000000);
        AdasSpiReadWrite(conf[0]);
        answer=AdasSpiReadWrite(conf[1]|0x80000000);
        AdasSpiReadWrite(conf[1]);
        answer=AdasSpiReadWrite(conf[2]|0x80000000);
        AdasSpiReadWrite(conf[2]);
        answer=AdasSpiReadWrite(conf[3]|0x80000000);
        AdasSpiReadWrite(conf[3]);
        answer=AdasSpiReadWrite(conf[4]|0x80000000);
        AdasSpiReadWrite(conf[4]);//
        answer=AdasSpiReadWrite(0);
    }
    AdasSpiReadWrite(0x40000000);//*/
}

uint32 FirWeighter(uint32 RawData)
{   uint32 ret=RawData;
    uint16 looper=169*4;
#asm
    PUSH W1
    PUSH W2
    PUSH W3
    PUSH W4
    PUSH W5
    PUSH W6
    PUSH W7
    PUSH W8
    PUSH W10
    MOV RawData+2, W1
    MOV #0x00ff, W2
    AND W1,W2,W1
    MOV W1, RawData+2
    MOV looper,W1
    MOV (&BufferForWeight), W8  //W1 contains the BaseAddress
    ADD W8,W1,W8
    ADD W8, #2,W8
    MOV (&Weight),W10
    LSR W1,W2
    ADD W10,W2,W10
    CLR A
    CLR B
    MOV [W8], W4
    SUB W8, #2,W8
    MOV [W10], W5
    SUB W10, #2,W10
    label:
        MAC W4*W5, A, [W8]-=2, W4
        ADD W8, #4,W3
        MOV [W8],[W3]
        MAC W4*W5, B, [W8]-=2, W4, [W10]-=2, W5
        ADD W8, #4,W3
        MOV [W8],[W3]
        SUB #4  , W1
        BRA NZ,label
    MOV (&RawData), W3
    MOV [W3++], [W8++]
    MOV [W3], [W8--]
    MOV [W3--], W4;
    MAC W4*W5, A, [W8], W4;
    MAC W4*W5, B
    SFTAC A,#-16
    //BCLR 0x44, #13
    ADD A
    //BSET 0x44, #13
    SFTAC A,#15
    SAC A,#0,[++W3]
    SFTAC A, #-16
    SAC A,#0,[--W3]
    //MOV #0x00ff, W0
    //AND (RawData+2)
    POP W10
    POP W8
    POP W7
    POP W6
    POP W5
    POP W4
    POP W3
    POP W2
    POP W1
#endasm
    //return BufferForWeight[210];
    return RawData;
}
uint32 globalvar=0;
void ruAdasHandler(void)
{
#ifndef SIMULATOR
    if(0 == input(DRDY))
#endif
    {
        //output_toggle(PIN_B0);
        uint16 looper;
        uint32 Temporary=0;
        for(looper = 0; looper<6; looper++, BufferIterator++)
        {
#ifndef SIMULATOR
            Temporary = AdasSpiReadWrite(0x00000000);
#else
            Temporary=0x11000000;
#endif
            if(((Temporary&0xFF000000)>>24) != 0x11)
            {
                Message.Data.Data[BufferIterator] = ChangeEndiannes32Bit(Temporary);
            }
            else
            {
                output_high(PIN_B0);
                Message.Data.Data[BufferIterator] = ChangeEndiannes32Bit(FirWeighter(Temporary));
                //Message.Data.Data[BufferIterator] = ChangeEndiannes32Bit(globalvar);
                output_low(PIN_B0);
            }
            //Message.Data.Data[BufferIterator] = ChangeEndiannes32Bit(AdasSpiReadWrite(0x00000000));
            
            if(BufferIterator == ((BUFFER_SIZE/4)-1))
            {
                BufferIterator = -1;
            }
        }
        BufferElementAmount+=6;
        if(BufferElementAmount >(BUFFER_SIZE/4)) BufferElementAmount=(BUFFER_SIZE/4);
    }
}

void ruBufferTester(void)
{
    unsigned long looper;
    unsigned long looper2;
    for(looper=0,looper2=0;looper<BUFFER_SIZE/4;looper++, looper2++,BufferIterator++,BufferElementAmount++)
    {
        switch(looper2)
        {
            case 0:
                Message.Data.Data[BufferIterator]=ChangeEndiannes32Bit(0x11000000|looper);
                break;
            case 1:
                Message.Data.Data[BufferIterator]=ChangeEndiannes32Bit(0x12000000|looper);
                break;
            case 2:
                Message.Data.Data[BufferIterator]=ChangeEndiannes32Bit(0x13000000|looper);
                break;
            case 3:
                Message.Data.Data[BufferIterator]=ChangeEndiannes32Bit(0x14000000|looper);
                break;
            case 4:
                Message.Data.Data[BufferIterator]=ChangeEndiannes32Bit(0x15000000|looper);
                looper2=-1;
                break;
            default:
                looper2=-1;
        }
        if(BufferIterator == ((BUFFER_SIZE/4)-1))
        {
            BufferIterator = -1;
        }
        if(BufferElementAmount >(BUFFER_SIZE/4)) BufferElementAmount=(BUFFER_SIZE/4);
    }
}

int main() 
{
#asm
    BCLR 0x44, #5
    BSET 0x44, #0
    BSET 0x44, #13
    BCLR 0x44, #12
#endasm
    output_high(PIN_F3);
    output_high(PIN_B1);
#ifndef SIMULATOR
    usb_cdc_init();
    usb_init();
    AdasInit();
#endif
    output_high(PIN_B0);
    int looper=0;
    for(looper=0;looper<211;looper++)
    {
        BufferForWeight[looper]=1+looper;
        //Weight[looper]=0x8000;
    }
    unsigned long Size=8192;
    ruBufferTester();
    Size=8192;
    Message.Header.Start='S';
    Message.Header.Sign=0x02;
    Message.Header.Size1=Size>>8;
    Message.Header.Size2=Size;
    while(1)
    {
#ifndef SIMULATOR
        ruUsb();
        ruUsbInputProcess();
#endif
        ruAdasHandler();
        //if(UsbState == USB_SENDING) output_toggle(PIN_B0);
    }
    return (1);
}