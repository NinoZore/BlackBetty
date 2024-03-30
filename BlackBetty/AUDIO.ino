bool setupCoder()
{
    uint32_t ret_val = ESP_OK;

    Wire.beginTransmission(ES7210_ADDR);
    uint8_t error = Wire.endTransmission();
    if (error != 0)
    {
        Serial.println("ES7210 address not found");
        return false;
    }

    audio_hal_codec_config_t cfg = {
        .adc_input = AUDIO_HAL_ADC_INPUT_ALL,
        .codec_mode = AUDIO_HAL_CODEC_MODE_ENCODE,
        .i2s_iface =
            {
                .mode = AUDIO_HAL_MODE_SLAVE,
                .fmt = AUDIO_HAL_I2S_NORMAL,
                .samples = AUDIO_HAL_16K_SAMPLES,
                .bits = AUDIO_HAL_BIT_LENGTH_16BITS,
            },
    };

    ret_val |= es7210_adc_init(&Wire, &cfg);
    ret_val |= es7210_adc_config_i2s(cfg.codec_mode, &cfg.i2s_iface);
    ret_val |= es7210_adc_set_gain(
        (es7210_input_mics_t)(ES7210_INPUT_MIC1 | ES7210_INPUT_MIC2),
        (es7210_gain_value_t)GAIN_6DB);
    ret_val |= es7210_adc_ctrl_state(cfg.codec_mode, AUDIO_HAL_CTRL_START);
    return ret_val == ESP_OK;
}

void taskplaySong(void *p)
{
    while (1)
    {
        if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE)
        {
            /////////////////////////////////////////////////////////////////////////////playTTS("hello.mp3");
            xSemaphoreGive(xSemaphore);
        }
        vTaskSuspend(NULL);
    }
}

#ifdef USE_ESP_VAD
void vadTask(void *params)
{
    Serial.println("vadTask(void *params)");

    vTaskSuspend(NULL);
    while (1)
    {
        size_t read_len = 0;
        if (i2s_read(MIC_I2S_PORT, (char *)vad_buff, vad_buffer_size, &read_len, portMAX_DELAY) == ESP_OK)
        {
            // if (watch.readMicrophone((char *) vad_buff, vad_buffer_size, &read_len)) {
            // Feed samples to the VAD process and get the result
#if ESP_IDF_VERSION_VAL(4, 4, 1) == ESP_IDF_VERSION
            vad_state_t vad_state = vad_process(vad_inst, vad_buff);
#elif ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(4, 4, 1) && ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 0, 0)
            vad_state_t vad_state = vad_process(vad_inst, vad_buff, MIC_I2S_SAMPLE_RATE, VAD_FRAME_LENGTH_MS);
#else
#error "ESP VAD Not support Version > V5.0.0 , please use IDF V4.4.4"
#endif
            if (vad_state == VAD_SPEECH)
            {
                Serial.print(millis());
                Serial.println(" -> Noise detected!!!");
                if (vad_btn_label)
                {
                    lv_label_set_text_fmt(vad_btn_label, "Noise %u", vad_detected_counter++);
                }
            }
        }
        delay(5);
    }
}

#else

void audioLoopbackTask(void *params)
{
    vTaskSuspend(NULL);
    while (1)
    {
        delay(5);
        size_t bytes_read = 0, bytes_write = 0;
        memset(loopbackBuffer, 0, sizeof(loopbackBuffer));
        i2s_read(MIC_I2S_PORT, loopbackBuffer, sizeof(loopbackBuffer), &bytes_read, 15);
        if (bytes_read)
        {
            i2s_write(SPK_I2S_PORT, loopbackBuffer, bytes_read, &bytes_write, 15);
        }
    }
    vTaskDelete(NULL);
}

#endif

void setupMicrophoneI2S(i2s_port_t i2s_ch)
{
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = MIC_I2S_SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ALL_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = 64,
        .use_apll = false,
        .tx_desc_auto_clear = true,
        .fixed_mclk = 0,
        .mclk_multiple = I2S_MCLK_MULTIPLE_256,
        .bits_per_chan = I2S_BITS_PER_CHAN_16BIT,
        .chan_mask = (i2s_channel_t)(I2S_TDM_ACTIVE_CH0 | I2S_TDM_ACTIVE_CH1 |
                                     I2S_TDM_ACTIVE_CH2 | I2S_TDM_ACTIVE_CH3),
        .total_chan = 4,
    };
    i2s_pin_config_t pin_config = {0};
    pin_config.data_in_num = BOARD_ES7210_DIN;
    pin_config.mck_io_num = BOARD_ES7210_MCLK;
    pin_config.bck_io_num = BOARD_ES7210_SCK;
    pin_config.ws_io_num = BOARD_ES7210_LRCK;
    pin_config.data_out_num = -1;
    i2s_driver_install(i2s_ch, &i2s_config, 0, NULL);
    i2s_set_pin(i2s_ch, &pin_config);
    i2s_zero_dma_buffer(i2s_ch);

#ifdef USE_ESP_VAD
    // Initialize esp-sr vad detected
#if ESP_IDF_VERSION_VAL(4, 4, 1) == ESP_IDF_VERSION
    vad_inst = vad_create(VAD_MODE_0, MIC_I2S_SAMPLE_RATE, VAD_FRAME_LENGTH_MS);
#elif ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(4, 4, 1)
    vad_inst = vad_create(VAD_MODE_0);
#else
#error "No support this version."
#endif
    vad_buff = (int16_t *)ps_malloc(vad_buffer_size);
    if (vad_buff == NULL)
    {
        while (1)
        {
            Serial.println("Memory allocation failed!");
            delay(1000);
        }
    }
    xTaskCreate(vadTask, "vad", 8 * 1024, NULL, 12, &vadTaskHandler);
#else
    xTaskCreate(audioLoopbackTask, "vad", 8 * 1024, NULL, 12, &vadTaskHandler);
#endif
}

void playTTS(const char *filename)
{
    vTaskSuspend(vadTaskHandler);
#ifdef USE_ESP_VAD
    lv_label_set_text(vad_btn_label, "VAD detect");
#else
    lv_label_set_text(vad_btn_label, "loopback");
#endif

    bool findMp3 = false;

    if (SD.exists("/" + String(filename)))
    {
        findMp3 = audio.connecttoFS(SD, filename);
        Serial.println("findMp3SD :");
        Serial.println(findMp3);
    }

    else if (SPIFFS.exists("/" + String(filename)))
    {
        findMp3 = audio.connecttoFS(SPIFFS, filename);
        Serial.println("findMp3SPIFFS :");
        Serial.println(findMp3);
    }

    if (findMp3)
    {
        while (audio.isRunning())
        {
            audio.loop();
            delay(3);
        }
    }
}

void setupAmpI2S(i2s_port_t i2s_ch)
{
    audio.setPinout(BOARD_I2S_BCK, BOARD_I2S_WS, BOARD_I2S_DOUT);
    audio.setVolume(21);
}