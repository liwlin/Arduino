/*
 Project: buttonLed；按钮控制LED,每次按下，led灯自动切换亮灭
 BY Robot_LI
 Time: 2017/10/12
*/

#define LED 13  
#define KEY 4  
int KEY_NUM = 0;            //按键键值存放变量，不等于1说明有按键按下  
  
void setup()
{  
  pinMode(LED,OUTPUT);          //定义LED为输出引脚  
  pinMode(KEY,INPUT_PULLUP);    //定义KEY为带上拉输入引脚  
} 
  
void loop()  
{  
  if(digitalRead(KEY) == LOW)   //有按键按下  
  {  
    delay(20);                //延时去抖动  
    if(digitalRead(KEY) == LOW)    //有按键按下  
    {  
        
        digitalWrite(LED,!digitalRead(LED));  //LED的状态翻转  
       while(digitalRead(KEY) == LOW);      //等待按键松手  
     }   
   }  
}  