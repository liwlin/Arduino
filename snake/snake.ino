// 矩阵引脚（数字）
#define CLK  11
#define CS   10
#define DIN  12
 
// XY摇杆引脚（模拟）
#define X    0
#define Y    1
#define SW   2
 
#define LEFT  0
#define RIGHT 1
#define UP    2
#define DOWN  3
#define BTN   4
 
#define PRESS   0
#define RELEASE 1
 
#define MAXLEN 64
 
#define MAIN    0
#define GAME    1
#define SCORE   2
#define SETTING 3
#define TOP    4
 
#define START     0
#define INTENSITY 1
#define HIGHSCORE 2
 
#define BTN_L  7
#define BTN_R  6
#define BTN_U  5
#define BTN_D  4
#define BTN_S  3
 
typedef struct{
  byte x, y;
}position;
 
// （类似）循环队列操作
void in(position p); // 插入新位置
position out(); // 弹出最末尾的位置
position get(); // 获取第一个位置
position get2(); // 获取第二个位置
byte length(); // 获取当前长度
 
// 数字，字母位图
const short numbers[10][5] = {
  {7, 5, 5, 5, 7},//0
  {2, 2, 2, 2, 2},//1
  {7, 4, 7, 1, 7},//2    
  {7, 4, 7, 4, 7},//3
  {5, 5, 7, 4, 4},//4
  {7, 1, 7, 4, 7},//5
  {7, 1, 7, 5, 7},//6
  {7, 4, 4, 4, 4},//7
  {7, 5, 7, 5, 7},//8
  {7, 5, 7, 4, 7}//9
}, chars[26][8] = {
  {4, 6, 9, 9, 9, 15, 9, 9},//A
  {4, 7, 9, 9, 7, 9, 9, 7},//B
  {4, 6, 9, 1, 1, 1, 9, 6},
  {4, 7, 9, 9, 9, 9, 9, 7},
  {4, 15, 1, 1, 7, 1, 1, 15},
  {4, 15, 1, 1, 7, 1, 1, 1},
  {4, 6, 9, 1, 13, 9, 9, 6},
  {4, 9, 9, 9, 15, 9, 9, 9},
  {3, 7, 2, 2, 2, 2, 2, 7},
  {4, 8, 8, 8, 8, 9, 9, 6},
  {4, 9, 9, 5, 3, 5, 9, 9},
  {4, 1, 1, 1, 1, 1, 1, 15},
  {4, 9, 15, 15, 9, 9, 9, 9},
  {4, 9, 11, 11, 13, 13, 9, 9},
  {4, 6, 9, 9, 9, 9, 9, 6},
  {4, 7, 9, 9, 7, 1, 1, 1},
  {4, 6, 9, 9, 9, 11, 13, 14},
  {4, 7, 9, 9, 7, 3, 5, 9},
  {4, 6, 9, 1, 6, 8, 9, 6},
  {5, 31, 4, 4, 4, 4, 4, 4},
  {4, 9, 9, 9, 9, 9, 9, 6},
  {4, 9, 9, 9, 9, 6, 6, 6},
  {4, 9, 9, 9, 9, 15, 15, 9},
  {4, 9, 9, 6, 6, 6, 9, 9},
  {5, 17, 17, 10, 4, 4, 4, 4},
  {4, 15, 8, 4, 6, 2, 1, 15}//Z
};
 
byte disp[8][8], tail, head, direction, buttons = 0, changing, timing[3], rsw = 0, flowleng, part = MAIN, status = START, intensity, highscore = 0;
short valx, valy, flowx;
unsigned long delays[3], times[3], rswt;
position snake[MAXLEN], food;
char* flows;
 
void in(position p){
  head = (head + 1) % MAXLEN;
  snake[head] = p;
}
 
position out(){
  position rtn = snake[tail];
  tail = (tail + 1) % MAXLEN;
  return rtn;
}
 
position get(){
  return snake[head];
}
 
position get2(){
  if (head) return snake[head - 1];
  return snake[MAXLEN];
}
 
byte length(){
  if (head >= tail) return head - tail + 1;
  return MAXLEN + head - tail + 1;
}
 
// 输出数字
void printNum(byte num, short x, short y){
  for(int i = 0; i < 5; i++){
    for(int j = 0; j < 3; j++){
      if (numbers[num] && (1 << j)) disp[i + y][j + x] = !0;
      else disp[i + y][j + x] = 0;
    }
  }
}
 
