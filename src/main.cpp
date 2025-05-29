#include <Arduino.h>
#include <string>
#include <vector>

/* Пины */
const int led_pin_1{2};    // Синий светодиод
const int led_pin_2{39};   // Красный светодиод
const int button_pin_1{16};
const int button_pin_2{18};

/* Глобальные переменные */
short numbersCliks{0}; // счётчик нажатий
const short max_numbers_cliks{1}; // сколько нажатий нужно для срабатывания
std::string buffer = "";

bool lastButtonState = LOW; // предыдущее состояние кнопки
bool currentButtonState = LOW; // текущее состояние кнопки
bool light_on{false}; // светодиод выключен

std::vector<int> commands; // история команд
const size_t maxCommands{4};
unsigned long currentTime{0};
unsigned long printInterval{20000};

/* Функции */
int getPower();
void ledControl(int power, int ledPin, int buttonPin); // управление светодиодом
void timerSleep(int command);

void timerSleep(int command)
{
    if (commands.size() >= maxCommands)
    {
        commands.erase(commands.begin());
    }
    commands.push_back(command);
}

void setup()
{
    pinMode(led_pin_1, OUTPUT);
    pinMode(led_pin_2, OUTPUT);
    pinMode(button_pin_1, INPUT);
    pinMode(button_pin_2, INPUT);
    Serial.begin(115200);

    // Настройка ШИМ для ESP32
    ledcSetup(0, 5000, 8); // канал 0 — пин 2
    ledcAttachPin(led_pin_1, 0);

    ledcSetup(1, 5000, 8); // канал 1 — пин 39
    ledcAttachPin(led_pin_2, 1);
}

void loop()
{
    int power{getPower()};
    ledControl(power, led_pin_1, button_pin_1);
    ledControl(power, led_pin_2, button_pin_2);

    
    if (millis() - currentTime >= printInterval)
    {
        String output = "Последние команды: ";
        bool firstCommand = true;

        for (int cmd : commands)
        {
            if (!firstCommand)
            {
                output += ", ";
            }
            else
            {
                firstCommand = false;
            }
            output += String(cmd);
        }

        Serial.println(output);
        currentTime = millis();
    }
    
}

int getPower()
{
    static int power{255};

    if (Serial.available() > 0)
    {
        buffer = "";
        while (Serial.available() > 0)
        {
            buffer += std::string(1, char(Serial.read()));
            delay(500);
        }

        try {
            power = std::stoi(buffer);
            if (power < 0) power = 0;
            if (power > 255) power = 255;
            timerSleep(power);
            Serial.print("Светодиод: ");
            Serial.println(power);
        } catch (...) {
            Serial.println("Ошибка преобразования числа.");
        }
    }

    return power;
}

void ledControl(int power, int ledPin, int buttonPin)
{
    currentButtonState = digitalRead(buttonPin);

    if (currentButtonState != lastButtonState)
    {
        delay(50);  // антидребезг
        currentButtonState = digitalRead(buttonPin);

        if (currentButtonState == HIGH && !light_on)
        {
            numbersCliks++;

            if (numbersCliks >= max_numbers_cliks)
            {
                int channel = (ledPin == led_pin_1) ? 0 : 1;
                ledcWrite(channel, power);  // включение

                Serial.print("Свечение светодиода: PIN=");
                Serial.print(ledPin);
                Serial.print(", Мощность=");
                Serial.println(power);

                delay(1000);
                ledcWrite(channel, 0);      // выключение
                numbersCliks = 0;
                light_on = false;
            }
        }
    }
    lastButtonState = currentButtonState;
}
