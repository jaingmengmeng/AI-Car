//=============================================================================
//  智能小车黑线循迹，红外避障综合实验
//=============================================================================
//#include <Servo.h>
int Left_motor_go = 8;     //左电机前进(IN1)
int Left_motor_back = 9;   //左电机后退(IN2)
int Right_motor_go = 10;   // 右电机前进(IN3)
int Right_motor_back = 11; // 右电机后退(IN4)

int key = A2;  //定义按键 数字A2 接口
int beep = A3; //定义蜂鸣器 数字A3 接口
int LED = 7;   //定义蜂鸣器 数字A3 接口

const int SensorRight = 3; //右循迹红外传感器()
const int SensorLeft = 4;  //左循迹红外传感器()

const int SensorLeft2 = 5; // 中间红外避障传感器()

int SL; //左循迹红外传感器状态
int SR; //右循迹红外传感器状态

int SL2; //中间避障红外传感器状态

void setup()
{
    //初始化电机驱动IO为输出方式
    pinMode(Left_motor_go, OUTPUT);    // PIN 8 (PWM)
    pinMode(Left_motor_back, OUTPUT);  // PIN 9 (PWM)
    pinMode(Right_motor_go, OUTPUT);   // PIN 10 (PWM)
    pinMode(Right_motor_back, OUTPUT); // PIN 11 (PWM)

    pinMode(LED, OUTPUT); // PIN 7 LED输出
    pinMode(key, INPUT);  //定义按键接口为输入接口
    pinMode(beep, OUTPUT);
    pinMode(SensorRight, INPUT); //定义右循迹红外传感器为输入
    pinMode(SensorLeft, INPUT);  //定义左循迹红外传感器为输入
    pinMode(SensorLeft2, INPUT); //定义左循迹红外传感器为输入
    Serial.begin(9600);          // 初始化串口
}

//=======================智能小车的基本动作=========================
//void run(int time)     // 前进
void run()
{
    digitalWrite(Right_motor_go, HIGH); // 右电机前进
    digitalWrite(Right_motor_back, LOW);
    analogWrite(Right_motor_go, 140); //PWM比例0~255调速，左右轮差异略增减
    analogWrite(Right_motor_back, 0);
    digitalWrite(Left_motor_go, LOW); // 左电机前进
    digitalWrite(Left_motor_back, HIGH);
    analogWrite(Left_motor_go, 0); //PWM比例0~255调速，左右轮差异略增减
    analogWrite(Left_motor_back, 140);
    //delay(time * 100);   //执行时间，可以调整
}

//void brake(int time)  //刹车，停车
void brake()
{
    digitalWrite(Right_motor_go, LOW);
    digitalWrite(Right_motor_back, LOW);
    digitalWrite(Left_motor_go, LOW);
    digitalWrite(Left_motor_back, LOW);
    //delay(time * 100);//执行时间，可以调整
}

//void left(int time)         //左转(左轮不动，右轮前进)
void left()
{
    digitalWrite(Right_motor_go, HIGH); // 右电机前进
    digitalWrite(Right_motor_back, LOW);
    analogWrite(Right_motor_go, 140);
    analogWrite(Right_motor_back, 0); //PWM比例0~255调速
    digitalWrite(Left_motor_go, LOW); //左轮后退
    digitalWrite(Left_motor_back, LOW);
    analogWrite(Left_motor_go, 0);
    analogWrite(Left_motor_back, 0); //PWM比例0~255调速
                                     //delay(time * 100);	//执行时间，可以调整
}

void spin_left() //左转(左轮后退，右轮前进)
{
    digitalWrite(Right_motor_go, HIGH); // 右电机前进
    digitalWrite(Right_motor_back, LOW);
    analogWrite(Right_motor_go, 140);
    analogWrite(Right_motor_back, 0);  //PWM比例0~255调速
    digitalWrite(Left_motor_go, HIGH); //左轮后退
    digitalWrite(Left_motor_back, LOW);
    analogWrite(Left_motor_go, 100);
    analogWrite(Left_motor_back, 0); //PWM比例0~255调速
                                     // delay(time * 100);	//执行时间，可以调整
}