// 获取字母对应序号
byte getIndex(char c){
  if ((c >= 'a') && (c <= 'z')) return c - 97; // 'a' = 97
  if ((c >= 'A') && (c <= 'Z')) return c - 65; // 'A' = 65
  if (c >= 26) return -1;
  return c;
}
 
// 输出字母
byte printChar(char c, short x, short y){
  if ((c >= '0') && (c <= '9')){
    printNum(c - 48, x, y); // '0' = 48
    return 3;
  }
  c = getIndex(c);
  if (c == -1) return 0;
  for(int i = 0; i < 7; i++){
    if (i + y > 7) break;
    if (i + y < 0) continue;
    for(int j = 0; j < chars[c][0]; j++){
      if (j + x > 7) break;
      if (j + x < 0) continue;
      if (chars[c][i + 1] & (1 << j)) disp[i + y][j + x] = !0;
      else disp[i + y][j + x] = 0;
    }
  }
  return chars[c][0];
}
 
// 打印字符串
void printString(char* s, short x, short y){
  for(int i = 0; i < strlen(s); i++){
    x += printChar(s, x, y + ((s >= '0') && (s <= '9'))) + 1;
    if (x > 7) return;
  }
}
 
// 流动显示字符串
void flowString(char* s){
  flowleng = strlen(s);
  for(int i = 0; i < strlen(s); i++){
    char c = getIndex(s);
    if (c != -1) flowleng += chars[c][0];
  }
  flowx = 0;
  flows = s;
  timerContinue(2);
}
 
// 停止显示流动字符串
void stopFlowing(){
  timerPause(2);
}
 
// 生成食物
void newFood(){
  byte num = random(64 - length()); // 能生成的位置只有 64 - length() 个
  if ((food.x != get().x) || (food.y != get().y)){ // 初始化时可能排除上一次食物的位置
    disp[food.y][food.x] = 0;
    update(food.y);
  }
  for(int i = 0; i < 8; i++)
    for(int j = 0; j < 8; j++)
      if (!disp[j]){ // 身体不能生成食物
        if (num) num--;
        else{
          food.x = i;
          food.y = j;
          disp[food.y][food.x] = 1;
          update(food.y);
          return;
        }
      }
}
 
// 向矩阵模块输出一个字节的数据
void writeByte(byte data){
  for(int i = 0; i < 8; i++){
    digitalWrite(DIN, (data << i) & 0x80);
    digitalWrite(CLK, LOW);
    digitalWrite(CLK, HIGH);
  }
}
 
// 修改矩阵模块指定寄存器的值
void writeReg(byte addr, byte data){
  digitalWrite(CS, LOW);
  writeByte(addr);
  writeByte(data);
  digitalWrite(CS, HIGH);
}
 
// 刷新指定行
void update(byte row){
  byte t = 0;
  for(int i = 0; i < 8; i++) if (disp[row]) t |= 128 >> i;
  writeReg(row + 1, t);
}
 
// 刷新全屏幕
void update(){
  for(int i = 0; i < 8; i++){
    update(i);
  }
}
 
// 清屏
void clrscr(){
  for(int i = 0; i < 8; i++)
    for(int j = 0; j < 8; j++)
      disp[i][j] =  0; 
}
 
// 重绘
void repaint(){
  clrscr();
  for(int i = tail; i != head; i = (i + 1) % MAXLEN)   disp[snake.x][snake.y] = 1;
}
 
// 初始化地图
void initMap(){
  clrscr();
  for(int i = 0; i < 4; i++){
   disp[3][i + 2] = 1;
 snake.x =  i + 2;
 snake.y = 3; 
     
  }
  tail = 0;
  head = 3;
  direction = 1 << RIGHT;
  newFood();
  update();
  timerContinue(0);
  timerContinue(1);
}
 
// 获取前进方向
byte getDirection(){
  byte d1 = 255, d2 = 255;
  for(int i = 0; i < 4; i++) if (direction & (1 << i)){
    if (d1 == 255) d1 = i;
    else {
      d2 = i;
      break;
    }
  }
  if (d2 == 255) return d1;
  changing = !changing; // 如果摇杆掰到角落上就两个轮流来(ノへ￣、)
  if (changing) return d2;
  return d1;
}
 
