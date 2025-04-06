#define BOT_TOKEN "8122694396:AAGj9SRNcdVbeYZnoX-4HvaHtR_NJj58zSg"
#define Wifi_wait_time 15000
#define stm_check D1
#define esp_check D2

#include <ESP8266WiFi.h>
#include <SimplePortal.h>
#include <FastBot.h>
#include <EEPROM.h>
#include <String.h>
#include <Strings.h>
#include <AsyncStream.h>

struct {
  String WiFi_SSID = "", WiFi_PASS = "";
} WiFi_settings;

bool con_lost = false, con_succ = false, get_press = false,
     get_temp = false, get_all = false, get_humid = false, change_sett_sel = false, chng_settings = false;

uint32_t WIFI_time = 0, con_check = 0, close_menu_time = 0;

FastBot bot(BOT_TOKEN);
AsyncStream<100> serial(&Serial, ';');

void setup(void) {
  pinMode(stm_check, INPUT);
  pinMode(esp_check, OUTPUT);
  WIFI_time = millis();
  startup();
  con_check = millis();
  if (digitalRead(stm_check) == 0) {
    bot.tickManual();
    bot.sendMessage("Главная часть недоступна! Перезапустите устройство, в крайнем случае обратитесь к специалисту.");
    while (1) {
      delay(100);
    }
  }
}

void loop(void) {
  bot.tick();
  recive_mes();
  if (digitalRead(stm_check) == 0) {
    bot.tickManual();
    bot.sendMessage("Главная часть недоступна! Перезапустите устройство, в крайнем случае обратитесь к специалисту.");
    while (1) {
      delay(100);
    }
  }
  if (millis() - con_check > 10000 && !chng_settings) {
    con_check = millis();
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("CL");
      bot.tickManual();
      while (WiFi.status() != WL_CONNECTED) {
        yield();
        con_check = millis();
      }
    } else {
      Serial.println("CP");
    }
  }
  if (chng_settings) {
    con_check = millis();
    Change_settings();
  }
}

void startup(void) {
  eeprom_check();
  Serial.begin(115200);
  Serial.println("ST");
  delay(1000);
  connect();

  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(esp_check, HIGH);
    Serial.println("CP");
    //Serial.println(WiFi.localIP());
    bot.setChatID("5142370828");
    bot.sendMessage("Подключение выполнено!");
    bot.attach(Msg);
  }
  while (WiFi.status() != WL_CONNECTED) {
    yield();
  }
}


void Change_settings(void) {
  Serial.println("AP");
  delay(1000);
  Serial.println("AP");
  portalRun(120000);
  //Serial.println(portalStatus());
  if (portalStatus() == SP_SUBMIT) {
    //Serial.println(portalCfg.SSID);
    WiFi_settings.WiFi_SSID = portalCfg.SSID;
    //Serial.println(portalCfg.pass);
    WiFi_settings.WiFi_PASS = portalCfg.pass;
    EEPROM.commit();
    EEPROM.put(1, WiFi_settings);
    EEPROM.commit();
    Serial.println("RS");
    delay(1000);
    ESP.restart();
  } else if (portalStatus() == SP_ERROR) {
    Serial.println("RS");
    delay(1000);
    ESP.restart();
  } else if (portalStatus() == SP_EXIT) {
    Serial.println("RS");
    delay(1000);
    ESP.restart();
  } else if (portalStatus() == SP_SWITCH_AP) {
    Serial.println("RS");
    delay(1000);
    ESP.restart();
  } else if (portalStatus() == SP_SWITCH_LOCAL) {
    Serial.println("RS");
    delay(1000);
    ESP.restart();
  } else if (portalStatus() == SP_TIMEOUT) {
    Serial.println("RS");
    delay(1000);
    ESP.restart();
  }
}

