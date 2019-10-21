/*
  SD card read/write

  This example shows how to read and write data to and from an SD card file
  The circuit:
   SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

  created   Nov 2010
  by David A. Mellis
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

*/

#include <Wire.h>
#include "Sodaq_DS3231.h"
#include <SPI.h>
#include <SD.h>

File myFile;

char weekDay[][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

//year, month, date, hour, min, sec and week-day(starts from 0 and goes to 6)
//writing any non-existent time-data may interfere with normal operation of the RTC.
//Take care of week-day also.
DateTime dt(2011, 11, 10, 15, 18, 0, 5);

const int pinObstaculo = 7;
int picudosCaidos = 0;

bool hayObstaculo = HIGH;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(57600);

  pinMode(hayObstaculo, INPUT); //Se leen los datos declarando el pin como entrada

  Wire.begin();
  rtc.begin();
  rtc.setDateTime(dt); //Adjust date-time as defined 'dt' above

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE); //esto tambien

  // if the file opened okay, write to it:
  if (myFile) {   // lo unico que me sirve
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

void loop() {
  DateTime now = rtc.now(); //get the current date-time
  hayObstaculo = digitalRead(pinObstaculo);
  //Serial.println(hayObstaculo); 0 = Obstaculo detectado 1 = Despejado
  if (hayObstaculo == LOW) {
    Serial.println("Obstaculo Detectado");
    picudosCaidos++;
    Serial.println(picudosCaidos);
    myFile.println(picudosCaidos);
    myFile.print(now.year(), DEC);
    myFile.print('/');
    myFile.print(now.month(), DEC);
    myFile.print('/');
    myFile.print(now.date(), DEC);
    myFile.print(' ');
    myFile.print(now.hour(), DEC);
    myFile.print(':');
    myFile.print(now.minute(), DEC);
    myFile.print(':');
    myFile.print(now.second(), DEC);
    myFile.println();
    myFile.print(weekDay[now.dayOfWeek()]);
    myFile.println();
    myFile.close();
  }
}
