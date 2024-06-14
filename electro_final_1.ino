#include <Servo.h>
#include <DHT.h>

#define DHTPIN 7
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// Pin definitions
const int greenLightPin = 2;
const int redLightPin = 3;
const int whiteLED = 12;
const int servoMotorPin = 4;
const int lightSensorPin = A1;
const int soilMoisturePin = 9;
const int lightBulbPin = 5;

// Threshold values
const float temperatureMin = 22.0;      // Minimum temperature in degrees Celsius
const float temperatureMax = 28.0;      // Maximum temperature in degrees Celsius
const int lightThreshold = 100;         // Light intensity (0-1023)

// Variables
float temperature;
float humidity;
bool soilMoistureStatus;
int lightIntensity;

Servo servoMotor;  // Declare servoMotor object

void setup() {
  pinMode(greenLightPin, OUTPUT);
  pinMode(redLightPin, OUTPUT);
  pinMode(lightBulbPin, OUTPUT);

  servoMotor.attach(servoMotorPin);  // Attach servo motor to the pin

  Serial.begin(9600);
  dht.begin();
}

void loop() {
  // Read temperature and humidity
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  Serial.println();
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("°C");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  // Check temperature range and control lights accordingly
  if (temperature >= temperatureMin && temperature <= temperatureMax) {
    digitalWrite(greenLightPin, HIGH);    // Turn on green light
    digitalWrite(redLightPin, LOW);       // Turn off red light
  } else {
    digitalWrite(greenLightPin, LOW);     // Turn off green light
    digitalWrite(redLightPin, HIGH);      // Turn on red light
  }

  // Read soil moisture
  soilMoistureStatus = readSoilMoisture();
  Serial.print("Soil Moisture: ");
  Serial.println(soilMoistureStatus);

  // Check soil moisture
  if (soilMoistureStatus) {
    rotateServoMotor();                   // Rotate the servo motor
  } else {
    servoMotor.write(0);                  // Rotate the servo motor to 0 degrees if soil moisture is low
  }

  // Read light intensity
  lightIntensity = readLightIntensity();
  Serial.print("Light Intensity: ");
  Serial.println(lightIntensity);

  // Check light intensity
  if (lightIntensity < lightThreshold) {
    digitalWrite(lightBulbPin, HIGH);     // Turn on light bulb
  } else {
    digitalWrite(lightBulbPin, LOW);      // Turn off light bulb
  }

  delay(1000); // Wait for a second
}

bool readSoilMoisture() {
  bool moistureStatus = digitalRead(soilMoisturePin);
  return moistureStatus;
}

int readLightIntensity() {
  int lightValue = analogRead(lightSensorPin);
  return lightValue;
}

void rotateServoMotor() {
  servoMotor.write(90); // Rotate servo motor to 90 degrees
}