//void right(int time)        //右转(右轮不动，左轮前进)
void right()
{
    digitalWrite(Right_motor_go, LOW); //右电机后退
    digitalWrite(Right_motor_back, LOW);
    analogWrite(Right_motor_go, 0);
    analogWrite(Right_motor_back, 0); //PWM比例0~255调速
    digitalWrite(Left_motor_go, LOW); //左电机前进
    digitalWrite(Left_motor_back, HIGH);
    analogWrite(Left_motor_go, 0);
    analogWrite(Left_motor_back, 140); //PWM比例0~255调速
                                       //delay(time * 100);	//执行时间，可以调整
}

void spin_right() //右转(右轮后退，左轮前进)
{
    digitalWrite(Right_motor_go, LOW); //右电机后退
    digitalWrite(Right_motor_back, HIGH);
    analogWrite(Right_motor_go, 0);
    analogWrite(Right_motor_back, 100); //PWM比例0~255调速
    digitalWrite(Left_motor_go, LOW);   //左电机前进
    digitalWrite(Left_motor_back, HIGH);
    analogWrite(Left_motor_go, 0);
    analogWrite(Left_motor_back, 140); //PWM比例0~255调速
                                       //delay(time * 100);	//执行时间，可以调整
}

//void back(int time)          //后退
void back(int time)
{
    digitalWrite(Right_motor_go, LOW); //右电机后退
    digitalWrite(Right_motor_back, HIGH);
    analogWrite(Right_motor_go, 0);
    analogWrite(Right_motor_back, 200); //PWM比例0~255调速
    digitalWrite(Left_motor_go, LOW);   //左电机前进
    digitalWrite(Left_motor_back, HIGH);
    analogWrite(Left_motor_go, 0);
    analogWrite(Left_motor_back, 200); //PWM比例0~255调速
                                       //delay(time * 100);	//执行时间，可以调整
}
//==========================================================

void keyscan() //按键扫描
{
    int val;
    val = digitalRead(key);   //读取数字7 口电平值赋给val
    while (!digitalRead(key)) //当按键没被按下时，一直循环
    {
        val = digitalRead(key); //此句可省略，可让循环跑空
    }
    while (digitalRead(key)) //当按键被按下时
    {
        delay(10);              //延时10ms
        val = digitalRead(key); //读取数字7 口电平值赋给val
        if (val == HIGH)        //第二次判断按键是否被按下
        {
            digitalWrite(beep, HIGH);    //蜂鸣器响
            while (!digitalRead(key))    //判断按键是否被松开
                digitalWrite(beep, LOW); //蜂鸣器停止
        }
        else
            digitalWrite(beep, LOW); //蜂鸣器停止
    }
}

void loop()
{
    keyscan(); //调用按键扫描函数
    while (1)
    {
        //有信号为LOW  没有信号为HIGH
        SR = digitalRead(SensorRight); //有信号表明在白色区域，车子底板上L3亮；没信号表明压在黑线上，车子底板上L3灭
        SL = digitalRead(SensorLeft);  //有信号表明在白色区域，车子底板上L2亮；没信号表明压在黑线上，车子底板上L2灭

        SL2 = digitalRead(SensorLeft2);

        if (SL2 == LOW) //如果中间的避障传感器检测到障碍物
        {
            brake(); //停车
            digitalWrite(beep, HIGH);
            digitalWrite(LED, HIGH);
        }
        else
        {
            digitalWrite(beep, LOW);
            digitalWrite(LED, LOW);
            if (SL == LOW && SR == LOW)
                run();                       //调用前进函数
            else if (SL == HIGH & SR == LOW) // 左循迹红外传感器,检测到信号，车子向右偏离轨道，向左转
                //spin_left();
                left();
            else if (SR == HIGH & SL == LOW) // 右循迹红外传感器,检测到信号，车子向左偏离轨道，向右转
                //spin_right();
                right();
            else // 都是黑色, 停止
                brake();
        }
    }
}