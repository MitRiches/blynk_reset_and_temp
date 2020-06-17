#include <Arduino.h>



/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
#define TINY_GSM_MODEM_SIM800
#include <TinyGsmClient.h>
#include "BlynkSimpleTinyGSM.h"
#include <OneWire.h> //для работы на Dallas
#include <DallasTemperature.h> //для работы на Dallas

// You should get Auth Token in the Blynk App.
char auth[] = "Ss3sS_XRWtc8k25ALZQ6_XYnGZQjls64";


// Your GPRS credentials
char apn[]  = "internet";
char user[] = "";
char pass[] = "";

BlynkTimer timer;  //инициализация timer 

// Hardware Serial on Mega, Leonardo, Micro
//#define SerialAT Serial1
// or Software Serial on Uno, Nano
#include <SoftwareSerial.h>
SoftwareSerial SerialAT(3, 2); // RX, TX

TinyGsm modem(SerialAT);

WidgetLED led1(V21); //светодиод показывающий что сработала команда на перезагрузку



#define ONE_WIRE_BUS 15 // DS18B20 подключаем на 15(A1)
OneWire oneWire(ONE_WIRE_BUS); //
DallasTemperature dallas(&oneWire); //
byte qty; // определяем количество датчиков на шине
//функция тестирует датчики и при изменении количества останавливает работу программы
void testDS(){  // ищим датчики на шине, если количество изменилось, останавливаем работу
   dallas.begin();
}
/***************************************************
 * Отправляем данные с датчиков в Blynk
 **************************************************/
//функция датчиков
  void getSendData() {  
  dallas.requestTemperatures(); //считывание теmпературы с датчиков
  float temp0 = dallas.getTempCByIndex(0); // данные в цельсиях с датчика 1

  Blynk.virtualWrite(V22, temp0); // отправка значений датчика 1 на виртуальный порт 22
  }

  void reconnectBlynk() {
  digitalWrite(14, HIGH);   // A0 
}

