#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#define WIFI_SSID "Thu Thao"
#define WIFI_PASSWORD "12052002"
#define DATABASE_URL "https://do-luong-mo-hoi-default-rtdb.firebaseio.com"
#define API_KEY "wFqQkfCGiCjrBWOYEGAAsOuvvSDvUoqgjAQ2yEnP"

#include <ArduinoJson.h>
FirebaseData firebaseData;
FirebaseJson json;

#define cambien A0 /////Chân cảm biến độ ẩm
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
int canchinh = 68;  // luọng mồ hôi cân chỉnh
int dientich = 7.0; //cm2
int gioihan = 0.6;
void setup() {
  Serial.begin(9600);
  pinMode(cambien, INPUT);
  
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);}
  // put your setup code here, to run once:
Firebase.begin(DATABASE_URL,API_KEY);
  delay(1000);
}

void loop() {
  int val = analogRead(cambien);
  int doam = map(val, 500, 1000, 100, 0); // Chuyển đổi giá trị analog ngưỡng từ 450-1024(8 bit) sang độ ẩm 0 -100%///Giá trị tối đa của analog 10 bit là 1024 nhưng chỉ chọn ngưỡng từi 0 đến 1020
  doam = constrain(doam,0,100);
  int val2 = map(doam,0,100,0, canchinh);// Chuyển đơn vị % sang mg/Cm2
  float luongmohoi = float(val2)/dientich;
  Firebase.setInt(firebaseData, "/DATA/DO_AM",doam);
  Firebase.setString(firebaseData, "/DATA/MO_HOI",String(luongmohoi,2));
  lcd.setCursor(0, 0);
    lcd.print("DO AM:         %");
    lcd.setCursor(7, 0);
    lcd.print(doam);
    lcd.setCursor(0, 1);
    lcd.print("L:        ml/cm2");
    lcd.setCursor(3, 1);
    lcd.print(luongmohoi);
    Serial.print("DO AM: ");
    Serial.println(doam);
    Serial.print("MO HOI: ");
    Serial.println(luongmohoi);
    if(luongmohoi >= gioihan){
      Firebase.setInt(firebaseData, "/DATA/THONG_BAO",1);
    }
    else Firebase.setInt(firebaseData, "/DATA/THONG_BAO",0);
 delay(100);///Thòi gian chờ khi đo và gửi lần tiếp theo
  }
