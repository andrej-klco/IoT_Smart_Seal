#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <SigFox.h>
#include <ArduinoLowPower.h>

#define sampling_freq 20    // sensor sampling frequency for printouts in Hz
#define light_sensor_pin A1 // light sensor analog pin

/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

void sendString(String str) {

  if (str.length() > 12) {
    Serial.println("Message too long, only first 12 bytes will be sent");
  }

  SigFox.begin(); // Start the module
  delay(100); // Wait at least 30mS after first configuration (100mS before)
  SigFox.status(); // Clears all pending interrupts
  delay(1);

  SigFox.beginPacket();
  SigFox.print(str);
  int ret = SigFox.endPacket();  // send buffer to SIGFOX network

  if (ret > 0) {
    Serial.println("No transmission");
  } else {
    Serial.println("Transmission ok");
  }

  Serial.println(SigFox.status(SIGFOX));
  Serial.println(SigFox.status(ATMEL));
  SigFox.end();
}


void setup(void) 
{
  Serial.begin(9600);
  
  /* Initialise the sensor */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("No ADXL345 detected ... Check your wiring!");
    while(1);
  }

  /* Set the range to whatever is appropriate for your project */
  accel.setRange(ADXL345_RANGE_16_G);

  if (!SigFox.begin()) { // Initialise the sigfox module
    Serial.println("No Sigfox module detected!");
    while(1);
  }

  sendString("test"); // Send a "test" string through the Sigfox once on boot 
}

void loop(void) 
{
  /* Read accelerometer sensor event */ 
  sensors_event_t event; 
  accel.getEvent(&event);
 
  /* Print the accelerations (acceleration is measured in m/s^2) */
  Serial.print("X:"); Serial.print(event.acceleration.x); Serial.print(",");
  Serial.print("Y:"); Serial.print(event.acceleration.y); Serial.print(",");
  Serial.print("Z:"); Serial.print(event.acceleration.z); Serial.print(",");

  Serial.print("A:"); Serial.print(analogRead(light_sensor_pin)); // Print light sensor value
  Serial.println(""); // Print line break
  delay(1/sampling_freq);
}
