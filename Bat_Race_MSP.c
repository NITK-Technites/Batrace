#include <msp430G2231.h>
/*------]]]]-------------------------------------------
TECHNITES 2016
#include <msp430G2553.h>
------]]]]-------------------------------------------
TECHNITES 2016


                        MSP430G2553
                      -----------------
            3.3 V -->|Vcc          Gnd |<-- Ground
                     |                 |
          INT1    -->|P1.0         P2.7|--> unused
                     |                 |
             INT2 -->|P1.1         P2.6|--> unused
                     |                 |
            SCLK1 <--|P1.2         TST |--> Open
                     |                 |
            LATCH1<--|P1.3         RST |<-- Pulled high
                     |                 |
        SIN2      <--|P1.4         P1.7|--> SCLK2
                     |                 |
        SIN1      <--|P1.5         P1.6|--> LATCH2
                     |                 |
                  <--|P2.0         P2.5|<-- unused
                     |                 |
                    <--|P2.1         P2.4|<--
                     |                 |
                  <--|P2.2         P2.3|<--
                      -----------------
  -------------------------------------------------*/

//PIN 5916 PORT1
#define SIN1      BIT5
#define SCLK1    BIT2
#define LATCH1      BIT3
#define SIN2      BIT4
#define SCLK2    BIT7
#define LATCH2      BIT6
#define INT1      BIT0
#define INT2     BIT1

int count1=0,count2=0, cc1=0, cc2=0;
void InitializeClocks(void);

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;
    InitializeClocks();
    P1DIR=0xFC;
    P1OUT=0x03;
    P1REN=0x03;
    P1IE=0x03;
    P1IES=0x03; /* HIGH TO LOW*/
    P1IFG=0x00; //interrupt
    int i=0;

    while(1)                      // Setup clock
    {
    	if((cc1==0) && (cc2==0))
    {
    		for(i=0;i<56;i++)

        {   P1OUT&=~(SIN1+SIN2);
            P1OUT|=(SCLK1+SCLK2);
            P1OUT&=~(SCLK1+SCLK2);
        }
        
        P1OUT|=(SIN1+SIN2); //Next four statements to light first L.E.Ds of both tracks.
        P1OUT|=(SCLK1+SCLK2);
        P1OUT&=~(SCLK1+SCLK2);
        P1OUT&=~(SIN1+SIN2);
        P1OUT|=(LATCH1+LATCH2);
        P1OUT&=~(LATCH1+LATCH2);

        }


    	_BIS_SR(LPM1_bits+GIE);


}

}
#pragma vector = PORT1_VECTOR
__interrupt void Port1(void)
{
         if(P1IFG&0x01)
        {

            if(cc1<56)
            {

            count1=count1+1;
            if(count1 % 4==0)
            {
                P1OUT|=SCLK1;
                P1OUT&=~SCLK1;
                P1OUT|=LATCH1;
                P1OUT&=~LATCH1;
                //count1=0;
                cc1++;
            }
            }
            else
            {
            	int a = 0;
            	while(a<56)
            	{
            		P1OUT|=SIN1; //sending 1 to register SIN always give 1
            		P1OUT|=(SCLK1+SCLK2);
            		P1OUT&=~(SCLK1+SCLK2);
                    P1OUT|=(LATCH1+LATCH2);
                    P1OUT&=~LATCH1;
                    a++;
            	}
            	cc1=0;
            	cc2=0;
            	_delay_cycles(5000000);
            (*(void(**)(void))(0xfffe))(); //software reset
            }
          }
          if(P1IFG&0x02)
        {

        if(cc2<56)
        {
            count2=count2+1;
            if(count2%4==0)
            {   P1OUT|=SCLK2;
                P1OUT&=~SCLK2;
                P1OUT|=LATCH2;
                P1OUT&=~LATCH2;
                count2=0;
                cc2++;
          }
        }
        else
                    {
                    	int a = 0;
                    	while(a<56)
                    	{
                    		P1OUT|=SIN2; //sending 1 to register SIN always give 1
                    		P1OUT|=(SCLK1+SCLK2);
                    		P1OUT&=~(SCLK1+SCLK2);
                            P1OUT|=(LATCH1+LATCH2);
                            P1OUT&=~(LATCH1+LATCH2);
                            a++;
                    	}
                    	cc1=0;
                        cc2=0;
                        _delay_cycles(5000000);
                        (*(void(**)(void))(0xfffe))(); //software reset
                    }
        }

    P1IFG=0x00;
}


void InitializeClocks(void)
{
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL = CALDCO_1MHZ;
}
