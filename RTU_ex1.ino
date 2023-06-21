#include <ModbusMaster.h>   //thư viện modbus
#include <Wire.h>  // thư viện i2c để hiển thị LCD
#include <LiquidCrystal_I2C.h>
#include<DHT.h>
DHT dht(6, DHT11);
LiquidCrystal_I2C lcd(0x27,16,2);   // địa chỉ i2c lcd là 0x27
#define DE  3 // chân DE được nối với chân số 3 arduino
#define RE  2 // chân RE được nối với chân số 2 arduino
ModbusMaster node;        
void preTransmission()           
{
  digitalWrite(RE, 1);  //Đưa chân RE lên mức cao           
  digitalWrite(DE, 1);  // Đưa chân DE lên mức cao
}
void postTransmission()
{
  digitalWrite(RE, 0);
  digitalWrite(DE, 0);
}
void setup()
{
  dht.begin();
  lcd.init();
  lcd.backlight();// bật tắt màn hình LCD
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);
  
  pinMode(7,INPUT);// Chân cảm biến hồng ngoại được nối với chân số 4 arduino

  digitalWrite(RE, 0);//Đưa chân RE xuống mức thấp
  digitalWrite(DE, 0);//Đưa chân DE xuống mức thấp

  Serial.begin(9600);  //tốc độ baud 115200

node.begin(1, Serial);  //Slave có ID là 1
node.preTransmission(preTransmission);  
node.postTransmission(postTransmission);
}
void loop()
{
int value = analogRead(A0);// Gán giá trị AS cho biến value
node.writeSingleRegister(0x40000,value);   // lưu giá trị cảm biến đó vào thanh ghi có địa chỉ 0x40000
  
  lcd.setCursor(0,0);
  lcd.print("CBAS:");
  lcd.print(value);//xong AS
  
  int gtcb= digitalRead(7);   // đọc giá trị cảm biến hồng ngoại lưu vào biến gtcb

  if (gtcb == 1)
  {
    node.writeSingleRegister(0x40001,0);// lưu giá trị cảm biến đó vào thanh ghi có địa chỉ 0x40001
    lcd.setCursor(9,0);
    lcd.print("CBHN:0");
  }
  else
  {
    node.writeSingleRegister(0x40001,1); // lưu giá trị cảm biến đó vào thanh ghi có địa chỉ 0x40001
    lcd.setCursor(9,0);
    lcd.print("CBHN:1");
  }
//  float h = dht.readHumidity();    //Đọc độ ẩm
//  float t = dht.readTemperature(); //Đọc nhiệt độ
//  node.writeSingleRegister(0x40002,h);
//  node.writeSingleRegister(0x40003,t);
//  lcd.setCursor(0,1);
//  lcd.print("DA:");
//  lcd.print(h);//xong DA
//
//  lcd.setCursor(8,1);
//  lcd.print("ND:");
//  lcd.print(t);//xong ND
  
  delay(1000);
  lcd.clear();
  }

