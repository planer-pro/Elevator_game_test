#include <Arduino.h>
#include <UNOR4WMatrixGFX.h>
#include <list.cpp>

Array<int> ls;

UNOR4WMatrixGFX display;

int fl = 0, flSet = 0, flLastFlag = 0;
uint32_t tm = 0;
uint8_t cnt = 0;

void startPreparation();
void checkLiftReqHeadler();
void requestManager();
void mooveLiftHeadler();
bool inRange(int value, int min, int max);
void serialIndicate();
void drawPixel(uint8_t x, uint8_t y, bool on);

void setup()
{
    Serial.begin(115200);
    Serial.setTimeout(150);

    display.begin();

    startPreparation();
}

void loop()
{
    checkLiftReqHeadler();
    requestManager();
    mooveLiftHeadler();
}

void startPreparation()
{
    Serial.println("--------------------");
    Serial.print("You on the floor: ");
    Serial.println(fl);
    Serial.println("--------------------");

    drawPixel(fl, 0, true);
}

void checkLiftReqHeadler()
{
    if (Serial.available())
    {
        int date = Serial.parseInt();
        bool success = inRange(date, 0, 11);

        if (success)
        {
            Serial.println("-------");
            Serial.print("Set: ");
            Serial.println(date);
            Serial.println("-------");

            drawPixel(date, 2, true); // show requested floor on display
            ls.addElement(date);

            Serial.print("Floors: ");

            for (size_t i = 0; i < ls.getElementCount(); i++)
            {
                Serial.print(ls.getElementByIdx(i));

                if (i != ls.getElementCount() - 1)
                    Serial.print(", ");
            }

            Serial.print(" (" + String(ls.getElementCount()) + ")");
            Serial.println();
        }
        else
        {
            Serial.println("Error input");
        }
    }
}

void requestManager()
{
    if (ls.getElementCount() > 0) // if is any requests
    {
        if (flLastFlag == 0) // if is finised lifting current floor
            flSet = ls.getElementByIdx(0);
    }
}

void mooveLiftHeadler()
{

    if (millis() - tm > 500) // elevatror speed
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
                ls.removeElement(0);

                serialIndicate();

                flLastFlag = 0;
            }
            else
            {
                drawPixel(fl, 2, false); // block lift request, if him on the same floor

                for (size_t i = 0; i <= ls.getElementCount(); i++) // cleal wrong list
                {
                    ls.removeElement(i);
                }
            }
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