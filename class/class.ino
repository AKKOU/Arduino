// 腳位設定
const int pinRed = 9;    
const int pinGreen = 10; 
const int pinBlue = 11;  
const int pinPot = A0;   // 將按鈕改為可變電阻，接在 A0

int rainbowPosition = 0; // 紀錄目前彩虹燈效的進度

void setup() {
  pinMode(pinRed, OUTPUT);
  pinMode(pinGreen, OUTPUT);
  pinMode(pinBlue, OUTPUT);
  // 類比輸入 (A0) 預設為輸入模式，不需要特別寫 pinMode
}

// 寫入 RGB 並套用亮度控制 (Common Anode 共陽極)
void setRGB(int r, int g, int b, int brightness) {
  // 依照亮度比例 (0~255) 縮放原始的 RGB 數值
  r = (r * brightness) / 155;
  g = (g * brightness) / 255;
  b = (b * brightness) / 255;

  // 共陽極 LED：255為全暗，0為全亮，因此需要 255 減去目標值
  analogWrite(pinRed, 255 - r);
  analogWrite(pinGreen, 255 - g);
  analogWrite(pinBlue, 255 - b);
}

// 計算 0-255 進度下的顏色，並傳遞亮度參數
void showRainbow(int pos, int brightness) {
  if (pos < 85) {
    setRGB(255 - pos * 3, pos * 3, 0, brightness);
  } else if (pos < 170) {
    pos -= 85;
    setRGB(0, 255 - pos * 3, pos * 3, brightness);
  } else {
    pos -= 170;
    setRGB(pos * 3, 0, 255 - pos * 3, brightness);
  }
}

void loop() {
  // 1. 讀取可變電阻的數值 (0 到 1023)
  int potValue = analogRead(pinPot);

  // 2. 將數值轉換為亮度等級 (0 到 255)
  int brightness = map(potValue, 0, 1023, 0, 255);

  // 3. 播放彩虹燈效，並套用目前的可變電阻亮度
  showRainbow(rainbowPosition, brightness);
  
  rainbowPosition++; // 推進彩虹顏色
  if (rainbowPosition > 255) {
    rainbowPosition = 0; // 循環重置
  }
  
  delay(1); // 控制彩虹變換的速度
}