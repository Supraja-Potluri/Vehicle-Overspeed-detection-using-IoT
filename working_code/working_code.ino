#include <TinyGPS++.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

TinyGPSPlus gps;
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Change 0x27 to your LCD address
int sw=7;
int buz=6;
void setup() {
  Serial.begin(9600);         // GPS module baud rate
  lcd.begin();
 pinMode(sw,INPUT_PULLUP);
 pinMode(buz,OUTPUT);
  lcd.backlight();
  
  Serial.println("Waiting for GPS data...");
}

void loop() {
  // Check for GPS data
  while (Serial.available() > 0) {
    gps.encode(Serial.read());  // Parse GPS data
  }

  // If GPS has valid location data
  if (gps.location.isValid()) {
    float latitude = gps.location.lat();
    float longitude = gps.location.lng();
    float speed = gps.speed.kmph();  // Speed in kilometers per hour

    // Display latitude and longitude on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Lat:");
    lcd.print(latitude, 6);  // Show up to 6 decimal places

    lcd.setCursor(0, 1);
    lcd.print("Lng:");
    lcd.print(longitude, 6); // Show up to 6 decimal places

    delay(2000);  // Delay for readability
    
    // Display speed on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Speed:");
    lcd.print(speed, 2);  // Display speed up to 2 decimal places
    lcd.print(" km/h");

    if(speed>20 || digitalRead(sw)==0)
    {
      digitalWrite(buz,1);
      delay(1000);
      digitalWrite(buz,0);
      send_sms(latitude,longitude,speed);
      
    }

    delay(2000);  // Delay to display speed
  } else {
    // No valid location data available
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Waiting for GPS...");
  }
}




void send_sms(float flat,float flon,float spd)
{

Serial.println("Sending SMS...");
Serial.println("AT");    
delay(1000);  
Serial.println("ATE0");    
delay(1000);  
Serial.println("AT+CMGF=1");    
delay(1000);   
Serial.print("AT+CMGS=\"XXXXXXXXXX\"\r\n");// Replace x with mobile number
delay(1000);
Serial.println("Alert: VEHICLE OVER SPEED DETECTED at Loc:");

  Serial.println("https://www.google.com/maps/search/?api=1&query=" + String(flat,6)+ "," + String(flon,6));

delay(100);
Serial.println((char)26);// ASCII code of CTRL+Z
delay(5000);
Serial.println("ATD9666189396;");
delay(30000);

}
