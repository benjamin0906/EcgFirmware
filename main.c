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
#banky
uint16 Weight[211]=
{
0x1,
0x2,
0x3,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1
};

uint16 Weight2[211]=
{
0x1E6,
0x1DF,
0x1D9,
0x1D3,
0x1CD,
0x1C7,
0x1C1,
0x1BB,
0x1B6,
0x1B1,
0x1AB,
0x1A6,
0x1A1,
0x19D,
0x198,
0x193,
0x18F,
0x18A,
0x186,
0x182,
0x17E,
0x17A,
0x176,
0x172,
0x16F,
0x16B,
0x168,
0x164,
0x161,
0x15E,
0x15B,
0x157,
0x154,
0x152,
0x14F,
0x14C,
0x149,
0x146,
0x144,
0x141,
0x13F,
0x13C,
0x13A,
0x138,
0x135,
0x133,
0x131,
0x12F,
0x12D,
0x12B,
0x129,
0x127,
0x125,
0x124,
0x122,
0x120,
0x11F,
0x11D,
0x11B,
0x11A,
0x118,
0x117,
0x116,
0x114,
0x113,
0x112,
0x110,
0x10F,
0x10E,
0x10D,
0x10C,
0x10B,
0x10A,
0x109,
0x108,
0x107,
0x106,
0x105,
0x104,
0x104,
0x103,
0x102,
0x102,
0x101,
0x100,
0x100,
0xFF,
0xFF,
0xFE,
0xFE,
0xFD,
0xFD,
0xFC,
0xFC,
0xFC,
0xFB,
0xFB,
0xFB,
0xFB,
0xFB,
0xFA,
0xFA,
0xFA,
0xFA,
0xFA,
0xFA,
0xFA,
0xFA,
0xFA,
0xFA,
0xFA,
0xFB,
0xFB,
0xFB,
0xFB,
0xFB,
0xFC,
0xFC,
0xFC,
0xFD,
0xFD,
0xFE,
0xFE,
0xFF,
0xFF,
0x100,
0x100,
0x101,
0x102,
0x102,
0x103,
0x104,
0x104,
0x105,
0x106,
0x107,
0x108,
0x109,
0x10A,
0x10B,
0x10C,
0x10D,
0x10E,
0x10F,
0x110,
0x112,
0x113,
0x114,
0x116,
0x117,
0x118,
0x11A,
0x11B,
0x11D,
0x11F,
0x120,
0x122,
0x124,
0x125,
0x127,
0x129,
0x12B,
0x12D,
0x12F,
0x131,
0x133,
0x135,
0x138,
0x13A,
0x13C,
0x13F,
0x141,
0x144,
0x146,
0x149,
0x14C,
0x14F,
0x152,
0x154,
0x157,
0x15B,
0x15E,
0x161,
0x164,
0x168,
0x16B,
0x16F,
0x172,
0x176,
0x17A,
0x17E,
0x182,
0x186,
0x18A,
0x18F,
0x193,
0x198,
0x19D,
0x1A1,
0x1A6,
0x1AB,
0x1B1,
0x1B6,
0x1BB,
0x1C1,
0x1C7,
0x1CD,
0x1D3,
0x1D9,
0x1DF,
0x1E6
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

uint32 FirWeighter2(uint32 RawData)
{
    return RawData;
    return (RawData&0xff000000)|((RawData&0xffffff)>>8);
}

uint32 FirWeighter(uint32 RawData)
{   uint32 ret=0;
    uint16 looper=200*4;
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
    SE W1,W1
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
    ADD A
    SFTAC A,#15
    SAC A,#0,[++W3]
    SFTAC A, #-16
    SAC A,#0,[--W3]
    
    /*MPY W4*W5, A, [W8]+=2, W4,
    MPY W4*W5, B, [W8]+=2, W4, [W10]+=2, W5
    MPY W4*W5, A, [W8]+=2, W4
    MPY W4*W5, B, [W8]+=2, W4, [W10]+=2, W5
    MPY W4*W5, A, [W8]+=2, W4
    MPY W4*W5, B, [W8]+=2, W4, [W10]+=2, W5
    MOV looper, W2              //W2: looper
            label:
            ADD W1, W2, W3              //W3: address of the high word of previous element
            ADD W3, #2, W4              //W4: address of the low word of previous element
            SUB W3, #4, W5               //W5: address of the high word of previous element
            SUB W3, #2, W6               //W6: address of the low word of previous element
            MOV [W5], [W3]
            MOV [W6], [W4]
            SUB #4  , W2
            BRA NZ,label
            MOV RawData, W3
            MOV W3,[W5]
    MOV RawData+2, W3
    MOV W3,[W6]*/
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
        Weight[looper]=0x3fff;
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