void setup()
{
  dallas.begin(); //старт датчиков
  timer.setInterval(10000L, getSendData); //задержка в 10сек
  qty = dallas.getDeviceCount(); //получаем данные с кол-вом доступных датчиков


timer.setInterval(43200000, reconnectBlynk); //43200000


#define IN_MUX1 4// РІС…РѕРґ 1 Р�?СѓР»СЊС‚РёРїР»РµРєСЃРѕСЂР° РЅР° С†РёС„СЂРѕРІРѕР№ РІС…РѕРґ PD4
#define IN_MUX2 8// РІС…РѕРґ 2 Р�?СѓР»СЊС‚РёРїР»РµРєСЃРѕСЂР° РЅР° С†РёС„СЂРѕРІРѕР№ РІС…РѕРґ PB0

  // Р—Р°РґР°РµР�? СѓРїСЂР°РІР»СЏСЋС‰РёРµ СЃРёРіРЅР°Р»С‹ РґР»СЏ Р�?СѓР»СЊС‚РёРїР»РµРєСЃРѕСЂР°
#define A 5 // РЅРѕРіР° Р°РґСЂРµСЃР° Р�?СѓР»СЊС‚РёРїР»РµРєСЃРѕСЂРѕРІ // CD4051 pin 11 (A)
#define B 6 // РІС‚РѕСЂР°СЏ// CD4051 pin 10 (B)
#define C 7 // С‚СЂРµС‚СЊСЏ // CD4051 pin 9  (C)
#define GET_BIT(value, bit_number) (((value) >> (bit_number)) & 0x1) // СЃРєСЂРёРїС‚ РїРµСЂРµРІРѕРґР° С‡РёСЃР»Р° РІ РґРІРѕРёС‡РЅС‹Р№ РєРѕРґ, РЅРѕ СЌС‚Рѕ РЅРµ С‚РѕС‡РЅРѕ.

  pinMode(IN_MUX1, OUTPUT);
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(IN_MUX2, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  
  pinMode(14, OUTPUT);
  
  // Debug console
  Serial.begin(9600);

  delay(10);

  // Set GSM module baud rate
  SerialAT.begin(9600);
  delay(3000);

  // Restart takes quite some time
  // To skip it, call init() instead of restart()
 Serial.println("Initializing modem...");
 modem.restart();

  // Unlock your SIM card with a PIN
  //modem.simUnlock("1 234");

  Blynk.begin(auth, modem, apn, user, pass, "80.211.186.40", 8080);
}



void OUTPIN (int i, int pin, int ButtonSW) //функция выбора порта пина
{
  static int Button_last = 0;
  if (ButtonSW == 1 && Button_last == 0) 
  { Button_last = ButtonSW;

    //
    digitalWrite(A, GET_BIT(i, 0));
    digitalWrite(B, GET_BIT(i, 1));
    digitalWrite(C, GET_BIT(i, 2));
    //
    digitalWrite(pin, Button_last);
    led1.on();
    delay (1000);
  }

  if (ButtonSW == 0)
  { Button_last = 0;
    digitalWrite(pin, Button_last);
    led1.off();
  } }


BLYNK_WRITE(V0)
{
  int ButtonSW = param.asInt(); // РџСЂРёРЅРёР�?Р°РµР�? Р·РЅР°С‡РµРЅРёРµ РѕС‚ BUTTON
  OUTPIN (0, IN_MUX1, ButtonSW);
}

BLYNK_WRITE(V1)
{
  int ButtonSW = param.asInt(); // РџСЂРёРЅРёР�?Р°РµР�? Р·РЅР°С‡РµРЅРёРµ РѕС‚ BUTTON
  OUTPIN (1, IN_MUX1, ButtonSW);
  // Serial.println("V1");
}

BLYNK_WRITE(V2)
{
  int ButtonSW = param.asInt(); // РџСЂРёРЅРёР�?Р°РµР�? Р·РЅР°С‡РµРЅРёРµ РѕС‚ BUTTON
  OUTPIN (2, IN_MUX1, ButtonSW);
}

BLYNK_WRITE(V3)
{
  int ButtonSW = param.asInt(); // РџСЂРёРЅРёР�?Р°РµР�? Р·РЅР°С‡РµРЅРёРµ РѕС‚ BUTTON
  OUTPIN (3, IN_MUX1, ButtonSW);
}

BLYNK_WRITE(V4)
{
  int ButtonSW = param.asInt(); // РџСЂРёРЅРёР�?Р°РµР�? Р·РЅР°С‡РµРЅРёРµ РѕС‚ BUTTON
  OUTPIN (4, IN_MUX1, ButtonSW);
}

BLYNK_WRITE(V5)
{
  int ButtonSW = param.asInt(); // РџСЂРёРЅРёР�?Р°РµР�? Р·РЅР°С‡РµРЅРёРµ РѕС‚ BUTTON
  OUTPIN (5, IN_MUX1, ButtonSW);
}

BLYNK_WRITE(V6)
{
  int ButtonSW = param.asInt(); // РџСЂРёРЅРёР�?Р°РµР�? Р·РЅР°С‡РµРЅРёРµ РѕС‚ BUTTON
  OUTPIN (6, IN_MUX1, ButtonSW);
}

BLYNK_WRITE(V7)
{
  int ButtonSW = param.asInt(); // РџСЂРёРЅРёР�?Р°РµР�? Р·РЅР°С‡РµРЅРёРµ РѕС‚ BUTTON
  OUTPIN (7, IN_MUX1, ButtonSW);
}

BLYNK_WRITE(V8)
{
  int ButtonSW = param.asInt(); // РџСЂРёРЅРёР�?Р°РµР�? Р·РЅР°С‡РµРЅРёРµ РѕС‚ BUTTON
  OUTPIN (0, IN_MUX2, ButtonSW);
}

BLYNK_WRITE(V9)
{
  int ButtonSW = param.asInt(); // РџСЂРёРЅРёР�?Р°РµР�? Р·РЅР°С‡РµРЅРёРµ РѕС‚ BUTTON
  OUTPIN (1, IN_MUX2, ButtonSW);
}

BLYNK_WRITE(V10)
{
  int ButtonSW = param.asInt(); // РџСЂРёРЅРёР�?Р°РµР�? Р·РЅР°С‡РµРЅРёРµ РѕС‚ BUTTON
  OUTPIN (2, IN_MUX2, ButtonSW);
}

BLYNK_WRITE(V11)
{
  int ButtonSW = param.asInt(); // РџСЂРёРЅРёР�?Р°РµР�? Р·РЅР°С‡РµРЅРёРµ РѕС‚ BUTTON
  OUTPIN (3, IN_MUX2, ButtonSW);
}

BLYNK_WRITE(V12)
{
  int ButtonSW = param.asInt(); // РџСЂРёРЅРёР�?Р°РµР�? Р·РЅР°С‡РµРЅРёРµ РѕС‚ BUTTON
  OUTPIN (4, IN_MUX2, ButtonSW);
}

BLYNK_WRITE(V13)
{
  int ButtonSW = param.asInt(); // РџСЂРёРЅРёР�?Р°РµР�? Р·РЅР°С‡РµРЅРёРµ РѕС‚ BUTTON
  OUTPIN (5, IN_MUX2, ButtonSW);
}

BLYNK_WRITE(V14)
{
  int ButtonSW = param.asInt(); // РџСЂРёРЅРёР�?Р°РµР�? Р·РЅР°С‡РµРЅРёРµ РѕС‚ BUTTON
  OUTPIN (6, IN_MUX2, ButtonSW);
}

BLYNK_WRITE(V15)
{
  int ButtonSW = param.asInt(); // РџСЂРёРЅРёР�?Р°РµР�? Р·РЅР°С‡РµРЅРёРµ РѕС‚ BUTTON
  OUTPIN (7, IN_MUX2, ButtonSW);
}

BLYNK_WRITE(V16)
{
  int ButtonSW = param.asInt(); // РџСЂРёРЅРёР�?Р°РµР�? Р·РЅР°С‡РµРЅРёРµ РѕС‚ BUTTON
  OUTPIN (0, 9, ButtonSW);
}

BLYNK_WRITE(V17)
{
  int ButtonSW = param.asInt(); // РџСЂРёРЅРёР�?Р°РµР�? Р·РЅР°С‡РµРЅРёРµ РѕС‚ BUTTON
  OUTPIN (0, 10, ButtonSW);
}

BLYNK_WRITE(V18)
{
  int ButtonSW = param.asInt(); // РџСЂРёРЅРёР�?Р°РµР�? Р·РЅР°С‡РµРЅРёРµ РѕС‚ BUTTON
  OUTPIN (0, 11, ButtonSW);
}

BLYNK_WRITE(V19)
{
  int ButtonSW = param.asInt(); // РџСЂРёРЅРёР�?Р°РµР�? Р·РЅР°С‡РµРЅРёРµ РѕС‚ BUTTON
  OUTPIN (0, 12, ButtonSW);
}



void loop()
{
  Blynk.run();
  timer.run();
}

