#include <Arduino.h>

/* Инициализация переменных */
const int led_pin = 39;
const int button_pin = 17;
const int pwmChannel = 0;   // PWM-канал
const int pwmFreq = 5000;   // Частота ШИМ
const int pwmResolution = 8; // Разрешение 8 бит (0-255)

bool lastButtonState = LOW;
short numbersClicks = 0;
const short maxNumbersClicks = 3;

int power = 255;
String buffer = "";

void setup() 
{
    pinMode(led_pin, OUTPUT);
    pinMode(button_pin, INPUT_PULLUP);
    Serial.begin(115200);

    // Настройка PWM
    ledcSetup(pwmChannel, pwmFreq, pwmResolution);
    ledcAttachPin(led_pin, pwmChannel);
}

void loop() 
{
    if (Serial.available() > 0)
    {
        buffer = "";
        while (Serial.available() > 0)
        {
            char c = Serial.read();
            if (isDigit(c) || c == '\n') // Фильтрация символов
                buffer += c;
            delay(50);
        }

        if (buffer.length() > 0)
        {
            power = buffer.toInt();
            Serial.print("Received power: ");
            Serial.println(power);
        }
    }

    bool currentButtonState = digitalRead(button_pin);

    /* Проверка нажатия кнопки */
    if (currentButtonState != lastButtonState)
    {
        delay(50); // Антидребезг
        if (currentButtonState == LOW) // Кнопка нажата (если PULLUP)
        {
            numbersClicks++;
            Serial.print("Click count: ");
            Serial.println(numbersClicks);

            if (numbersClicks >= maxNumbersClicks)
            {
                Serial.println("LED ON");
                ledcWrite(pwmChannel, power);
                delay(1000);
                Serial.println("LED OFF");
                ledcWrite(pwmChannel, 0);
                numbersClicks = 0;
            }
        }
    }
    lastButtonState = currentButtonState;
}