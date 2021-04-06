#include <DallasTemperature.h>
#include <DS3231.h>
#include <OneWire.h>
#include <SPI.h>
#include <SD.h>

#define SensorABus 2
#define SensorBBus 4
#define SensorCBus 7
#define SensorDBus 8

RTClib rtc;

OneWire sensorAWire(SensorABus);
OneWire sensorBWire(SensorBBus);
OneWire sensorCWire(SensorCBus);
OneWire sensorDWire(SensorDBus);

DallasTemperature sensorA(&sensorAWire);
DallasTemperature sensorB(&sensorBWire);
DallasTemperature sensorC(&sensorCWire);
DallasTemperature sensorD(&sensorDWire);


void setup() {
  Wire.begin();
  
  // Start the sensors
  sensorA.begin();
  sensorB.begin();
  sensorC.begin();
  sensorD.begin();
  
  Serial.begin(9600);
  while (!Serial)
  {
    // Wait for the serial port to connect
  }
  // Write a message to the SD card, since setup runs every time the system is powered up
  // stamp in a system restart message
  pinMode(10, OUTPUT);
  SD.begin(10);
  File TemperatureLog = SD.open("TData.txt", FILE_WRITE);
  if(TemperatureLog){
    Serial.println("System Start");
    TemperatureLog.println("System Start");
    TemperatureLog.close();
  }
  else{
    Serial.println("Error opening file");
  }
}

void loop() {
  // Create a datetime object to provide date time stamps
  DateTime now = rtc.now();
  
  sensorA.requestTemperatures();
  sensorB.requestTemperatures();
  sensorC.requestTemperatures();
  sensorD.requestTemperatures();
  
  // Get the temperatures
  float sensorATempC = sensorA.getTempCByIndex(0);
  float sensorBTempC = sensorB.getTempCByIndex(0);
  float sensorCTempC = sensorC.getTempCByIndex(0);
  float sensorDTempC = sensorD.getTempCByIndex(0);
  
  File TemperatureLog = SD.open("TData.txt", FILE_WRITE);
  if(TemperatureLog){
  TemperatureLog.println(String(now.year()) + "-" + String(now.month()) + "-" + String(now.day())+ "," +  String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second()) + ","+ String(sensorATempC)+ ","+ String(sensorBTempC)+ ","+ String(sensorCTempC)+ ","+ String(sensorDTempC));
  TemperatureLog.close();
  }
  else {
    Serial.println("Error opening file while recording");
  }
delay(60000);
}
