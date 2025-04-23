#include "sharedVar.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

int xCursorPressure;
int xCursorID;
int xCursorArea;
int xCursorData;
int xCursorMail;
int xCursorCutoff;
int xCursorUpdate;
int xCursorMAC;
int xCursorTitle;
int xCursorSSID;
int xCursorPWD;
int xCursorSens;
int xCursorWIFI;
int startView=10;
int startSend=36;
int startSync=36;
int loadingSend=0;
int loadingSync=0;
bool calibrated=false;
bool credUpdated=false;

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

static const unsigned char PROGMEM load_bmp[] = {
  0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 
	0x01, 0xff, 0xc0, 0x00, 0x00, 0x07, 0xff, 0xf0, 0x00, 0x00, 0x07, 0xff, 0xfc, 0x00, 0x00, 0x07, 
	0xe3, 0xfe, 0x00, 0x00, 0x07, 0xc1, 0xff, 0x80, 0x03, 0x07, 0xc0, 0x7f, 0xe0, 0x0f, 0x07, 0xc0, 
	0x1f, 0xf8, 0x1f, 0x07, 0xc0, 0x07, 0xfc, 0x1f, 0x07, 0xc0, 0x03, 0xfc, 0x1f, 0x07, 0xc8, 0x00, 
	0xfc, 0x1f, 0x07, 0xfe, 0x00, 0x7c, 0x1f, 0x07, 0xff, 0x80, 0x7c, 0x1f, 0x07, 0xff, 0xe0, 0x7c, 
	0x1f, 0x07, 0xff, 0xe0, 0x7c, 0x1f, 0x07, 0xff, 0xe0, 0x7c, 0x1f, 0x07, 0xff, 0xe0, 0x7c, 0x1f, 
	0x07, 0xff, 0xe0, 0x7c, 0x1f, 0x07, 0xff, 0xe0, 0x7c, 0x1f, 0x07, 0xff, 0xe0, 0x7c, 0x1f, 0x07, 
	0xff, 0xe0, 0x7c, 0x1f, 0x07, 0xff, 0xe0, 0x7c, 0x1f, 0x07, 0xff, 0xe0, 0x7c, 0x1f, 0x01, 0xff, 
	0xc0, 0x7c, 0x1f, 0x00, 0xff, 0x00, 0x7c, 0x1f, 0x00, 0x3e, 0x00, 0x7c, 0x1f, 0x80, 0x08, 0x01, 
	0xfc, 0x1f, 0xe0, 0x00, 0x07, 0xfc, 0x0f, 0xf8, 0x00, 0x1f, 0xf8, 0x03, 0xfe, 0x00, 0x3f, 0xe0, 
	0x01, 0xff, 0x00, 0xff, 0x80, 0x00, 0x7f, 0xc3, 0xff, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x00, 
	0x07, 0xff, 0xf0, 0x00, 0x00, 0x03, 0xff, 0xc0, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 
	0x3e, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00
  };

static const unsigned char PROGMEM alert_bmp[] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x7e, 0x00, 0x00, 
	0xff, 0x00, 0x01, 0xff, 0x80, 0x01, 0xc7, 0x80, 0x03, 0xc3, 0xc0, 0x07, 0xc3, 0xe0, 0x07, 0xc3, 
	0xe0, 0x0f, 0xe7, 0xf0, 0x0f, 0xe7, 0xf0, 0x1f, 0xe7, 0xf8, 0x3f, 0xe7, 0xfc, 0x3f, 0xff, 0xfc, 
	0x7f, 0xff, 0xfe, 0xff, 0xe7, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 
	0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };

  
static const unsigned char PROGMEM send_bmp[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x07, 0xc0, 0x00, 0x0f, 0xf0, 0x00, 0x1f, 0xf8, 0x07, 0xff, 0xf8, 0x0f, 0xff, 0xfc, 0x1f, 0xff, 
	0xfc, 0x1f, 0xe7, 0xfc, 0x3f, 0xc3, 0xfc, 0x7f, 0xc3, 0xfe, 0x7f, 0x81, 0xfe, 0xff, 0x00, 0xff, 
	0x7f, 0x00, 0xfe, 0x7f, 0xc3, 0xfe, 0x3f, 0xc3, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };


