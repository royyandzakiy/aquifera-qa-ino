void setupFlowSensor();
void resetEeprom();
void setupRTC();
void setTimeRTC();
void setupSDCard();
void InterruptDebitAir();

void setup()
{
  // Setup_Start
  setupSerial();
  // setupSim() // [SALMAN] cek ini
  setupRTC();
  // setupFlowSensor(); // initializes flow sensor through pin connections
  setupSDCard(); // [SALMAN] cek ini
  // Setup_End
  
  Serial.println("Setup: Initialization done.");
  EspSerial.println("Setup: Initialization done.");
}

// Setup Serial
void setupSerial() {
  Serial.begin(9600);
  EspSerial.begin(9600);

  while (!Serial);
  Serial.println("Setup: Initialize...");
  EspSerial.println("Setup: Initialize...");
}

// Setup SIM
void setupSim() {
  Serial.println("Setup: Initializing SIM...");
  // delay(7000); //delay  detik untuk memastikan modul sim sudah siap 
  _buffer.reserve(50);
  sim.begin(9600);

  testSim();
}

void testSim() {
  // send GPRS here
  // ...still nothing
}

// Setup SD Card
void setupSDCard() {
  Serial.println("Setup: Initializing SD Card...");
 
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
  
  Serial.println("Setup: SD Card Done!");
}

// Setup RTC
void setupRTC() {
  // initializes RTC
  Serial.println("Setup: Initializing RTC...");

  Wire.begin();
  DS3231_init(DS3231_CONTROL_INTCN);

  testRTC();
}

void testRTC() {
  // set time
  t.hour=12; 
  t.min=30;
  t.sec=0;
  t.mday=25;
  t.mon=12;
  t.year=2020;
  
  DS3231_set(t);

  // get time
  DS3231_get(&t);
  Serial.print("Date : ");
  Serial.print(t.mday);
  Serial.print("/");
  Serial.print(t.mon);
  Serial.print("/");
  Serial.print(t.year); 
  Serial.print("\t Hour : ");
  Serial.print(t.hour);
  Serial.print(":");
  Serial.print(t.min);
  Serial.print(".");
  Serial.print(t.sec);
  Serial.println();  
}

// Setup Flow Sensor
void setupFlowSensor() 
{
  // initializes flow sensor
  Serial.println("Setup: Initializing Flow Sensor...");
 
  pinMode(FLOWSENSOR_PIN, INPUT);
  digitalWrite(FLOWSENSOR_PIN, HIGH);
  attachInterrupt(0,InterruptDebitAir,RISING); // di pin 2
  sei();

  xTaskCreate(DebitTask, "DebitTask", 128, NULL, 2, NULL);
}

void InterruptDebitAir()
{
  frekuensi_aliran++;
}

void HitungDebitAir()
{
    DebitAir = (frekuensi_aliran/7.5)*K/60.0/WaktuDebitAir; //L/s
    frekuensi_aliran = 0;
    Serial.print("Debit Air: ");
    Serial.println(DebitAir);
}

void DebitTask(void *param) {
    (void) param;
    Serial.println("debitTask: Executing on core ");
    
     for(;;) {
         HitungDebitAir();
         vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay between loops to reset watchdog timer portMAX_DELAY
     }
    vTaskDelete(NULL);
}
