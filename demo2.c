#include <18F4620.h>
#fuses HS,NOLVP,NOWDT
#use delay (clock=40M)
#use fast_io(D)
#USE SPI (MASTER, SPI1, BAUD=1000000, MODE=0, BITS=16, LOAD_ACTIVE=1, STREAM=SPI_1, MSB_FIRST,IDLE=1)
#include <max7912.c>

#define Chip_Select PIN_D0
#define Chip_Select_Direction tris_d(0)

byte VR[8];                           //Video RAM used to draw the screen before sending to the 8x8 matrix

int x=5 ,y=0;                         // starting position of ball one
int xdir=1, ydir=1;                 //the director ball one will go

int x1=0 ,y1=3;                    //starting poistion of ball two
int x1dir=1, y1dir=-1;          //its starting position


//clear the "screen" - its the video ram thats getting cleared
void grx_cls(void)
{
   byte x;
   for (x=0;x<8; x++)
  {
     VR[x] = 0;
   }
}


//send it to the hardware
void grx_display(void)
{
   byte Addr, Row=0;
   FOR (Addr = 1; Addr < 9; Addr++)
   {
     send7219 (Addr, VR[row++]);
   }
}

//light up that LED at x,y position
void grx_setxy(byte x,y)
{
   byte temp;
   temp = 1;
   temp <<=x;
   VR[y]|=temp;        // read row X at column Y
}

//turn off that LED at x,y position
void grx_unsetxy(byte x,y)
{
    byte temp;
    temp = 1;
    temp <<=x;
    temp = 255 - temp;
    VR[y]&=temp;     // read row X at column Y
}



//Write CHAR to the screen
VOID grx_WriteChar(byte myChar)
{
     byte Column, Start_Byte;
    Start_Byte = (myChar - 65) * 6; // 65 represents the letter "A" in ASCII code.

    // We are using only columns from 2 through 7 FOR displaying the character.
    FOR (Column = 7; Column > 1; Column--)
   {
      VR[Column]=Alphabet[Start_Byte++];
   }
}

//scroll left the contents in the video ram
void grx_scroll(byte x,clip,dir)
{
    byte scroll,edge;
    edge=VR[7];                 //save last row

   for (scroll=7;scroll>0;scroll--)
   {
      VR[scroll]=VR[scroll-1];
   }
   VR[0]=edge;                 //put in the first row
}




//anitmate the bouncing balls.
void bouncing(void)
{
     grx_setxy(x,y);      //draw the ball one
     grx_setxy(x1,y1);  //draw ball two
     grx_display();        //display it
     Delay_ms (5) ;
     grx_unsetxy(x,y);   //turn them off
     grx_unsetxy(x1,y1);
     x= x + xdir;           //new position
     y =y + ydir;
     if (x>=7||x<=0)     //check if they hit the edges if so change direction.
    {
       xdir=-xdir;
    }

    if (y>=7||y<=0)
   {
     ydir=-ydir;
    }

    x1= x1 + x1dir;     //same for ball two
    y1 =y1 + y1dir;
    if (x1>=7||x1<=0)
   {
      x1dir=-x1dir;
   }
   if (y1>=7||y1<=0)
  {
     y1dir=-y1dir;
  }
}

//demo scrolling left
VOID demoone(VOID)
{

    grx_scroll(1,0,0);
    delay_ms(5);
    grx_display(); //display it

}

 VOID demotwo(VOID)
{
   bouncing();
}


// Here we have the main function.
VOID main()
{

    Delay_ms(100);
    InitMax7219 ();

    grx_cls(); // clear RAM
    grx_display(); //display it
    grx_WriteChar('X');
    grx_display(); //display it

   DO // infinite loop.
   {
   // You can write the characters this way, one at a time.

    demoone();
    //demotwo();

  
   }WHILE (1); // do forever.
}
