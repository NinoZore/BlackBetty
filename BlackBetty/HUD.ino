

lv_obj_t *main_count;
lv_obj_t *label;
static lv_style_t style;

enum DemoEvent
{
    DEMO_TX_BTN_CLICK_EVENT,
    DEMO_RX_BTN_CLICK_EVENT,
    DEMO_OTA_BTN_CLICK_EVENT,
    DEMO_VAD_BTN_CLICK_EVENT,
    DEMO_PLAY_BTN_CLICK_EVENT,
    DEMO_SLEEP_BTN_CLICK_EVENT,
    DEMO_RETURNHOME_BTN_CLICK_EVENT,
    DEMO_APP_BTN_CLICK_EVENT,
    DEMO_SETTINGS_BTN_CLICK_EVENT
};

static const DemoEvent event[] = {
    DEMO_TX_BTN_CLICK_EVENT,
    DEMO_RX_BTN_CLICK_EVENT,
    DEMO_OTA_BTN_CLICK_EVENT,
    DEMO_VAD_BTN_CLICK_EVENT,
    DEMO_PLAY_BTN_CLICK_EVENT,
    DEMO_SLEEP_BTN_CLICK_EVENT,
    DEMO_RETURNHOME_BTN_CLICK_EVENT,
    DEMO_APP_BTN_CLICK_EVENT,
    DEMO_SETTINGS_BTN_CLICK_EVENT
};

  
	lv_obj_t *screen_main;
	bool screen_main_del;
	lv_obj_t *screen_main_cont_1;
	lv_obj_t *screen_main_btn_1;
	lv_obj_t *screen_main_btn_1_label;
	lv_obj_t *screen_main_bar_1;
	lv_obj_t *screen_main_btn_2;
	lv_obj_t *screen_main_btn_2_label;
	lv_obj_t *screen_main_label_1;
	bool screen_param_del;
	lv_obj_t *screen_param_menu_1;
	lv_obj_t *screen_param_menu_1_sidebar_page;
	lv_obj_t *screen_param_menu_1_subpage_1;
	lv_obj_t *screen_param_menu_1_cont_1;
	lv_obj_t *screen_param_menu_1_label_1;
	lv_obj_t *screen_param_menu_1_subpage_2;
	lv_obj_t *screen_param_menu_1_cont_2;
	lv_obj_t *screen_param_menu_1_label_2;
	lv_obj_t *screen_param_menu_1_subpage_3;
	lv_obj_t *screen_param_menu_1_cont_3;
	lv_obj_t *screen_param_menu_1_label_3;
	lv_obj_t *screen_param_menu_1_subpage_4;
	lv_obj_t *screen_param_menu_1_cont_4;
	lv_obj_t *screen_param_menu_1_label_4;
	lv_obj_t *screen_param_menu_1_subpage_5;
	lv_obj_t *screen_param_menu_1_cont_5;
	lv_obj_t *screen_param_menu_1_label_5;
	lv_obj_t *screen_param_menu_1_subpage_6;
	lv_obj_t *screen_param_menu_1_cont_6;
	lv_obj_t *screen_param_menu_1_label_6;
	lv_obj_t *screen_param_menu_1_subpage_7;
	lv_obj_t *screen_param_menu_1_cont_7;
	lv_obj_t *screen_param_menu_1_label_7;
	lv_obj_t *screen_param_cont_2;
	lv_obj_t *screen_param_cb_2;
	lv_obj_t *screen_param_cb_4;
	lv_obj_t *screen_param_cont_3;
	lv_obj_t *screen_param_cb_3;
	lv_obj_t *screen_param_btn_1;
	lv_obj_t *screen_param_btn_1_label;
	lv_obj_t *screen_app;
	bool screen_app_del;
	lv_obj_t *screen_app_menu_1;
	lv_obj_t *screen_app_menu_1_sidebar_page;
	lv_obj_t *screen_app_menu_1_subpage_1;
	lv_obj_t *screen_app_menu_1_cont_1;
	lv_obj_t *screen_app_menu_1_label_1;
	lv_obj_t *screen_app_menu_1_subpage_2;
	lv_obj_t *screen_app_menu_1_cont_2;
	lv_obj_t *screen_app_menu_1_label_2;
	lv_obj_t *screen_app_menu_1_subpage_3;
	lv_obj_t *screen_app_menu_1_cont_3;
	lv_obj_t *screen_app_menu_1_label_3;
	lv_obj_t *screen_app_menu_1_subpage_4;
	lv_obj_t *screen_app_menu_1_cont_4;
	lv_obj_t *screen_app_menu_1_label_4;
	lv_obj_t *screen_app_btn_1;
	lv_obj_t *screen_app_btn_1_label;


static bool getTouch(int16_t &x, int16_t &y);


// !!! LVGL !!!
// !!! LVGL !!!
// !!! LVGL !!!
static void disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE)
    {
        tft.startWrite();
        tft.setAddrWindow(area->x1, area->y1, w, h);
        tft.pushColors((uint16_t *)&color_p->full, w * h, false);
        tft.endWrite();
        lv_disp_flush_ready(disp);
        xSemaphoreGive(xSemaphore);
    }
}

static bool getTouch(int16_t &x, int16_t &y)
{
    uint8_t rotation = tft.getRotation();
    if (!touch->read())
    {
        return false;
    }
    TP_Point t = touch->getPoint(0);
    switch (rotation)
    {
    case 1:
        x = t.y;
        y = tft.height() - t.x;
        break;
    case 2:
        x = tft.width() - t.x;
        y = tft.height() - t.y;
        break;
    case 3:
        x = tft.width() - t.y;
        y = t.x;
        break;
    case 0:
    default:
        x = t.x;
        y = t.y;
    }
    Serial.printf("R:%d X:%d Y:%d\n", rotation, x, y);
    return true;
}

static void mouse_read(lv_indev_drv_t *indev, lv_indev_data_t *data)
{
    static int16_t last_x;
    static int16_t last_y;
    bool left_button_down = false;
    const uint8_t dir_pins[5] = {BOARD_TBOX_G02,
                                 BOARD_TBOX_G01,
                                 BOARD_TBOX_G04,
                                 BOARD_TBOX_G03,
                                 BOARD_BOOT_PIN};
    static bool last_dir[5];
    uint8_t pos = 20; // vitesse du curseur
    for (int i = 0; i < 5; i++)
    {
        bool dir = digitalRead(dir_pins[i]);
        if (dir != last_dir[i])
        {
            last_dir[i] = dir;
            switch (i)
            {
            case 0:
                if (last_x < (lv_disp_get_hor_res(NULL) - image_cursor.header.w))
                {
                    last_x += pos;
                    Serial.println("0");
                }
                break;
            case 1:
                if (last_y > image_cursor.header.h)
                {
                    last_y -= pos;
                    Serial.println("1");
                }
                break;
            case 2:
                if (last_x > image_cursor.header.w)
                {
                    last_x -= pos;
                    Serial.println("2");
                }
                break;
            case 3:
                if (last_y < (lv_disp_get_ver_res(NULL) - image_cursor.header.h))
                {
                    last_y += pos;
                    Serial.println("3");
                }
                break;
            case 4:
                left_button_down = true;
                break;
            default:
                break;
            }
        }
    }

    // Serial.printf("indev:X:%04d  Y:%04d \n", last_x, last_y);
    /*Store the collected data*/
    data->point.x = last_x;
    data->point.y = last_y;
    data->state = left_button_down ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
}

/*Read the touchpad*/
static void touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
    data->state = getTouch(data->point.x, data->point.y) ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
}


