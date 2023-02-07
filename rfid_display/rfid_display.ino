#include <SPI.h>
#include <MFRC522.h>
#include<Wire.h>
#include<LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 16, 2);
/*
In the ESP8266, D3 pin is RST_PIN and
D4 pin is SS_PIN
*/
#define RST_PIN D3
#define SS_PIN D4

MFRC522 reader(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
void setup()
{
  Serial.begin(9600); 
  while (!Serial)
  {
  }
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.print("Tech For FUN");
  lcd.setCursor(0,1);
  lcd.print("Subscribe");
  delay(2000);
  lcd.clear();
  lcd.print("Scan The Card");
  SPI.begin();
  reader.PCD_Init();
  delay(4);
  for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF; 
  }
  Serial.println("Ready!");
}

void loop()
{
 if (!reader.PICC_IsNewCardPresent())
  {
    return;
  }
 if (!reader.PICC_ReadCardSerial())
  {
    return;
  }
 String serial = "";
  for (int x = 0; x < reader.uid.size; x++)
  {
    if (reader.uid.uidByte[x] < 0x10)
    {
      serial += "0";
    }
    serial += String(reader.uid.uidByte[x], HEX);
    if (x + 1 != reader.uid.size)
    {
      serial += " ";
    }
  }
  serial.toUpperCase();
  Serial.println("Read serial is: " + serial);
  lcd.clear();
  lcd.print(serial);
  if(serial=="E3 51 55 05"){
    lcd.setCursor(0,1);
    lcd.print("Access Granted");
    delay(700);
    lcd.clear();
    lcd.print("Please scan ");
   }
  else{
    lcd.setCursor(0,1);
    lcd.print("Access Denied");
    delay(700);
    lcd.clear();
    lcd.print("Please scan ");
   }
  reader.PICC_HaltA();
  reader.PCD_StopCrypto1();
}
