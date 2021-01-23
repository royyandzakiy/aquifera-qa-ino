//Volume air akan dihitung setiap 5 detik dan dijumlahkan secara langsung
//Tiap jam akan disimpan data berupa jumlah volume air pada jam tersebut sehingga
//dapat diketahui pengeluaran terbesar pada jam-jam berapa saja

// #include <EEPROM.h>
#include <SoftwareSerial.h>
#include <Wire.h>
// #include <ds3231.h>
// #include <SD.h>
// #include <SPI.h>
// #include <Arduino_FreeRTOS.h>
#include <Time.h>
#include <TimeAlarms.h>
//======================================================================//

#ifndef CREDENTIALS_H
  char dest_phone_no[16] = "+000000000000";
#endif

SoftwareSerial EspSerial(6, 5);

// SIM MODULE
SoftwareSerial sim(10, 9);
String  _buffer;
String  number = dest_phone_no;

// FLOW METER SENSOR
#define FLOWSENSOR_PIN 2
const long WaktuDebitAir = 5; //pengambilan data setiap 5 detik
int K = 2.6; // Konstanta flow sensor
float DebitAir = 0.0;
int frekuensi_aliran = 0;

//RTC
// struct ts t;

//MICROSD CARD
// File myFile;

// --------

void setupFlowSensor();
void resetEeprom();
void setupRTC();
void setTimeRTC();
void setupSDCard();
void interruptFlowSensor();

//AlarmId id;

void setup()
{
  // ******* Setup_Start ******* 
  setupSerial();
  // setupSim(); // [SALMAN] cek ini
  // setupRTC(); // abaikan aja ini
  setupFlowSensor(); // abaikan aja ini
  // setupSDCard(); // [SALMAN] cek ini
  // ******* Setup_End *******

  setTime(22,15,30,23,1,21);
  Alarm.timerRepeat(5, updateDebitCount);
  Alarm.timerRepeat(15, reportDebitCount);
  
  Serial.println("Setup: Initialization done.");
  EspSerial.println("Setup: Initialization done.");
}

void loop() 
{
  echo();
  listenEsp();
//   digitalClockDisplay();
   Alarm.delay(0); // wait one second between clock display
}

// ------

void echo() {
  if(Serial.available()) {
    String str = Serial.readStringUntil('\n');
    Serial.println(str);
    sendEsp(str);
    delay(10);
  }
}

void listenEsp() {
  if(EspSerial.available()) {
    String str = EspSerial.readStringUntil('\n');
    Serial.println(str);

    processListenEsp(str);
    delay(10);
  }
}

void processListenEsp(String str) {
  if (str=="set_time") {
    // set time of internal Alarm.time
    Serial.println("set_time command engegaed");
  } else if (str=="get_time") {
    // send current internal Alarm.time
    Serial.println("get_time command engegaed");
  } else if (str=="set_time_rtc") {
    // set current internal time from rtc
    Serial.println("set_time_rtc command engegaed");
  } else if (str=="set_time_rtc") {
    // send current internal time from rtc
    Serial.println("get_time_rtc command engegaed");
  } else if (str=="get_info_waterbox") {
    // send info of waterbox ID
    Serial.println("get_info_waterbox command engegaed");
  } else if (str=="get_debit_count") {
    // send current debit count
    Serial.println("get_debit_count command engegaed");
  } else if (str=="get_debit_data") {
    // send debit data in sd card
    Serial.println("get_debit_data command engegaed");
  }
}

void sendEsp(String str) {
  EspSerial.println(str);
}

// Setup Serial
void setupSerial() {
  Serial.begin(9600);
  EspSerial.begin(9600);

  while (!Serial);
  Serial.println("Setup: Initialize Serial...");
  EspSerial.println("Setup: Initialize Serial...");
}

//void digitalClockDisplay() {
//  // digital clock display of the time
//  Serial.print(hour());
//  printDigits(minute());
//  printDigits(second());
//  Serial.println();
//}
//
//void printDigits(int digits) {
//  Serial.print(":");
//  if (digits < 10)
//    Serial.print('0');
//  Serial.print(digits);
//}

// Setup SIM
//void setupSim() {
//  Serial.println("Setup: Initializing SIM...");
//  // delay(7000); //delay  detik untuk memastikan modul sim sudah siap 
//  _buffer.reserve(50);
//  sim.begin(9600);
//
//  testSim();
//}

