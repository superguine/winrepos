#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// I2C address for the SSD1306
#define OLED_RESET -1 // Reset pin not used
#define SCREEN_ADDRESS 0x3C // I2C address

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Global variables for eye positions
int posX, posY;   // Target position
int x = 0, y = 0; // Current position
int w=0,inc = 4,dec=19,W=0,old_W,t;
bool ta=false,tb=false,pet=false,first=true; 
void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN,OUTPUT);
  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  // Set initial random target position
  posX = random(2, 70);
  posY = random(2, 35);
}
void loop() {
  Serial.print(touchRead(12));
  Serial.print(" ");
  Serial.println(touchRead(13));
  idle();
  animate_happy();
}


void drawEye(int finalx, int finaly) {
  display.clearDisplay();
  // Draw eye
  display.fillRoundRect(finalx, finaly, 32, 32, 7, SSD1306_WHITE);
  display.fillRoundRect(finalx + 40, finaly, 32, 32, 7, SSD1306_WHITE);
  display.display();
}

void moveEye(int desx,int desy){
  while(true){
    animate_happy();
    int s = 4; 
    // Update X position
    if (desx > x) x += s; // Move 3 pixels per iteration
    if (desx < x) x -= s;

    // Update Y position
    if (desy > y) y += s;
    if (desy < y) y -= s;

    // Draw the eyes at the current position
    drawEye(x, y);
    if (abs(desx - x) < s && abs(desy - y) < s) { 
    // If close enough, set a new random target position
    break;
    }
  }
}

void blink(int blx, int bly){
  int h=8;
  while (h>=1){
    animate_happy();
    display.clearDisplay();
    display.fillRoundRect(blx, bly +=2, 32, h*4, 7, SSD1306_WHITE);
    display.fillRoundRect(blx + 40, bly+=2, 32, h*4, 7, SSD1306_WHITE);
    display.display();
    h -=2;
  }
  delay(10);
  h=0;
  while ( h<9){
    animate_happy();
    display.clearDisplay();
    display.fillRoundRect(blx, bly -=2, 32, h*4, 7, SSD1306_WHITE);
    display.fillRoundRect(blx + 40, bly -=2, 32, h*4, 7, SSD1306_WHITE);
    display.display();
    h +=2;
  }
}

void idle(){
  posX = random(2, 60);
  posY = random(2, 28);
  moveEye(posX,posY);
  animate_happy();
  delay(500);
  animate_happy();
  blink(x,y);
  animate_happy();
  delay(500);
  animate_happy();
}

void happy(){
  while(true){
    int s = 2; 
    // Update X position
    if (30 > x) x += s; // Move  pixels per iteration
    if (30 < x) x -= s;

    // Update Y position
    if (10 > y) y += s;
    if (10 < y) y -= s;

    // Draw the eyes at the current position
    int i =32;
    
    while (i>16 && x== 30 && y==10){
      display.clearDisplay();
      display.fillRoundRect(x, y, 32, 32, 7, SSD1306_WHITE);
      display.fillRoundRect(x + 40, y, 32, 32, 7, SSD1306_WHITE);
      display.fillRoundRect(x, y+i, 32, 32, 7, SSD1306_BLACK);
      display.fillRoundRect(x + 40, y+i, 32, 32, 7, SSD1306_BLACK);
      display.display();
      i--;
    }
    if (i==16 && x== 30 && y==10){break;}
  }
}

bool trigger_happy() {
  if (touchRead(13) < 88) {
    t = 0;
    tb = true;
    digitalWrite(LED_BUILTIN, HIGH);
  }

  while (tb && t < 500) {
    if (touchRead(12) < 95) {
      return true;
    }
    delay(1);
    t += 1;
  }
  
  tb = false;
  digitalWrite(LED_BUILTIN, LOW);
  return false;
}


void animate_happy(){
  if (trigger_happy()){
    while(first){
      happy();
      break;
    }
    display.clearDisplay();
    display.fillRoundRect(30, 10, 32, 32, 7, SSD1306_WHITE);
    display.fillRoundRect(30 + 40, 10, 32, 32, 7, SSD1306_WHITE);
    display.fillRoundRect(30, 10+16, 32, 32, 7, SSD1306_BLACK);
    display.fillRoundRect(30 + 40, 10+16, 32, 32, 7, SSD1306_BLACK);
    //Serial.print(tb);
    display.display();
    int p = 0;
    while (p<200){
    if (trigger_happy()){
      first=false;
      animate_happy();
    }
    p +=1;
    delay(1);
    }
    digitalWrite(LED_BUILTIN,0);
    first=true;
  }

}