static const unsigned char PROGMEM sync_bmp[] = {
	0x00, 0x0c, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x0f, 0x80, 0x00, 0xff, 0xe0, 0x03, 0xff, 0xf0, 0x07, 
	0xff, 0xe0, 0x0f, 0xff, 0xc0, 0x0f, 0x8f, 0x00, 0x1f, 0x0c, 0x00, 0x1e, 0x00, 0x38, 0x1e, 0x00, 
	0x78, 0x1e, 0x00, 0x78, 0x1e, 0x00, 0x78, 0x1e, 0x00, 0x78, 0x1c, 0x00, 0x78, 0x18, 0x30, 0xf8, 
	0x00, 0xf1, 0xf0, 0x01, 0xff, 0xf0, 0x07, 0xff, 0xe0, 0x0f, 0xff, 0xc0, 0x03, 0xff, 0x00, 0x01, 
	0xf8, 0x00, 0x00, 0x70, 0x00, 0x00, 0x30, 0x00
  };


static const unsigned char PROGMEM update_bmp[] = {
	0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x3c, 0x00, 0x0d, 0xff, 0xb0, 0x1f, 0xff, 0xf0, 0x1f, 
	0xcf, 0xf8, 0x0f, 0x0f, 0xf0, 0x0e, 0x3f, 0xf0, 0x1c, 0xff, 0xf8, 0x1c, 0xff, 0xf8, 0x79, 0xff, 
	0xbe, 0x79, 0xff, 0x1e, 0x79, 0xfe, 0x0e, 0x78, 0xff, 0x3e, 0x1c, 0xff, 0x38, 0x1e, 0x7e, 0x78, 
	0x0e, 0x18, 0x70, 0x0f, 0x81, 0xf0, 0x1f, 0xe7, 0xf8, 0x0f, 0xff, 0xf0, 0x04, 0xff, 0x20, 0x00, 
	0x3c, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00
  };

static const unsigned char PROGMEM signal_bmp[] = {
	0x1f, 0x80, 0x7d, 0xe0, 0xc0, 0x30, 0x9f, 0x90, 0x39, 0xc0, 0x20, 0x40, 0x0f, 0x00, 0x09, 0x00, 
	0x00, 0x00, 0x06, 0x00, 0x06, 0x00, 0x00, 0x00
  };

  
static const unsigned char PROGMEM signalLost_bmp[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x06, 0x00, 0x06, 0x00, 0x00, 0x00
};

static const unsigned char PROGMEM online_bmp[] = {
	0x06, 0x00, 0x0f, 0x00, 0x1f, 0x80, 0x1f, 0x80, 0x3f, 0xc0, 0x3f, 0xc0, 0x3f, 0xc0, 0x3f, 0xc0, 
	0x3f, 0xc0, 0x7f, 0xe0, 0x7f, 0xe0, 0x06, 0x00
  };

static const unsigned char PROGMEM offline_bmp[] = {
	0x04, 0x00, 0x4e, 0x00, 0x27, 0x80, 0x33, 0x80, 0x39, 0xc0, 0x3c, 0xc0, 0x3e, 0x40, 0x3f, 0x00, 
	0x3f, 0x80, 0x7f, 0xc0, 0x06, 0x20, 0x06, 0x00
  };


static const unsigned char PROGMEM auto_bmp[] = {
	0x06, 0x00, 0x06, 0x00, 0x1f, 0x80, 0x34, 0xc0, 0x2c, 0x40, 0x5c, 0x20, 0x5c, 0x20, 0x5e, 0x20, 
	0x5f, 0xa0, 0x2f, 0x40, 0x10, 0x80, 0x0f, 0x00
  };
void setupShow(){
  Serial.begin(115200);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);   
  display.clearDisplay();
  display.setCursor(0,0);
  loadView();
}



void show(String text){
  display.setCursor(display.getCursorX(),display.getCursorY());
  display.print(text);
  display.display();
}

void showNew(String text){  
  display.clearDisplay();   
  display.setCursor(0,0);
  display.setTextSize(1);
  display.print(text);
  display.display();
}

void showMode(){  
  if (!btnHold){ 
    defaultView();
  }else{
    updateView();
  }
  display.display();
}