void Msg(FB_msg& msg) {
  if (msg.text == "/start") {
    bot.tickManual();
    bot.sendMessage("Приветствую тебя странник, ты попал к чат-боту Wi-Fi STMeteo (версия 1.0), так что усаживайся по удобнее и спрашивай о влажности/температуре/давлении.\nПриятного использования(не шали особо!)\nMorshu8800, (C)2025.");
  } else if (msg.text == "/reset") {
    bot.sendMessage("Перезагрузка...");
    Serial.println("RS");
    delay(1000);
    bot.tickManual();
    ESP.restart();
  } else if (msg.OTA && msg.fileName == "WEATHER_TARO.bin") {
    Serial.println("UD");
    bot.update();
  } else if (msg.OTA && msg.fileName != "WEATHER_TARO.bin") bot.sendMessage("Неизвестный файл, отправьте нужный файл для обновления по воздуху.\n!!БУДБТЕ ОСТОРОЖНЫ! НЕ СКИДЫВАЙТЕ ТЕ ФАЙЛЫ, КОТОРЫЕ МОГУТ ПРИЧИНИТЬ ВРЕД ВАМ И ВАШИМ УСТРОЙСТВАМ!!");
  else if (msg.text == "/get_pressure") {
    if (digitalRead(stm_check) == 0) {
      bot.tickManual();
      bot.sendMessage("Главная часть недоступна! Перезапустите устройство, в крайнем случае обратитесь к специалисту.");
      while (1) {
        delay(100);
      }
    }
    Serial.println("GP");
    get_press = true;
  } else if (msg.text == "/get_temperature") {
    if (digitalRead(stm_check) == 0) {
      bot.tickManual();
      bot.sendMessage("Главная часть недоступна! Перезапустите устройство, в крайнем случае обратитесь к специалисту.");
      while (1) {
        delay(100);
      }
    }
    Serial.println("GT");
    get_temp = true;
  } else if (msg.text == "/get_humidity") {
    if (digitalRead(stm_check) == 0) {
      bot.tickManual();
      bot.sendMessage("Главная часть недоступна! Перезапустите устройство, в крайнем случае обратитесь к специалисту.");
      while (1) {
        delay(100);
      }
    }
    Serial.println("GH");
    get_humid = true;
  } else if (msg.text == "/get_all_stats") {
    if (digitalRead(stm_check) == 0) {
      bot.tickManual();
      bot.sendMessage("Главная часть недоступна! Перезапустите устройство, в крайнем случае обратитесь к специалисту.");
      while (1) {
        delay(100);
      }
    }
    Serial.println("GA");
    get_all = true;
  } else if (msg.text == "/change_settings") {
    change_sett_sel = true;
    close_menu_time = millis();
    bot.showMenuText("Вы точно хотите изменить настройки?", "ДА \t НЕТ \t", true);
    bot.tickManual();
  }
  while (change_sett_sel) {
    yield();
    bot.tick();
    if (msg.text == "ДА" && change_sett_sel) {
      close_menu_time = millis();
      bot.closeMenuText("Переход в режим точки доступа \"ESP Config\".");
      bot.tickManual();
      change_sett_sel = false;
      chng_settings = true;
    }
    if (msg.text == "НЕТ" && change_sett_sel) {
      close_menu_time = millis();
      change_sett_sel = false;
      chng_settings = false;
      bot.closeMenuText("Изменения отменены.");
      bot.tickManual();
    }
    if (change_sett_sel && millis() - close_menu_time > 30000) {
      change_sett_sel = false;
      chng_settings = false;
      bot.closeMenuText("Изменения отменены.");
      bot.tickManual();
    }
    yield();
  }
}
void recive_mes(void) {
  if (serial.available()) {
    if (strcmp(serial.buf, "CONC") == 0 && WiFi.status() == WL_CONNECTED) {
      Serial.println("CP");
      //bot.sendMessage(serial.buf);
    }
    if (get_press) {
      bot.sendMessage(serial.buf);
      memset(serial.buf, 0, sizeof(serial.buf));
      get_press = false;
    }
    if (get_temp) {
      bot.sendMessage(serial.buf);
      memset(serial.buf, 0, sizeof(serial.buf));
      get_temp = false;
    }
    if (get_humid) {
      bot.sendMessage(serial.buf);
      memset(serial.buf, 0, sizeof(serial.buf));
      get_humid = false;
    }
    if (get_all) {
      bot.sendMessage(serial.buf);
      memset(serial.buf, 0, sizeof(serial.buf));
      get_all = false;
    }
  }
}


void connect(void) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WiFi_settings.WiFi_SSID, WiFi_settings.WiFi_PASS);
  while (WiFi.status() != WL_CONNECTED && millis() - WIFI_time <= Wifi_wait_time) {
    delay(500);
  }
  if (millis() - WIFI_time > Wifi_wait_time && WiFi.status() != WL_CONNECTED) {
    digitalWrite(esp_check, HIGH);
    delay(100);
    Serial.println("AP");
    delay(2000);
    portal();
  } else {
    return;
  }
}

void portal(void) {
  portalRun(120000);
  //Serial.println(portalStatus());
  if (portalStatus() == SP_SUBMIT) {
    //Serial.println(portalCfg.SSID);
    WiFi_settings.WiFi_SSID = portalCfg.SSID;
    //Serial.println(portalCfg.pass);
    WiFi_settings.WiFi_PASS = portalCfg.pass;
    EEPROM.commit();
    EEPROM.put(1, WiFi_settings);
    EEPROM.commit();
    Serial.println("RS");
    delay(1000);
    ESP.restart();
  } else if (portalStatus() == SP_ERROR) {
    Serial.println("RS");
    delay(1000);
    ESP.restart();
  } else if (portalStatus() == SP_EXIT) {
    Serial.println("RS");
    delay(1000);
    ESP.restart();
  } else if (portalStatus() == SP_SWITCH_AP) {
    Serial.println("RS");
    delay(1000);
    ESP.restart();
  } else if (portalStatus() == SP_SWITCH_LOCAL) {
    Serial.println("RS");
    delay(1000);
    ESP.restart();
  } else if (portalStatus() == SP_TIMEOUT) {
    Serial.println("RS");
    delay(1000);
    ESP.restart();
  }
}

void eeprom_check(void) {
  EEPROM.begin(256);
  if (EEPROM.read(0) != 'w') {
    EEPROM.commit();
    EEPROM.put(1, WiFi_settings);
    EEPROM.commit();
    EEPROM.write(0, 'w');
    EEPROM.commit();
  } else {
    EEPROM.get(1, WiFi_settings);
    EEPROM.commit();
  }
}