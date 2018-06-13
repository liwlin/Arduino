//入门17示例: 步进电机
/* 
   描述:使用电位器控制步进电机
   by www.naozhendang.com
*/
#include <Stepper.h>
#define STEPS 64         //设置步进电机的一周的步数
Stepper stepper(STEPS, 8, 10, 9, 11);   //创建步进电机的对象实例

int previous = 0; 

void setup(){
  stepper.setSpeed(100); // 设置步进电机的转速为100PRM
}

void loop(){
  int val = analogRead(0); //获取电位器的读数

  stepper.step(val - previous); //步进电机前进到目标位置

  previous = val; // 储存当前读数，为下次计算做准备
}
