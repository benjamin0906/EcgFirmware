/* 
 * File:   main.c
 * Author: Benjamin
 *
 * Created on 2018. május 21., 11:31
 */
#include <main.h>
#ifdef SIMULATOR
//#include <math.h>
#endif

uint8 UsbState=0;
uint8 UsbInputBuffer[USB_INPUT_BUFFER_SIZE];
uint16 BufferReadPosition=0;
uint16 BufferIterator=0;
uint16 BufferElementAmount=0;
uint16 SavedBufferElementAmount=0;
uint16 BufferReadCounter=0;

#bankx
sint32 FilterBufferCh11[FILTER_ORDER];
#bankx
sint32 FilterBufferCh12[FILTER_ORDER];
#bankx
sint32 FilterBufferCh13[FILTER_ORDER];
#bankx
sint32 FilterBufferCh14[FILTER_ORDER];
#bankx
sint32 FilterBufferCh15[FILTER_ORDER];
//#BANK_DMA //This preprocessor instruction ensure that the following data will be pressed into the Y Ram space.
uint16 Weight[FILTER_ORDER]=
{
0xFFFB,
0xFFFB,
0xFFFA,
0xFFFA,
0xFFFA,
0xFFFA,
0xFFFA,
0xFFFA,
0xFFFA,
0xFFFA,
0xFFFA,
0xFFFA,
0xFFFA,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFFA,
0xFFFA,
0xFFFA,
0xFFFA,
0xFFFA,
0xFFFA,
0xFFFA,
0xFFFB,
0xFFFB,
0xFFFB,
0xFFFB,
0xFFFB,
0xFFFC,
0xFFFC,
0xFFFC,
0xFFFC,
0xFFFD,
0xFFFD,
0xFFFD,
0xFFFD,
0xFFFE,
0xFFFE,
0xFFFE,
0xFFFF,
0xFFFF,
0x0000,
0x0000,
0x0000,
0x0001,
0x0001,
0x0002,
0x0002,
0x0003,
0x0003,
0x0004,
0x0004,
0x0005,
0x0005,
0x0006,
0x0006,
0x0007,
0x0008,
0x0008,
0x0009,
0x0009,
0x000A,
0x000B,
0x000B,
0x000C,
0x000D,
0x000E,
0x000E,
0x000F,
0x0010,
0x0011,
0x0012,
0x0012,
0x0013,
0x0014,
0x0015,
0x0016,
0x0017,
0x0018,
0x0019,
0x001A,
0x001B,
0x001C,
0x001D,
0x001E,
0x001F,
0x0020,
0x0021,
0x0022,
0x0023,
0x0024,
0x0025,
0x0026,
0x0027,
0x0028,
0x002A,
0x002B,
0x002C,
0x002D,
0x002E,
0x0030,
0x0031,
0x0032,
0x0033,
0x0035,
0x0036,
0x0037,
0x0038,
0x003A,
0x003B,
0x003C,
0x003E,
0x003F,
0x0040,
0x0042,
0x0043,
0x0044,
0x0046,
0x0047,
0x0049,
0x004A,
0x004B,
0x004D,
0x004E,
0x0050,
0x0051,
0x0052,
0x0054,
0x0055,
0x0057,
0x0058,
0x005A,
0x005B,
0x005C,
0x005E,
0x005F,
0x0061,
0x0062,
0x0064,
0x0065,
0x0066,
0x0068,
0x0069,
0x006B,
0x006C,
0x006E,
0x006F,
0x0070,
0x0072,
0x0073,
0x0075,
0x0076,
0x0077,
0x0079,
0x007A,
0x007C,
0x007D,
0x007E,
0x0080,
0x0081,
0x0082,
0x0083,
0x0085,
0x0086,
0x0087,
0x0089,
0x008A,
0x008B,
0x008C,
0x008D,
0x008F,
0x0090,
0x0091,
0x0092,
0x0093,
0x0094,
0x0096,
0x0097,
0x0098,
0x0099,
0x009A,
0x009B,
0x009C,
0x009D,
0x009E,
0x009F,
0x00A0,
0x00A0,
0x00A1,
0x00A2,
0x00A3,
0x00A4,
0x00A5,
0x00A5,
0x00A6,
0x00A7,
0x00A8,
0x00A8,
0x00A9,
0x00AA,
0x00AA,
0x00AB,
0x00AB,
0x00AC,
0x00AC,
0x00AD,
0x00AD,
0x00AE,
0x00AE,
0x00AF,
0x00AF,
0x00AF,
0x00B0,
0x00B0,
0x00B0,
0x00B0,
0x00B1,
0x00B1,
0x00B1,
0x00B1,
0x00B1,
0x00B1,
0x00B1,
0x00B2,
0x00B2,
0x00B2,
0x00B1,
0x00B1,
0x00B1,
0x00B1,
0x00B1,
0x00B1,
0x00B1,
0x00B0,
0x00B0,
0x00B0,
0x00B0,
0x00AF,
0x00AF,
0x00AF,
0x00AE,
0x00AE,
0x00AD,
0x00AD,
0x00AC,
0x00AC,
0x00AB,
0x00AB,
0x00AA,
0x00AA,
0x00A9,
0x00A8,
0x00A8,
0x00A7,
0x00A6,
0x00A5,
0x00A5,
0x00A4,
0x00A3,
0x00A2,
0x00A1,
0x00A0,
0x00A0,
0x009F,
0x009E,
0x009D,
0x009C,
0x009B,
0x009A,
0x0099,
0x0098,
0x0097,
0x0096,
0x0094,
0x0093,
0x0092,
0x0091,
0x0090,
0x008F,
0x008D,
0x008C,
0x008B,
0x008A,
0x0089,
0x0087,
0x0086,
0x0085,
0x0083,
0x0082,
0x0081,
0x0080,
0x007E,
0x007D,
0x007C,
0x007A,
0x0079,
0x0077,
0x0076,
0x0075,
0x0073,
0x0072,
0x0070,
0x006F,
0x006E,
0x006C,
0x006B,
0x0069,
0x0068,
0x0066,
0x0065,
0x0064,
0x0062,
0x0061,
0x005F,
0x005E,
0x005C,
0x005B,
0x005A,
0x0058,
0x0057,
0x0055,
0x0054,
0x0052,
0x0051,
0x0050,
0x004E,
0x004D,
0x004B,
0x004A,
0x0049,
0x0047,
0x0046,
0x0044,
0x0043,
0x0042,
0x0040,
0x003F,
0x003E,
0x003C,
0x003B,
0x003A,
0x0038,
0x0037,
0x0036,
0x0035,
0x0033,
0x0032,
0x0031,
0x0030,
0x002E,
0x002D,
0x002C,
0x002B,
0x002A,
0x0028,
0x0027,
0x0026,
0x0025,
0x0024,
0x0023,
0x0022,
0x0021,
0x0020,
0x001F,
0x001E,
0x001D,
0x001C,
0x001B,
0x001A,
0x0019,
0x0018,
0x0017,
0x0016,
0x0015,
0x0014,
0x0013,
0x0012,
0x0012,
0x0011,
0x0010,
0x000F,
0x000E,
0x000E,
0x000D,
0x000C,
0x000B,
0x000B,
0x000A,
0x0009,
0x0009,
0x0008,
0x0008,
0x0007,
0x0006,
0x0006,
0x0005,
0x0005,
0x0004,
0x0004,
0x0003,
0x0003,
0x0002,
0x0002,
0x0001,
0x0001,
0x0000,
0x0000,
0x0000,
0xFFFF,
0xFFFF,
0xFFFE,
0xFFFE,
0xFFFE,
0xFFFD,
0xFFFD,
0xFFFD,
0xFFFD,
0xFFFC,
0xFFFC,
0xFFFC,
0xFFFC,
0xFFFB,
0xFFFB,
0xFFFB,
0xFFFB,
0xFFFB,
0xFFFA,
0xFFFA,
0xFFFA,
0xFFFA,
0xFFFA,
0xFFFA,
0xFFFA,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFF9,
0xFFFA,
0xFFFA,
0xFFFA,
0xFFFA,
0xFFFA,
0xFFFA,
0xFFFA,
0xFFFA,
0xFFFA,
0xFFFA,
0xFFFA,
0xFFFB,
0xFFFB
};
#locate Weight=0x5000
typedef struct
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
        uint32 Data[(BUFFER_SIZE)/4];
    } Data;
}dtMessagePacket;
dtMessagePacket Message;
#locate Message=0x5500
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
                        0x05E00003,//Common mode register
                        0x08800005,//Test tone register: utolsó bájt: 15: négyszög; 05: 10Hz szinusz
                        0x0B000000,//Filter control register: a defaul megfelel?
                        0x01F804AE//ECG control register analog lead mode
                        //0x01F800AE//ECG control register digital lead mode
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
uint32 TracerVariable;
uint32 FirWeighter(uint32 RawData)
{
    uint32 ret;//=FirFilterBuffer[248];
    uint16 Address;
#asm
    /* Save the working registers in to the stack. */
    PUSH W1;
    PUSH W3;
    PUSH W4;
    PUSH W5;
    PUSH W8;
    PUSH W10;
    PUSH W2
    PUSH W0
    
    /*This part of code cuts the address from the data and shifts it right with 5.*/
    MOV RawData+2, W1
    MOV W1,(Address)
    MOV #0x00ff, W3
    AND W1,W3,W1
    AND W1,#0x1F,W3
    LSR W1, #5, W1
    SL W3,#11,W3
    MOV W1, RawData+2
    MOV W1, ret+2//TODO
    MOV RawData, W1
    LSR W1,#5,W1
    IOR W1,W3,W1
    MOV W1, RawData
    MOV W1, ret//TODO:
    MOV #0xff00, W0
    AND Address
    
    /* Set up the size of the filter order to loop */
    MOV ASM_FILTER_ORDER,W1
    DEC W1,W1
    SL W1,#2,W1
    
    /* Set W8 to point to the last word of the buffer array. */
    MOV Address, W2
    MOV #0x1100, W3
    MOV #0x1200,W4
    MOV #0x1300,W5
    MOV #0x1400,W10
    MOV #0x1500,W0
    CPSNE W2,W3
    MOV (&FilterBufferCh11), W8  //W8 contains the BaseAddress
    CPSNE W2,W4
    MOV (&FilterBufferCh12), W8  //W8 contains the BaseAddress
    CPSNE W2,W5
    MOV (&FilterBufferCh13), W8  //W8 contains the BaseAddress
    CPSNE W2,W10
    MOV (&FilterBufferCh14), W8  //W8 contains the BaseAddress
    CPSNE W2,W0
    MOV (&FilterBufferCh15), W8  //W8 contains the BaseAddress
    ADD W8,W1,W8
    ADD W8, #2,W8

    /* Set the W10 register to the last element of the weight array. */
    MOV (&Weight),W10
    LSR W1,W3
    ADD W10,W3,W10
    
    /* Clear the accumulators */
    CLR A
    CLR B
    
    /* Set the elements into the W3 and W4 and step away the W8 and W10 pointers. */
    MOV [W8--], W4
    MOV [W10--], W5

    /* This is a cycle that loop till the first element. In this cycle the buffer will be multiplied by the weight
     * the high word in the accumulator A and the low word in the accumulator B. During this the elements of the  
     * buffer will be shifted away. */
    MOV #4,W3
    LSR W1,#2,W1
    DEC W1,W1
    DO W1,loop
        MAC W4*W5, A, [W8]-=2, W4
        MOV [W8], [W8+W3]
        MAC W4*W5, B, [W8]-=2, W4, [W10]-=2, W5
        loop:
        MOV [W8], [W8+W3]
        
    /* Load the most fresh data in the front of the buffer. After that this will be multiplied too. */
    MOV (&RawData), W3
    MOV [W3++], [W8++]
    MOV [W3], [W8--]
    MOV [W3--], W4;
    MAC W4*W5, A, [W8], W4;
    MAC W4*W5, B
            
    /* The higher and the lower part of the output data will be merged. */
    SFTAC A,#-1
    SFTAC B,#15
    ADD A
            
    /* The accumulator read out and save into the RawData variable. */
    SAC A,#0,[++W3]
    SFTAC A, #-16
    SAC A,#0,[--W3]
    
    /* Load the working registers back from the stack. */
    POP W0
    POP W2
    POP W10
    POP W8
    POP W5
    POP W4
    POP W3
    POP W1
#endasm
    ret=(ret-RawData);
#asm
    MOV #0x00ff, W0
    AND ret+2
    MOV Address,W0
    IOR ret+2
#endasm
    return ret;
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
        uint32 Temporary=0x11000000;
        for(looper = 0; looper<6; looper++, BufferIterator++)
        {
#ifndef SIMULATOR
            Temporary = AdasSpiReadWrite(0x00000000);
#endif
            switch((uint16)(Temporary>>24))
            {
                case 0x11:
                    Message.Data.Data[BufferIterator] = ChangeEndiannes32Bit(FirWeighter(Temporary));
                    //Message.Data.Data[BufferIterator] = ChangeEndiannes32Bit((Temporary));
                    break;
                case 0x12:
                    Message.Data.Data[BufferIterator] = ChangeEndiannes32Bit(FirWeighter(Temporary));
                    //Message.Data.Data[BufferIterator] = ChangeEndiannes32Bit((Temporary));
                    break;
                case 0x13:
                    Message.Data.Data[BufferIterator] = ChangeEndiannes32Bit(FirWeighter(Temporary));
                    //Message.Data.Data[BufferIterator] = ChangeEndiannes32Bit((Temporary));
                    break;
                case 0x14:
                    Message.Data.Data[BufferIterator] = ChangeEndiannes32Bit(FirWeighter(Temporary));
                    //Message.Data.Data[BufferIterator] = ChangeEndiannes32Bit((Temporary));
                    break;
                case 0x15:
                    Message.Data.Data[BufferIterator] = ChangeEndiannes32Bit(FirWeighter(Temporary));
                    //Message.Data.Data[BufferIterator] = ChangeEndiannes32Bit((Temporary));
                    break;
                default:
                    Message.Data.Data[BufferIterator] = ChangeEndiannes32Bit((Temporary));
                    break;
            }
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
//write_extended_ram(1,0x100,&Weight,8);
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
    uint16 period=FILTER_ORDER;
    uint16 sample = FILTER_ORDER;
    for(looper=0;looper<FILTER_ORDER;looper++)
    {
        //BufferForWeight[looper]=1000+1000*sin(2*PI/period*looper);
        FilterBufferCh11[looper]=0x07ffff;
        //Weight[looper]=0x7fff;
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
