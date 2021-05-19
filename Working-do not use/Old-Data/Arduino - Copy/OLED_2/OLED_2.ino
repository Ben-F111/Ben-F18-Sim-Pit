#include "U8glib.h"
U8GLIB_SH1106_128X64 u8g(13, 11, 10, 9); // SCK = 13, MOSI = 11, CS = 10, A0 = 9 
int xmax=128;                                  
int ymax=62;                                   
int xcenter=xmax/2;                            
int ycenter=ymax/2+10;                        
int arc=ymax/2;                             
int angle=0;                                   
char* label[] = {"LOAD","COOLANT","INTAKE", "VOLT"};  
int labelXpos[] = {53, 45, 49, 53};                     
#define potmeterPin A1                                
int p, w, m,a=10;
u8g_uint_t xx=0;
void gauge(uint8_t angle) {
  // draw border of the gauge
//  u8g.drawCircle(xcenter,ycenter,arc+6, U8G_DRAW_UPPER_RIGHT);
 // u8g.drawCircle(xcenter,ycenter,arc+4, U8G_DRAW_UPPER_RIGHT);
 // u8g.drawCircle(xcenter,ycenter,arc+6, U8G_DRAW_UPPER_LEFT);
//  u8g.drawCircle(xcenter,ycenter,arc+4, U8G_DRAW_UPPER_LEFT);
  // draw the needle
  float x1=sin(2*angle*2*3.14/360);           
  float y1=cos(2*angle*2*3.14/360); 
  u8g.drawLine(xcenter, ycenter, xcenter+arc*x1, ycenter-arc*y1);
 // u8g.drawDisc(xcenter, ycenter, 5, U8G_DRAW_UPPER_LEFT);
//  u8g.drawDisc(xcenter, ycenter, 5, U8G_DRAW_UPPER_RIGHT);
  u8g.setFont(u8g_font_chikita); 
  // show scale labels
  u8g.drawStr( 20, 42, "0");                   
 u8g.drawStr( 25, 18, "50");
  u8g.drawStr( 60, 14, "100");
  //u8g.drawStr( 95, 18, "75");
// u8g.drawStr( 105, 42, "100"); 
  // show gauge label
  //u8g.setPrintPos(labelXpos[0],32);            
  //u8g.print(label[0]); 
  // show digital value and align its position
  u8g.setFont(u8g_font_profont22);             
  u8g.setPrintPos(54,60);
 // if (w<10){                              
 //   u8g.print("0");
// }
 // if (w>99) {                                  
 //   u8g.setPrintPos(47,60);
 // }
//  u8g.print(w);
}
void setup(void) {
  u8g.setFont(u8g_font_chikita);
  u8g.setColorIndex(1);                        
  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);                    
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);                      
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);                      
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
}
void loop(void) { 
   p = analogRead(A0);                  
 w = map(p,0,1023,0,100);                    
 m = map(p,0,1023,0,51);                      
  // show needle and dial
  xx = m;                                    
  if (xx<45){                                
    xx=xx+135;
  }
  else {
    xx=xx-45;
  } 
  // picture loop
  {
    u8g.firstPage(); 
    do {             
      gauge(xx);
    }
    while( u8g.nextPage() );
  }
}
