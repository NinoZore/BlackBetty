void setFlag(void)
{
    // check if the interrupt is enabled
    if (!enableInterrupt)
    {
        return;
    }
    // we got a packet, set the flag
    transmissionFlag = true;
}

bool checkKb()
{
    int retry = 3;
    do
    {
        Wire.requestFrom(0x55, 1);
        if (Wire.read() != -1)
        {
            return true;
        }
    } while (retry--);
    return false;
}


void scanDevices(TwoWire *w)
{
    uint8_t err, addr;
    int nDevices = 0;
    uint32_t start = 0;
    for (addr = 1; addr < 127; addr++)
    {
        start = millis();
        w->beginTransmission(addr);
        delay(2);
        err = w->endTransmission();
        if (err == 0)
        {
            nDevices++;
            Serial.print("I2C device found at address 0x");
            if (addr < 16)
            {
                Serial.print("0");
            }
            Serial.print(addr, HEX);
            Serial.println(" !");

            if (addr == GT911_SLAVE_ADDRESS2)
            {
                touchAddress = GT911_SLAVE_ADDRESS2;
                Serial.println("Find GT911 Drv Slave address: 0x14");
            }
            else if (addr == GT911_SLAVE_ADDRESS1)
            {
                touchAddress = GT911_SLAVE_ADDRESS1;
                Serial.println("Find GT911 Drv Slave address: 0x5D");
            }
        }
        else if (err == 4)
        {
            Serial.print("Unknow error at address 0x");
            if (addr < 16)
            {
                Serial.print("0");
            }
            Serial.println(addr, HEX);
        }
    }
    if (nDevices == 0)
        Serial.println("No I2C devices found\n");
}

bool setupSD()
{
    digitalWrite(BOARD_SDCARD_CS, HIGH);
    digitalWrite(RADIO_CS_PIN, HIGH);
    digitalWrite(BOARD_TFT_CS, HIGH);

    if (SD.begin(BOARD_SDCARD_CS, SPI, 800000U))
    {
        uint8_t cardType = SD.cardType();
        if (cardType == CARD_NONE)
        {
            Serial.println("No SD_MMC card attached");
            return false;
        }
        else
        {
            Serial.print("SD_MMC Card Type: ");
            if (cardType == CARD_MMC)
            {
                Serial.println("MMC");
            }
            else if (cardType == CARD_SD)
            {
                Serial.println("SDSC");
            }
            else if (cardType == CARD_SDHC)
            {
                Serial.println("SDHC");
            }
            else
            {
                Serial.println("UNKNOWN");
            }
            uint32_t cardSize = SD.cardSize() / (1024 * 1024);
            uint32_t cardTotal = SD.totalBytes() / (1024 * 1024);
            uint32_t cardUsed = SD.usedBytes() / (1024 * 1024);
            Serial.printf("SD Card Size: %lu MB\n", cardSize);
            Serial.printf("Total space: %lu MB\n", cardTotal);
            Serial.printf("Used space: %lu MB\n", cardUsed);
            return true;
        }
    }
    return false;
}

void loobBattery()
{
  VbatAnalog = analogRead(BOARD_BAT_ADC);
  Vbat = map(VbatAnalog, 300, 1020, 0, 100);
  Serial.printf("VbatAnalog: %d", VbatAnalog);
  Serial.printf("Vbat: %d", Vbat);
  /////
  sprintf(Vbattexte, "%d", VbatAnalog);
  lv_label_set_text(labelVbat, Vbattexte);
  /*
  if(VbatAnalog == 1023){
    sprintf(Vbattexte, "%d%%", Vbat);
    lv_label_set_text(labelVbat, "charge");
  }  
  else if(Vbat != Vbat_){
  sprintf(Vbattexte, "%d%%", Vbat);
  lv_label_set_text(labelVbat, Vbattexte);
  Vbat_ = Vbat;
  }
  */
}

