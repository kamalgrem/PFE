#include "config.c"        
#include <xc.h>

#define _XTAL_FREQ 4000000
#define Baud_rate 9600

char data,channel,charachter ;
int somme;

void ADC_Init();
unsigned int ADC_Read(unsigned int channel);
void ADC();
void UART_send_char(char bt);

void ADC_Init()
{
  ADCON0 = 0x81;               
  ADCON1 = 0x43;               
}

unsigned int ADC_Read( unsigned int channel)
{if(channel>2)
    channel+=1;
  if(channel > 7)              //Channel range is 0 ~ 7
    return 0;
 
  ADCON0 &= 0x85;              //Clearing channel selection bits
  ADCON0 |= channel<<3;
  ADCON1=0x43;
  //Setting channel selection bits
  
  __delay_ms(2);               //Actmaxuisition time to charge hold capacitor
  GO_nDONE = 1;                //Initializ1es A/D conversion
  while(GO_nDONE);             //Waiting for conversion to complete
  return (ADRESH); //Return result
}
void ADC(){
   
    for(int i=0; i<10;i++)
    {
         somme=somme+ADC_Read(channel);;
         __delay_ms(10);
    }
    data=somme/10;
    somme=0;
}

void Initialize_UART(void)
{

    TRISC6 = 0; // TX Pin set as output
    TRISC7 = 1; // RX Pin set as input
   
    

    SPBRG = ((_XTAL_FREQ/16)/Baud_rate) - 1;
    BRGH  = 1;  // for high baud_rate
  
    SYNC  = 0;    // Asynchronous
    SPEN  = 1;    // Enable serial port pins
   

    TXEN  = 1;    // enable transmission
    CREN  = 1;    // enable reception

    
 
    TX9   = 0;    
    RX9   = 0;   
       
}

void UART_send_char(char bt)  
{
    TXREG = bt; 
}

void main (void ){
    
    TRISB=0x00;
    TRISD=0x00;

    TRISC=0x00;
    
    Initialize_UART();    //Initialize UART module   
    ADC_Init();
    charachter=0x61;
    __delay_ms(600);        
    __delay_ms(100);
 
    while(1)
    {  
            for(channel=0;channel<4;channel++)
            {    
                ADC();
                for(int i=0;i<2;i++)
                {
                    UART_send_char(charachter);
                    __delay_ms(10); 
                    charachter++; 
                    if(charachter>0x68)
                        charachter=0x61;
                }
                UART_send_char(data);
                 __delay_ms(100);    
            }
      }

}