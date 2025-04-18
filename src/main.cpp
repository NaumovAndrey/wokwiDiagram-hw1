#include <Arduino.h>
#include <string>
#include <vector>

/*инициализация переменных*/
const int led_pin{39};
const int button_pin{17};
short numbersCliks{0};
const short max_numbers_cliks{2};
int power{255};
std::string buffer = "";

bool lastButtonState = LOW; //состояние кнопки
bool currentButtonState = LOW; //состояние кнопки
bool light_on{false}; //состояние светодиода

std::vector<int> commands;
const size_t maxCommands{4};
unsigned long currentTime{0};
unsigned long  printInterval{20000};

void const timerSleep(int command)
{
    if(commands.size() >= maxCommands)
    {
        commands.erase(commands.begin());
    }
    commands.push_back(command);
}


void setup() 
{
    pinMode(led_pin, OUTPUT);
    Serial.begin(115200);
}

void loop() 
{
    if(Serial.available() > 0)
    {
        buffer = "";
        while (Serial.available() > 0)
        {
            buffer += std::string(1, char(Serial.read()));
            delay(500);
        }

        power = std::stoi(buffer);
        timerSleep(power);

        Serial.print("Светодиод: ");
        Serial.println(power);
    }

    currentButtonState = digitalRead(button_pin);

    /*проверка нажатия кнопки, без проверки неработает*/
    if(currentButtonState != lastButtonState)
    {
        delay(50);

        if(currentButtonState == HIGH && !light_on)
        {
            numbersCliks++;

            if(numbersCliks >= max_numbers_cliks)
            {
                analogWrite(led_pin, power);
                light_on = true;
                delay(1000);
                analogWrite(led_pin, 0);
                numbersCliks = 0;
                light_on = false;
            }
        }
    }
    lastButtonState = currentButtonState;

    if (millis() - currentTime >= printInterval)
    {
        Serial.println("Последние команды:");
        for (int cmd : commands)
        {
            Serial.println(cmd);
        }
        currentTime = millis();
    }
}