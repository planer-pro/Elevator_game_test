#include <Arduino.h>
#include <UNOR4WMatrixGFX.h>

UNOR4WMatrixGFX display;

int fl = 0, flSet = 0, flLastFlag = 0;
uint32_t tm = 0;
uint8_t cnt = 0;

void drawPixel(uint8_t x, uint8_t y, bool on);
void serialIndicate();
void mooveHeadler();
bool inRange(int value, int min, int max);
void reqLiftKeyBtnHeadler();

void setup()
{
    Serial.begin(115200);

    display.begin();

    Serial.println("--------------------");
    Serial.print("You on the floor: ");
    Serial.println(fl);
    Serial.println("--------------------");

    drawPixel(fl, 0, true);
}

void loop()
{
    reqLiftKeyBtnHeadler();
    mooveHeadler();
}

void reqLiftKeyBtnHeadler()
{
    if (Serial.available())
    {
        flSet = Serial.parseInt();
        bool success = inRange(flSet, 0, 11);

        if (success)
        {
            Serial.println("-------");
            Serial.print("Set: ");
            Serial.println(flSet);
            Serial.println("-------");

            drawPixel(flSet, 2, true);
        }
        else
        {
            Serial.println("Error input");

            flSet = fl;
        }
    }
}

void mooveHeadler()
{
    if (millis() - tm > 250)
    {
        tm = millis();

        if (fl < flSet)
        {
            flLastFlag = -1;

            serialIndicate();

            drawPixel(fl, 0, true);

            if (fl != 0)
                drawPixel(fl - 1, 0, false);

            fl++;
        }
        else if (fl > flSet)
        {
            flLastFlag = 1;

            serialIndicate();

            drawPixel(fl, 0, true);

            if (fl != 12)
                drawPixel(fl + 1, 0, false);

            fl--;
        }
        else
        {
            if (flLastFlag != 0)
            {
                switch (flLastFlag)
                {
                case -1:
                    drawPixel(fl - 1, 0, false);
                    drawPixel(fl, 0, true);
                    break;
                case 1:
                    drawPixel(fl + 1, 0, false);
                    drawPixel(fl, 0, true);
                    break;
                }

                drawPixel(fl, 2, false);

                serialIndicate();

                // flSet = 0; // go to the zero floor
                flLastFlag = 0;
            }
            else
                drawPixel(fl, 2, false); // if request lift from same floor
        }
    }
}

bool inRange(int value, int min, int max)
{
    return (value >= min && value <= max);
}

void serialIndicate()
{
    Serial.print(fl);
    Serial.print(" -> ");
    Serial.println(flSet);
}

void drawPixel(uint8_t x, uint8_t y, bool on)
{
    display.drawPixel(x, y, on ? MATRIX_WHITE : MATRIX_BLACK);
    display.display();
}