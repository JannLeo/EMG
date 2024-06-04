/*
  MyoWare Example_01_analogRead_SINGLE
  SparkFun Electronics
  Pete Lewis
  3/24/2022
  License: This code is public domain but you buy me a beverage if you use this and we meet someday.
  This code was adapted from the MyoWare analogReadValue.ino example found here:
  https://github.com/AdvancerTechnologies/MyoWare_MuscleSensor

  This example streams the data from a single MyoWare sensor attached to ADC A0.
  Graphical representation is available using Serial Plotter (Tools > Serial Plotter menu).

  *Only run on a laptop using its battery. Do not plug in laptop charger/dock/monitor.

  *Do not touch your laptop trackpad or keyboard while the MyoWare sensor is powered.

  Hardware:
  SparkFun RedBoard Artemis (or Arduino of choice)
  USB from Artemis to Computer.
  Output from sensor connected to your Arduino pin A0

  This example code is in the public domain.
*/
/******************************************************************************
  Example_03 WiFi Communcation

  Connect WiFi using the provided network credentials
  Talk to NTP server to set the current date/time
  Update the time to the correct time zone
  Print the current time approx. once every second

  Development environment specifics:
  IDE: Arduino 1.8.13
  Hardware Platform: SparkFun RedBoard Qwiic (3.3V LOGIC!!!!)

  Hardware Connections:
  Connect the shield to a 3.3V logic Arduino R3 board
  Make sure switch is in the "SW" position

  ARDUINO --> WiFi Shield
  8       --> RX1
  9       --> TX1
  4       --> RTC_PWR_KEY
  3.3V    --> 3.3V
  GND     --> GND

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/
#include<SoftwareSerial.h>

#define RX1 8
#define TX1 9
#define RTC_PWR_KEY 4
#define SOFTWARE_WIFISERIAL_BAUD 115200
#define SOFTWARE_SERIAL_BAUD 9600

String wifiSSID = "jannleoo";
String wifiPass = "00000000";
int timezoneOffset = 0; //The hours offset from UTC (Mountain time is -6 for daylight savings, and -7 for standard)
SoftwareSerial WiFiSerial(RX1,TX1); //Configure SoftwareSerial

// 目标服务器的IP地址和端口
String serverIP = "192.168.50.135";
int serverPort = 8080;

void setup() 
{
  Serial.begin(SOFTWARE_SERIAL_BAUD);
  WiFiSerial.begin(SOFTWARE_WIFISERIAL_BAUD); //Set SoftwareSerial baud
  //Enable DA16200 Module RTC power block
  pinMode(RTC_PWR_KEY,OUTPUT);
  digitalWrite(RTC_PWR_KEY,HIGH);
  Serial.println("Connecting to WiFi\n");

  //等待WIFI模块的初始化完成并且接收初始化消息
  //Listen for ready message ("+INIT:DONE")
  byte count = 0;
  String msg = "";
  while(count<20)
  {
    while(WiFiSerial.available())
    {
      msg += char(WiFiSerial.read());
    }
    if(msg.length() > 5) break;
    count++;
    delay(100);
  }
  msg = msg.substring(3,msg.length()); //Remove NULL,CR,LF characters from response
  // 异常处理
  if(msg.length()>5)
  {
    Serial.println("Expecting: \"INIT:DONE,(0 or 1)");
    Serial.println("Received: " + msg);
  }
  else
  {
    Serial.println("Failed to receive initialization message.\n" \
                   "Make sure you're using the correct baud rate.\n");
    while(1);
  }

  // 当初始化完成后设置WIFI模式为STA模式，并且重启模块完成设置
  //Configure module for STA mode
  Serial.println("Sending:AT+WFMODE=0");
  WiFiSerial.println("AT+WFMODE=0");

  //Wait for "OK" response
  while(1)
  {
    msg = "";
    while(WiFiSerial.available())
    {
      msg += char(WiFiSerial.read());
      delay(1);
    }
    Serial.print(msg);
    if(msg.length() > 1) break;
  }
  // 软复位
  //Apply a software reset to finish changing the mode
  Serial.println("Sending:AT+RESTART");
  WiFiSerial.println("AT+RESTART");

  //Wait for "OK" response
  while(1)
  {
    msg = "";
    while(WiFiSerial.available())
    {
      msg += char(WiFiSerial.read());
      delay(1);
    }
    Serial.print(msg);
    if(msg.length() > 1) break;
  }

  //Listen for ready message ("+INIT:DONE") after the reset is finished
  count = 0;
  msg = "";
  while(count<20)
  {
    while(WiFiSerial.available())
    {
      msg += char(WiFiSerial.read());
    }
    if(msg.length() > 5) break;
    count++;
    delay(100);
  }

  Serial.println(count);
  Serial.println(msg);
  msg = msg.substring(3,msg.length()); //Remove NULL,CR,LF characters from response

  if(msg.length()>5)
  {
    Serial.println("Expecting: \"INIT:DONE,(0 or 1)");
    Serial.println("Received: " + msg);
  }
  else
  {
    Serial.println("Failed to receive initialization message.\n" \
                   "Continuing anyway...\n");
  }

  //Connect to WiFi using the provided credentials
  Serial.println("Sending:AT+WFJAPA=" + wifiSSID + "," + wifiPass);
  WiFiSerial.println("AT+WFJAPA=" + wifiSSID + "," + wifiPass);

  Serial.println("Waiting for connection response...");
  while(1)
  {
    msg = "";
    while(WiFiSerial.available())
    {
      msg += char(WiFiSerial.read());
      delay(1);
    }

    if(msg.length() > 10) 
    {
      Serial.print("Response:");
      Serial.println(msg);
      break;
    }
  }

  msg = msg.substring(3,msg.length()); //Remove NULL,CR,LF characters from response

  //If connection to AP is successful, response will be WFJAP:1,SSID,IP_ADDRESS, or WJAP:0 if failed
  if(msg.startsWith("WFJAP:1"))
  {
      //Talk to NTP server to get the current time, along with how often to get time sync
      Serial.println("Sending:AT+NWSNTP=1,pool.ntp.org,86400");
      WiFiSerial.println("AT+NWSNTP=1,pool.ntp.org,86400");

      //Wait for "OK" response
      while(1)
      {
        String msg = "";
        while(WiFiSerial.available())
        {
          msg += char(WiFiSerial.read());
          delay(1);
        }
        Serial.print(msg);
        if(msg.length() > 1) break;
      }

      //Provides the correct UTC offset for the current time
      Serial.println("Sending:AT+TZONE="+String(timezoneOffset*3600));
      WiFiSerial.println("AT+TZONE="+String(timezoneOffset*3600));

      //Wait for "OK" response
      while(1)
      {
        String msg = "";
        while(WiFiSerial.available())
        {
          msg += char(WiFiSerial.read());
          delay(1);
        }
        Serial.print(msg);
        if(msg.length() > 1) break;
      }  
  }
  else
  {
    Serial.println("Connection unsucessful :(\n\n" \
                   "Make sure the WiFi credentials are correct, and the module is in the station mode");
    while(1);
  }
  while (!Serial); // optionally wait for serial terminal to open

  // 建立TCP连接  用于传输数据
  establishTCPConnection();
}

void establishTCPConnection() {
  Serial.println("Establishing TCP connection...");
  
  // Establish TCP connection to the server
  WiFiSerial.println("AT+NCTCP=" + serverIP + "," + String(serverPort));
  String msg = "";
  Serial.println("Waiting for connection response...");
  while(1)
  {
    msg = "";
    while(WiFiSerial.available())
    {
      msg += char(WiFiSerial.read());
      delay(1);
    }

    if(msg.length() > 10) 
    {
      Serial.print("Response:");
      Serial.println(msg);
      break;
    }
  }

}

void sendSensorValue(int value) {
  // Send sensor value
  String data = String(value);
  WiFiSerial.print("AT+NSOST=0," + serverIP + "," + String(serverPort) + "," + String(data.length()) + "," + data);
  // delay(1000); // Adjust this delay if necessary

  Serial.println("Data sent: " + data);
}

void loop() 
{  
  int sensorValue = analogRead(A0); // read the input on analog pin A0
  // Serial.println("Sending:AT+TIME");
  Serial.println(sensorValue); // print out the value you read
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue * (5.0 / 1023.0);
  // print out the value you read:
  Serial.println(voltage);
  // WiFiSerial.println("AT+TIME");
  sendSensorValue(voltage);
  while(1);
  // while(WiFiSerial.available())
  // {
  //   Serial.print(char(WiFiSerial.read()));
  //   delay(1);
  // }
  // delay(1000);
  delay(50); // to avoid overloading the serial terminal
}