// 注册计时器
void timerRegister(byte id, unsigned long time){
  delays[id] = time;
  times[id] = millis();
  timing[id] = 0;
}
 
// 计时器暂停
void timerPause(byte id){
  timing[id] = 0;
}
 
// 计时器继续
void timerContinue(byte id){
  times[id] = millis();
  timing[id] = !0;
}
 
// 返回计时器状态
bool timerTiming(byte id){
  return timing[id];
}
 
// 计时器事件
void timerAction(byte id){
  if (id == 0){ // 蛇该动了
    byte drtn = getDirection();
    position np = get();
    // 穿墙判定不解释
    if (drtn == LEFT){
      if (np.x) np.x--;
      else np.x = 7;
    } else if (drtn == RIGHT){
      if (np.x == 7) np.x = 0;
      else np.x++;
    } else if (drtn == UP){
      if (np.y) np.y--;
      else np.y = 7;
    } else if (drtn == DOWN){
      if (np.y == 7) np.y = 0;
      else np.y++;
    }
    if ((np.x == food.x) && (np.y == food.y)){ // 吃到东西了
      disp[np.y][np.x] = 1;
      update(np.y);
      in(np);
      newFood();
    } else {
      position last = out();
      disp[last.y][last.x] = 0;
      if (disp[np.y][np.x]){ // 咬到自己了
        byte score = length() - 3;
        timerPause(0);
        timerPause(1);
        part = SCORE;
        if (score > highscore) highscore = score;
        clrscr();
        printNum(score / 10, 0, 1);
        printNum(score % 10, 4, 1);
        update();
      }else{ // 没咬到就挪一下
        in(np);
        disp[np.y][np.x] = !0;
        update(np.y);
        update(last.y);
      }
    }
  } else if (id == 1){ // 食物闪一下
    disp[food.y][food.x] = !disp[food.y][food.x];
    update(food.y);
  } else if (id == 2){ // 字符串挪一下
    clrscr();
    printString(flows, -flowx, 0);
    update();
    flowx++;
    if (flowx == flowleng) flowx = -8;
  }
}
 
// 按钮按下事件
void buttonPressed(byte id){
  if (part == GAME){
    if (id == BTN){
    } else { // 游戏中变方向，同时还要防止头转180°这种奇葩情况。。
      position s1 = get(), s2 = get2();
      if ((id == LEFT) && ((get().x - get2().x == 1) || (get2().x - get().x == 7))) return;
      if ((id == RIGHT) && ((get2().x - get().x == 1) || (get().x - get2().x == 7))) return;
      if ((id == UP) && ((get().y - get2().y == 1) || (get2().y - get().y == 7))) return;
      if ((id == DOWN) && ((get2().y - get().y == 1) || (get().y - get2().y == 7))) return;
      direction = buttons;
      if (buttons & !(1 << id)) changing = 0;
    }
  }
}
 
// 按钮释放事件
void buttonReleased(byte id){
  if (part == MAIN){
    if (id == BTN){ // 主菜单选择了什么东西
      stopFlowing();
      if (status == START){
        part = GAME;
        initMap();
      } else if (status == INTENSITY){
        part = SETTING;
        clrscr();
        printNum(intensity / 10, 0, 1);
        printNum(intensity % 10, 4, 1);
        update();
      } else if (status == HIGHSCORE){
        part = TOP;
        clrscr();
        printNum(highscore / 10, 0, 1);
        printNum(highscore % 10, 4, 1);
        update();
      }
    } else if ((id == LEFT) || (id == UP)){ // 换选项，下同  其实这最好用数组，但是只有三个就没写。。
      if (status == START){
        status = HIGHSCORE;
        flowString("highscore");
      } else if (status == INTENSITY){
        status = START;
        flowString("start");
      } else if (status == HIGHSCORE){
        status = INTENSITY;
        flowString("intensity");
      }
    } else {
      if (status == START){
        status = INTENSITY;
        flowString("intensity");
      } else if (status == INTENSITY){
        status = HIGHSCORE;
        flowString("highscore");
      } else if (status == HIGHSCORE){
        status = START;
        flowString("start");
      }
    }
  } else if (part == GAME){
    if (id == BTN){ // 暂停
      if (timerTiming(0)) timerPause(0);
      else timerContinue(0);
    } else {
      if (direction & !(1 << id)) direction &= !(1 << id);
    }
  } else if (part == SCORE){
    if (id == BTN){ // 回主菜单
      part = MAIN;
      flowString("start");
    }
  } else if (part == SETTING){
    if (id == BTN){ // 还是回主菜单
      part = MAIN;
      flowString("intensity");
    } else{ // 改亮度
      if ((id == LEFT) || (id == UP)){
        if (intensity != 0) intensity--;
      } else {
        if (intensity != 15) intensity++;
      }
      writeReg(0xA, intensity);
      printNum(intensity / 10, 0, 1);
      printNum(intensity % 10, 4, 1);
      update();
    }
  } else if (part == TOP){
    if (id == BTN){ // 仍然是回主菜单
      part = MAIN;
      flowString("highscore");
    }
  }
}
 
