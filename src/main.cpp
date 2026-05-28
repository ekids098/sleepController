#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>
#include <SPI.h>
#include <FS.h>
#include <SD.h>
#include <Preferences.h>

#define SLEEP_BUTTON 2
#define WAKE_BUTTON 4
#define SD_CS_PIN 5

RTC_DS3231 rtc;
Preferences prefs;

int lastSleepState = 0;
int lastWakeState = 0;

uint32_t sleepTimeStamp = 0;
bool hasSlept = false;

void logToSDCard(const char *data)
{
    File file = SD.open("/sleep_log.csv", FILE_APPEND);
    if (!file)
    {
        Serial.println("SD Error");
        return;
    }
    file.println(data);
    file.close();
    Serial.println("Saved to SD");
}

void setup()
{
    Serial.begin(115200);
    pinMode(SLEEP_BUTTON, INPUT_PULLDOWN);
    pinMode(WAKE_BUTTON, INPUT_PULLDOWN);

    prefs.begin("sleep_data", false);
    hasSlept = prefs.getBool("hasSlept", false);
    sleepTimeStamp = prefs.getUInt("sleepTime", 0);

    if (!rtc.begin())
    {
        Serial.println("RTC Error");
        while (1)
            delay(10);
    }

    if (rtc.lostPower())
    {
        Serial.println("Warning: RTC lost power!");
    }

    if (!SD.begin(SD_CS_PIN))
    {
        Serial.println("No SD Card");
    }
    else
    {
        if (!SD.exists("/sleep_log.csv"))
        {
            File file = SD.open("/sleep_log.csv", FILE_WRITE);
            if (file)
            {
                file.println("Sleep_Time,Wake_Time,Duration_Hours");
                file.close();
            }
        }
    }
    Serial.println("System Ready");
}

void loop()
{
    int sleepState = digitalRead(SLEEP_BUTTON);
    int wakeState = digitalRead(WAKE_BUTTON);

    if (sleepState == 1 && lastSleepState == 0)
    {
        DateTime sleepTime = rtc.now();
        sleepTimeStamp = sleepTime.unixtime();
        hasSlept = true;

        prefs.putBool("hasSlept", hasSlept);
        prefs.putUInt("sleepTime", sleepTimeStamp);

        Serial.printf("Sleep: %04d/%02d/%02d %02d:%02d\n",
                      sleepTime.year(), sleepTime.month(), sleepTime.day(),
                      sleepTime.hour(), sleepTime.minute());

        delay(50);
    }

    if (wakeState == 1 && lastWakeState == 0)
    {
        if (!hasSlept)
        {
            Serial.println("Error: No Sleep Record");
        }
        else
        {
            DateTime wakeTime = rtc.now();
            DateTime sleepTimeObj = DateTime(sleepTimeStamp);

            TimeSpan sleepDuration = wakeTime - sleepTimeObj;
            long totalSeconds = sleepDuration.totalseconds();

            int hours = totalSeconds / 3600;
            int minutes = (totalSeconds % 3600) / 60;

            double excelHours = totalSeconds / 3600.0;

            Serial.printf("Wake: %04d/%02d/%02d %02d:%02d\n",
                          wakeTime.year(), wakeTime.month(), wakeTime.day(),
                          wakeTime.hour(), wakeTime.minute());
            Serial.printf("Duration: %d Hours %d Minutes\n", hours, minutes);

            char logBuffer[100];
            snprintf(logBuffer, sizeof(logBuffer), "%04d/%02d/%02d %02d:%02d,%04d/%02d/%02d %02d:%02d,%.2f",
                    sleepTimeObj.year(), sleepTimeObj.month(), sleepTimeObj.day(), sleepTimeObj.hour(), sleepTimeObj.minute(),
                    wakeTime.year(), wakeTime.month(), wakeTime.day(), wakeTime.hour(), wakeTime.minute(),
                    excelHours);

            logToSDCard(logBuffer);

            hasSlept = false;
            sleepTimeStamp = 0;
            prefs.putBool("hasSlept", hasSlept);
            prefs.putUInt("sleepTime", sleepTimeStamp);
        }
        delay(50);
    }

    lastSleepState = sleepState;
    lastWakeState = wakeState;
    delay(20);
}
