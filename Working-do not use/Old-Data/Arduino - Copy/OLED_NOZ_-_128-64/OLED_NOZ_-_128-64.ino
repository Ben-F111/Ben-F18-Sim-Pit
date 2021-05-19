#include "U8glib.h"
U8GLIB_SH1106_128X64 u8g(13, 11, 10, 9); // SCK = 13, MOSI = 11, CS = 10, A0 = 9 
int xmax=128;                                  
int ymax=64;                                   
int xcenter=xmax/2;                            
int ycenter=ymax/2+30;                        
int arc=ymax/1.25;                             
int angle=0;                                   
                   
#define potmeterPin A0                                
int p, w, m,a=10;
u8g_uint_t xx=0;
void gauge(uint8_t angle) {
 
  float x1=sin(2*angle*2*3.14/360);           
  float y1=cos(2*angle*2*3.14/360); 
  u8g.drawLine(xcenter, ycenter, xcenter+arc*x1, ycenter-arc*y1);
  //u8g.setFont(u8g_font_chikita); 
  //  u8g.setFont(u8g_font_profont22);             
  u8g.setPrintPos(54,64);
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
 m = map(p,0,1023,25,75);                      
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
