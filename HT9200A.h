/*************************************************************************************************************
/*                                HT9200A Library                                                            *
/*************************************************************************************************************                            
Date             : 25/03/2017           
Project Name     : DTMF Dialer with Holtek HT9200A (Serial Mode) v1.0
Project Author   : Carlos Andres Vargas C. - carlos_vargas_c@outlook.com
Compiler         : CCS 5.70
MCU              : ---
Pins Description : 
                                             ___________      
      Chip On is low, Chip Off is high   CE-|1         8|- VDD   2.5V -5.5V 
      Oscillator 3.59MHZ                OSC-|2 HT9200A 7|- DTMF  DTMF Signal Output 0,45V-0,75V
      Oscillator 3.59MHZ                OSC-|3         6|- DATA  Bits D0,D1,D2,D3,D4 bits
      GND                               VSS-|4_________5|- CLK   Data synchronous clock
                           
Description      : HT9200A uses a 5-bit code, and a synchronous clock to generate a DTMF signal.
                   Each DTMF digit is made by serie of 5 bits. D0 is LSB (first received bit).
                   HT9200A will latch data on the falling edge of the clock (CLK pin).

Functions        :    
void init_HT9200A(void);              HT9200A Initialization
void DTMF_out(int8,int16,int16);      DTMF Tone to generate with time duration and time pause between digit
                                      pause in ms >= 1,  duration in ms >= 1  
void DialerNumber(char,int16,int16);  Dialer string numbers with time duration and time pause between digit

Library need:  string.h

*************************************************************************************************************/
#INCLUDE <string.h>
// Table code, D4 is the MSB
// Key                            Digit      D4 D3 D2 D1 D0
//                                1          00001
//                                2          00010
//                                3          00011
//                                4          00100
//                                5          00101
//                                6          00110
//                                7          00111
//                                8          01000
//                                9          01001
//                                0          01010
const int8 S_KEY    = 0x0B;    // *          01011              decimal 11
const int8 N_KEY    = 0x0C;    // #          01100              decimal 12
const int8 A_KEY    = 0x0D;    // A          01101              decimal 13
const int8 B_KEY    = 0x1E;    // B          01110              decimal 14
const int8 C_KEY    = 0x0F;    // C          01111              decimal 15
const int8 D_KEY    = 0x00;    // D          00000              decimal 0
const int8 DTMF_OFF = 0x1F;    // DTMF OFF   11111              decimal 31 (DTMF Sequence END)

// Frequencies 
#define HZ_697    16
#define HZ_770    17
#define HZ_852    18
#define HZ_941    19
#define HZ_1209   20
#define HZ_1336   21
#define HZ_1477   22
#define HZ_1633   23

// PINs MCU to HT9200A should be configured as OUT
#define  DATA_PIN    PIN_A0                          // DATA, PINOUT  Data Serial
#define  CLOCK_PIN   PIN_A1                          // CLOCK,PINOUT  CLK
#define  CE_PIN      PIN_A2                          // CE,   PINOUT  OFF/ON Chip HT9200A

char  DTMF_Code[20] = "";                            // String temporal
void init_HT9200A(void);                             // HT9200A Initialization
void DTMF_out(int8,int16,int16);                     // DTMF_OUT(tone,duration,pause)
                                                     // pause between DTMF codes in ms   >= 1
                                                     // duration DTMF tone  in ms        >= 1  
void DialerNumber(char,int16,int16);                 // DialerNumber(string,duration,pause)
//----------------------------------------------------------------------------
// Configuration HT9200A...
//----------------------------------------------------------------------------
void init_HT9200A(){
   output_high(CE_PIN);                               // CE Turn Off chip
   delay_ms(10);                                      // Wait 10ms for oscillator stabilization
   output_high(CLOCK_PIN);                            // Set CLK high before startup
   output_low(CE_PIN);                                // CE turn chip ON
   delay_us(5);                                       // 1/2 of 100K Clock speed, Wait 5us
}
//----------------------------------------------------------------------------
// Dialer Only One Tone...
//----------------------------------------------------------------------------
void DTMF_out(int8 digit,int16 duration,int16 pause){ 
  int i=0;                                            //
  if (digit == 0) digit = 10;                         // Take care of 0 here
 
  init_HT9200A();                                     // HT9200A Initialization
 //****** SendDigit ********  
  for(i=0;i<=5;i++) {                                 // Loop for 5 bits for each digit
      output_high(CLOCK_PIN);                         // Set CLK high
      if(bit_test(digit,i))                           // Set data LSB->MSB
       {
         output_high(DATA_PIN);
       } 
      else
       {
        output_low(DATA_PIN);
      }  
      delay_us(5);                                    // 1/2 of 100K Clock speed, Wait 5us
      output_low(CLOCK_PIN);                          // CLK Disable
      delay_us(5);                                    // 1/2 of 100K Clock speed, Wait 5us
  }
  //****** Duration Tone **********
  delay_ms(duration);                                 // Sound duration of digit
  //****** StopBits ***************
  for(i=0;i<=5;i++) {                                 // Loop for 5 bits for each digit
      output_high(CLOCK_PIN);                         // Set CLK high
      if(bit_test(DTMF_OFF,i))                        // set data LSB->MSB
      {
      output_high(DATA_PIN);                          // If bitx =1, put data=1
      } 
      else
      { 
      output_low(DATA_PIN);                           // If bitx =0, Put data=0
      }  
      delay_us(5);                                    // 1/2 of 100K Clock speed, Wait 5us
      output_low(CLOCK_PIN);                          // Set CLK low
      delay_us(5);                                    // 1/2 of 100K Clock speed, Wait 5us
  } 
  //*** Chip disable *** 
  output_high(CE_PIN);                                // CE Turn Off chip, this is very important
  //*** Pause *********************
  delay_ms(pause);                                    // Pause between DTMF codes
}
//----------------------------------------------------------------------------
// Dialer Number...
//----------------------------------------------------------------------------
void DialerNumber(char numbers[],int16 duration,int16 pause){
int i,lenN;
    strcpy(DTMF_code, numbers);                        // Copy input DTMF code sequence from string numbers
    lenN = strlen(DTMF_code);                          // Calculates the length of the DTMF_Code
    for(i=0;i<=lenN-1;i++){                            // Loop for each DTMF code
      DTMF_out((DTMF_code[i]-'0'),duration,pause);     // To each character subtract 0x30 hex, (Tone=Char-0x30)  
   }
}
