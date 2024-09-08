#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include "Adafruit_Sensor.h"
#include "Adafruit_AM2320.h"


// PINNIT & MUUT CONSTANTIT
const int SERVO_PIN = 9;
const char LCD_ADDRESS = 0x27;


// ASETUKSIA
const float ERROR = -999999;


// LCD-NÄYTÖN TEKSTIT
char TITLE_TEXT[] = " Tiedot ";
char TEMP_TEXT[] = "Lampotila: ";
char HUM_TEXT[] = "Kosteus: ";


// LAITTEET
Adafruit_AM2320 am2320 = Adafruit_AM2320(); // lämpötila-/kosteusanturi
Servo myservo; // servo
LiquidCrystal_I2C lcd(LCD_ADDRESS, 20, 4); // lcd


// Raja-arvot
float upperThreshold = 70.0;  // Ylärajan oletus
float lowerThreshold = 20.0;  // Alarajan oletus


// Ilmankostuttimen tila
bool humidifierOn = false;


unsigned long previousMillis = 0;
const long interval = 10000;  // 10 sekunnin välein


void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SERVO_PIN, OUTPUT);


  // Alusta lämpötila-/kosteusanturi
  if (!am2320.begin()) {
    Serial.println("AM2320 sensoria ei löydy");
    while (1) delay(10);
  }


  // Alusta servo
  myservo.attach(SERVO_PIN);
  myservo.write(0);  // Aseta servo aloitusasentoon (0 astetta)


  // Alusta lcd
  lcd.init();
  lcd.backlight();


  Serial.println("Ilmankosteuden hallintajärjestelmä");
}


void loop() {
  unsigned long currentMillis = millis();


  // Tulosta raja-arvot 10 sekunnin välein
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Serial.print("Yläraja: ");
    Serial.println(upperThreshold);
    Serial.print("Alaraja: ");
    Serial.println(lowerThreshold);
  }


  // Lue anturien arvot
  float temperature = am2320.readTemperature();
  float humidity = am2320.readHumidity();


  // Tarkista, että anturin lukemat ovat kelvollisia
  if (!isnan(humidity)) {
    // Tulosta sarjaporttiin
    Serial.print("Kosteus: ");
    Serial.print(humidity);
    Serial.println(" %");
    Serial.print("Lämpötila: ");
    Serial.print(temperature);
    Serial.println(" *C");


    // Tyhjennä näyttö ja tulosta LCD:lle
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(TITLE_TEXT);


    lcd.setCursor(0, 1);
    lcd.print(TEMP_TEXT);
    lcd.print(temperature);
    lcd.print(" C");


    lcd.setCursor(0, 2);
    lcd.print(HUM_TEXT);
    lcd.print(humidity);
    lcd.print(" %");


    // Ohjaa servoa kosteuden perusteella
    if (humidity > upperThreshold && !humidifierOn) {
      myservo.write(90);  // Käännä servo 90 astetta (yläraja ylittynyt)
      Serial.println("Yläraja ylittynyt, ilmankostutin pois päältä");
      humidifierOn = true;  // Merkitään, että kostutin on päällä
    } else if (humidity < lowerThreshold && humidifierOn) {
      myservo.write(90);  // Käännä servo 180 astetta (alaraja alittunut)
      Serial.println("Alaraja ylittynyt, ilmankostutin päälle");
      humidifierOn = false;  // Merkitään, että kostutin on pois päältä
    } else {
      myservo.write(0);  // Palauta servo 0 asteeseen
    }
  } else {
    Serial.println("Virhe luettaessa kosteusarvoa");
  }


  // Lue ja aseta raja-arvot sarjaportin kautta
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    if (input.startsWith("ASETA YLÄRAJA ")) {
      String newValueStr = input.substring(14);
      float newValue = newValueStr.toFloat();
      upperThreshold = newValue;
      Serial.print("ASETA YLÄRAJA ");
      Serial.println(newValueStr);
      Serial.print("Yläraja asetettu: ");
      Serial.println(upperThreshold);
    } else if (input.startsWith("ASETA ALARAJA ")) {
      String newValueStr = input.substring(15);
      float newValue = newValueStr.toFloat();
      lowerThreshold = newValue;
      Serial.print("ASETA ALARAJA ");
      Serial.println(newValueStr);
      Serial.print("Alaraja asetettu: ");
      Serial.println(lowerThreshold);
    }
  }


  delay(1000);  // Pieni viive loopin nopean toistumisen estämiseksi
}
