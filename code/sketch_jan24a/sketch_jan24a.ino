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
#include <SoftwareSerial.h>

#define RX1 8
#define TX1 9
#define RTC_PWR_KEY 4
#define SOFTWARE_WIFISERIAL_BAUD 115200
#define SOFTWARE_SERIAL_BAUD 9600
#define LOCAL_PORT 1230

String wifiSSID = "jannleoo";
String wifiPass = "00000000";
int timezoneOffset = 0; //The hours offset from UTC
SoftwareSerial WiFiSerial(RX1, TX1); // Configure SoftwareSerial

String serverIP = "192.168.114.135";
int serverPort = 13888;

void setup() {
  Serial.begin(SOFTWARE_SERIAL_BAUD);
  WiFiSerial.begin(SOFTWARE_WIFISERIAL_BAUD); // Set SoftwareSerial baud
  
  pinMode(RTC_PWR_KEY, OUTPUT);
  digitalWrite(RTC_PWR_KEY, HIGH);
  Serial.println("Connecting to WiFi\n");

  // Initialize WiFi module
  if (!initializeWiFiModule()) {
    Serial.println("Failed to initialize WiFi module");
    while (1);
  }

  // Connect to WiFi
  if (!connectToWiFi()) {
    Serial.println("Failed to connect to WiFi");
    while (1);
  }

  // Establish TCP connection
  if (!establishTCPConnection()) {
    Serial.println("Failed to establish TCP connection");
    while (1);
  }
}

bool initializeWiFiModule() {
  // Listen for ready message ("+INIT:DONE")
  String msg = "";
  byte count = 0;
  while (count < 20) {
    while (WiFiSerial.available()) {
      msg += char(WiFiSerial.read());
    }
    if (msg.length() > 5) break;
    count++;
    delay(100);
  }
  msg = msg.substring(3, msg.length()); // Remove NULL, CR, LF characters from response
  
  if (msg.length() > 5) {
    Serial.println("Expecting: \"INIT:DONE,(0 or 1)");
    Serial.println("Received: " + msg);
    return true;
  } else {
    Serial.println("Failed to receive initialization message.\nMake sure you're using the correct baud rate.\n");
    return false;
  }
}

bool connectToWiFi() {
  Serial.println("Sending:AT+WFMODE=0");
  WiFiSerial.println("AT+WFMODE=0");
  if (!waitForResponse("OK")) return false;

  Serial.println("Sending:AT+RESTART");
  WiFiSerial.println("AT+RESTART");
  if (!waitForResponse("OK")) return false;

  // Listen for ready message ("+INIT:DONE") after the reset is finished
  byte count = 0;
  String msg = "";
  while (count < 20) {
    while (WiFiSerial.available()) {
      msg += char(WiFiSerial.read());
    }
    if (msg.length() > 5) break;
    count++;
    delay(100);
  }
  msg = msg.substring(3, msg.length()); // Remove NULL, CR, LF characters from response

  if (msg.length() > 5) {
    Serial.println("Expecting: \"INIT:DONE,(0 or 1)");
    Serial.println("Received: " + msg);
  } else {
    Serial.println("Failed to receive initialization message.\nContinuing anyway...\n");
  }

  // Connect to WiFi using the provided credentials
  Serial.println("Sending:AT+WFJAPA=" + wifiSSID + "," + wifiPass);
  WiFiSerial.println("AT+WFJAPA=" + wifiSSID + "," + wifiPass);

  Serial.println("Waiting for connection response...");
  while (1) {
    msg = "";
    while (WiFiSerial.available()) {
      msg += char(WiFiSerial.read());
      delay(1);
    }

    if (msg.length() > 10) {
      Serial.print("Response:");
      Serial.println(msg);
      break;
    }
  }

  msg = msg.substring(3, msg.length()); // Remove NULL, CR, LF characters from response

  // If connection to AP is successful, response will be WFJAP:1,SSID,IP_ADDRESS, or WFJAP:0 if failed
  return msg.startsWith("WFJAP:1");
}

bool establishTCPConnection() {
  Serial.println("Establishing TCP connection...");

  Serial.println("Sending:AT+TRTC=" + String(serverIP) + "," + String(serverPort)+","+String(LOCAL_PORT));
  WiFiSerial.println("AT+TRTC=" + String(serverIP) + "," + String(serverPort)+","+String(LOCAL_PORT));

  Serial.println("Sending:AT+TRSAVE");
  WiFiSerial.println("AT+TRSAVE");
  
  return waitForResponse("OK");
}

bool waitForResponse(String expected) {
  String msg = "";
  unsigned long start = millis();
  while (millis() - start < 5000) { // 5 seconds timeout
    while (WiFiSerial.available()) {
      msg += char(WiFiSerial.read());
      delay(1);
    }
    if (msg.indexOf(expected) != -1) {
      Serial.print(msg);
      return true;
    }
  }
  Serial.print(msg);
  return false;
}

void sendSensorValue(int value) {
  // Send sensor value
  WiFiSerial.write(0x1B); // Send ESC character
  WiFiSerial.print("S10,0,0," + String(value));
  Serial.println("Data sent: " + String(value));
}

void loop() {  
  int sensorValue = analogRead(A0); // read the input on analog pin A0
  Serial.println(sensorValue); // print out the value you read
  sendSensorValue(sensorValue);
  delay(50); // to avoid overloading the serial terminal
  while(1);
}

