#include "virtuabotixRTC.h"   
#include <SPI.h>
#include <SD.h>

#define LED 3
#define LM35 A4 
#define RTC_RST 6 // CS
#define RTC_DAT 7 // MISO
#define RTC_CLK 8 // SCK
#define interval 5UL // minutes (UL is needed for compiler to make it unsigned long type)

File SDCardFile; // a file into which data will be written 
virtuabotixRTC RTC( RTC_CLK, RTC_DAT, RTC_RST); // a RTC 
unsigned long timestamp = 0; // a variable to set a timestamp to measure a time between temperature measuments
unsigned long timeNow; // a variable to measure a current time
int fileNum = 0; // file number
char filename[] = "Tem000.csv"; // first file name
bool seconds_OnOff_flag = false; // flag for turning on (true) or off {false} saving seconds

void setup(){
pinMode(LED, OUTPUT);
  
  // UART
  //Serial.begin(9600);

   //Set a current time to the RTC [seconds, minutes, hours, dayofweek, dayofmonth, month, year]
   //                               (ss, mm, hh, w, dd, mm, yyyy) w - day of week 
   //RTC.setDS1302Time(00, 8, 21, 2, 12, 10, 2021);

delay(200);
  // Initialize an SD card
  SD_init();

  // Create file and make headers
  CreateFile();

  // Initial data save  
  CollectData();
}


void loop(){
//digitalWrite(LED, HIGH);
  // Check the time
  timeNow = millis();
  
  if(timeNow - timestamp >= interval * 60 * 1000)
  {
    // Save the new timestamp
    timestamp = timeNow;

    // Save data
    CollectData();
  }
}

void CollectData()
{
  // Update user RTC time variables
  RTC.updateTime();
   
  // Measure the temperature
  float temp = ((analogRead(LM35) * 5.0) / 1024.0) * 100;

  // Save data on SD card
  SaveDataOnSDCard(RTC, temp);

  // Send info to UART
  //Info_UART(RTC, temp);
}

void SD_init()
{
  if (!SD.begin(10)) {
    //Serial.println("SD card initialization failed!");
    error(500);
  }
  else
    blinkLEDStart();
}

void CreateFile()
{
  // Search for non existing filename
  while(SD.exists(filename))
  {
    if(fileNum >= 1000)
    {
      //digitalWrite(LED, HIGH);
      //while(1){}
      sprintf(filename, "TemRES.csv", fileNum);
      break;
    }
    
    //Serial.print("File exists: ");
    //Serial.println(filename);
    fileNum++;
    sprintf(filename, "Tem%03d.csv", fileNum);
    //sprintf(filename, "Tem0.csv");
  }
  // Info to UART
  //Serial.print("File name: ");
  //Serial.println(filename);

  // Open the file to write headers
  SDCardFile = SD.open(filename, FILE_WRITE);

  // Save data if the file is opened
  if(SDCardFile)
  {
    SDCardFile.print("Year");
    SDCardFile.print(",");
    SDCardFile.print("Month");
    SDCardFile.print(",");
    SDCardFile.print("Day");
    SDCardFile.print(",");
    SDCardFile.print("Hour");
    SDCardFile.print(",");
    SDCardFile.print("Minute");
    SDCardFile.print(",");
    if(seconds_OnOff_flag)
    {
      SDCardFile.print("Seconds");
      SDCardFile.print(",");
    }
    SDCardFile.println("Temperature");
    
    // Close the file
    SDCardFile.close();

    blinkLED();
  }
  else
  {
    //Serial.print("Failed to open file: ");
    //Serial.println(filename);
    error(35);
  }
}

// Save data on SD card
void SaveDataOnSDCard(virtuabotixRTC RTC, float temp)
{
  // Open the file
  SDCardFile = SD.open(filename, FILE_WRITE);

  // Save data if the file is opened
  if(SDCardFile)
  {
    SDCardFile.print(RTC.year);
    SDCardFile.print(",");
    SDCardFile.print(RTC.month);
    SDCardFile.print(",");
    SDCardFile.print(RTC.dayofmonth);
    SDCardFile.print(",");
    SDCardFile.print(RTC.hours);
    SDCardFile.print(",");
    SDCardFile.print(RTC.minutes);
    SDCardFile.print(",");
    if(seconds_OnOff_flag)
    {
      SDCardFile.print(RTC.seconds);
      SDCardFile.print(",");
    }
    SDCardFile.println(temp);
    
    // Close the file
    SDCardFile.close();

    blinkLED();
  }
  else
  {
    //Serial.print("Failed to open file: ");
    //Serial.println(filename);
    //error(100);
  }
  
}

// Send info to UART
/*void Info_UART(virtuabotixRTC RTC, float temp)
{
  Serial.print(RTC.year);
  Serial.print(",");
  Serial.print(RTC.month);
  Serial.print(",");
  Serial.print(RTC.dayofmonth);
  Serial.print(",");
  Serial.print(RTC.hours);
  Serial.print(",");
  Serial.print(RTC.minutes);
  Serial.print(",");
  Serial.print(RTC.seconds);
  Serial.print(",");
  Serial.println(temp);
}*/

void blinkLED()
{
  digitalWrite(LED, HIGH);
  delay(200);
  digitalWrite(LED, LOW);

}

void blinkLEDStart()
{
  for(int i =0; i <=10; i++){
  digitalWrite(LED, HIGH);
  delay(100);
  digitalWrite(LED, LOW);
  delay(100);
}

}

void error(int t)
{
    while(1)
    {
      digitalWrite(LED, HIGH);
      delay(t);
      digitalWrite(LED, LOW);
      delay(t);
    }
}
