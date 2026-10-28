  const int n_red = 3;
  const int n_green = 5;
  const int n_blue = 6;

  const int blue_led = 2;
  const int yellow_led = 4;

  const int btn = 8;

  int buttonState = 0;
  int ledcolor = 0;

  int heart_interval = 2500;
  unsigned long previousMillis = 0;

  int mood = 0;
  const int neutraMood = 10;

  unsigned long lastPetMillis = 0;

  int lastButtonState = HIGH;

  void setup() {
    mood = 10;
    pinMode(blue_led, OUTPUT);
    pinMode(yellow_led, OUTPUT);

    pinMode(n_red, OUTPUT);
    pinMode(n_green, OUTPUT);
    pinMode(n_blue, OUTPUT);

    Serial.begin(9600);
    pinMode(btn, INPUT);
  }


  void loop() {
    if (millis() - previousMillis >= heart_interval) {
      previousMillis = millis();
      HeartBeats();
    }

    buttonState = digitalRead(btn);

  if (buttonState == LOW && lastButtonState == HIGH) {
    mood++;

    if (mood > 10) {
      mood = 10;
    }

    lastPetMillis = millis();

    Serial.print("Pet! Mood: ");
    Serial.println(mood);
  }

  lastButtonState = buttonState;

    if (millis() - lastPetMillis >= 2000) {
      if (mood > 0) {
        mood--;
      }

      lastPetMillis = millis();
    }
    ShowLedState(mood);
  }
void HeartBeats() {
  digitalWrite(blue_led, HIGH);
  delay(100);
  digitalWrite(blue_led, LOW);
  delay(100);
  digitalWrite(yellow_led, HIGH);
  delay(100);
  digitalWrite(yellow_led, LOW);
  delay(100);
}
void ShowLedState(int state) {
  float bright_interval = 255/ 10.0;
  if(state >= neutraMood){
    analogWrite(n_red,0);
    analogWrite(n_green,255 - bright_interval * (neutraMood - state));
    analogWrite(n_blue,bright_interval * (neutraMood - state));
  
  } 
  else{
    analogWrite(n_red,bright_interval * (neutraMood - state));
    analogWrite(n_green,255 - bright_interval * (neutraMood - state));
    analogWrite(n_blue,0);    
  }
}