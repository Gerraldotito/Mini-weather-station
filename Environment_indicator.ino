
#include <Adafruit_BMP280.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <dht11.h>

#define DHT11PIN 2
#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)
int pencet = 0;
double ref = 1013.2; //Sebagai tekanan referensi di permukaan laut untuk altimeter
bool backlight = false;

Adafruit_BMP280 bmp; // I2C
//Adafruit_BMP280 bmp(BMP_CS); // hardware SPI
//Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);

LiquidCrystal_I2C lcd(0x27,  16, 2);
dht11 DHT11;

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.noBacklight();

  //if (!bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID)) {
  if (!bmp.begin(0x76)) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
    while (1) delay(10);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_FORCED,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

void loop() {
  cek_push_button();
  if(pencet == 0){
    terpencet();
    }
  else if(pencet==1){
    notpencet();
  }else if(pencet==2){
    notpencet_2();
  }

  backlek();
  if(backlight == true){
    lcd.backlight();
  }else{
    lcd.noBacklight();
  }
}

void terpencet(){
  if (bmp.takeForcedMeasurement()) {
    // can now print out the new measurements
    lcd.setCursor(0,0);
    lcd.print("Suhu = ");
    lcd.print(bmp.readTemperature(),1);
    lcd.print(" *C");

    lcd.setCursor(0,1);
    lcd.print("Tek = ");
    lcd.print(bmp.readPressure()/100,1);
    lcd.print(" hPa");

    delay(500);
    } else {
      Serial.println("Forced measurement failed!");
  }

}

void notpencet(){
  //untuk menu kedua
  int chk = DHT11.read(DHT11PIN);

  lcd.setCursor(0,0);
  lcd.print("Kelembapan (%): ");
  lcd.setCursor(0,1);
  lcd.print((float)DHT11.humidity, 2);
  delay(500);

}

void notpencet_2(){ //altitude
  if (bmp.takeForcedMeasurement()){
    tek_referensi();
    lcd.setCursor(0,0);
    lcd.print("Alt |ref=");
    lcd.print(ref,1);
    lcd.setCursor(0,1);
    lcd.print(bmp.readAltitude(ref),0);
    lcd.print(" m dpl");
  }
  delay(200);
}

void cek_push_button() {
  const int push_button = 7; 
  bool tanda = false;
  if (pencet == 0) { // If pencet is false
    if (digitalRead(push_button) == HIGH) { // If the button is not pressed
      if(tanda==false){
        pencet = 1; // Set pencet to true
        lcd.clear();
        tanda = true;
        delay(200);
    }}
  }if(pencet == 1) { // If pencet is true
    if (digitalRead(push_button) == HIGH) { // If the button is not pressed
      if(tanda ==false){ 
        pencet = 2; // Set pencet to false
        lcd.clear();
        tanda = true;
        delay(200);
    }}
  }else{
    if (digitalRead(push_button) == HIGH){
      if(tanda == false){
        pencet = 0;
        lcd.clear();
        tanda = true;
        delay(200);
    }}
  }
  }

  void tek_referensi(){
    const int naik = 4;  //pushbutton untuk menaikkan tekanan referensi pada digital pin nomor 4
    const int turun = 5;  //pushbutton untuk menurunkan tekanan referensi pada digital pin no 5
    
    if(digitalRead(naik) == HIGH){
      lcd.clear();
        ref = ref+0.1;
        lcd.print(ref,1);
        delay(10);
      }
  
    else if(digitalRead(turun) == HIGH){
      lcd.clear();
        ref = ref - 0.1;
        lcd.print(ref,1);
        delay(10);
      }  
  }

  void backlek(){
    const int naik = 4;  //pushbutton untuk menaikkan tekanan referensi pada digital pin nomor 4
    const int turun = 5;  //pushbutton untuk menurunkan tekanan referensi pada digital pin no 5
    if(digitalRead(naik) == HIGH & digitalRead(turun) == HIGH){
      if(backlight == false){
        backlight = true;
      }else{
        backlight = false;
      }
    }
  }
