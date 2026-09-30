// Define the RGB pins
const int pinRed = 9;    
const int pinGreen = 11; 
const int pinBlue = 10;  
const int pinBt = 2;

int rainbowPosition = 0; // Tracks where we are in the rainbow

void setup() {
  pinMode(pinRed, OUTPUT);
  pinMode(pinGreen, OUTPUT);
  pinMode(pinBlue, OUTPUT);
  pinMode(pinBt, INPUT); 
}

// Helper function for Common Anode LED. 
// It inverts the values so 255 is fully ON, and 0 is fully OFF.
void setRGB(int r, int g, int b) {
  analogWrite(pinRed, 255 - r);
  analogWrite(pinGreen, 255 - g);
  analogWrite(pinBlue, 255 - b);
}

// Function that calculates the color mix based on a 0-255 position
void showRainbow(int pos) {
  if (pos < 85) {
    setRGB(255 - pos * 3, pos * 3, 0);
  } else if (pos < 170) {
    pos -= 85;
    setRGB(0, 255 - pos * 3, pos * 3);
  } else {
    pos -= 170;
    setRGB(pos * 3, 0, 255 - pos * 3);
  }
}

void loop() {
  int btStatus = digitalRead(pinBt);

  if (btStatus == HIGH) {
    // BUTTON PRESSED: Play the rainbow
    showRainbow(rainbowPosition);
    
    rainbowPosition++; // Move to the next color in the rainbow
    if (rainbowPosition > 255) {
      rainbowPosition = 0; // Reset the rainbow cycle
    }
    
    delay(15); // Controls the speed of the rainbow. Lower = faster!
  } 
  else {
    // BUTTON NOT PRESSED: Show solid white
    // 255 Red + 255 Green + 255 Blue = White
    setRGB(255, 255, 255); 
  }
}