void lv_button_event_cb(lv_event_t *e)
{
    static uint8_t btn_index = 0;
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    if (code == LV_EVENT_CLICKED)
    {
        DemoEvent event = *(DemoEvent *)lv_event_get_user_data(e);
        switch (event)
        {
          case DEMO_SETTINGS_BTN_CLICK_EVENT: //(void *)&event[0]
          interface_clean();
          setup_scr_screen_param();

        break;


        case DEMO_APP_BTN_CLICK_EVENT://(void *)&event[1]
          interface_clean();
          setup_scr_screen_app();

        break;


        case DEMO_RETURNHOME_BTN_CLICK_EVENT://(void *)&event[2]
          interface_clean();
          setup_scr_screen_main();

        break;


        
          /*
        case DEMO_TX_BTN_CLICK_EVENT:
        {
            if (!hasRadio)
            {
                lv_textarea_set_text(radio_ta, "Radio is not online");
                return;
            }
            lv_textarea_set_text(radio_ta, "");
            // send the first packet on this node
            Serial.print(F("[Radio] Sending first packet ... "));
            transmissionState = radio.startTransmit("Hello World!");
            sender = true;
        }

        break;
        case DEMO_RX_BTN_CLICK_EVENT:
            Serial.println("DEMO_RX_BTN_CLICK_EVENT");
            if (!hasRadio)
            {
                lv_textarea_set_text(radio_ta, "Radio is not online");
                return;
            }
            {
                lv_textarea_set_text(radio_ta, "");
                Serial.print(F("[Radio] Starting to listen ... "));
                int state = radio.startReceive();
                if (state == RADIOLIB_ERR_NONE)
                {
                    Serial.println(F("success!"));
                }
                else
                {
                    Serial.print(F("failed, code "));
                    Serial.println(state);
                }
                sender = false;
            }
            break;
        case DEMO_OTA_BTN_CLICK_EVENT:

        OTA();
        while(p <=600000 ){//10 min d'attente avant la desactivation de l'OTA
            server.handleClient();
            delay(1);
            p++;
            }
        server.close();
            break;
        
        case DEMO_VAD_BTN_CLICK_EVENT:
            Serial.println("DEMO_VAD_BTN_CLICK_EVENT");
            {
                lv_state_t state = lv_obj_get_state(obj);
                if (state == 2)
                {
                    vTaskSuspend(vadTaskHandler);
#ifdef USE_ESP_VAD
                    lv_label_set_text(vad_btn_label, "VAD detect");
#else
                    lv_label_set_text(vad_btn_label, "loopback");
#endif
                }
                else
                {
                    vad_detected_counter = 0;
                    vTaskResume(vadTaskHandler);
                }
            }
            break;
        case DEMO_PLAY_BTN_CLICK_EVENT:
            Serial.println("DEMO_PLAY_BTN_CLICK_EVENT");
            if (playHandle)
            {
                vTaskResume(playHandle);
            }
            break;
        case DEMO_SLEEP_BTN_CLICK_EVENT:
            enterSleep = true;
            break;
        default:
            break;
        }
*/

    }
}
}




void serialToScreen(lv_obj_t *parent, String string, bool result)
{
    lv_obj_t *cont = lv_obj_create(parent);
    lv_obj_set_scroll_dir(cont, LV_DIR_NONE);
    lv_obj_set_size(cont, LV_PCT(100), lv_font_get_line_height(&lv_font_montserrat_28) + 2);

    lv_obj_t *label1 = lv_label_create(cont);
    lv_label_set_recolor(label1, true);
    lv_label_set_text(label1, string.c_str());
    lv_obj_align(label1, LV_ALIGN_LEFT_MID, 0, 0);

    lv_obj_t *label_ = lv_label_create(cont);
    lv_label_set_recolor(label_, true);
    lv_label_set_text(label_, result ? "#FFFFFF [# #00ff00 PASS# #FFFFFF ]#" : "#FFFFFF [# #ff0000  FAIL# #FFFFFF ]#");
    lv_obj_align(label_, LV_ALIGN_RIGHT_MID, 0, 0);

    lv_obj_scroll_to_y(parent, lv_disp_get_ver_res(NULL), LV_ANIM_ON);

    int i = 200;
    while (i--)
    {
        lv_task_handler();
        delay(1);
    }
}


// LilyGo  T-Deck  control backlight chip has 16 levels of adjustment range
// The adjustable range is 0~15, 0 is the minimum brightness, 15 is the maximum brightness
void setBrightness(uint8_t value)
{
    static uint8_t level = 0;
    static uint8_t steps = 16;
    if (value == 0)
    {
        digitalWrite(BOARD_BL_PIN, 0);
        delay(3);
        level = 0;
        return;
    }
    if (level == 0)
    {
        digitalWrite(BOARD_BL_PIN, 1);
        level = steps;
        delayMicroseconds(30);
    }
    int from = steps - level;
    int to = steps - value;
    int num = (steps + to - from) % steps;
    for (int i = 0; i < num; i++)
    {
        digitalWrite(BOARD_BL_PIN, 0);
        digitalWrite(BOARD_BL_PIN, 1);
    }
    level = value;
}

void setupLvgl()
{
    static lv_disp_draw_buf_t draw_buf;
    static lv_color_t *buf = (lv_color_t *)ps_malloc(LVGL_BUFFER_SIZE);
    if (!buf)
    {
        Serial.println("menory alloc failed!");
        delay(5000);
        assert(buf);
    }
    String LVGL_Arduino = "Hello Arduino! ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
    Serial.println(LVGL_Arduino);
    Serial.println("I am LVGL_Arduino");

    lv_init();

    lv_group_set_default(lv_group_create());

    lv_disp_draw_buf_init(&draw_buf, buf, NULL, LVGL_BUFFER_SIZE);

    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);

    /*Change the following line to your display resolution*/
    disp_drv.hor_res = TFT_HEIGHT;
    disp_drv.ver_res = TFT_WIDTH;
    disp_drv.flush_cb = disp_flush;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.full_refresh = 1;
    lv_disp_drv_register(&disp_drv);

    /*Initialize the  input device driver*/

    /*Register a touchscreen input device*/
    if (touchDected)
    {
        static lv_indev_drv_t indev_touchpad;
        lv_indev_drv_init(&indev_touchpad);
        indev_touchpad.type = LV_INDEV_TYPE_POINTER;
        indev_touchpad.read_cb = touchpad_read;
        touch_indev = lv_indev_drv_register(&indev_touchpad);
    }

    /*Register a mouse input device*/
    static lv_indev_drv_t indev_mouse;
    lv_indev_drv_init(&indev_mouse);
    indev_mouse.type = LV_INDEV_TYPE_POINTER;
    indev_mouse.read_cb = mouse_read;
    mouse_indev = lv_indev_drv_register(&indev_mouse);
    lv_indev_set_group(mouse_indev, lv_group_get_default());

    lv_obj_t *cursor_obj;
    cursor_obj = lv_img_create(lv_scr_act());     /*Create an image object for the cursor */
    lv_img_set_src(cursor_obj, &image_cursor);    /*Set the image source*/
    lv_indev_set_cursor(mouse_indev, cursor_obj); /*Connect the image  object to the driver*/

    if (kbDected)
    {
        Serial.println("Keyboard registered!!");
        /*Register a keypad input device*/
        static lv_indev_drv_t indev_keypad;
        lv_indev_drv_init(&indev_keypad);
        indev_keypad.type = LV_INDEV_TYPE_KEYPAD;
        indev_keypad.read_cb = keypad_read;
        kb_indev = lv_indev_drv_register(&indev_keypad);
        lv_indev_set_group(kb_indev, lv_group_get_default());
    }
}

// Read key value from esp32c3
static uint32_t keypad_get_key(void)
{
    char key_ch = 0;
    Wire.requestFrom(0x55, 1);
    while (Wire.available() > 0)
    {
        key_ch = Wire.read();
    }
    return key_ch;
}

/*Will be called by the library to read the mouse*/
static void keypad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    static uint32_t last_key = 0;
    uint32_t act_key;
    act_key = keypad_get_key();
    if (act_key != 0)
    {
        data->state = LV_INDEV_STATE_PR;
        Serial.printf("Key pressed : 0x%x\n", act_key);
        last_key = act_key;
    }
    else
    {
        data->state = LV_INDEV_STATE_REL;
    }
    data->key = last_key;
}
























