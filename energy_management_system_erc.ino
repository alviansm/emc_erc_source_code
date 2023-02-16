#include <TouchScreen.h> //touch library
#include <LCDWIKI_GUI.h> //Core graphics library
#include <LCDWIKI_KBV.h> //Hardware-specific library
#include<KRrtc.h> // RTC DS1307 Module library

//====DISPLAY CODE====
#define TOUCH_ORIENTATION  1 //1 for landscape
#define TITLE "Energy Management System Dashboard"

LCDWIKI_KBV my_lcd(ILI9341,A3,A2,A1,A0,A4); //model,cs,cd,wr,rd,reset
#define YP A1  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 7   // can be a digital pin
#define XP 6   // can be a digital pin

//param calibration from kbv
//touch sensitivity for X
#define TS_MINX 124
#define TS_MAXX 906

//touch sensitivity for Y
#define TS_MINY 83
#define TS_MAXY 893

#define MINPRESSURE 10
#define MAXPRESSURE 1000

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

//human readable color
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

TouchScreen mytouch(XP, YP, XM, YM, 300);
TSPoint tp;                      //Touchscreen_due branch uses Point

//variables
uint32_t cx, cy;
uint32_t rx[8], ry[8];
int32_t clx, crx, cty, cby;
float px, py;
int dispx, dispy, text_y_center, swapxy;
uint32_t calx, caly, cals;
char buf[13];
char page = '0';
int randNm;

//====DISPLAY PAGES FUNCTIONS====
void show_string(uint8_t *str,int16_t x,int16_t y,uint8_t csize,uint16_t fc, uint16_t bc,boolean mode)
{
    my_lcd.Set_Text_Mode(mode);
    my_lcd.Set_Text_Size(csize);
    my_lcd.Set_Text_colour(fc);
    my_lcd.Set_Text_Back_colour(bc);
    my_lcd.Print_String(str,x,y);
}

void Read_Resistive(void)
{
    tp = mytouch.getPoint();
    pinMode(YP, OUTPUT);      //restore shared pins
    pinMode(XM, OUTPUT);
    digitalWrite(YP, HIGH);   //because TFT control pins
    digitalWrite(XM, HIGH);
}

boolean is_pressed(int16_t x1,int16_t y1,int16_t x2,int16_t y2,int16_t px,int16_t py)
{
    if((px > x1 && px < x2) && (py > y1 && py < y2))
    {
        return true;  
    } 
    else
    {
        return false;  
    }
}

//==================HELPER FUNCTIONS=======================
void draw_square_info() {

}

void draw_menu_button() {
  
}

//startup page to main dashboard
void draw_home_screen()
{
    my_lcd.Fill_Screen(BLUE);
    // CIRCLE ACTIVE COLOR LIGHT BLUE (186, 230, 253)
    // CIRCLE INACTIVE COLOR LIGHT GRAY (203, 213, 225)
  
    //display date
    show_string("22/09/22", LEFT, 6, 2, WHITE, BLACK, 1);
    show_string("10:21:45", LEFT, 32, 2, WHITE, BLACK, 1);

    //display status
    my_lcd.Set_Draw_color(220, 255, 255);
    my_lcd.Fill_Circle(288, 32, 16);
    show_string("COMP", 278, 4, 1, WHITE, BLACK, 1);

    my_lcd.Set_Draw_color(220, 255, 255);
    my_lcd.Fill_Circle(246, 32, 16);
    show_string("COND", 238, 4, 1, WHITE, BLACK, 1);

    my_lcd.Set_Draw_color(220, 255, 255);
    my_lcd.Fill_Circle(206, 32, 16);
    show_string("EVAP", 198, 4, 1, WHITE, BLACK, 1);

    my_lcd.Set_Draw_color(220, 255, 255);
    my_lcd.Fill_Circle(166, 32, 16);
    show_string("TXV", 158, 4, 1, WHITE, BLACK, 1);

    my_lcd.Set_Draw_color(155, 0, 0);
    my_lcd.Fill_Circle(126, 32, 16);
    show_string("IoT", 118, 4, 1, WHITE, BLACK, 1);

    //===========================monitoring square=====================
    my_lcd.Set_Draw_color(220, 255, 255);
    my_lcd.Fill_Rectangle(4, 64, 68, 96);
    //uint8_t coba_random = coba;
    //Serial.println(coba_random);
    show_string("38", 24, 74, 2, BLACK, BLACK, 1);
    show_string("AMBIENT (C)", 5, 102, 1, WHITE, BLACK, 1);

    my_lcd.Set_Draw_color(220, 255, 255);
    my_lcd.Fill_Rectangle(87, 64, 151, 96);
    show_string("20", 104, 74, 2, BLACK, BLACK, 1);
    show_string("INSIDE (C)", 92, 102, 1, WHITE, BLACK, 1);

    my_lcd.Set_Draw_color(220, 255, 255);
    my_lcd.Fill_Rectangle(170, 64, 234, 96);
    show_string("0,6", 182, 74, 2, BLACK, BLACK, 1);
    show_string("POWER (kW)", 174, 102, 1, WHITE, BLACK, 1);
    
    my_lcd.Set_Draw_color(220, 255, 255);
    my_lcd.Fill_Rectangle(252, 64, 314, 96);
    show_string("2,44", 262, 74, 2, BLACK, BLACK, 1);
    show_string("COP", 272, 102, 1, WHITE, BLACK, 1);

    //set point
    my_lcd.Set_Draw_color(220, 255, 255);
    my_lcd.Fill_Rectangle(87, 128, 151, 160);
    show_string("-20", 104, 140, 2, BLACK, BLACK, 1);
    show_string("SET POINT (C)", 84, 166, 1, WHITE, BLACK, 1);
    
    //pcm
    my_lcd.Set_Draw_color(220, 255, 255);
    my_lcd.Fill_Rectangle(170, 128, 234, 160);
    show_string("-20", 182, 140, 2, BLACK, BLACK, 1);
    show_string("PCM (C)", 180, 166, 1, WHITE, BLACK, 1);
    
    //Eco Activated
    show_string("ECO MODE", 4, 200, 2, GREEN, BLACK, 1);
    show_string("Activated", 4, 220, 1, WHITE, BLACK, 1);
    
    //button goto setting
    my_lcd.Set_Draw_color(0, 255, 255);
    my_lcd.Fill_Rectangle(320, 240, 246, 208);    
    show_string("->", 270, 220, 2, BLACK, BLACK, 2);
}

