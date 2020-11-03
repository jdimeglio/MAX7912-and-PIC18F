#include <18F4620.h>
#fuses HS,NOLVP,NOWDT
#use delay (clock=40M)
#use fast_io(D)
#USE SPI (MASTER, SPI1, BAUD=1000000, MODE=0, BITS=16, LOAD_ACTIVE=1, STREAM=SPI_1, MSB_FIRST,IDLE=1)

#include <max7912.c>                             //my driver file :-)


#define Chip_Select PIN_D0      //this is the pin on the PIC the LOAD must be connected to.
#define Chip_Select_Direction tris_d(0)     

// Main function.
VOID main()
{
   InitMax7219 ();

   DO // infinite loop.
  {
     // You can write the characters this way, one at a time.

       WriteChar7219("J");
       delay_ms(50);
       WriteChar7219("O");
       delay_ms(50);
      WriteChar7219("E");
      delay_ms(50);
   }WHILE (1); // do forever.
}