void setup() {
  pinMode(CLK, OUTPUT);
  pinMode(CS, OUTPUT);
  pinMode(DIN, OUTPUT);
 
  pinMode(BTN_L, INPUT_PULLUP);
  pinMode(BTN_R, INPUT_PULLUP);
  pinMode(BTN_U, INPUT_PULLUP);
  pinMode(BTN_D, INPUT_PULLUP);
  pinMode(BTN_S, INPUT_PULLUP);
 
  intensity = 3;
  writeReg(0x9, 0); // Decode Mode
  writeReg(0xA, intensity); // Intensity
  writeReg(0xB, 7); // Scan Limit
  writeReg(0xC, 1); // Shutdown
  writeReg(0xF, 0); // Display Test
  update();
  timerRegister(0, 1000); // move timer
  timerRegister(1, 200);  // food twinkle
  timerRegister(2, 100);  // flow string
  flowString("start");
}
 
void loop() {
  // 循环检测计时器
  for(int i = 0; i < 3; i++) if ((timing) && (millis() - times >= delays)){
    timerAction(i);
    times += delays;
  }
  // 循环检测摇杆
  //if (analogRead(X) == 1023){
    if (digitalRead(BTN_R) == 0){
    if (!(buttons & (1 << RIGHT))){
      buttons |= 1 << RIGHT;
      buttonPressed(RIGHT);
    }
  } //else if (analogRead(X) == 0){
    else if (digitalRead(BTN_L) == 0){
    if (!(buttons & (1 << LEFT))){
      buttons |= 1 << LEFT;
      buttonPressed(LEFT);
    }
  } 
  else if (buttons & (1 << RIGHT)){
    //if (analogRead(X) < 923){
      if (digitalRead(BTN_R)){
      buttons &= !(1 << RIGHT);
      buttonReleased(RIGHT);
    }
  } else if (buttons & (1 << LEFT)){
    //if (analogRead(X) >= 100){
      if (digitalRead(BTN_L) ){
      buttons &= !(1 << LEFT);
      buttonReleased(LEFT);
    }
  }
  if (digitalRead(BTN_U) == 0){
  //if (analogRead(Y) == 1023){
    if (!(buttons & (1 << UP))){
      buttons |= 1 << UP;
      buttonPressed(UP);
    }
  } //else if (analogRead(Y) == 0){
    else if (digitalRead(BTN_D) == 0){
    if (!(buttons & (1 << DOWN))){
      buttons |= 1 << DOWN;
      buttonPressed(DOWN);
    }
  } 
  else if (buttons & (1 << UP)){
    //if (analogRead(Y) < 923){
      if (digitalRead(BTN_U)){
      buttons &= !(1 << UP);
      buttonReleased(UP);
    }
  } else if (buttons & (1 << DOWN)){
    //if (analogRead(Y) >= 100){
      if (digitalRead(BTN_D)){
      buttons &= !(1 << DOWN);
      buttonReleased(DOWN);
    }
  }
  // 循环检测按钮
  if (buttons & (1 << BTN)){
    //if (analogRead(SW)){
      if (digitalRead(BTN_S)==0){
      if (rsw){
        if (micros() - rswt > 1000){ // 防止按钮按下时出现有时认为没有按下的时候，认为用户连续按下了多次
          buttons &= !(1 << BTN);
          rsw = 0;
          buttonReleased(BTN);
        }
      } else {
        rsw = 1;
        rswt = micros();
      }
    } else rsw = 0;
  } //else if (!analogRead(SW)){
    else if (digitalRead(BTN_S)){
    buttons |= 1 << BTN;
    buttonPressed(BTN);
  }
}