void interface_verif(){
    main_count = lv_obj_create(lv_scr_act());
    lv_obj_set_style_bg_img_src(main_count, &boot, LV_PART_MAIN);
    lv_obj_set_style_border_opa(main_count, LV_OPA_100, 0);
    lv_obj_set_style_radius(main_count, 0, 0);
    lv_obj_set_size(main_count, LV_PCT(100), LV_PCT(100));
    lv_obj_set_flex_flow(main_count, LV_FLEX_FLOW_COLUMN);
    lv_obj_center(main_count);

    // Show device state

    serialToScreen(main_count, "Keyboard C3", kbDected);

    serialToScreen(main_count, "Capacitive Touch", touchDected);
    serialToScreen(main_count, "Radio SX1262", hasRadio);
    if (SD.cardType() != CARD_NONE)
    {
        serialToScreen(main_count, "Mass storage #FFFFFF [# #00ff00  " + String(SD.cardSize() / 1024 / 1024.0) + "MB# #FFFFFF ]#", true);
    }
    else
    {
        serialToScreen(main_count, "Mass storage", false);
    }
}
void interface_clean(){
  lv_obj_clean(main_count);
}
void interface_noscroll(){
  lv_obj_set_scroll_dir(main_count, LV_DIR_NONE);
  lv_obj_set_scrollbar_mode(main_count, LV_SCROLLBAR_MODE_OFF); 
}

