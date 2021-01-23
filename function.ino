/*****************************************************
 * DAFTAR FUNGSI
 * InterruptDebitAir() = interrupt dari pin 2
 * HitungDebitAir() = hitung debit tiap detik (L/s)
 * HitungVolumeAir() = hitung volume tiap 10 detik (mL) dan menyimpan data tiap jam dalam eeprom
 * SendMessage() = kirim sms hasil perhitungan volume
 * EEPROM16_Write(....) = menyimpan nilai dalam eeprom > kelemahan akan dibulatkan. tetapi selama arduino tidak mati hasil Volume terakhir tidak akan dibulatkan
 * HapusEeprom() = mengubah isi eeprom dengan nilai 0
 * getFilename(..) = membuat file baru tiap hari yang isinya data dari perhitungan volume tiap sekian detik
 * getFilename2(..) = membuat file data tiap bulan isinya data volume akhir tiap hari
 */

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

void BlinkTask(void *param) {
    (void) param;
    pinMode(LED_BUILTIN, OUTPUT);
    for(;;) {
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
      Serial.println("On");
      vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
      digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
      Serial.println("Off");
      vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
  }
    vTaskDelete(NULL);
}
