#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 7
#define DHTTYPE DHT11

#define MOTOR_PIN_ENA 9
#define MOTOR_PIN_IN1 10
#define MOTOR_PIN_IN2 11

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Change address if needed

void setup() {
  Serial.begin(9600);
  dht.begin();

  lcd.init();
  lcd.backlight();

  pinMode(MOTOR_PIN_ENA, OUTPUT);
  pinMode(MOTOR_PIN_IN1, OUTPUT);
  pinMode(MOTOR_PIN_IN2, OUTPUT);

  digitalWrite(MOTOR_PIN_IN1, HIGH);
  digitalWrite(MOTOR_PIN_IN2, LOW);
}

void loop() {
  float temperature = dht.readTemperature();
  
  if (isnan(temperature)) {
    Serial.println("Failed to read temperature from DHT sensor!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  int fanSpeed;
  String speedLevel;

  if (temperature >= 15 && temperature < 20) {
    fanSpeed = 30;   // 1st Level (very low)
    speedLevel = "1st Level";
  } else if (temperature >= 20 && temperature < 25) {
    fanSpeed = 60;   // 2nd Level (low)
    speedLevel = "2nd Level";
  } else if (temperature >= 25 && temperature < 30) {
    fanSpeed = 90;   // 3rd Level (medium-low)
    speedLevel = "3rd Level";
  } else if (temperature >= 30 && temperature < 35) {
    fanSpeed = 120;  // 4th Level (medium)
    speedLevel = "4th Level";
  } else if (temperature >= 35 && temperature < 40) {
    fanSpeed = 160;  // 5th Level (medium-high)
    speedLevel = "5th Level";
  } else if (temperature >= 40) {
    fanSpeed = 200;  // Max Speed (instead of 255)
    speedLevel = "Max Speed";
  } else {
    fanSpeed = 0;
    speedLevel = "Fan Off";
  }

  analogWrite(MOTOR_PIN_ENA, fanSpeed);

  Serial.print("Fan Speed: ");
  Serial.println(speedLevel);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("Speed: ");
  lcd.print(speedLevel);

  delay(2000);
}