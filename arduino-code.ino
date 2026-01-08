
// declare variables for pins
const int sensorpin = A0;
const int sensorpower = 8;
const int LED1 = 2;
const int LED2 = 3;
const int LED3 = 4;
const int buzzerPin = 11;

// Light sensor pins
const int lightSensorPin = A1;  
const int lightLED = 13;      

// Light threshold (you can calibrate later)
int lightGood = 700;  

// variable for sensor reading
int sensor;

// Track whether the soil was previously NOT green
bool wasNotGreen = false;
bool lightWasLow = false;

// delay time between sensor readings 
const int delayTime = 1000; 

// "wet" and "dry" thresholds - these require calibration
int wet = 800;
int dry = 500;

void setup(){ // code that only runs once
  // set pins as outputs
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);
  pinMode(sensorpower,OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(lightLED, OUTPUT);
  
  // initialize serial communication
  Serial.begin(9600);
}

void loop(){ // code that loops forever
  // power on sensor and wait briefly
  digitalWrite(sensorpower,HIGH);
  delay(10);
  // take reading from sensor
  sensor = analogRead(sensorpin);
  // turn sensor off to help prevent corrosion
  digitalWrite(sensorpower,LOW);
  
  // print sensor reading
  Serial.print("Moisture: ");
  Serial.println(sensor);
  
  // If sensor reading is greater than "wet" threshold,
  // turn on the green LED. If it is less than the "dry"
  // threshold, turn on the red LED. If it is in between
  // the two values, turn on the yellow LED.
  if(sensor>wet){
    digitalWrite(LED1,LOW);
    digitalWrite(LED2,LOW);
    digitalWrite(LED3,HIGH);
    
    // If soil was previously dry or medium,
    // and now becomes green then beep
    if (wasNotGreen) {
      beepWhenGreen();
      wasNotGreen = false;
    }
    
  }
  else if(sensor<dry){
    digitalWrite(LED1,HIGH);
    digitalWrite(LED2,LOW);
    digitalWrite(LED3,LOW);
    wasNotGreen = true;
  }
  else{
    digitalWrite(LED1,LOW);
    digitalWrite(LED2,HIGH);
    digitalWrite(LED3,LOW);
    wasNotGreen = true;
  }
  
int lightValue = analogRead(lightSensorPin);
Serial.print("Light: ");
Serial.println(lightValue);
if (lightValue > lightGood) {
  // Bright enough
  digitalWrite(lightLED, LOW);   // LED OFF in good light

  // If light was low and now recovers, beep
  if (lightWasLow) {
    beepLightRecovered();
    lightWasLow = false;
  }

} else {
  // Too dark
  digitalWrite(lightLED, HIGH);  // LED ON when it's dark
  lightWasLow = true;
}
  
  // wait before taking next reading
  delay(delayTime); 
}

void beepWhenGreen() {
  // Three short beeps to indicate “stop watering”
  for (int i = 0; i < 3; i++) {
    tone(buzzerPin, 1200); // 1.2kHz tone
    delay(200);
    noTone(buzzerPin);
    delay(150);
  }
}
void beepLightRecovered() {
  tone(buzzerPin, 350, 300);  // deep, low-pitch tone
  delay(350);
  noTone(buzzerPin);
}
