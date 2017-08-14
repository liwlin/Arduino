#include <dht.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C mylcd(0x27,16,2);
dht myDHT_A1;
int tem;
void setup() {
  // put your setup code here, to run once:
  mylcd.init();
  mylcd.backlight(); 
   Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  tem = dht_A1_gettemperature();
 mylcd.setCursor(0, 0);
  mylcd.print("temperature:");
  mylcd.setCursor(0, 1);
  mylcd.print(tem);
  delay(1000);

}


int dht_A1_gettemperature() {
  int chk = myDHT_A1.read11(A1);
  int value = myDHT_A1.temperature;
  return value;
}
