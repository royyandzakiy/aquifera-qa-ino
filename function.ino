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
}
