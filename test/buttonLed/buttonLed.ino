/*
 Project: buttonLed；按钮控制LED,每次按下，led灯自动切换亮灭
 BY Robot_LI
 Time: 2017/10/12
*/

//////////////////////////////*声明变量、常量*//////////////////////////////////
int led = 10; //定义数字引脚10为led灯   
int button = 2;  //定义数字引脚2为按钮
bool state =false;  //记录灯的亮灭状态


//////////////////////////////*初始化*//////////////////////////////////
void setup()
{
    pinMode(led,OUTPUT);// 10号引脚为输出模式
    pinMode(button,INPUT); //2号引脚为输入模式
    
}

//////////////////////////////*主函数*//////////////////////////////////

void loop()
{
   
    if(!state && digitalRead(button)) //判断按钮状态，如果是按下，则状态为真（高），如果没有按下则运行下一条语句
    {
        state = HIGH;
    }
    if(state && !digitalRead(button)) //判断按钮状态，如果是按下的，则切换led的亮灭，
    {
        digitalWrite(led,!digitalRead(led));   //led灯亮灭切换。
        state = LOW;   //把state的状态恢复为假（LOW）
    }

}

