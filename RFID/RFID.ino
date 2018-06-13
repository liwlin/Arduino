#include <SPI.h>
#include <RFID.h>
#include <Servo.h>
Servo myservo;
extern uint8_t SmallFont[];
RFID     rfid(10,8);    //D10--接上SDA、D8--接RST  D13--接上SCK D11--接上MOSI D12--接上MISO RQ不接
unsigned char serNum[5]; //卡的数据（要换成你自己的卡号（去串口监视器例里看），也可以增加）
void setup()
{
  Serial.begin(9600);//串口用来读取需要添加的卡号，然后手动写到程序里
  myservo.attach(9);//舵机针脚位9
  myservo.write(0); //舵机初始化0度
  SPI.begin();
  rfid.init();
}
 
void loop()
{
  long randNumber = random(0, 20);
  unsigned char i,tmp;
  unsigned char status;
  unsigned char str[MAX_LEN];
  unsigned char RC_size;
 
  //找卡
  rfid.isCard();
  //读卡号
  if (rfid.readCardSerial())
  {
    Serial.print("your card id is   : ");
    Serial.print(rfid.serNum[0]);
    Serial.print(" , ");
    Serial.print(rfid.serNum[1],BIN);
    Serial.print(" , ");
    Serial.print(rfid.serNum[2],BIN);
    Serial.print(" , ");
    Serial.print(rfid.serNum[3],BIN);
    Serial.print(" , ");
    Serial.print(rfid.serNum[4],BIN);
    Serial.println(" ");
//下面是改卡号区域
    if(rfid.serNum[0]==177||rfid.serNum[0]==227||rfid.serNum[0]==136||rfid.serNum[0]==161||rfid.serNum[0]==68)//第一道筛选
    {
      for(int i=0;i<100;i++) //i的值为人数上限。
      {
         
        if(rfid.serNum[0]==177)//第二道筛选，支持一人一号；rfid.serNum[0]==161 这个0是id位置，可以自行更改
        {
 
          Serial.println("Welcome test 1");
          myservo.write(180); 
        }
        if(rfid.serNum[0]==227)
        {
 
          Serial.println("Welcome test 2");
          myservo.write(180); 
        }
        if(rfid.serNum[0]==136)
        {
 
          Serial.println("Welcome test 3");
          myservo.write(180); 
 
        }
      }     
      delay(1000);
      myservo.write(0); 
      Serial.println("closed");
 
    }
  }  
  if (!rfid.readCardSerial()){
  }
  rfid.halt();                //休眠
}