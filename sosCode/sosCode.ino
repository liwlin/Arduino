/*
 Project: S.O.S-code；... --- ...sos莫尔斯码求救信号。3短--3长--3短的声音或灯光
 BY Robot_LI
 Time: 2017/10/8
*/

//////////////////////////////*声明变量、常量*//////////////////////////////////
int ledPin = 13;   //声明ledPin变量并赋值为13

//////////////////////////////*初始化*////////////////////////////////////////////////
void setup() 
{   
    digitalWrite(ledPin,OUTPUT);    
    Serial.begin(9600);
    Serial.println("sos");
}

/////////////////////////////*主程序*//////////////////////////////////////////////////
void loop() 
{
    S(); delay(5000);
    O(); delay(5000);
    S(); delay(5000); //每个SOS信号间隔5秒
}

//////////////////////////////////*S信号子程序*////////////////////////////////////////////////////
//“.”之间的延时可以设置为150毫秒。
void S()   
  { 
      for(int i=0;i<3;i++)
      {
        digitalWrite(ledPin,HIGH); delay(150);
        digitalWrite(ledPin,LOW);  delay(150);
      }
    
  }

//////////////////////////////////////*O信号子程序*///////////////////////////////////////////////

void O()
//“-”之间的延时可以设置为300毫秒。
  {
    for(int i=0;i<3;i++)
    {
      digitalWrite(ledPin,HIGH); delay(300);
      digitalWrite(ledPin,LOW);  delay(300);
    }
  }
