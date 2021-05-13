#include <DHT.h>                            //引入<DHT.h>头文件
#include "U8glib.h"                         //引入Oled头文件
U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE); //设置通讯方式为IIC通讯
#define DHTPIN 8                            //湿度传感器OUT接8号口
#define DHTTYPE DHT11                       //定义DHT11传感器
DHT dht(DHTPIN, DHTTYPE);                   //定义湿度传感器OUT与8号口连接并且读取温湿度传感器的数值
int buzzer = 3;                             //定义蜂鸣器引脚3
int led = 4;                                //定义led引脚4
int HH;                                     //定义HH变量，用于储存湿度数据
int TT;                                     //定义TT变量，用于储存温度数据
void setup()
{
  pinMode(buzzer, OUTPUT);                 //设置蜂鸣器引脚为输出模式
  pinMode(led, OUTPUT);                    //设置led引脚为输出模式
  Serial.begin(9600); //串口波特率
  dht.begin();        //湿度传感器初始化

}

void loop()
{
  get_temp_humi();           //获得温湿度子函数
  dis_play();                //oled显示数据子函数
  if (TT > 50)                //如果温度大于50°，蜂鸣器响
    analogWrite(buzzer, 100);
  else                        //否则，蜂鸣器不响
    analogWrite(buzzer, 0);
  if (HH < 30)               //如果湿度小于30，led闪烁
  {
    digitalWrite(led, HIGH);
    delay(50);
    digitalWrite(led, LOW);
    delay(50);
  }
  else                      //否则，led熄灭
    digitalWrite(led, LOW);
}

void get_temp_humi()             //获取温湿度数据子函数
{
  HH = int(dht.readHumidity());      //获得湿度
  TT = int(dht.readTemperature());   //获得温度
  Serial.print("Temp: ");           //串口显示温湿度信息
  Serial.println(TT);
  Serial.print("Humi: ");
  Serial.println(HH);
}

void dis_play()              //OLED数据显示子函数
{
  u8g.firstPage(); //  OLED首页
  do
  {
    char buffer1[1]; //定义字符buffer1
        //  将int转为char显示出来
    itoa(TT, buffer1, 10);        //10：10进制显示
    u8g.setFont(u8g_font_gdr10r); //设置字体
    u8g.drawStr(25, 26, "Temp:"); //第26行25列显示“Temp：”
    u8g.drawStr(70, 26, buffer1); //第26行70列显示温度数据
    char buffer2[1];  //定义字符buffer2
    //  将int转为char显示出来
    itoa(HH, buffer2, 10);        //10：10进制显示
    u8g.setFont(u8g_font_gdr10r); //设置字体
    u8g.drawStr(25, 60, "Humi:"); //第60行25列显示“Humi：”
    u8g.drawStr(70, 60, buffer2); //第60行70列显示湿度数据
  } while (u8g.nextPage());       //执行配置
}
