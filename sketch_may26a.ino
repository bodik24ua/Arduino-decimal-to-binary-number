int randNumber; // для рандомного числа
String binaryNumber = ""; // для двійкового числа
String decimalNumber = ""; // для десяткового
int binary[8];  // Масив для зберігання двійкових бітів
String enteredNumber = ""; // для введеного користувачем номера
boolean firstLoop = true; // ініціалізація першого циклу
int buttonStateRed = HIGH; // ініт червової кнопки
int buttonStateBlack = HIGH; // ініт чорної кнопки
const int redPin = 10; // червоний світлодіод
const int greenPin = 11; // зелений світлодіод
const int bluePin = 12; // синій світлодіод
const int redBtn = 8; // червона кнопка
const int blackBtn = 9; // чорна кнопка

// підкл. бібліотек 
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//ініц. дисплея
LiquidCrystal_I2C LCD(0x27, 16, 2);



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // відлагодження
  LCD.begin();  // запуск дисплея
  LCD.backlight(); // увімкнення підсвітки дисплею
  randomSeed(analogRead(0)); // генер випадкового числа
  randNumber = random(99); // запамятовуваня випадкового числа макс до 99 у randNumber
  decimalNumber = randNumber; // збереж у string decimalNumber
  LCD.print("Number: "); // вивід на екрані  
  LCD.print(randNumber); // число на екрані

  LCD.setCursor(0, 1); // курсор переміщається до низу.
  delay(1000); 
  //LCD.print(".");

  while (randNumber > 0) { // щоб випадкове число було більше 0
    binaryNumber = String(randNumber % 2) + binaryNumber; // збереження бінарного числа
    randNumber /= 2; // ділення із залишком
  }

  // Виведення двійкового числа
  Serial.print(binaryNumber + "\n");
  Serial.print(randNumber + "\n");

  //ініціалізуємо світлодіоди
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  //ініціалізуємо нажаття кнопок
  pinMode(redBtn, INPUT);
  pinMode(blackBtn, INPUT);
  LCD.blink(); // увімкнення курсору на екранні
}

void loop() {
  // put your main code here, to run repeatedly:
  int currentButtonStateRed = digitalRead(redBtn); // поточний стан кнопок
  int currentButtonStateBlack = digitalRead(blackBtn);
  if (currentButtonStateRed == LOW && buttonStateRed == HIGH) {  // Якщо червона кнопка нажата
    if (!firstLoop) {
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, LOW);
      digitalWrite(bluePin, HIGH); // діє синій світлодіод
      LCD.print(1); // друк нажатої цифри 1
      enteredNumber += "1"; // збереження введеної цифри 1
      delay(500); // затримка
    }
  }
  if (currentButtonStateBlack == LOW && buttonStateBlack == HIGH) {  // Якщо чорна кнопка нажата
    if (!firstLoop) {
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, LOW);
      digitalWrite(bluePin, HIGH); // діє синій світлодіод
      LCD.print(0); // друк нажатої цифри 0
      enteredNumber += "0"; // збереження введеної цифри 1
      delay(500); // затримка
    }
  }
  if (enteredNumber == binaryNumber) { // Якщо введено вірно
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, HIGH); // Світіння зеленого світлодіода
    digitalWrite(bluePin, LOW);
    Serial.print("YES!");
    LCD.clear();
    LCD.print("You`re right!"); // виведення що вірна відповідь
    LCD.setCursor(0, 1); // зміщення курсора донизу
    LCD.print(enteredNumber + " = " + decimalNumber); // введене число відповідає десятковому
    delay(5000);
  }
  if (enteredNumber.length() >= binaryNumber.length() && enteredNumber != binaryNumber) { // Якщо введено невірно
    LCD.clear();
    LCD.print(enteredNumber + " Wrong!"); // виведення числа яке введене невірно
    LCD.setCursor(0, 1); // зміщення курсора донизу
    LCD.print(binaryNumber + " = " + decimalNumber); // яке бінарне число відповідає десятковому
    digitalWrite(redPin, HIGH); // Мигання червоного світлодіода
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, LOW);
    delay(500);
    digitalWrite(redPin, LOW);
    delay(500);
    digitalWrite(redPin, HIGH);
    delay(500);
    digitalWrite(redPin, LOW);
    delay(500);
    digitalWrite(redPin, HIGH);
    delay(500);
    digitalWrite(redPin, LOW);
    delay(500);
    digitalWrite(redPin, HIGH);
    delay(500);
    digitalWrite(redPin, LOW);
    delay(500);
  }
  buttonStateRed = currentButtonStateRed; // Збереження стану кнопки
  buttonStateBlack = currentButtonStateBlack;
  digitalWrite(redPin, LOW); // відключення світла у діода
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, LOW);
  firstLoop = false;
  if (buttonStateRed == HIGH && buttonStateBlack == HIGH) { // рестарт якщо нажаті дві кнопки
    LCD.clear();
    LCD.print("Restart ...");
    delay(2000);
    asm volatile("jmp 0x00");
  }
}
