
// LCD Pin Definitions
const int RS = 7;
const int EN = 6;
const int D4 = 5;
const int D5 = 4;
const int D6 = 3;
const int D7 = 2;

// Sensor Pins
const int TEMP_PIN = A0;
const int LDR_PIN = A1;

// Custom character locations in CGRAM
#define TEMP_SYMBOL 0  // CGRAM location 0 for temperature symbol
#define LIGHT_SYMBOL 1 // CGRAM location 1 for light/brightness symbol

// Helper: Pulse Enable to latch data
void lcdPulseEnable() {
  digitalWrite(EN, LOW);
  delayMicroseconds(1);
  digitalWrite(EN, HIGH);
  delayMicroseconds(1);
  digitalWrite(EN, LOW);
  delayMicroseconds(100);
}

// Send 4 bits to LCD
void lcdSend4Bits(byte data) {
  digitalWrite(D4, (data >> 0) & 1);
  digitalWrite(D5, (data >> 1) & 1);
  digitalWrite(D6, (data >> 2) & 1);
  digitalWrite(D7, (data >> 3) & 1);
  lcdPulseEnable();
}

// Send command (RS = 0)
void lcdCommand(byte cmd) {
  digitalWrite(RS, LOW);
  lcdSend4Bits(cmd >> 4);
  lcdSend4Bits(cmd & 0x0F);
  delay(2);
}

// Send data (RS = 1)
void lcdWriteChar(char chr) {
  digitalWrite(RS, HIGH);
  lcdSend4Bits(chr >> 4);
  lcdSend4Bits(chr & 0x0F);
  delay(2);
}

// Print string to LCD
void lcdPrint(const char* str) {
  while (*str) {
    lcdWriteChar(*str++);
  }
}

// Go to row/col
void lcdSetCursor(byte col, byte row) {
  byte row_offsets[] = {0x00, 0x40};
  lcdCommand(0x80 | (col + row_offsets[row]));
}

// Create a custom character
void lcdCreateChar(byte location, byte charmap[]) {
  location &= 0x7; // Keep location in bounds (0-7)
  lcdCommand(0x40 | (location << 3)); // Set CGRAM address
  
  digitalWrite(RS, HIGH); // Set RS to data mode
  for (int i = 0; i < 8; i++) {
    lcdSend4Bits(charmap[i] >> 4);
    lcdSend4Bits(charmap[i] & 0x0F);
    delay(1);
  }
  
  lcdCommand(0x80); // Return to DDRAM address 0
}

// LCD Initialization (4-bit mode)
void lcdInit() {
  pinMode(RS, OUTPUT);
  pinMode(EN, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);

  delay(50); // Wait for LCD to power up

  digitalWrite(RS, LOW);
  digitalWrite(EN, LOW);

  lcdSend4Bits(0x03); delay(5);
  lcdSend4Bits(0x03); delay(5);
  lcdSend4Bits(0x03); delay(5);
  lcdSend4Bits(0x02); delay(5); // Set to 4-bit mode

  lcdCommand(0x28); // Function set: 4-bit, 2-line, 5x8
  lcdCommand(0x0C); // Display ON, Cursor OFF
  lcdCommand(0x06); // Entry mode: left to right
  lcdCommand(0x01); // Clear display
  delay(5);
  
  // Create custom characters
  createCustomCharacters();
}

// Define and create custom characters
void createCustomCharacters() {
  // Temperature symbol (thermometer)
  byte tempChar[8] = {
    0b00100,
    0b01010,
    0b01010,
    0b01010,
    0b01010,
    0b10001,
    0b10001,
    0b01110
  };
  
  // Light/brightness symbol (sun)
  byte lightChar[8] = {
    0b00000,
    0b00100,
    0b10101,
    0b01110,
    0b11111,
    0b01110,
    0b10101,
    0b00100
  };
  
  // Store the custom characters in CGRAM
  lcdCreateChar(TEMP_SYMBOL, tempChar);
  lcdCreateChar(LIGHT_SYMBOL, lightChar);
}

// sliding window average
int readAverage(int pin) {
    static int readings[10];    // Array to store readings
    static int readIndex = 0;   // Current position in array
    static long total = 0;      // Running total
    static int initialized = 0;

    // Initialize array on first run
    if (!initialized) {
        for (int i = 0; i < 10; i++) {
            readings[i] = analogRead(pin);
            total += readings[i];
            delay(10);
        }
        initialized = true;
    }

    // Subtract the oldest reading
    total = total - readings[readIndex];
    // Read new value
    readings[readIndex] = analogRead(pin);
    // Add the new reading
    total = total + readings[readIndex];
    // Advance to next position
    readIndex = (readIndex + 1) % 10;

    delay(10);
    return total / 10;
}

void setup() {
  lcdInit(); // Initialize LCD with custom characters
}

void loop() {
  static int displayState = 0;  // 0 for temperature, 1 for light

  // === Read and average temperature ===
  int rawTemp = readAverage(TEMP_PIN);
  float voltage = rawTemp * (5.0 / 1023.0);
  float tempC = voltage * 100;

  // === Read and average light level ===
  int lightVal = readAverage(LDR_PIN);

  // Clear display
  lcdCommand(0x01);
  
  if (displayState == 0) {
    // Display temperature reading
    lcdSetCursor(0, 0);
    lcdWriteChar(TEMP_SYMBOL);
    lcdPrint(": ");
    char tempStr[10];
    dtostrf(tempC, 4, 1, tempStr);
    lcdPrint(tempStr);
    lcdPrint(" C");
  } else {
    // Display light level reading
    lcdSetCursor(0, 0);
    lcdWriteChar(LIGHT_SYMBOL);
    lcdPrint(": ");
    char lightStr[10];
    itoa(lightVal, lightStr, 10);
    lcdPrint(lightStr);
  }
  
  delay(2000);
  displayState = !displayState;  // Toggle between 0 and 1
}