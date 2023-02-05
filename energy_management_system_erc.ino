#include <TouchScreen.h> //touch library
#include <LCDWIKI_GUI.h> //Core graphics library
#include <LCDWIKI_KBV.h> //Hardware-specific library

//====DISPLAY CODE====
#define TOUCH_ORIENTATION  1 //1 for landscape
#define TITLE "Energy Management System Dashboard"

LCDWIKI_KBV my_lcd(ILI9341,A3,A2,A1,A0,A4); //model,cs,cd,wr,rd,reset
#define YP A1  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 7   // can be a digital pin
#define XP 6   // can be a digital pin

//param calibration from kbv
#define TS_MINX 124
#define TS_MAXX 906

#define TS_MINY 83  
#define TS_MAXY 893

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

bool is_pressed(void)
{
    bool state;
    Read_Resistive();
    state = (tp.z > 20 && tp.z < 1000);
    return state;
}

void waitForTouch()
{
    while (is_pressed() == true) {}
    while (is_pressed() == false) {}
    while (is_pressed() == true) {}
}

//startup page to main dashboard
void startup()
{
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

    my_lcd.Set_Draw_color(55, 55, 55);
    my_lcd.Fill_Circle(126, 32, 16);
    show_string("IoT", 118, 4, 1, WHITE, BLACK, 1);

    //monitoring square

    //button goto setting
    my_lcd.Set_Draw_color(0, 255, 255);
    my_lcd.Fill_Rectangle(320, 240, 246, 208);
    my_lcd.Set_Draw_color(0, 0, 0);
    my_lcd.Draw_Line(320, 208, 246, 208);
    my_lcd.Draw_Line(320, 238, 246, 238);
    my_lcd.Draw_Line(245, 208, 245, 238);
}

void setting() {    
    show_string("Welcome to setting", 118, 4, 1, WHITE, BLACK, 1);
}

void setup() {
  Serial.begin(9600);
  Serial.println(TITLE);
  digitalWrite(A0, HIGH);
  pinMode(A0, OUTPUT);
  my_lcd.Init_LCD();
  my_lcd.Set_Rotation(TOUCH_ORIENTATION);
  my_lcd.Fill_Screen(BLUE);
  dispx = my_lcd.Get_Display_Width();
  dispy = my_lcd.Get_Display_Height();
  text_y_center = (dispy/2) - 6;
}

void loop() {
  // variables or data to be update in startup
  // int x = 5;
  if (page == '0') {
    Serial.println("Go to page 0");
    startup();
  }   else if (page == '1') {
    Serial.println("Go to page 1");    
    setting();
  }
  
  delay(1000);
}
