//Volume air akan dihitung setiap 5 detik dan dijumlahkan secara langsung
//Tiap jam akan disimpan data berupa jumlah volume air pada jam tersebut sehingga
//dapat diketahui pengeluaran terbesar pada jam-jam berapa saja

#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <ds3231.h>
#include <SD.h>
#include <SPI.h>
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
float DebitAir;
volatile long frekuensi_aliran;

//RTC
struct ts t;

//MICROSD CARD
File myFile;
