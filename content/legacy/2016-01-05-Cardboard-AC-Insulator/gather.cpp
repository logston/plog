#include <Arduino.h>
// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SD.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2
#define TEMPERATURE_PRECISION 9

// Prototypes
void printAddress(DeviceAddress);

// Setup a oneWire instance to communicate with any 
// OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// arrays to hold device addresses
//DeviceAddress insideThermometer, outsideThermometer;
DeviceAddress devices[10];

int device_count;

File myFile;

char *TEMPERATURES_FILE = "temps.txt";

void setup(void)
{
  // start serial port
  Serial.begin(9600);
  Serial.println();
  Serial.println("# Dallas Temperature IC Control Library Demo");

  // Start up the library
  sensors.begin();

  // locate devices on the bus
  Serial.print("# Locating devices...");
  Serial.print("Found ");
  device_count = sensors.getDeviceCount();
  Serial.print(device_count, DEC);
  Serial.println(" devices.");

  // report parasite power requirements
  Serial.print("# Parasite power is: "); 
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");

  // Search for devices on the bus and assign based on an index. Ideally,
  // you would do this to initially discover addresses on the bus and then 
  // use those addresses and manually assign them (see above) once you know 
  // the devices on your bus (and assuming they don't change).
  // 
  // method 1: by index
  for (int i = 0; i < device_count; i++) {
      if (!sensors.getAddress(devices[i], i)) {
          Serial.print("# Unable to find address for Device ");
	  Serial.print(i, DEC);
	  Serial.println();
      }
  }

  // show the addresses we found on the bus
  for (int i = 0; i < device_count; i++) {
      Serial.print("# Device ");
      Serial.print(i, DEC);
      Serial.print(" Address: ");
      printAddress(devices[i]);
      Serial.println();
  }

  // set the resolution to 9 bit per device
  for (int i = 0; i < device_count; i++) {
      sensors.setResolution(devices[i], TEMPERATURE_PRECISION);
  }

  for (int i = 0; i < device_count; i++) {
      Serial.print("# Device ");
      Serial.print(i, DEC);
      Serial.print(" Resolution: ");
      Serial.print(sensors.getResolution(devices[i]), DEC); 
      Serial.println();
  }

  Serial.print("# Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
   pinMode(10, OUTPUT);

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  myFile = SD.open(TEMPERATURES_FILE, FILE_WRITE);
  if (!myFile) {
    Serial.println("# Unable to open card for writing");
    return;
  } else {
    myFile.println("# Restarting ...");
    myFile.close();
  }
}


// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}


// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print("Temp C: ");
  Serial.print(tempC);
  Serial.print(" Temp F: ");
  Serial.print(DallasTemperature::toFahrenheit(tempC));
}


// function to print a device's resolution
void printResolution(DeviceAddress deviceAddress)
{
  Serial.print("Resolution: ");
  Serial.print(sensors.getResolution(deviceAddress));
  Serial.println();
}


// main function to print information about a device
void printData(DeviceAddress deviceAddress)
{
  Serial.print("Device Address: ");
  printAddress(deviceAddress);
  Serial.print(" ");
  printTemperature(deviceAddress);
  Serial.println();
}

void storeData(DeviceAddress deviceAddress)
{
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open(TEMPERATURES_FILE, FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.println("# Writing data to disk");

    for (uint8_t i = 0; i < 8; i++)
    {
      // zero pad the address if necessary
      if (deviceAddress[i] < 16) myFile.print("0");
      myFile.print(deviceAddress[i], HEX);
    }

    myFile.print(",");

    float tempC = sensors.getTempC(deviceAddress);

    myFile.print(tempC);
    myFile.println();
    // close the file:
    myFile.close();
  } else {
    Serial.println("# Unable to write data to disk");
  }
}


/*
 * Main function, calls the temperatures in a loop.
 */
void loop(void)
{ 
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("# Requesting temperatures...");
  sensors.requestTemperatures();
  Serial.println("DONE");

  // print the device information
  for (int i = 0; i < device_count; i++) {
      printData(devices[i]);
      storeData(devices[i]);
  }
  delay(5000);
}

