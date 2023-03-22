#include <Arduino.h>
#include "WiFi.h"
#include "time.h"

const char *ssid = "";
const char *pass =  "";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;

const int hhLeds[5] = {33, 32, 27, 26, 25};
const int mmLeds[6] = {23, 22, 21, 19, 18, 17};
const int ssLeds[6] = {16, 15, 14, 13, 5, 4};
struct tm timeinfo;

byte binaryHH[8];
byte binaryMM[8];
byte binarySS[8];

void gpio_set_leds() {  //Initialize LEDs for HH:MM:SS
  Serial.print("Setting HH pins: ");
  for (int i=0; i<5; i++){
    Serial.print(hhLeds[i]);
    Serial.print(", ");
    pinMode(hhLeds[i], OUTPUT);
  }
  Serial.println("");
  Serial.print("Setting MM pins: ");
  for (int i=0; i<6; i++){
    Serial.print(mmLeds[i]);
    Serial.print(", ");
    pinMode(mmLeds[i], OUTPUT);
  }
  Serial.println("");
  Serial.print("Setting SS pins: ");
  for (int i=0; i<6; i++){
    Serial.print(ssLeds[i]);
    Serial.print(", ");
    pinMode(ssLeds[i], OUTPUT);
  }
  Serial.println("");
}

int serial_mon_start () {  //Initialize Seial monitor with max speed
  Serial.begin(115200);
  Serial.println("Hello!");
  return 0;
}

int connect_to_wifi () {  //Connect to WiFi with hard coded credentials
  Serial.print("WiFi connect attempt: ");
  Serial.println(ssid);
  Serial.print("connecting...");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("ok!");
  Serial.print("Connected to the WiFi network: ");
  Serial.println(ssid);
  
  return 0;
}

int print_time() {  // Print time to serial
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return 1;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  /*Serial.print("Day of week: ");
  Serial.println(&timeinfo, "%A");
  Serial.print("Month: ");
  Serial.println(&timeinfo, "%B");
  Serial.print("Day of Month: ");
  Serial.println(&timeinfo, "%d");
  Serial.print("Year: ");
  Serial.println(&timeinfo, "%Y");
  Serial.print("Hour: ");
  Serial.println(&timeinfo, "%H");
  Serial.print("Hour (12 hour format): ");
  Serial.println(&timeinfo, "%I");
  Serial.print("Minute: ");
  Serial.println(&timeinfo, "%M");
  Serial.print("Second: ");
  Serial.println(&timeinfo, "%S");

  Serial.println("Time variables");
  char timeHour[3];
  strftime(timeHour,3, "%H", &timeinfo);
  Serial.println(timeHour);
  char timeWeekDay[10];
  strftime(timeWeekDay,10, "%A", &timeinfo);
  Serial.println(timeWeekDay);
  Serial.println();*/
  return 0;
}

int set_rtc () {  // Get time from ntp server. Appearently after that ESP counts time itself.
  Serial.print("Getting time from: ");
  Serial.print(ntpServer);
  Serial.print("...");
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("ok!");
  return print_time();
}

void time_to_binary(int hours, int minutes, int seconds) {  // Convert intigers to binary arrays  
  //HH
   for (int i=0; i<8; i++) {
      binaryHH[i] = (hours >> i) & 1; //shifts right and &s last bit with 1
   }
  //MM
   for (int i=0; i<8; i++) {
      binaryMM[i] = (minutes >> i) & 1;
   }
  //SS
   for (int i=0; i<8; i++) {
      binarySS[i] = (seconds >> i) & 1;
   }
}

void update_time_leds() {  //Light the LEDS!
  //HH
  for (int i=0; i<5; i++) {
    if (binaryHH[i]) {digitalWrite(hhLeds[i], HIGH);}
    else {digitalWrite(hhLeds[i], LOW);}
  } 
  //MM
  for (int i=0; i<6; i++) {
    if (binaryMM[i]) {digitalWrite(mmLeds[i], HIGH);}
    else {digitalWrite(mmLeds[i], LOW);}
  }
  //SS 
  for (int i=0; i<6; i++) {
    if (binarySS[i]) {digitalWrite(ssLeds[i], HIGH);}
    else {digitalWrite(ssLeds[i], LOW);}
  } 
}

void print_int_arrays() {  //prints intiger arrays to serial - DEBUG ONLY
  for (int i=7; i>=0; i=i-1) {Serial.print(int(binaryHH[i]));}
  Serial.println("");
  for (int i=7; i>=0; i=i-1) {Serial.print(int(binaryMM[i]));}
  Serial.println("");
  for (int i=7; i>=0; i=i-1) {Serial.print(int(binarySS[i]));}
  Serial.println("");
}
void print_LEDs_state()  {  //prints LEDs state to serial - DEBUG ONLY
  //HH
  for (int i=0; i<5; i++) {
    Serial.print(hhLeds[i]);
    Serial.print("  ");
  }
  Serial.println("");
  for (int i=0; i<5; i++) {
    int ledRead = digitalRead(hhLeds[i]);
    Serial.print(ledRead);
    Serial.print("  ");
  }
  Serial.println("");
  for (int i=0; i<5; i++) {
    Serial.print(int(binaryHH[i]));
    Serial.print("  ");
  }
  Serial.println("");
  //MM
  for (int i=0; i<6; i++) {
    Serial.print(mmLeds[i]);
    Serial.print("  ");
  }
  Serial.println("");
  for (int i=0; i<6; i++) {
    int ledRead = digitalRead(mmLeds[i]);
    Serial.print(ledRead);
    Serial.print("  ");
  }
  Serial.println("");
  for (int i=0; i<6; i++) {
    Serial.print(int(binaryMM[i]));
    Serial.print("  ");
  }
  //SS
  Serial.println("");
  for (int i=0; i<6; i++) {
    Serial.print(ssLeds[i]);
    Serial.print("  ");
  }
  Serial.println("");
  for (int i=0; i<6; i++) {
    int ledRead = digitalRead(ssLeds[i]);
    Serial.print(ledRead);
    Serial.print("  ");
  }
  Serial.println("");
  for (int i=0; i<6; i++) {
    Serial.print(int(binarySS[i]));
    Serial.print("  ");
  }
  Serial.println("");
}

void setup() {
  serial_mon_start();
  gpio_set_leds();
  connect_to_wifi();
  set_rtc();
}

void loop() {
  delay(1000);
  print_time();
  

  time_to_binary(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  update_time_leds();
  print_LEDs_state();

}

#kkdkodn
