#include <Wire.h>
#include <HX711.h>
#include <cmath>
#include <I2CKeyPad.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
const uint8_t KEYPAD_ADDRESS = 0x20; 
I2CKeyPad keyPad(KEYPAD_ADDRESS);

const int LOADCELL_DOUT_PIN = 12;
const int LOADCELL_SCK_PIN = 13;
const float scale_factor = 716.0;
HX711 scale;

// Price variables
float store_price = 0.18/2;
float weig_price = 0;
int add_price = 0;
bool isAdd = true;


unsigned long lastUpdate = 0; 
const int UPDATE_INTERVAL = 200; 
unsigned long lastKeyTime = 0;      
const int DEBOUNCE_DELAY = 250;

void setup() {
  Serial.begin(9600);
  
  // Wire init 建議放在最前面
  Wire.begin(D2, D1); 

  // LCD init
  lcd.init();
  lcd.backlight();
  

  // HX711 init
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  Serial.print("Start weight scale");
  scale.set_scale(scale_factor);
  scale.tare();
}

void loop() {
  
  if (keyPad.isPressed()) {
    if (millis() - lastKeyTime > DEBOUNCE_DELAY) {
      
      lastKeyTime = millis(); // 更新按鍵時間，開始冷卻

      uint8_t idx = keyPad.getKey();
      int add_amount = 0;
      
      
      switch(idx) {
        case 7:  add_amount = 1; break;
        case 6:  add_amount = 5; break;
        case 4:  add_amount = 10; break;
        case 15: add_amount = 15; break;
        case 14: add_amount = 20; break;
        case 12: add_amount = 25; break;
        case 11: 
          isAdd = !isAdd; 
          Serial.println("+/- Switch"); 
          break;
        case 10: 
          weig_price = 0; 
          add_price = 0; 
          
          Serial.println("Price Reset"); 
          break;
        case 8:  
          scale.tare(); 
          Serial.println("Weight Reset"); 
          break;
      }
      
      if(isAdd) add_price += add_amount;
      else add_price -= add_amount;


      updateDisplay(); 
      Serial.print("Add amount:");
      Serial.println(add_amount);
    }
  } 


  if (millis() - lastUpdate > UPDATE_INTERVAL) {
    lastUpdate = millis(); // 更新時間標記
    
    if(scale.is_ready()) {
      
      long weight_scale = scale.get_units(1); 
      
      weig_price = round(weight_scale * store_price);
      
      updateDisplay();
    }
  }
}


void updateDisplay() {
  
  lcd.setCursor(0, 0);
  lcd.print("Gram:$");
  lcd.print(int(weig_price));
  lcd.print(" "); 

  lcd.print("Add:$");
  lcd.print(add_price);
  lcd.print("   ");

  lcd.setCursor(0, 1);
  lcd.print("Mode:");
  if(isAdd) lcd.print("+  ");
  else lcd.print("-  ");

  lcd.print("Sum:$");
  lcd.print(int(round(weig_price + add_price)));
  lcd.print("    "); 
}