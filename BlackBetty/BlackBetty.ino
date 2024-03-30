/**
 * @file      UnitTest.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2023-04-11
 * @note      Arduino Setting
 *            Tools ->
 *                  Board:"ESP32S3 Dev Module"
 *                  USB CDC On Boot:"Enable"
 *                  USB DFU On Boot:"Disable"
 *                  Flash Size : "16MB(128Mb)"
 *                  Flash Mode"QIO 80MHz
 *                  Partition Scheme:"16M Flash(3M APP/9.9MB FATFS)"
 *                  PSRAM:"OPI PSRAM"
 *                  Upload Mode:"UART0/Hardware CDC"
 *                  USB Mode:"Hardware CDC and JTAG"
 */
#include <Arduino.h>
#include <SPI.h>
#include <RadioLib.h>
#include <TFT_eSPI.h>
#include <lvgl.h>
#include <SD.h>
#include "es7210.h"
#include <Audio.h>
#include <driver/i2s.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
#include "time.h"
#include <ESP32Time.h>

const char* host = "BlackBetty";
const char* ssid = "Livebox-Ncls";
const char* password = "caramel75";

WebServer server(80);

ESP32Time rtc(3600);  // offset in seconds GMT+1

void setupLvgl();

// By default, the audio pass-through speaker is used for testing, and esp_sr can also be used for noise detection.
// #define USE_ESP_VAD

#define TOUCH_MODULES_GT911
#include "TouchLib.h"
#include "utilities.h"

#if TFT_DC != BOARD_TFT_DC || TFT_CS != BOARD_TFT_CS || TFT_MOSI != BOARD_SPI_MOSI || TFT_SCLK != BOARD_SPI_SCK
#error "Not using the already configured T-Deck file, please remove <Arduino/libraries/TFT_eSPI> and replace with <lib/TFT_eSPI>, please do not click the upgrade library button when opening sketches in ArduinoIDE versions 2.0 and above, otherwise the original configuration file will be replaced !!!"
#error "Not using the already configured T-Deck file, please remove <Arduino/libraries/TFT_eSPI> and replace with <lib/TFT_eSPI>, please do not click the upgrade library button when opening sketches in ArduinoIDE versions 2.0 and above, otherwise the original configuration file will be replaced !!!"
#error "Not using the already configured T-Deck file, please remove <Arduino/libraries/TFT_eSPI> and replace with <lib/TFT_eSPI>, please do not click the upgrade library button when opening sketches in ArduinoIDE versions 2.0 and above, otherwise the original configuration file will be replaced !!!"
#endif

#ifndef BOARD_HAS_PSRAM
#error "Detected that PSRAM is not turned on. Please set PSRAM to OPI PSRAM in ArduinoIDE"
#endif

#ifndef RADIO_FREQ
#define RADIO_FREQ 868.0
#endif

#define DEFAULT_COLOR (lv_color_make(252, 218, 72))
#define MIC_I2S_SAMPLE_RATE 16000
#define MIC_I2S_PORT I2S_NUM_1
#define SPK_I2S_PORT I2S_NUM_0
#define VAD_SAMPLE_RATE_HZ 16000
#define VAD_FRAME_LENGTH_MS 30
#define VAD_BUFFER_LENGTH (VAD_FRAME_LENGTH_MS * VAD_SAMPLE_RATE_HZ / 1000)
#define LVGL_BUFFER_SIZE (TFT_WIDTH * TFT_HEIGHT * sizeof(lv_color_t))

LV_IMG_DECLARE(image);
// LV_IMG_DECLARE(image1);
// LV_IMG_DECLARE(image2);
// LV_IMG_DECLARE(image3);
// LV_IMG_DECLARE(image4);
LV_IMG_DECLARE(boot); // WALLPAPER
LV_IMG_DECLARE(image_cursor);

enum DemoEvent
{
    DEMO_TX_BTN_CLICK_EVENT,
    DEMO_RX_BTN_CLICK_EVENT,
    DEMO_OTA_BTN_CLICK_EVENT,
    DEMO_VAD_BTN_CLICK_EVENT,
    DEMO_PLAY_BTN_CLICK_EVENT,
    DEMO_SLEEP_BTN_CLICK_EVENT
};

static const DemoEvent event[] = {
    DEMO_TX_BTN_CLICK_EVENT,
    DEMO_RX_BTN_CLICK_EVENT,
    DEMO_OTA_BTN_CLICK_EVENT,
    DEMO_VAD_BTN_CLICK_EVENT,
    DEMO_PLAY_BTN_CLICK_EVENT,
    DEMO_SLEEP_BTN_CLICK_EVENT,
};

#ifdef USE_ESP_VAD
#include <esp_vad.h>
int16_t *vad_buff;
vad_handle_t vad_inst;
const size_t vad_buffer_size = VAD_BUFFER_LENGTH * sizeof(short);
#else
uint16_t loopbackBuffer[3200] = {0};
#endif

TouchLib *touch = NULL;
SX1262 radio = new Module(RADIO_CS_PIN, RADIO_DIO1_PIN, RADIO_RST_PIN, RADIO_BUSY_PIN);
TFT_eSPI tft;
Audio audio;
size_t bytes_read;
uint8_t status;
TaskHandle_t playHandle = NULL;
TaskHandle_t radioHandle = NULL;

