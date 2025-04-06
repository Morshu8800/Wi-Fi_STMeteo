# Wi-Fi_STMeteo
Wi-Fi STMeteo версия 1.0
  Метеостанция на базе микроконтроллера STM32G070xx (Cortex-M0+)
# Материалы:
  - Stm32G031xx (Минимум 64КБ флеш-памяти и 6КБ оперативной памяти);
  - Wi-Fi микроконтроллер ESP8266 (WeMos D1 mini);
  - Дисплей на контроллере HD44780 с переходником I2C на PCF8574;
  - Датчик температуры и влажности SHT3x;
  - Датчик температуры и давления BMP280;
  - Модуль часов реального времени DS3231mini;
  - Разъём Type-C в корпусе с проводами;
  - Кнопка тактовая 3 шт;
  - Корпус (любой);
  - Резисторы: 220 Ом, 10 КОм.
# Что может это устройство
  - Устройство может измерять Температуру, влажность, давление;
  - Показывать время и дату, а также по запросу отправлять данные на телефон через Telegram бота;
  - Если Telegram бот не нужен или Wi-Fi точка недоступна, то в дополнительных настройках Wi-Fi модуль можно отключить.
# Места использования устройства
  - Дом
  - Офис
# Светодиод состояния Wi-Fi соединения
  - Не светится     -> Не подключено к Wi-Fi
  - Медленно мигает -> В режиме настройки Wi-Fi подключения
  - Быстро мигает   -> Подключено к Wi-Fi
  - Постоянно горит -> Wi-Fi модуль перезапущен
# Пример готового устройства
![Sample by Morshu8800 ](https://github.com/Morshu8800/Wi-Fi_STMeteo/blob/main/Docs/Sample.jpg)

#
Окончательный дизайн устройства может изменяться!
ВНИМАНИЕ! Устройство имеет вентялиционные отверстия и не имеет защиты от влаги и пыли
#

# Возможные доработки
  - Добавить микросхему EEPROM (FRAM) для сохранения настроек.
  - Улучшить обработчик срабатывания кнопок
  - Заменить кнопки на более эргономичные
  - Изменить дизайн корпуса
  - Изменить страницу настройки Wi-Fi соединения