void defaultView(){
  display.clearDisplay(); 

  //centering text
  if(WiFi.status() == WL_CONNECTED){
    display.drawBitmap(112,0,signal_bmp, 12, 12, 1);
  }else{
    display.drawBitmap(112,0,signalLost_bmp, 12, 12, 1);
  }
  display.setTextSize(1);
  for(int i=0;i<128;i++){
    display.drawPixel(i,15,SSD1306_WHITE);
    display.drawPixel(i,39,SSD1306_WHITE);
    display.drawPixel(i,63,SSD1306_WHITE);
  }for(int j=15;j<64;j++){
    display.drawPixel(0,j,SSD1306_WHITE);
    display.drawPixel(42,j,SSD1306_WHITE);
    display.drawPixel(84,j,SSD1306_WHITE);
    display.drawPixel(127,j,SSD1306_WHITE);
  }


  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  display.setCursor(3,0);
  display.print(ip);
  if(BPMS0001S.equals("Yes")){
    if(flashState){
      for(int i=0;i<42;i++){
        for(int j=15;j<39;j++){
        display.drawPixel(i,j,SSD1306_WHITE);
        }
      }
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.setCursor(3,18);
      display.println(BPMS0001A);
      display.setCursor(3,31);
      display.println(BPMS0001P);
    }else{
      display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
      display.setCursor(3,18);
      display.println(BPMS0001A);
      display.setCursor(3,31);
      display.println(BPMS0001P);
    }
  }else{
      display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
      display.setCursor(3,18);
      display.println(BPMS0001A);
      display.setCursor(3,31);
      display.println(BPMS0001P);
  }
  
  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  display.setCursor(45,18);
  display.println(BPMS0002A);
  display.setCursor(87,18);
  display.println(BPMS0003A);

  display.setCursor(45,31);
  display.println(BPMS0002P);
  display.setCursor(87,31);
  display.println(BPMS0002P);

  display.setCursor(3,42);
  display.println("Veit");
  display.setCursor(45,42);
  display.println("Macpi");
  display.setCursor(87,42);
  display.println("Dev");
  
  display.setCursor(3,54);
  display.println("0");
  display.setCursor(45,54);
  display.println("0");
  display.setCursor(87,54);
  display.println("0");
  display.display();
}


void updateView(){
  xCursorTitle=centerText("SETUP MODE",1);
  xCursorMAC=centerMAC(1);
  xCursorSSID=alignRightSSID();
  xCursorPWD=alignRightPWD();
  xCursorSens=alignRightSens();
  display.clearDisplay();  
  for(int i=0;i<128;i++){
    display.drawPixel(i,0,SSD1306_WHITE);
    for(int j=0;j<10;j++){
      display.drawPixel(i,j,SSD1306_WHITE);
    }
  }
  for(int i=0;i<128;i++){
    display.drawPixel(i,22,SSD1306_WHITE);
    display.drawPixel(i,52,SSD1306_WHITE);
  }for(int j=22;j<53;j++){
    display.drawPixel(0,j,SSD1306_WHITE);
    display.drawPixel(127,j,SSD1306_WHITE);
  }
  display.setCursor((128-xCursorTitle)/2,1);
  display.setTextSize(1);
  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
  display.println("SETUP MODE");
  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  display.setTextSize(1);
  display.setCursor((128-xCursorMAC)/2,14);
  display.print("MAC ");
  display.println(WiFi.macAddress());
  display.setTextSize(1);
  display.setCursor(30,26);
  display.print("SSID: "); 
  display.setCursor(125-xCursorSSID,display.getCursorY());
  display.println(APssid);
  display.setCursor(30,display.getCursorY());
  display.print("PWD: ");
  display.setCursor(125-xCursorPWD,display.getCursorY());
  display.println(APpassword);
  display.setCursor(30,display.getCursorY());
  display.drawBitmap(3,27,update_bmp, 24, 24, 1);
  if(credUpdated){
    display.setCursor(6,55);
    display.print("Credentials Updated");
  }else{
    display.setCursor(15,55);
    display.print("IP : ");
    display.println(WiFi.softAPIP());
  }
  display.display();
}





void loadingScreen(){
  display.clearDisplay();
  display.drawBitmap(
    (display.width()- 40 )/2,
    (display.height() - 65)/2,
    load_bmp, 40, 40, 1);
  display.setTextSize(1);
  display.setCursor(43,45); 
  display.print("Loading");
  display.display();
  loading=loading+10;
  loadView();
}

void syncingScreen(){
  defaultView();
  display.setTextSize(1);
  display.setCursor(0,0); 
  display.print("Sending Data");
  display.display();
}

void sendingScreen(){
  defaultView();
  display.setTextSize(1);
  display.setCursor(0,0); 
  display.print("Sending Data");
  display.display();
}