void interface_main(){
    // Simple GUI for factory test
    lv_obj_t *win_ui = lv_obj_create(main_count);
    lv_obj_set_style_border_width(win_ui, 0, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(win_ui, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_pad_all(win_ui, 0, LV_PART_MAIN);
    lv_obj_set_size(win_ui, LV_PCT(100), LV_PCT(55));

    radio_ta = lv_textarea_create(win_ui);
    lv_obj_set_style_bg_opa(radio_ta, LV_OPA_50, 0);
    lv_textarea_set_cursor_click_pos(radio_ta, false);
    lv_textarea_set_text_selection(radio_ta, false);
    lv_obj_set_size(radio_ta, LV_PCT(100), LV_PCT(100));
    lv_textarea_set_text(radio_ta, "");
    lv_textarea_set_max_length(radio_ta, 1024);

    lv_obj_t *btn_ui = lv_obj_create(main_count);
    lv_obj_set_style_bg_opa(btn_ui, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_size(btn_ui, LV_PCT(100), LV_PCT(20));
    lv_obj_set_flex_flow(btn_ui, LV_FLEX_FLOW_ROW);
    lv_obj_align_to(btn_ui, radio_ta, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_set_style_pad_top(btn_ui, 1, LV_PART_MAIN);
    lv_obj_set_style_pad_bottom(btn_ui, 1, LV_PART_MAIN);

    lv_obj_t *btn1 = lv_btn_create(btn_ui);
    lv_obj_set_size(btn1, LV_PCT(21), LV_PCT(100));
    lv_obj_add_event_cb(btn1, lv_button_event_cb, LV_EVENT_CLICKED, (void *)&event[0]);
    label = lv_label_create(btn1);
    lv_label_set_text(label, "Tx");
    lv_obj_set_user_data(btn1, label);
    lv_obj_center(label);

    lv_obj_t *btn2 = lv_btn_create(btn_ui);
    lv_obj_set_size(btn2, LV_PCT(21), LV_PCT(100));
    lv_obj_add_event_cb(btn2, lv_button_event_cb, LV_EVENT_CLICKED, (void *)&event[1]);
    label = lv_label_create(btn2);
    lv_label_set_text(label, "Rx");
    lv_obj_center(label);

    lv_obj_t *btn3 = lv_btn_create(btn_ui);
    lv_obj_set_size(btn3, LV_PCT(21), LV_PCT(100));
    lv_obj_add_event_cb(btn3, lv_button_event_cb, LV_EVENT_CLICKED, (void *)&event[2]);
    label = lv_label_create(btn3);
    lv_label_set_text(label, "OTA");
    lv_obj_center(label);

    lv_obj_t *sleep = lv_btn_create(btn_ui);
    lv_obj_set_size(sleep, LV_PCT(25), LV_PCT(100));
    lv_obj_add_event_cb(sleep, lv_button_event_cb, LV_EVENT_CLICKED, (void *)&event[5]);
    label = lv_label_create(sleep);
    lv_label_set_text(label, "Sleep");
    lv_obj_center(label);

    lv_obj_t *btn_ui2 = lv_obj_create(main_count);
    lv_obj_set_style_bg_opa(btn_ui2, LV_OPA_TRANSP, 0);
    lv_obj_set_size(btn_ui2, LV_PCT(100), LV_PCT(20));
    lv_obj_set_flex_flow(btn_ui2, LV_FLEX_FLOW_ROW);
    lv_obj_align_to(btn_ui2, btn_ui, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_set_style_pad_top(btn_ui2, 1, LV_PART_MAIN);
    lv_obj_set_style_pad_bottom(btn_ui2, 1, LV_PART_MAIN);
    
    lv_obj_t *btn4 = lv_btn_create(btn_ui2);
    lv_obj_set_size(btn4, LV_PCT(21), LV_PCT(100));
    lv_obj_add_flag(btn4, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_add_event_cb(btn4, lv_button_event_cb, LV_EVENT_CLICKED, (void *)&event[3]);
    vad_btn_label = lv_label_create(btn4);
    lv_label_set_text(vad_btn_label, "loopback");
    lv_obj_center(vad_btn_label);

    lv_obj_t *btn5 = lv_btn_create(btn_ui2);
    lv_obj_set_size(btn5, LV_PCT(21), LV_PCT(100));
    lv_obj_add_event_cb(btn5, lv_button_event_cb, LV_EVENT_CLICKED, (void *)&event[4]);
    label = lv_label_create(btn5);
    lv_label_set_text(label, "Play");
    lv_obj_center(label);
    xTaskCreate(taskplaySong, "play", 1024 * 4, NULL, 10, &playHandle);

  btn6 = lv_btn_create(btn_ui2);
    lv_obj_set_size(btn6, LV_PCT(21), LV_PCT(100));
    //lv_obj_add_event_cb(btn6, lv_button_event_cb, LV_EVENT_CLICKED, (void *)&event[2]);
    labelVbat = lv_label_create(btn6);
    lv_label_set_text(labelVbat, "bat");
    lv_obj_center(labelVbat);
    

  btn7 = lv_btn_create(btn_ui2);
    lv_obj_set_size(btn7, LV_PCT(21), LV_PCT(100));
    //lv_obj_add_event_cb(btn7, lv_button_event_cb, LV_EVENT_CLICKED, (void *)&event[2]);
    labelTime = lv_label_create(btn7);
    lv_label_set_text(labelTime, "0");
    lv_obj_center(labelTime);
    
  lv_obj_t *btn8 = lv_btn_create(btn_ui2);
    lv_obj_set_size(btn8, LV_PCT(21), LV_PCT(100));
    //lv_obj_add_event_cb(btn8, lv_button_event_cb, LV_EVENT_CLICKED, (void *)&event[2]);
    label = lv_label_create(btn8);
    lv_label_set_text(label, "0");
    lv_obj_center(label);

  lv_obj_t *btn9 = lv_btn_create(btn_ui2);
    lv_obj_set_size(btn9, LV_PCT(21), LV_PCT(100));
    //lv_obj_add_event_cb(btn9, lv_button_event_cb, LV_EVENT_CLICKED, (void *)&event[2]);
    label = lv_label_create(btn9);
    lv_label_set_text(label, "0");
    lv_obj_center(label);

  lv_obj_t *btn10 = lv_btn_create(btn_ui2);
    lv_obj_set_size(btn10, LV_PCT(21), LV_PCT(100));
    //lv_obj_add_event_cb(btn10, lv_button_event_cb, LV_EVENT_CLICKED, (void *)&event[2]);
    label = lv_label_create(btn10);
    lv_label_set_text(label, "0");
    lv_obj_center(label);    
}

void interface_sleep(){
  lv_obj_t *label_ = lv_label_create(main_count);
  lv_label_set_text(label_, "Sleep");
  lv_obj_center(label_);
}



void interface_dev(){
    lv_style_init(&style);

    /*Set a background color and a radius*/
    lv_style_set_bg_opa(&style, LV_OPA_TRANSP);

    /*Add outline*/
    lv_style_set_outline_width(&style, 2);
    lv_style_set_outline_color(&style, lv_palette_main(LV_PALETTE_GREEN));
    lv_style_set_outline_pad(&style, 2);


    // Simple GUI for factory test
lv_obj_t *uiTOP = lv_obj_create(main_count);
    lv_obj_set_style_border_width(uiTOP, 0, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(uiTOP, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_pad_all(uiTOP, 0, LV_PART_MAIN);
    lv_obj_set_size(uiTOP, LV_PCT(100), LV_PCT(10));
    lv_obj_align(uiTOP, LV_ALIGN_TOP_MID, 0, 0);

lv_obj_t *gauche = lv_obj_create(uiTOP);
    lv_obj_set_size(gauche, LV_PCT(20), LV_PCT(100));
    //lv_obj_add_event_cb(gauche, lv_button_event_cb, LV_EVENT_CLICKED, (void *)&event[2]);
    label = lv_label_create(gauche);
    lv_label_set_text(label, "G");
    lv_obj_center(label);
    lv_obj_align(gauche, LV_ALIGN_TOP_LEFT, 10, 0);
    lv_obj_set_scroll_dir(gauche, LV_DIR_NONE);
    lv_obj_set_scrollbar_mode(gauche, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_style(gauche, &style, 0);  
    
lv_obj_t *droite = lv_obj_create(uiTOP);
    lv_obj_set_size(droite, LV_PCT(20), LV_PCT(100));
    //lv_obj_add_event_cb(droite, lv_button_event_cb, LV_EVENT_CLICKED, (void *)&event[2]);
    labelVbat = lv_label_create(droite);
    lv_label_set_text(labelVbat, "bat");
    lv_obj_center(labelVbat);
    lv_obj_align(droite, LV_ALIGN_TOP_RIGHT, -10, 0);
    lv_obj_set_scroll_dir(droite, LV_DIR_NONE);
    lv_obj_set_scrollbar_mode(droite, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_style(droite, &style, 0); 

lv_obj_t *centre = lv_obj_create(uiTOP);
    lv_obj_set_size(centre, LV_PCT(60), LV_PCT(100));
    //lv_obj_add_event_cb(centre, lv_button_event_cb, LV_EVENT_CLICKED, (void *)&event[2]);
    labelTime = lv_label_create(centre);
    lv_label_set_text(labelTime, "tik");
    lv_obj_center(labelTime);
    lv_obj_align(centre, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_scroll_dir(centre, LV_DIR_NONE);
    lv_obj_set_scrollbar_mode(centre, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_style(centre, &style, 0);  

lv_obj_t *win_ui = lv_obj_create(main_count);
    lv_obj_set_style_border_width(win_ui, 0, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(win_ui, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_pad_all(win_ui,2, LV_PART_MAIN);
    lv_obj_set_size(win_ui, LV_PCT(100), LV_PCT(66));
    lv_obj_align(win_ui, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(win_ui, &style, 0);  

    radio_ta = lv_textarea_create(win_ui);
    lv_obj_set_style_bg_opa(radio_ta, LV_OPA_50, 0);
    lv_textarea_set_cursor_click_pos(radio_ta, false);
    lv_textarea_set_text_selection(radio_ta, false);
    lv_obj_set_size(radio_ta, LV_PCT(100), LV_PCT(100));
    lv_textarea_set_text(radio_ta, "");
    lv_textarea_set_max_length(radio_ta, 1024);
    lv_obj_align(radio_ta, LV_ALIGN_CENTER, 0, 0);

    // Simple GUI for factory test
lv_obj_t *uiBOT = lv_obj_create(main_count);
    lv_obj_set_style_border_width(uiBOT, 0, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(uiBOT, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_pad_all(uiBOT, 0, LV_PART_MAIN);
    lv_obj_set_size(uiBOT, LV_PCT(100), LV_PCT(20));
    lv_obj_align(uiBOT, LV_ALIGN_BOTTOM_MID, 0, 0);

lv_obj_t *gaucheb = lv_obj_create(uiBOT);
    lv_obj_set_size(gaucheb, LV_PCT(20), LV_PCT(100));
    //lv_obj_add_event_cb(gaucheb, lv_button_event_cb, LV_EVENT_CLICKED, (void *)&event[2]);
    label = lv_label_create(gaucheb);
    lv_label_set_text(label, "G");
    lv_obj_center(label);
    lv_obj_align(gaucheb, LV_ALIGN_BOTTOM_LEFT, 10, 0);
    lv_obj_set_scroll_dir(gaucheb, LV_DIR_NONE);
    lv_obj_set_scrollbar_mode(gaucheb, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_style(gaucheb, &style, 0);  
    
lv_obj_t *droiteb = lv_obj_create(uiBOT);
    lv_obj_set_size(droiteb, LV_PCT(20), LV_PCT(100));
    //lv_obj_add_event_cb(droiteb, lv_button_event_cb, LV_EVENT_CLICKED, (void *)&event[2]);
    label = lv_label_create(droiteb);
    lv_label_set_text(label, "bat");
    lv_obj_center(label);
    lv_obj_align(droiteb, LV_ALIGN_BOTTOM_RIGHT, -10, 0);
    lv_obj_set_scroll_dir(droiteb, LV_DIR_NONE);
    lv_obj_set_scrollbar_mode(droiteb, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_style(droiteb, &style, 0); 
/*
  lv_obj_t *app_ui = lv_obj_create(main_count);
    lv_obj_set_style_bg_opa(app_ui, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_size(app_ui, LV_PCT(100), LV_PCT(22));
    lv_obj_set_flex_flow(app_ui, LV_FLEX_FLOW_ROW);
    lv_obj_align_to(app_ui, radio_ta, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_set_style_pad_top(app_ui, 1, LV_PART_MAIN);
    lv_obj_set_style_pad_bottom(app_ui, 1, LV_PART_MAIN);
    lv_obj_add_style(app_ui, &style, 0);  
*/  
    

}

void setup_scr_screen_main()
{
	//Write codes screen_main
  
	lv_obj_set_size(main_count, 320, 240);
	lv_obj_set_scrollbar_mode(main_count, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(main_count, LV_OBJ_FLAG_SCROLL_CHAIN);
	lv_obj_add_flag(main_count, LV_OBJ_FLAG_SCROLL_CHAIN_VER);
	lv_obj_add_flag(main_count, LV_OBJ_FLAG_SCROLL_CHAIN_HOR);

	//Write style for screen_main, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(main_count, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(main_count, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(main_count, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_img_src(main_count, &boot, LV_PART_MAIN);

	//Write codes screen_main_cont_1
	lv_obj_t *main_count_cont_1 = lv_obj_create(main_count);
	lv_obj_set_pos(main_count_cont_1, 0, 0);
	lv_obj_set_size(main_count_cont_1, 320, 240);
	lv_obj_set_scrollbar_mode(main_count_cont_1, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_main_cont_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(main_count_cont_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(main_count_cont_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(main_count_cont_1, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(main_count_cont_1, LV_BORDER_SIDE_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(main_count_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(main_count_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(main_count_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(main_count_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(main_count_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(main_count_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(main_count_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_main_btn_1
	lv_obj_t *main_count_btn_1 = lv_btn_create(main_count_cont_1);
	lv_obj_t *main_count_btn_1_label = lv_label_create(main_count_btn_1);
  lv_obj_add_event_cb(main_count_btn_1, lv_button_event_cb, LV_EVENT_CLICKED, (void *)&event[1]);
	lv_label_set_text(main_count_btn_1_label, "App");
	lv_label_set_long_mode(main_count_btn_1_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(main_count_btn_1_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(main_count_btn_1, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(main_count_btn_1, 210, 180);
	lv_obj_set_size(main_count_btn_1, 100, 50);

	//Write style for screen_main_btn_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(main_count_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(main_count_btn_1, lv_color_hex(0x00e7ff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(main_count_btn_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(main_count_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(main_count_btn_1, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(main_count_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(main_count_btn_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(main_count_btn_1, &lv_font_montserrat_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(main_count_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(main_count_btn_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_main_bar_1
	lv_obj_t *main_count_bar_1 = lv_bar_create(main_count_cont_1);
	lv_obj_set_style_anim_time(main_count_bar_1, 1000, 0);
	lv_bar_set_mode(main_count_bar_1, LV_BAR_MODE_NORMAL);
	lv_bar_set_range(main_count_bar_1, 0, 100);
	lv_bar_set_value(main_count_bar_1, 34, LV_ANIM_OFF);
	lv_obj_set_pos(main_count_bar_1, 222, 17);
	lv_obj_set_size(main_count_bar_1, 90, 20);

	//Write style for screen_main_bar_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(main_count_bar_1, 102, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(main_count_bar_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(main_count_bar_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(main_count_bar_1, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(main_count_bar_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_main_bar_1, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(main_count_bar_1, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(main_count_bar_1, lv_color_hex(0x00e7ff), LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(main_count_bar_1, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(main_count_bar_1, 10, LV_PART_INDICATOR|LV_STATE_DEFAULT);


	lv_obj_t *main_count_btn_2 = lv_btn_create(main_count_cont_1);
	lv_obj_t *main_count_btn_2_label = lv_label_create(main_count_btn_2);
  lv_obj_add_event_cb(main_count_btn_2, lv_button_event_cb, LV_EVENT_CLICKED, (void *)&event[0]);
	lv_label_set_text(main_count_btn_2_label, "Settings");
	lv_label_set_long_mode(main_count_btn_2_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(main_count_btn_2_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(main_count_btn_2, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(main_count_btn_2, 6, 181);
	lv_obj_set_size(main_count_btn_2, 100, 50);

	//Write style for screen_main_btn_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(main_count_btn_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(main_count_btn_2, lv_color_hex(0x00e7ff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(main_count_btn_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(main_count_btn_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(main_count_btn_2, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(main_count_btn_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(main_count_btn_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(main_count_btn_2, &lv_font_montserrat_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(main_count_btn_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(main_count_btn_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_main_label_1
	lv_obj_t *main_count_label_1 = lv_label_create(main_count_cont_1);
	lv_label_set_text(main_count_label_1, "bonjour =)\n\n");
	lv_label_set_long_mode(main_count_label_1, LV_LABEL_LONG_SCROLL_CIRCULAR);
	lv_obj_set_pos(main_count_label_1, 121, 70);
	lv_obj_set_size(main_count_label_1, 67, 32);

	//Write style for screen_main_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(main_count_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(main_count_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(main_count_label_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(main_count_label_1, &lv_font_montserrat_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(main_count_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(main_count_label_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(main_count_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(main_count_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(main_count_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(main_count_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(main_count_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(main_count_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(main_count_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(main_count_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//The custom code of screen_main.
	

	//Update current screen layout.
	lv_obj_update_layout(main_count);

}

void setup_scr_screen_app()
{
	//Write codes screen_app
	lv_obj_set_size(main_count, 320, 240);
	lv_obj_set_scrollbar_mode(main_count, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(main_count, LV_OBJ_FLAG_SCROLL_CHAIN);
	lv_obj_add_flag(main_count, LV_OBJ_FLAG_SCROLL_CHAIN_VER);
	lv_obj_add_flag(main_count, LV_OBJ_FLAG_SCROLL_CHAIN_HOR);

	//Write style for screen_app, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(main_count, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_app_menu_1
	screen_app_menu_1 = lv_menu_create(main_count);

	//Create sidebar page for menu screen_app_menu_1
	screen_app_menu_1_sidebar_page = lv_menu_page_create(screen_app_menu_1, "Application");
	lv_menu_set_sidebar_page(screen_app_menu_1, screen_app_menu_1_sidebar_page);

	//Create subpage for screen_app_menu_1
	screen_app_menu_1_subpage_1 = lv_menu_page_create(screen_app_menu_1, NULL);
	screen_app_menu_1_cont_1 = lv_menu_cont_create(screen_app_menu_1_sidebar_page);
	screen_app_menu_1_label_1 = lv_label_create(screen_app_menu_1_cont_1);
	lv_label_set_text(screen_app_menu_1_label_1, "OTA");
	lv_obj_set_size(screen_app_menu_1_label_1, LV_PCT(100), LV_SIZE_CONTENT);
	lv_menu_set_load_page_event(screen_app_menu_1, screen_app_menu_1_cont_1, screen_app_menu_1_subpage_1);

	//Create subpage for screen_app_menu_1
	screen_app_menu_1_subpage_2 = lv_menu_page_create(screen_app_menu_1, NULL);
  screen_app_menu_1_cont_2 = lv_menu_cont_create(screen_app_menu_1_sidebar_page);
	screen_app_menu_1_label_2 = lv_label_create(screen_app_menu_1_cont_2);
	lv_label_set_text(screen_app_menu_1_label_2, "SnifferLora");
	lv_obj_set_size(screen_app_menu_1_label_2, LV_PCT(100), LV_SIZE_CONTENT);
	lv_menu_set_load_page_event(screen_app_menu_1, screen_app_menu_1_cont_2, screen_app_menu_1_subpage_2);

	//Create subpage for screen_app_menu_1
	screen_app_menu_1_subpage_3 = lv_menu_page_create(screen_app_menu_1, NULL);
	screen_app_menu_1_cont_3 = lv_menu_cont_create(screen_app_menu_1_sidebar_page);
	screen_app_menu_1_label_3 = lv_label_create(screen_app_menu_1_cont_3);
	lv_label_set_text(screen_app_menu_1_label_3, "HackWifi");
	lv_obj_set_size(screen_app_menu_1_label_3, LV_PCT(100), LV_SIZE_CONTENT);
	lv_menu_set_load_page_event(screen_app_menu_1, screen_app_menu_1_cont_3, screen_app_menu_1_subpage_3);

	//Create subpage for screen_app_menu_1
	screen_app_menu_1_subpage_4 = lv_menu_page_create(screen_app_menu_1, NULL);
	screen_app_menu_1_cont_4 = lv_menu_cont_create(screen_app_menu_1_sidebar_page);
	screen_app_menu_1_label_4 = lv_label_create(screen_app_menu_1_cont_4);
	lv_label_set_text(screen_app_menu_1_label_4, "readData");
	lv_obj_set_size(screen_app_menu_1_label_4, LV_PCT(100), LV_SIZE_CONTENT);
	lv_menu_set_load_page_event(screen_app_menu_1, screen_app_menu_1_cont_4, screen_app_menu_1_subpage_4);
	lv_event_send(screen_app_menu_1_cont_1, LV_EVENT_CLICKED, NULL);
	lv_obj_set_pos(screen_app_menu_1, 0, 0);
	lv_obj_set_size(screen_app_menu_1, 320, 240);
	lv_obj_set_scrollbar_mode(screen_app_menu_1, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_app_menu_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(screen_app_menu_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(screen_app_menu_1, lv_color_hex(0xf2fbff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(screen_app_menu_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(screen_app_menu_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(screen_app_menu_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(screen_app_menu_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(screen_app_menu_1, LV_BORDER_SIDE_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(screen_app_menu_1, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(screen_app_menu_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for &style_screen_app_menu_1_extra_sidebar_page_main_default
	static lv_style_t style_screen_app_menu_1_extra_sidebar_page_main_default;
	
	lv_style_set_bg_opa(&style_screen_app_menu_1_extra_sidebar_page_main_default, 255);
	lv_style_set_bg_color(&style_screen_app_menu_1_extra_sidebar_page_main_default, lv_color_hex(0xdaf1ff));
	lv_style_set_bg_grad_dir(&style_screen_app_menu_1_extra_sidebar_page_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_radius(&style_screen_app_menu_1_extra_sidebar_page_main_default, 0);
	lv_obj_add_style(screen_app_menu_1_sidebar_page, &style_screen_app_menu_1_extra_sidebar_page_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for &style_screen_app_menu_1_extra_option_btns_main_default
	static lv_style_t style_screen_app_menu_1_extra_option_btns_main_default;
	
	lv_style_set_text_color(&style_screen_app_menu_1_extra_option_btns_main_default, lv_color_hex(0x151212));
	lv_style_set_text_font(&style_screen_app_menu_1_extra_option_btns_main_default, &lv_font_montserrat_16);
	lv_style_set_text_opa(&style_screen_app_menu_1_extra_option_btns_main_default, 255);
	lv_style_set_text_align(&style_screen_app_menu_1_extra_option_btns_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_top(&style_screen_app_menu_1_extra_option_btns_main_default, 10);
	lv_style_set_pad_bottom(&style_screen_app_menu_1_extra_option_btns_main_default, 10);
	lv_obj_add_style(screen_app_menu_1_cont_4, &style_screen_app_menu_1_extra_option_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_style(screen_app_menu_1_cont_3, &style_screen_app_menu_1_extra_option_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_style(screen_app_menu_1_cont_2, &style_screen_app_menu_1_extra_option_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_style(screen_app_menu_1_cont_1, &style_screen_app_menu_1_extra_option_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_CHECKED for &style_screen_app_menu_1_extra_option_btns_main_checked
	static lv_style_t style_screen_app_menu_1_extra_option_btns_main_checked;
	
	lv_style_set_text_color(&style_screen_app_menu_1_extra_option_btns_main_checked, lv_color_hex(0x00e7ff));
	lv_style_set_text_font(&style_screen_app_menu_1_extra_option_btns_main_checked, &lv_font_montserrat_16);
	lv_style_set_text_opa(&style_screen_app_menu_1_extra_option_btns_main_checked, 255);
	lv_style_set_text_align(&style_screen_app_menu_1_extra_option_btns_main_checked, LV_TEXT_ALIGN_CENTER);
	lv_style_set_border_width(&style_screen_app_menu_1_extra_option_btns_main_checked, 0);
	lv_style_set_radius(&style_screen_app_menu_1_extra_option_btns_main_checked, 0);
	lv_style_set_bg_opa(&style_screen_app_menu_1_extra_option_btns_main_checked, 255);
	lv_style_set_bg_color(&style_screen_app_menu_1_extra_option_btns_main_checked, lv_color_hex(0xdaf1ff));
	lv_style_set_bg_grad_dir(&style_screen_app_menu_1_extra_option_btns_main_checked, LV_GRAD_DIR_HOR);
	lv_style_set_bg_grad_color(&style_screen_app_menu_1_extra_option_btns_main_checked, lv_color_hex(0xf7fbff));
	lv_style_set_bg_main_stop(&style_screen_app_menu_1_extra_option_btns_main_checked, 0);
	lv_style_set_bg_grad_stop(&style_screen_app_menu_1_extra_option_btns_main_checked, 0);
	lv_obj_add_style(screen_app_menu_1_cont_4, &style_screen_app_menu_1_extra_option_btns_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_add_style(screen_app_menu_1_cont_3, &style_screen_app_menu_1_extra_option_btns_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_add_style(screen_app_menu_1_cont_2, &style_screen_app_menu_1_extra_option_btns_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_add_style(screen_app_menu_1_cont_1, &style_screen_app_menu_1_extra_option_btns_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style state: LV_STATE_DEFAULT for &style_screen_app_menu_1_extra_main_title_main_default
	static lv_style_t style_screen_app_menu_1_extra_main_title_main_default;
	
	lv_style_set_text_color(&style_screen_app_menu_1_extra_main_title_main_default, lv_color_hex(0x41485a));
	lv_style_set_text_font(&style_screen_app_menu_1_extra_main_title_main_default, &lv_font_montserrat_16);
	lv_style_set_text_opa(&style_screen_app_menu_1_extra_main_title_main_default, 255);
	lv_style_set_text_align(&style_screen_app_menu_1_extra_main_title_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_bg_opa(&style_screen_app_menu_1_extra_main_title_main_default, 255);
	lv_style_set_bg_color(&style_screen_app_menu_1_extra_main_title_main_default, lv_color_hex(0xa8dfff));
	lv_style_set_bg_grad_dir(&style_screen_app_menu_1_extra_main_title_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_pad_top(&style_screen_app_menu_1_extra_main_title_main_default, 5);
	lv_style_set_pad_bottom(&style_screen_app_menu_1_extra_main_title_main_default, 5);
	lv_menu_t * screen_app_menu_1_menu= (lv_menu_t *)screen_app_menu_1;
	lv_obj_t * screen_app_menu_1_title = screen_app_menu_1_menu->sidebar_header_title;
	lv_obj_set_size(screen_app_menu_1_title, LV_PCT(100), LV_SIZE_CONTENT);
	lv_obj_add_style(lv_menu_get_sidebar_header(screen_app_menu_1), &style_screen_app_menu_1_extra_main_title_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);









	//Write codes screen_app_btn_1
	screen_app_btn_1 = lv_btn_create(main_count);
	screen_app_btn_1_label = lv_label_create(screen_app_btn_1);
	lv_label_set_text(screen_app_btn_1_label, "Back");
	lv_label_set_long_mode(screen_app_btn_1_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(screen_app_btn_1_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(screen_app_btn_1, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(screen_app_btn_1, 257, 2);
	lv_obj_set_size(screen_app_btn_1, 60, 20);

	//Write style for screen_app_btn_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(screen_app_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(screen_app_btn_1, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(screen_app_btn_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(screen_app_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(screen_app_btn_1, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(screen_app_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(screen_app_btn_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(screen_app_btn_1, &lv_font_montserrat_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(screen_app_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(screen_app_btn_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//The custom code of screen_app.
	

	//Update current screen layout.
lv_obj_update_layout(main_count);
}





void setup_scr_screen_param()
{
	//Write codes main_count
	main_count = lv_obj_create(NULL);
	lv_obj_set_size(main_count, 320, 240);
	lv_obj_set_scrollbar_mode(main_count, LV_SCROLLBAR_MODE_OFF);

	//Write style for main_count, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(main_count, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_param_menu_1
	screen_param_menu_1 = lv_menu_create(main_count);

	//Create sidebar page for menu screen_param_menu_1
	screen_param_menu_1_sidebar_page = lv_menu_page_create(screen_param_menu_1, "Settings");
	lv_menu_set_sidebar_page(screen_param_menu_1, screen_param_menu_1_sidebar_page);

	//Create subpage for screen_param_menu_1
	screen_param_menu_1_subpage_1 = lv_menu_page_create(screen_param_menu_1, NULL);
	screen_param_menu_1_cont_1 = lv_menu_cont_create(screen_param_menu_1_sidebar_page);
	screen_param_menu_1_label_1 = lv_label_create(screen_param_menu_1_cont_1);
	lv_label_set_text(screen_param_menu_1_label_1, "WLAN");
	lv_obj_set_size(screen_param_menu_1_label_1, LV_PCT(100), LV_SIZE_CONTENT);
	lv_menu_set_load_page_event(screen_param_menu_1, screen_param_menu_1_cont_1, screen_param_menu_1_subpage_1);

	//Create subpage for screen_param_menu_1
	screen_param_menu_1_subpage_2 = lv_menu_page_create(screen_param_menu_1, NULL);
	screen_param_menu_1_cont_2 = lv_menu_cont_create(screen_param_menu_1_sidebar_page);
	screen_param_menu_1_label_2 = lv_label_create(screen_param_menu_1_cont_2);
	lv_label_set_text(screen_param_menu_1_label_2, "BLUETOOTH");
	lv_obj_set_size(screen_param_menu_1_label_2, LV_PCT(100), LV_SIZE_CONTENT);
	lv_menu_set_load_page_event(screen_param_menu_1, screen_param_menu_1_cont_2, screen_param_menu_1_subpage_2);

	//Create subpage for screen_param_menu_1
	screen_param_menu_1_subpage_3 = lv_menu_page_create(screen_param_menu_1, NULL);
	screen_param_menu_1_cont_3 = lv_menu_cont_create(screen_param_menu_1_sidebar_page);
	screen_param_menu_1_label_3 = lv_label_create(screen_param_menu_1_cont_3);
	lv_label_set_text(screen_param_menu_1_label_3, "LORA");
	lv_obj_set_size(screen_param_menu_1_label_3, LV_PCT(100), LV_SIZE_CONTENT);
	lv_menu_set_load_page_event(screen_param_menu_1, screen_param_menu_1_cont_3, screen_param_menu_1_subpage_3);

	//Create subpage for screen_param_menu_1
	screen_param_menu_1_subpage_4 = lv_menu_page_create(screen_param_menu_1, NULL);
	screen_param_menu_1_cont_4 = lv_menu_cont_create(screen_param_menu_1_sidebar_page);
	screen_param_menu_1_label_4 = lv_label_create(screen_param_menu_1_cont_4);
	lv_label_set_text(screen_param_menu_1_label_4, "AUDIO");
	lv_obj_set_size(screen_param_menu_1_label_4, LV_PCT(100), LV_SIZE_CONTENT);
	lv_menu_set_load_page_event(screen_param_menu_1, screen_param_menu_1_cont_4, screen_param_menu_1_subpage_4);

	//Create subpage for screen_param_menu_1
	screen_param_menu_1_subpage_5 = lv_menu_page_create(screen_param_menu_1, NULL);
	screen_param_menu_1_cont_5 = lv_menu_cont_create(screen_param_menu_1_sidebar_page);
	screen_param_menu_1_label_5 = lv_label_create(screen_param_menu_1_cont_5);
	lv_label_set_text(screen_param_menu_1_label_5, "ECRAN");
	lv_obj_set_size(screen_param_menu_1_label_5, LV_PCT(100), LV_SIZE_CONTENT);
	lv_menu_set_load_page_event(screen_param_menu_1, screen_param_menu_1_cont_5, screen_param_menu_1_subpage_5);

	//Create subpage for screen_param_menu_1
	screen_param_menu_1_subpage_6 = lv_menu_page_create(screen_param_menu_1, NULL);
	screen_param_menu_1_cont_6 = lv_menu_cont_create(screen_param_menu_1_sidebar_page);
	screen_param_menu_1_label_6 = lv_label_create(screen_param_menu_1_cont_6);
	lv_label_set_text(screen_param_menu_1_label_6, "MICRO");
	lv_obj_set_size(screen_param_menu_1_label_6, LV_PCT(100), LV_SIZE_CONTENT);
	lv_menu_set_load_page_event(screen_param_menu_1, screen_param_menu_1_cont_6, screen_param_menu_1_subpage_6);

	//Create subpage for screen_param_menu_1
	screen_param_menu_1_subpage_7 = lv_menu_page_create(screen_param_menu_1, NULL);
	screen_param_menu_1_cont_7 = lv_menu_cont_create(screen_param_menu_1_sidebar_page);
	screen_param_menu_1_label_7 = lv_label_create(screen_param_menu_1_cont_7);
	lv_label_set_text(screen_param_menu_1_label_7, "TIME");
	lv_obj_set_size(screen_param_menu_1_label_7, LV_PCT(100), LV_SIZE_CONTENT);
	lv_menu_set_load_page_event(screen_param_menu_1, screen_param_menu_1_cont_7, screen_param_menu_1_subpage_7);
	lv_event_send(screen_param_menu_1_cont_1, LV_EVENT_CLICKED, NULL);
	lv_obj_set_pos(screen_param_menu_1, 0, 0);
	lv_obj_set_size(screen_param_menu_1, 320, 240);
	lv_obj_set_scrollbar_mode(screen_param_menu_1, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_param_menu_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(screen_param_menu_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(screen_param_menu_1, lv_color_hex(0xf2fbff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(screen_param_menu_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(screen_param_menu_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(screen_param_menu_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(screen_param_menu_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(screen_param_menu_1, LV_BORDER_SIDE_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(screen_param_menu_1, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(screen_param_menu_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for &style_screen_param_menu_1_extra_sidebar_page_main_default
	static lv_style_t style_screen_param_menu_1_extra_sidebar_page_main_default;
	
	lv_style_set_bg_opa(&style_screen_param_menu_1_extra_sidebar_page_main_default, 255);
	lv_style_set_bg_color(&style_screen_param_menu_1_extra_sidebar_page_main_default, lv_color_hex(0xdaf1ff));
	lv_style_set_bg_grad_dir(&style_screen_param_menu_1_extra_sidebar_page_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_radius(&style_screen_param_menu_1_extra_sidebar_page_main_default, 0);
	lv_obj_add_style(screen_param_menu_1_sidebar_page, &style_screen_param_menu_1_extra_sidebar_page_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_DEFAULT for &style_screen_param_menu_1_extra_option_btns_main_default
	static lv_style_t style_screen_param_menu_1_extra_option_btns_main_default;
	
	lv_style_set_text_color(&style_screen_param_menu_1_extra_option_btns_main_default, lv_color_hex(0x151212));
	lv_style_set_text_font(&style_screen_param_menu_1_extra_option_btns_main_default, &lv_font_montserrat_16);
	lv_style_set_text_opa(&style_screen_param_menu_1_extra_option_btns_main_default, 255);
	lv_style_set_text_align(&style_screen_param_menu_1_extra_option_btns_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_pad_top(&style_screen_param_menu_1_extra_option_btns_main_default, 10);
	lv_style_set_pad_bottom(&style_screen_param_menu_1_extra_option_btns_main_default, 10);
	lv_obj_add_style(screen_param_menu_1_cont_7, &style_screen_param_menu_1_extra_option_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_style(screen_param_menu_1_cont_6, &style_screen_param_menu_1_extra_option_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_style(screen_param_menu_1_cont_5, &style_screen_param_menu_1_extra_option_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_style(screen_param_menu_1_cont_4, &style_screen_param_menu_1_extra_option_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_style(screen_param_menu_1_cont_3, &style_screen_param_menu_1_extra_option_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_style(screen_param_menu_1_cont_2, &style_screen_param_menu_1_extra_option_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_add_style(screen_param_menu_1_cont_1, &style_screen_param_menu_1_extra_option_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style state: LV_STATE_CHECKED for &style_screen_param_menu_1_extra_option_btns_main_checked
	static lv_style_t style_screen_param_menu_1_extra_option_btns_main_checked;
	
	lv_style_set_text_color(&style_screen_param_menu_1_extra_option_btns_main_checked, lv_color_hex(0x00e7ff));
	lv_style_set_text_font(&style_screen_param_menu_1_extra_option_btns_main_checked, &lv_font_montserrat_16);
	lv_style_set_text_opa(&style_screen_param_menu_1_extra_option_btns_main_checked, 255);
	lv_style_set_text_align(&style_screen_param_menu_1_extra_option_btns_main_checked, LV_TEXT_ALIGN_CENTER);
	lv_style_set_border_width(&style_screen_param_menu_1_extra_option_btns_main_checked, 0);
	lv_style_set_radius(&style_screen_param_menu_1_extra_option_btns_main_checked, 0);
	lv_style_set_bg_opa(&style_screen_param_menu_1_extra_option_btns_main_checked, 255);
	lv_style_set_bg_color(&style_screen_param_menu_1_extra_option_btns_main_checked, lv_color_hex(0xdaf1ff));
	lv_style_set_bg_grad_dir(&style_screen_param_menu_1_extra_option_btns_main_checked, LV_GRAD_DIR_HOR);
	lv_style_set_bg_grad_color(&style_screen_param_menu_1_extra_option_btns_main_checked, lv_color_hex(0xf7fbff));
	lv_style_set_bg_main_stop(&style_screen_param_menu_1_extra_option_btns_main_checked, 0);
	lv_style_set_bg_grad_stop(&style_screen_param_menu_1_extra_option_btns_main_checked, 0);
	lv_obj_add_style(screen_param_menu_1_cont_7, &style_screen_param_menu_1_extra_option_btns_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_add_style(screen_param_menu_1_cont_6, &style_screen_param_menu_1_extra_option_btns_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_add_style(screen_param_menu_1_cont_5, &style_screen_param_menu_1_extra_option_btns_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_add_style(screen_param_menu_1_cont_4, &style_screen_param_menu_1_extra_option_btns_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_add_style(screen_param_menu_1_cont_3, &style_screen_param_menu_1_extra_option_btns_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_add_style(screen_param_menu_1_cont_2, &style_screen_param_menu_1_extra_option_btns_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_add_style(screen_param_menu_1_cont_1, &style_screen_param_menu_1_extra_option_btns_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style state: LV_STATE_DEFAULT for &style_screen_param_menu_1_extra_main_title_main_default
	static lv_style_t style_screen_param_menu_1_extra_main_title_main_default;
	
	lv_style_set_text_color(&style_screen_param_menu_1_extra_main_title_main_default, lv_color_hex(0x41485a));
	lv_style_set_text_font(&style_screen_param_menu_1_extra_main_title_main_default, &lv_font_montserrat_16);
	lv_style_set_text_opa(&style_screen_param_menu_1_extra_main_title_main_default, 255);
	lv_style_set_text_align(&style_screen_param_menu_1_extra_main_title_main_default, LV_TEXT_ALIGN_CENTER);
	lv_style_set_bg_opa(&style_screen_param_menu_1_extra_main_title_main_default, 255);
	lv_style_set_bg_color(&style_screen_param_menu_1_extra_main_title_main_default, lv_color_hex(0xa8dfff));
	lv_style_set_bg_grad_dir(&style_screen_param_menu_1_extra_main_title_main_default, LV_GRAD_DIR_NONE);
	lv_style_set_pad_top(&style_screen_param_menu_1_extra_main_title_main_default, 5);
	lv_style_set_pad_bottom(&style_screen_param_menu_1_extra_main_title_main_default, 5);
	lv_menu_t * screen_param_menu_1_menu= (lv_menu_t *)screen_param_menu_1;
	lv_obj_t * screen_param_menu_1_title = screen_param_menu_1_menu->sidebar_header_title;
	lv_obj_set_size(screen_param_menu_1_title, LV_PCT(100), LV_SIZE_CONTENT);
	lv_obj_add_style(lv_menu_get_sidebar_header(screen_param_menu_1), &style_screen_param_menu_1_extra_main_title_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);



	//Write codes screen_param_cont_2
	screen_param_cont_2 = lv_obj_create(screen_param_menu_1_subpage_1);
	lv_obj_set_pos(screen_param_cont_2, -10, 23);
	lv_obj_set_size(screen_param_cont_2, 220, 230);
	lv_obj_set_scrollbar_mode(screen_param_cont_2, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_param_cont_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(screen_param_cont_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(screen_param_cont_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(screen_param_cont_2, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(screen_param_cont_2, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_TOP, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(screen_param_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(screen_param_cont_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(screen_param_cont_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(screen_param_cont_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(screen_param_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(screen_param_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(screen_param_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(screen_param_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(screen_param_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_param_cb_2
	screen_param_cb_2 = lv_checkbox_create(screen_param_cont_2);
	lv_checkbox_set_text(screen_param_cb_2, "activé wifi");
	lv_obj_set_pos(screen_param_cb_2, 10, 10);

	//Write style for screen_param_cb_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(screen_param_cb_2, lv_color_hex(0x0D3055), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(screen_param_cb_2, &lv_font_montserrat_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(screen_param_cb_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(screen_param_cb_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(screen_param_cb_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(screen_param_cb_2, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(screen_param_cb_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(screen_param_cb_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(screen_param_cb_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(screen_param_cb_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_param_cb_2, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(screen_param_cb_2, 2, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(screen_param_cb_2, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(screen_param_cb_2, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(screen_param_cb_2, LV_BORDER_SIDE_FULL, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(screen_param_cb_2, 6, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(screen_param_cb_2, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(screen_param_cb_2, lv_color_hex(0xffffff), LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(screen_param_cb_2, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);

	//Write codes screen_param_cb_4
	screen_param_cb_4 = lv_checkbox_create(screen_param_cont_2);
	lv_checkbox_set_text(screen_param_cb_4, "activé wifi");
	lv_obj_set_pos(screen_param_cb_4, 10, 50);

	//Write style for screen_param_cb_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(screen_param_cb_4, lv_color_hex(0x0D3055), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(screen_param_cb_4, &lv_font_montserrat_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(screen_param_cb_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(screen_param_cb_4, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(screen_param_cb_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(screen_param_cb_4, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(screen_param_cb_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(screen_param_cb_4, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(screen_param_cb_4, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(screen_param_cb_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_param_cb_4, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(screen_param_cb_4, 2, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(screen_param_cb_4, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(screen_param_cb_4, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(screen_param_cb_4, LV_BORDER_SIDE_FULL, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(screen_param_cb_4, 6, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(screen_param_cb_4, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(screen_param_cb_4, lv_color_hex(0xffffff), LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(screen_param_cb_4, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);



	//Write codes screen_param_cont_3
	screen_param_cont_3 = lv_obj_create(screen_param_menu_1_subpage_2);
	lv_obj_set_pos(screen_param_cont_3, 3, 27);
	lv_obj_set_size(screen_param_cont_3, 220, 230);
	lv_obj_set_scrollbar_mode(screen_param_cont_3, LV_SCROLLBAR_MODE_OFF);

	//Write style for screen_param_cont_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(screen_param_cont_3, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(screen_param_cont_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(screen_param_cont_3, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(screen_param_cont_3, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_TOP, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(screen_param_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(screen_param_cont_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(screen_param_cont_3, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(screen_param_cont_3, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(screen_param_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(screen_param_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(screen_param_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(screen_param_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(screen_param_cont_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes screen_param_cb_3
	screen_param_cb_3 = lv_checkbox_create(screen_param_cont_3);
	lv_checkbox_set_text(screen_param_cb_3, "activé wifi");
	lv_obj_set_pos(screen_param_cb_3, 10, 10);

	//Write style for screen_param_cb_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(screen_param_cb_3, lv_color_hex(0x0D3055), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(screen_param_cb_3, &lv_font_montserrat_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(screen_param_cb_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(screen_param_cb_3, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(screen_param_cb_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(screen_param_cb_3, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(screen_param_cb_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(screen_param_cb_3, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(screen_param_cb_3, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(screen_param_cb_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for screen_param_cb_3, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(screen_param_cb_3, 2, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(screen_param_cb_3, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(screen_param_cb_3, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(screen_param_cb_3, LV_BORDER_SIDE_FULL, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(screen_param_cb_3, 6, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(screen_param_cb_3, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(screen_param_cb_3, lv_color_hex(0xffffff), LV_PART_INDICATOR|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(screen_param_cb_3, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);











	//Write codes screen_param_btn_1
	screen_param_btn_1 = lv_btn_create(main_count);
	screen_param_btn_1_label = lv_label_create(screen_param_btn_1);
  lv_obj_add_event_cb(screen_param_btn_1, lv_button_event_cb, LV_EVENT_CLICKED, (void *)&event[2]);
	lv_label_set_text(screen_param_btn_1_label, "Back");
	lv_label_set_long_mode(screen_param_btn_1_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(screen_param_btn_1_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(screen_param_btn_1, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(screen_param_btn_1, 257, 2);
	lv_obj_set_size(screen_param_btn_1, 60, 20);

	//Write style for screen_param_btn_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(screen_param_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(screen_param_btn_1, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(screen_param_btn_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_width(screen_param_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(screen_param_btn_1, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(screen_param_btn_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(screen_param_btn_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(screen_param_btn_1, &lv_font_montserrat_16, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(screen_param_btn_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(screen_param_btn_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

	//The custom code of main_count.
	

	//Update current screen layout.
	lv_obj_update_layout(main_count);

}
