 

#define TIME_MSG_LEN  11   // time sync to PC is HEADER followed by unix time_t as ten ascii digits
#define TIME_HEADER  255   // Header tag for serial time sync message

const int Door1 = 2;

const int AlerttPin1 = 8;
const int AlerttPin2 = 11;
const int AlerttPin3 = 12;
const int LED_PIN = 13; // Use built-in LED as dark indicator
const int LIGHT_PIN = A0; // Pin connected to voltage divider output


// Measure the voltage at 5V and the actual resistance of your
// 47k resistor, and enter them below:
const float VCC = 4.98; // Measured voltage of Ardunio 5V line
const float R_DIV = 4660.0; // Measured resistance of 3.3k resistor

// Set this to the minimum resistance require to turn an LED on:
const float DARK_THRESHOLD = 10000.0;


int pirSensor1 = 7;
int Door1Close = 1;
int pirSensor1Count = 0;
int pirValue = 0;
int DetectedTrueCount = 0;
int DetectedFalseCount = 0;


void setup() {

  pinMode(AlerttPin1, OUTPUT); // Sets the trigPin as an Output
  pinMode(AlerttPin2, OUTPUT); // Sets the trigPin as an Output
  pinMode(AlerttPin3, OUTPUT); // Sets the trigPin as an Output
  pinMode(LED_PIN, OUTPUT);

  pinMode(pirSensor1, INPUT);
  pinMode(LIGHT_PIN, INPUT);
  pinMode(Door1, INPUT_PULLUP); // Door1
  //setTime(7,9,0,4,3,18); // set time to Saturday 8:29:00am Jan 1 2011

  Serial.begin(9600);
}

void loop() {

  pirValue = digitalRead(pirSensor1);
  Door1Close = digitalRead(Door1);
  
  if (pirValue > 0) {
    //digitalWrite(AlerttPin3, LOW);
    DetectedTrueCount++;
    Serial.println(DetectedTrueCount);
  } else {
    //digitalWrite(AlerttPin3, HIGH);
    DetectedFalseCount++;
  }

  Serial.print(Door1Close);
  Serial.print("-");
  Serial.println("-");

  if (DetectedTrueCount  > 3) {
     if(Door1Close == 0){
      digitalWrite(AlerttPin1, HIGH);
      DetectedTrueCount = 0;
      DetectedFalseCount = 0;
      delay(8050);
     }
  }

  if (DetectedFalseCount  > 2) {
    digitalWrite(AlerttPin3, HIGH);
    digitalWrite(AlerttPin1, LOW);

    DetectedTrueCount = 0;
    DetectedFalseCount = 0;
  }

  //digitalClockDisplay();
   if (GetLigthIsDark() == true) {
      digitalWrite(AlerttPin3, LOW);
      //delay(60050);
    }
    
  delay(50);

}

void digitalClockDisplay()
{  
  Serial.println();
}
void printDigits(int digits)
{
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}


bool GetLigthIsDark() {
  bool isDark = true;
  // Read the ADC, and calculate voltage and resistance from it
  int lightADC = analogRead(LIGHT_PIN);
  if (lightADC > 0)
  {
    // Use the ADC reading to calculate voltage and resistance
    float lightV = lightADC * VCC / 1023.0;
    float lightR = R_DIV * (VCC / lightV - 1.0);
    Serial.println("Voltage: " + String(lightV) + " V");
    Serial.println("Resistance: " + String(lightR) + " ohms");

    // If resistance of photocell is greater than the dark
    // threshold setting, turn the LED on.
    if (lightV >= 0.1) {
      digitalWrite(LED_PIN, HIGH);
      isDark = false;
    }
    else
    {
      digitalWrite(LED_PIN, LOW);
      isDark = true;
    }
    Serial.println();
  }
  return isDark;
}
