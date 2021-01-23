void setupFlowSensor();
void resetEeprom();
void setupRTC();
void setTimeRTC();
void setupSDCard();

void setup()
{
  // delay(7000); //delay  detik untuk memastikan modul sim sudah siap 
  setupSerial();

  // MODUL SIM
  // _buffer.reserve(50);
  // sim.begin(9600); // [SALMAN] cek ini
  
  // MODUL RTC
  setupRTC();
  
  // SENOR DEBIT
  setupFlowSensor(); // initializes flow sensor through pin connections
  delay(1000);

  // MODUL MIKROSD
  // setupSDCard(); // [SALMAN] cek ini

  // Setup done
  Serial.println("Setup: Initialization done.");
  EspSerial.println("Setup: Initialization done.");
}

void setupSerial() {
  Serial.begin(9600);
  EspSerial.begin(9600);

  while (!Serial);
  Serial.println("Setup: Initialize...");
  EspSerial.println("Setup: Initialize...");
}

void setupSDCard() {
  Serial.print("Setup: Initializing SD Card...");
 
  if (!SD.begin(4)) {
    Serial.println("Setup: Initialization SD Card failed!");
    while (1);
  } else {
    Serial.println("Setup: Initialization SD Card successs!");
    // try create, write, delete file
    // testSDCard();
  }
}

void testSDCard() {
  // perform create, write, delete file [still WIP]
  myFile = SD.open("data.txt", FILE_WRITE);
  int data = 0;
  for (int i=0; i<3; i++)
    myFile.write(data++);
  
  char line[100];
  int n=0;

  // continue here
  
  Serial.println("Setup: SD Card Done!");
}

void setupRTC() {
  // initializes RTC
  Serial.print("Setup: Initializing RTC...");

  Wire.begin();
  DS3231_init(DS3231_CONTROL_INTCN);
}

void setupFlowSensor() 
{
  // initializes flow sensor
  Serial.print("Setup: Initializing Flow Sensor...");
 
  pinMode(FLOWSENSOR_PIN, INPUT);
  digitalWrite(FLOWSENSOR_PIN, HIGH);
  attachInterrupt(0,InterruptDebitAir,RISING); // di pin 2
  sei();
}
