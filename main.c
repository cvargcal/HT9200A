#include <18F26K22.h>
#Device  PASS_STRINGS=IN_RAM  
#ZERO_RAM     
#fuses NOWDT, BROWNOUT, PUT, NOPBADEN, NOHFOFST,NOPLLEN,NOMCLR,PROTECT
#fuses HSH                                                                        // High speed Osc, high power 16MHz-25MHz
//#fuses INTRC_IO
//#use delay(clock=64M, internal=16MHz)  
#use delay(clock=20M) 
#use fast_io(A) 
#use fast_io(B) 
#use rs232(baud=9600,bits=8,parity=N,xmit=PIN_C6,rcv=PIN_C7,ERRORS,stream=debug)  // Configuración UART1 (Hardware)
#use rs232(baud=9600,bits=8,parity=N,xmit=PIN_B6,rcv=PIN_B7,ERRORS,stream=uart2)  // Configuración UART2 (Hardware)
#define FASTER_BUT_MORE_ROM                                                       // Variables en memoria
#include "HT9200A.h"                                                              // HT9200.h  defines for HT9200 DTMF chip in serial mode 

void main(){
   set_tris_a(0b11100000);
   output_a(0x00);
   output_c(0);
   
   while(1){
    output_toggle(PIN_C1);                                       // Run MCU 
    DialerNumber("123456789012",50,1);                           // <<-- DTMF code sequence
    delay_ms(3000);
    DTMF_out(1,50,1); fprintf(debug,"1 = 697, 1209\r\n");        // digito 1
    DTMF_out(2,50,1); fprintf(debug,"2 = 697, 1336\r\n");        // digito 2 
    DTMF_out(3,50,1); fprintf(debug,"3 = 697, 1477\r\n");        // digito 3
    DTMF_out(4,50,1); fprintf(debug,"4 = 770, 1209\r\n");        // digito 4 
    DTMF_out(5,50,1); fprintf(debug,"5 = 770, 1336\r\n");        // digito 5 
    DTMF_out(6,50,1); fprintf(debug,"6 = 770, 1477\r\n");        // digito 6 
    DTMF_out(7,50,1); fprintf(debug,"7 = 852, 1209\r\n");        // digito 7 
    DTMF_out(8,50,1); fprintf(debug,"8 = 852, 1336\r\n");        // digito 8 
    DTMF_out(9,50,1); fprintf(debug,"9 = 852, 1477\r\n");        // digito 9
    DTMF_out(0,50,1); fprintf(debug,"0 = 941, 1209\r\n");        // digito 0
    DTMF_out(S_KEY,50,1); fprintf(debug,"* = 941, 1336\r\n");    // digito *
    DTMF_out(N_KEY,50,1); fprintf(debug,"# = 941, 1477\r\n");    // digito #
    DTMF_out(A_KEY,50,1); fprintf(debug,"A\r\n");                // digito A
    DTMF_out(B_KEY,50,1); fprintf(debug,"B\r\n");                // digito B
    DTMF_out(C_KEY,50,1); fprintf(debug,"C\r\n");                // digito C
    DTMF_out(D_KEY,50,1); fprintf(debug,"D\r\n");                // digito D    
   }
}
   