void sendingMailScreen(){
  defaultView();
  display.setTextSize(1);
  display.setCursor(0,0); 
  display.print("Sending Alert");
  display.display();
}

int centerText(String text,int size){
    display.clearDisplay();
    display.setTextSize(size);
    display.setCursor(0,0);
    display.print(text);
    int xCursor=display.getCursorX();
    display.clearDisplay();
    return(xCursor);
}

int centerPressure(float number,int size){
    display.clearDisplay();
    display.setTextSize(size);
    display.setCursor(0,0);
    display.print(number);
    display.print(" Bar");
    int xCursor=display.getCursorX();
    display.clearDisplay();
    return(xCursor);
}

int centerCutoff(float number,int size){
    display.clearDisplay();
    display.setTextSize(size);
    display.setCursor(0,0);
    display.print("Cutoff: ");
    display.print(number);
    display.print(" Bar");
    int xCursor=display.getCursorX();
    display.clearDisplay();
    return(xCursor);
}

int centerUpload(int number,int size){
    display.clearDisplay();
    display.setTextSize(size);
    display.setCursor(0,0);
    display.print("Upload Delay: ");
    display.print(number);
    int xCursor=display.getCursorX();
    display.clearDisplay();
    return(xCursor);
}

int centerMail(int number,int size){
    display.clearDisplay();
    display.setTextSize(size);
    display.setCursor(0,0);
    display.print("Alert Delay: ");
    display.print(number);
    int xCursor=display.getCursorX();
    display.clearDisplay();
    return(xCursor);
}

int centerMAC(int size){
    display.clearDisplay();
    display.setTextSize(size);
    display.setCursor(0,0);
    display.print("MAC ");
    display.print(WiFi.macAddress());
    int xCursor=display.getCursorX();
    display.clearDisplay();
    return(xCursor);
}

int centerWIFI(int size){
    display.clearDisplay();
    display.setTextSize(size);
    display.setCursor(0,0);
    display.print("WiFi: ");
    display.print(ssid);
    int xCursor=display.getCursorX();
    display.clearDisplay();
    return(xCursor);
}

int alignRightSens(){
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0,0);
    display.print(7.0);
    int xCursor=display.getCursorX();
    display.clearDisplay();
    return(xCursor);
}
int alignRightSSID(){
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0,0);
    display.print(APssid);
    int xCursor=display.getCursorX();
    display.clearDisplay();
    return(xCursor);
}

int alignRightPWD(){
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0,0);
    display.print(APpassword);
    int xCursor=display.getCursorX();
    display.clearDisplay();
    return(xCursor);
}


void reconnectingView(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(36,45); 
  display.print("Reconnecting");
  display.drawBitmap(4,36,alert_bmp, 24, 24, 1);
  int progress1=36+((millis()-connectMillis)*82/30000);
  Serial.println(progress1);
  for(int i=36;i<progress1;i++){
    display.drawPixel(i,58,SSD1306_WHITE);
  }
  display.display();
}


void waitingView(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(36,45); 
  display.print("Waiting (");
  display.print(30-((millis()-reconnectWait)/1000));
  display.print("s)");
  display.drawBitmap(4,36,alert_bmp, 24, 24, 1);
  int progress=36+((millis()-reconnectWait)*82/30000);
  Serial.println(progress);
  for(int i=36;i<progress;i++){
    display.drawPixel(i,58,SSD1306_WHITE);
  }
  display.display();
}

void  loadView(){
  if (firstRun){
    int prog=10+(loading*107/100);
    for(int i=startView;i<prog;i++){
      display.drawPixel(i,58,SSD1306_WHITE);
      display.display();
    }
    startView=prog;
  }
}

void  loadSend(){
  int prog=36+(loadingSend*81/100);
  for(int i=startSend;i<prog;i++){
    display.drawPixel(i,58,SSD1306_WHITE);
    display.display();
  }
  startSend=prog;
}

void  loadSync(){
  int prog=36+(loadingSync*81/100);
  for(int i=startSync;i<prog;i++){
    display.drawPixel(i,58,SSD1306_WHITE);
    display.display();
  }
  startSync=prog;
}


void restartView(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(36,45); 
  display.println("No Internet");
  display.setCursor(36,display.getCursorY()); 
  display.println("Rebooting");
  display.drawBitmap(4,36,alert_bmp, 24, 24, 1);
  display.display();
  delay(3000);
}

