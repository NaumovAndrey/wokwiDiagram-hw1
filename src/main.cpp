#include <Arduino.h>

/*инициализация переменных*/
const int led_pin{39};
bool light_on{false};
short numbersCliks{0};
const short max_numbers_cliks{3};

bool lastButtonState = LOW;

void setup() 
{
    pinMode(led_pin, OUTPUT);
}

void loop() 
{
    
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
                digitalWrite(led_pin, HIGH);
                delay(1000);
                digitalWrite(led_pin, LOW);
                numbersCliks = 0;
            }
        }
    }
    lastButtonState = currentButtonState;
}