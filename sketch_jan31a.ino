// импортируем библиотеки
#include <GyverINA.h>
#include <Servo.h>
#include <Wire.h>
#include <Servo.h>
// инициализируем сервомотор
Servo myservo;             
// задаем необходимые значения
int initial_position = 90;   // мотор на 90
int LDR1 = A0;          // пин для первого датчика света
int LDR2 = A1;          // пин для второго датчика света
int step = 120;          // "шаг разброса"
int servopin = 9;        // пин для сервомотора
// содаем объекты ina219
INA219 ina1(0.05f, 2.0f, 0x41);
INA219 ina2(0.05f, 2.0f, 0x40);
void setup() {
  myservo.attach(servopin);
  pinMode(LDR1, INPUT);   // инициализация первого датчика света
  pinMode(LDR2, INPUT);   // инициализация второго датчика света
  // Открываем последовательный порт
  Serial.begin(9600);
  Serial.print(F("INA219..."));

  // Проверяем наличие и инициализируем INA219
  if (ina1.begin()) {
    Serial.println(F("ina1 connected!"));
  } else {
    Serial.println(F("ina1 not found!"));
    while (1);
  }

  if (ina2.begin()) {
    Serial.println(F("ina2 connected!"));
  } else {
    Serial.println(F("ina2 not found!"));
    while (1);
  }

  ina1.setResolution(INA219_VBUS, INA219_RES_12BIT_X4);      // Напряжение в 12ти битном режиме + 4х кратное усреднение
  ina1.setResolution(INA219_VSHUNT, INA219_RES_12BIT_X128);  // Ток в 12ти битном режиме + 128х кратное усреднение
  ina2.setResolution(INA219_VBUS, INA219_RES_12BIT_X4);      // Напряжение в 12ти битном режиме + 4х кратное усреднение
  ina2.setResolution(INA219_VSHUNT, INA219_RES_12BIT_X128);  // Ток в 12ти битном режиме + 128х кратное усреднение
}

void loop() {
  int R1 = analogRead(LDR1); // считывание данных с датичка света 1
  int R2 = analogRead(LDR2); // считывание данных с датчика света 2
  int diff1 = abs(R1 - R2);   // просчитиывание "шага разброса"
  int diff2 = abs(R2 - R1);
  // проверка разброса
  if((diff1 <= step) || (diff2 <= step)) {
    myservo.write(90); // остановка мотора
    initial_position = 90;
  } else {    
    if(R1 > R2)
    {
      initial_position = initial_position - 5;  // Поворот сервомотора
      myservo.write(initial_position); // загрузка данных в сервомотор
    }
    else 
    {
      initial_position = initial_position + 5; // Поворота сервомотора
      myservo.write(initial_position); // загрузка данных в сервомотор
    }
  }

  // ina1
  Serial.println("ina1 ");
  // Читаем напряжение
  Serial.print(F("Voltage: "));
  Serial.print(ina1.getVoltage(), 3);
  Serial.println(F(" V"));

  // Читаем ток
  Serial.print(F("Current: "));
  Serial.print(ina1.getCurrent(), 3);
  Serial.println(F(" A"));

  // Читаем мощность
  Serial.print(F("Power: "));
  Serial.print(ina1.getPower(), 3);
  Serial.println(F(" W"));

  // Читаем напряжение на шунте
  Serial.print(F("Shunt voltage: "));
  Serial.print(ina1.getShuntVoltage(), 6);
  Serial.println(F(" V"));

  Serial.println("");


  // ina2
  Serial.println("ina2 ");
  // Читаем напряжение
  Serial.print(F("Voltage: "));
  Serial.print(ina2.getVoltage(), 3);
  Serial.println(F(" V"));

  // Читаем ток
  Serial.print(F("Current: "));
  Serial.print(ina2.getCurrent(), 3);
  Serial.println(F(" A"));

  // Читаем мощность
  Serial.print(F("Power: "));
  Serial.print(ina2.getPower(), 3);
  Serial.println(F(" W"));

  // Читаем напряжение на шунте
  Serial.print(F("Shunt voltage: "));
  Serial.print(ina2.getShuntVoltage(), 6);
  Serial.println(F(" V"));

  Serial.println("");
}