//void testSim() {
//  // send GPRS here
//  // ...still nothing
//}

// Setup SD Card
//void setupSDCard() {
//  Serial.println("Setup: Initializing SD Card...");
// 
//  if (!SD.begin(4)) {
//    Serial.println("Setup: Initialization SD Card failed!");
//    while (1);
//  } else {
//    Serial.println("Setup: Initialization SD Card successs!");
//    // try create, write, delete file
//    // testSDCard();
//  }
//}

//void testSDCard() {
//  // perform create, write, delete file [still WIP]
//  myFile = SD.open("data.txt", FILE_WRITE);
//  int data = 0;
//  for (int i=0; i<3; i++)
//    myFile.write(data++);
//  
//  char line[100];
//  int n=0;
//  
//  Serial.println("Setup: SD Card Done!");
//}

// Setup RTC
//void setupRTC() {
//  // initializes RTC
//  Serial.println("Setup: Initializing RTC...");
//
//  Wire.begin();
//  DS3231_init(DS3231_CONTROL_INTCN);
//
//  testRTC();
//}
//
//void testRTC() {
//  // set time
//  t.hour=12; 
//  t.min=30;
//  t.sec=0;
//  t.mday=25;
//  t.mon=12;
//  t.year=2020;
//  
//  DS3231_set(t);
//
//  // get time
//  DS3231_get(&t);
//  Serial.print("Date : ");
//  Serial.print(t.mday);
//  Serial.print("/");
//  Serial.print(t.mon);
//  Serial.print("/");
//  Serial.print(t.year); 
//  Serial.print("\t Hour : ");
//  Serial.print(t.hour);
//  Serial.print(":");
//  Serial.print(t.min);
//  Serial.print(".");
//  Serial.print(t.sec);
//  Serial.println();  
//}

// Setup Flow Sensor
void setupFlowSensor() 
{
  // initializes flow sensor
  Serial.println("Setup: Initializing Flow Sensor...");
 
  pinMode(FLOWSENSOR_PIN, INPUT);
  digitalWrite(FLOWSENSOR_PIN, HIGH);
  attachInterrupt(0,interruptFlowSensor,RISING); // di pin 2
  sei();

//   xTaskCreate(DebitTask, "DebitTask", 128, NULL, 1, NULL);
  // xTaskCreate(BlinkTask, "BlinkTask", 128, NULL, 1, NULL);
}

void interruptFlowSensor()
{
  frekuensi_aliran++;
}

void updateDebitCount()
{
    DebitAir = (frekuensi_aliran/7.5)*K/60.0/WaktuDebitAir; //L/s

    // Print Debit Information
    printDebitCount();
}

void printDebitCount() {
  Serial.print("frekuensi_aliran: ");
  Serial.println(frekuensi_aliran);
  Serial.print("Debit Air: ");
  Serial.print(DebitAir);
  Serial.println(" L/s");
}

void reportDebitCount() {
  String topic = "waterbox/W0001/flow_sensor";
  String message = String(DebitAir);
  publishMqtt(topic,message);
  resetDebitCount();
}

void publishMqtt(String topic, String message) {
    EspSerial.println("pub:" + topic + ":" + message);
}

void resetDebitCount() {
  Serial.println("Debit Count Reset!");
  frekuensi_aliran = 0;
}

void commanadSetTime(int hr, int min, int sec, int day, int mon, int yr) {
  setTime(hr,min,sec,day,mon,yr);
}

//void DebitTask(void *param) {
//    (void) param;
//    // Serial.println("debitTask: Executing on core ");
//    
//     for(;;) {
//         // updateDebitCount();
//         // Serial.print("frekuensi_aliran: ");
//         Serial.println(frekuensi_aliran);
//         vTaskDelay(5000 / portTICK_PERIOD_MS); // Delay between loops to reset watchdog timer portMAX_DELAY
//     }
//    vTaskDelete(NULL);
//}
//
//void BlinkTask(void *param) {
//    (void) param;
//    pinMode(LED_BUILTIN, OUTPUT);
//    for(;;) {
//      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
//      Serial.println("On");
//      vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
//      digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
//      Serial.println("Off");
//      vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
//  }
//    vTaskDelete(NULL);
//}
