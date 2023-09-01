#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 2 
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2); 

void setup() {
  lcd.begin(16, 2); 
  lcd.print("Temp: ");
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  dht.begin(); 
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (!isnan(temperature) && !isnan(humidity)) {

    lcd.setCursor(6, 0);
    lcd.print(temperature, 1); 
    lcd.print("C");

    
    lcd.setCursor(10, 1);
    lcd.print(humidity, 1); 
    lcd.print(" %");

    
    if (temperature > 30.0) {
      lcd.setCursor(12, 0); 
      lcd.print("AL!!");
    } else {
      lcd.setCursor(14, 0); 
      lcd.print(" "); 
    }
  } else {
    
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error!");
    lcd.setCursor(0, 1);
    lcd.print("Check Wiring!");
  }

  delay(2000); 
}
