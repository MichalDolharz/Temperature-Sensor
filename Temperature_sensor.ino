#include "virtuabotixRTC.h"   
#include <SPI.h>
#include <SD.h>

#define LM35 A4 
#define interval 1UL // minutes (UL is needed for compiler to make it unsigned long type)

File SDCardFile; // a file into which data will be written 
virtuabotixRTC RTC(6, 7, 8); // a RTC 
unsigned long timestamp = 0; // a variable to set a timestamp to measure a time between temperature measuments
unsigned long timeNow; // a variable to measure a current time
int fileNum = 0;
char filename[] = "Temp00.csv";

void setup(){
  // UART
  Serial.begin(9600);

  // Set a current time to the RTC [seconds, minutes, hours, dayofweek, dayofmonth, month, year]
  //               (ss, mm, hh, w, dd, mm, yyyy) w - day of week 
  RTC.setDS1302Time(00, 22, 21, 3, 21, 07, 2021);

  // Initialize an SD card
  SD_init();

  // Create file and make headers
  CreateFile();

  // Initial data save  
  CollectData();
}


void loop(){

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
  Info_UART(RTC, temp);
}

void SD_init()
{
  if (!SD.begin(10)) {
    Serial.println("SD card initialization failed!");
    while (1);
  }
}

void CreateFile()
{
  // Search for non existing filename
  while(SD.exists(filename))
  {
    Serial.print("File exists: ");
    Serial.println(filename);
    fileNum++;
    sprintf(filename, "Temp%02d.csv", fileNum);
  }
  
  // Info to UART
  Serial.print("File name: ");
  Serial.println(filename);

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
    SDCardFile.println("Temperature");
    
    // Close the file
    SDCardFile.close();
  }
  else
  {
    Serial.print("Failed to open file: ");
    Serial.println(filename);
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
    SDCardFile.println(temp);
    
    // Close the file
    SDCardFile.close();
  }
  else
  {
    Serial.print("Failed to open file: ");
    Serial.println(filename);
  }
  
}

// Send info to UART
void Info_UART(virtuabotixRTC RTC, float temp)
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
  Serial.println(temp);
}