void show_setting_menu() {    
    my_lcd.Fill_Screen(BLUE);
    
    show_string("Menu Utama", 110, 16, 2, WHITE, BLACK, 1);

    //goto wifi setting
    my_lcd.Set_Draw_color(220, 255, 255);
    my_lcd.Fill_Rectangle(32, 48, 288, 80);
    show_string("Pengaturan Wi-FI", 64, 56, 2, BLACK, BLACK, 1);
    
    //goto 

    //button back to dashboard
    my_lcd.Set_Draw_color(220, 255, 255);
    my_lcd.Fill_Rectangle(0, 208, 74, 240);
    show_string("<-", 25, 220, 2, BLACK, BLACK, 2);
}

/*
 * Page
 * 0 -> Dashboard
 * 1 -> Main Menu
 */
void setup() {
  Serial.begin(9600);

  //dashboard setting
  Serial.println(TITLE);
  digitalWrite(A0, HIGH);
  pinMode(A0, OUTPUT);
  my_lcd.Init_LCD();
  my_lcd.Set_Rotation(TOUCH_ORIENTATION);  

  draw_home_screen();
}

void loop() {
  uint16_t i;
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  pinMode(XM, OUTPUT); 
  pinMode(YP, OUTPUT);
  
  //page naviagtion looping
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
     p.x = map(p.x, TS_MINX, TS_MAXX, 0, my_lcd.Get_Display_Width());
     p.y = map(p.y, TS_MINY, TS_MAXY, 0, my_lcd.Get_Display_Height());
     Serial.print(p.x);
     Serial.print(",");
     Serial.print(p.y);     
     Serial.println();     
     if (is_pressed(0, 180, 64, 240, p.x, p.y) && page=='0'){        
        //button goto main menu (page 1) - pressed
        my_lcd.Set_Draw_color(55, 55, 55);
        my_lcd.Fill_Rectangle(320, 240, 246, 208);  
        show_string("->", 270, 220, 2, WHITE, BLACK, 2);          
        delay(200);
        //back to semula -> ganti ke menu
        show_setting_menu();
        page = '1';
        Serial.println(page);
        //my_lcd.Set_Draw_color(0, 255, 255);
        //my_lcd.Fill_Rectangle(320, 240, 246, 208); 
     }
     if (is_pressed(0, 0, 64, 64, p.x, p.y) && page=='1') {
        //button goto dashboard (page 0) - pressed
        my_lcd.Set_Draw_color(55, 55, 55);
        my_lcd.Fill_Rectangle(0, 208, 74, 240);
        show_string("<-", 25, 220, 2, WHITE, BLACK, 2);
        delay(200);
        draw_home_screen();
        page = '0';
     }
  }  
}