static lv_obj_t *vad_btn_label;
static uint32_t vad_detected_counter = 0;
static TaskHandle_t vadTaskHandler;
bool transmissionFlag = true;
bool enableInterrupt = true;
int transmissionState;
bool hasRadio = false;
bool touchDected = false;
bool kbDected = false;
bool sender = true;
bool enterSleep = false;
uint32_t sendCount = 0;
uint32_t runningMillis = 0;
uint8_t touchAddress = GT911_SLAVE_ADDRESS2;

lv_indev_t *kb_indev = NULL;
lv_indev_t *mouse_indev = NULL;
lv_indev_t *touch_indev = NULL;
lv_group_t *kb_indev_group;
lv_obj_t *radio_ta;
SemaphoreHandle_t xSemaphore = NULL;

char lheure[20] = "";
char Vbattexte[20] = "";
int p = 0;
int minute = 0;
int timesincelastboot = 0;
lv_obj_t *labelVbat;
lv_obj_t *labelTime;
int VbatAnalog = 0;//valeur analogique de la battery
int Vbat = 0;//pourcentage de la battery
int Vbat_ = 0;//pourcentage de la battery
lv_obj_t *btn6;
lv_obj_t *btn7;

void setup()
{
    Serial.begin(115200);
    Serial.println("T-DECK factory");

    //! The board peripheral power control pin needs to be set to HIGH when using the peripheral
    pinMode(BOARD_POWERON, OUTPUT);
    digitalWrite(BOARD_POWERON, HIGH);

    //! Set CS on all SPI buses to high level during initialization
    pinMode(BOARD_SDCARD_CS, OUTPUT);
    pinMode(RADIO_CS_PIN, OUTPUT);
    pinMode(BOARD_TFT_CS, OUTPUT);

    digitalWrite(BOARD_SDCARD_CS, HIGH);
    digitalWrite(RADIO_CS_PIN, HIGH);
    digitalWrite(BOARD_TFT_CS, HIGH);

    pinMode(BOARD_SPI_MISO, INPUT_PULLUP);
    SPI.begin(BOARD_SPI_SCK, BOARD_SPI_MISO, BOARD_SPI_MOSI); // SD

    pinMode(BOARD_BOOT_PIN, INPUT_PULLUP);
    pinMode(BOARD_TBOX_G02, INPUT_PULLUP);
    pinMode(BOARD_TBOX_G01, INPUT_PULLUP);
    pinMode(BOARD_TBOX_G04, INPUT_PULLUP);
    pinMode(BOARD_TBOX_G03, INPUT_PULLUP);

    // Wakeup touch chip
    pinMode(BOARD_TOUCH_INT, OUTPUT);
    digitalWrite(BOARD_TOUCH_INT, HIGH);

    // Add mutex to allow multitasking access
    xSemaphore = xSemaphoreCreateBinary();
    assert(xSemaphore);
    xSemaphoreGive(xSemaphore);

    tft.begin();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    Wire.begin(BOARD_I2C_SDA, BOARD_I2C_SCL);

    // Set touch int input
    pinMode(BOARD_TOUCH_INT, INPUT);
    analogReadResolution(10);
    //analogSetPinAttenuation(BOARD_BAT_ADC, ADC_0db);
    delay(20);

    // Two touch screens, the difference between them is the device address,
    // use ScanDevices to get the existing I2C address
    scanDevices(&Wire);

    touch = new TouchLib(Wire, BOARD_I2C_SDA, BOARD_I2C_SCL, touchAddress);

    touch->init();

    Wire.beginTransmission(touchAddress);
    touchDected = Wire.endTransmission() == 0;

    kbDected = checkKb();

    setupLvgl();

    SPIFFS.begin();

    setupSD();

    setupRadio();

    setupCoder();

    setupAmpI2S(SPK_I2S_PORT);

    setupMicrophoneI2S(MIC_I2S_PORT);

    /*
        // Test screen
        lv_obj_t *label;

        const lv_img_dsc_t *img_src[4] = {&image1, &image2, &image3, &image4};
        lv_obj_t *img = lv_img_create(lv_scr_act());
        lv_img_set_src(img, (void *)(img_src[3]));

        delay(4000);

        int i = 2;
        while (i >= 0) {
            lv_img_set_src(img, (void *)(img_src[i]));
            lv_task_handler();
            i--;
            delay(2000);
        }

        lv_obj_del(img);
    */
    // Adjust backlight
    pinMode(BOARD_BL_PIN, OUTPUT);
    // T-Deck control backlight chip has 16 levels of adjustment range
    setBrightness(16);
    lv_task_handler();

    int z=0;
    while(z == 0){
    interface_verif();
    ///////////////////////////////////////////////////VERIF QUE TOUT FONCTIONNE
    if((kbDected == true)&&(touchDected == true)&&(hasRadio == true)&&(SD.cardType() != CARD_NONE)){
        z=1;
    }
    interface_clean();
    }

    uint32_t endTime = millis() + 500;
    while (millis() < endTime)
    {
        lv_task_handler();
        delay(1);
    }

    TimeOverNtp();

    interface_noscroll();

    //interface_main();

    interface_dev();

  
}

void loop()
{

    if (enterSleep){sleep();}

    loobBattery();
    //loopRadio();

    timesincelastboot ++;

    //lv_label_set_text(labelTime, rtc.getTimeDate().c_str());

  lv_task_handler(); // Gérer les tâches LVGL
  delay(0.5);


}
