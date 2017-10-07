byte ledPin[] ={4,5,6,7,8,9,10,11,12,13};  //声明byte类型的数组变量ledPin 并赋值。为LED引脚创
int ledDelay(65);  //变化之间的延时
int dir =1 ;
int currentLED =0 ;
unsigned long changeTime;
void setup() {
  for(int i=0;i<10;i++){
      pinMode(ledPin[i],OUTPUT);//定义led针脚为输出模式
    }
    changeTime = millis();//获取当前系统时间并赋值changeTime
}

void loop() {
  //如果距最后一次变灯，ledDelay毫秒已经过去。   再次获取当前系统运行时间和changeTime比较，如果大于ledDelay表示led已经改变了状态
  if((millis()-changeTime)>ledDelay){
    changeLED();
    changeTime = millis();
    }
}


//该函数的工作是关闭所有LED，然后打开当前LED
void changeLED(){
    for(int x;x<10;x++){
        digitalWrite(ledPin[x],LOW);  //关闭所有灯 
      }
    digitalWrite(ledPin[currentLED],HIGH);  //点亮所有灯
    currentLED += dir ;
    //如果到达led末端，改变dir的值。
    if(currentLED == 9){dir = -1;} 
    if(currentLED == 0) {dir=  1;}
  }
