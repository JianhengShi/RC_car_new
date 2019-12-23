#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

#include <ros.h>
#include <Servo.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Float32.h>
#include <math.h>

Servo motor;
Servo servo;

void cmd_callback(const geometry_msgs::Twist& data);

//舵机、电机初值
int angle = 1550;
int thrust = 1500;
int thrust_old = 1500;

//创建节点句柄
ros::NodeHandle nh;

//ros订阅topic key_vel
ros::Subscriber<geometry_msgs::Twist> cmd_sub("cmd_vel", cmd_callback);

void cmd_callback( const geometry_msgs::Twist& data){
  thrust =  1500 + 130*data.linear.x; // Teleop from handle
  // thrust =  1455 + 200*data.linear.x; // Teleop from key board
  angle = 1550 - 800*data.angular.z;
  // angle = 1550 + 300*data.angular.z; // Teleop from key board
  motor.writeMicroseconds(thrust);
  servo.writeMicroseconds(angle); 
  
  //go reversely
  if(thrust_old==1485&thrust<1485){
    delay(20);
    motor.writeMicroseconds(1500);
    delay(60);
    motor.writeMicroseconds(thrust);
  }
  // if(thrust_old==1455&thrust<1455){
  //   delay(60);
  //   motor.writeMicroseconds(1500);
  //   delay(60);
  //   motor.writeMicroseconds(thrust);
  // }

  thrust_old = thrust;
}

void setup(){
  nh.getHardware()->setBaud(57600);
  nh.initNode();
  nh.subscribe(cmd_sub);

  // 等待连接
  while(!nh.connected()) {nh.spinOnce();}

  motor.attach(13); //电机信号线接pin13
  servo.attach(12); //舵机信号线接pin12
  
  motor.writeMicroseconds(1000); //电调默认频率50hz,周期20ms（20000微秒），电调信号高电平占1ms(1000微秒)到2ms(2000微秒)(5%-10%周期)。给一个周期占空比5%，1ms的信号激活电调。刹车抱死,舵机中位信号均为高电平1500微秒。
  servo.writeMicroseconds(1000);
  delay(20);
}

void loop(){
  servo.writeMicroseconds(angle);
  
  nh.spinOnce();
}
