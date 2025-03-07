#include <Arduino.h>
#include <string>

/*инициализация переменных*/
const int led_pin{39};
short numbersCliks{0};
const short max_numbers_cliks{1};
int power{255};
std::string buffer = "";

bool lastButtonState = LOW;

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

        Serial.print("Светодиод: ");
        Serial.println(power);
    }

    bool currentButtonState = digitalRead(17);

    /*проверка нажатия кнопки, без проверки неработает*/
    if(currentButtonState != lastButtonState)
    {
        delay(50);
        if(currentButtonState == HIGH)
        {
            numbersCliks++;
            if(numbersCliks >= max_numbers_cliks)
            {
                analogWrite(led_pin, power);
                delay(1000);
                analogWrite(led_pin, 0);
                numbersCliks = 0;
            }
        }
    }
    lastButtonState = currentButtonState;
}