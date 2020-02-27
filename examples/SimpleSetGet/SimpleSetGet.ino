/*
  DS3231_set.pde byEric Ayars on 4/11

  Modified the "set" sketch to enable setting the time if sent via Serial Monitor,
  otherwise display the time in two optional formats: simplified string for comparing
  or full time from components.

  To set time type without quotes "HHMMDDwHHMMSSx" into terminal and hit enter

  ex. 200226w203100x

*/

#include <DS3231.h>
#include <Wire.h>

DS3231 Clock;

byte Year;
byte Month;
byte Date;
byte DoW;
byte Hour;
byte Minute;
byte Second;

bool Century = false;
bool h12;
bool PM;
byte ADay, AHour, AMinute, ASecond, ABits;
bool ADy, A12h, Apm;

bool shortString = true;

void GetDateStuff(byte& Year, byte& Month, byte& Day, byte& DoW,
                  byte& Hour, byte& Minute, byte& Second) {
  // Call this if you notice something coming in on
  // the serial port. The stuff coming in should be in
  // the order YYMMDDwHHMMSS, with an 'x' at the end.
  boolean GotString = false;
  char InChar;
  byte Temp1, Temp2;
  char InString[20];

  byte j = 0;
  while (!GotString) {
    if (Serial.available()) {
      InChar = Serial.read();
      InString[j] = InChar;
      j += 1;
      if (InChar == 'x') {
        GotString = true;
      }
    }
  }
  Serial.println(InString);
  // Read Year first
  Temp1 = (byte)InString[0] - 48;
  Temp2 = (byte)InString[1] - 48;
  Year = Temp1 * 10 + Temp2;
  // now month
  Temp1 = (byte)InString[2] - 48;
  Temp2 = (byte)InString[3] - 48;
  Month = Temp1 * 10 + Temp2;
  // now date
  Temp1 = (byte)InString[4] - 48;
  Temp2 = (byte)InString[5] - 48;
  Day = Temp1 * 10 + Temp2;
  // now Day of Week
  DoW = (byte)InString[6] - 48;
  // now Hour
  Temp1 = (byte)InString[7] - 48;
  Temp2 = (byte)InString[8] - 48;
  Hour = Temp1 * 10 + Temp2;
  // now Minute
  Temp1 = (byte)InString[9] - 48;
  Temp2 = (byte)InString[10] - 48;
  Minute = Temp1 * 10 + Temp2;
  // now Second
  Temp1 = (byte)InString[11] - 48;
  Temp2 = (byte)InString[12] - 48;
  Second = Temp1 * 10 + Temp2;
}

void setup() {
  // Start the serial port
  Serial.begin(9600);

  // Start the I2C interface
  Wire.begin();
}

void loop() {

  // If something is coming in on the serial line, it's
  // a time correction so set the clock accordingly.
  if (Serial.available()) {
    GetDateStuff(Year, Month, Date, DoW, Hour, Minute, Second);

    Clock.setClockMode(false);	// set to 24h

    Clock.setYear(Year);
    Clock.setMonth(Month);
    Clock.setDate(Date);
    Clock.setDoW(DoW);
    Clock.setHour(Hour);
    Clock.setMinute(Minute);
    Clock.setSecond(Second);
  }

  else if (shortString) {
    String hour =  String(Clock.getHour(h12, PM), DEC);
    String minute =  String(Clock.getMinute(), DEC);
    String time = hour + minute;
    Serial.println(time);
  }

  else {
    // send what's going on to the serial monitor.
    // Start with the year
    Serial.print("2");
    if (Century) {      // Won't need this for 89 years.
      Serial.print("1");
    } else {
      Serial.print("0");
    }
    Serial.print(Clock.getYear(), DEC);
    Serial.print(' ');
    // then the month
    Serial.print(Clock.getMonth(Century), DEC);
    Serial.print(' ');
    // then the date
    Serial.print(Clock.getDate(), DEC);
    Serial.print(' ');
    // and the day of the week
    Serial.print(Clock.getDoW(), DEC);
    Serial.print(' ');
    // Finally the hour, minute, and second
    Serial.print(Clock.getHour(h12, PM), DEC);
    Serial.print(' ');
    Serial.print(Clock.getMinute(), DEC);
    Serial.print(' ');
    Serial.print(Clock.getSecond(), DEC);
    
    // Add AM/PM indicator
    if (h12) {
      if (PM) {
        Serial.println(" PM ");
      } else {
        Serial.println(" AM ");
      }
    } else {
      Serial.println(" 24h ");
    }
  }
  delay(1000);
}
