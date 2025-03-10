#include <Wire.h>
#include <Adafruit_BMP085.h>

#define MQ_SENSOR A0  // MQ sensor connected to analog pin A0
#define LED_PIN D7    // LED connected to GPIO13 (D7)
#define NUM_SAMPLES 10 // Number of readings to average

Adafruit_BMP085 bmp;

void setup() {
    Serial.begin(115200);
    pinMode(MQ_SENSOR, INPUT);
    pinMode(LED_PIN, OUTPUT);
    
    if (!bmp.begin()) {
        Serial.println("Could not find a valid BMP180 sensor, check wiring!");
        while (1);
    }
}

void loop() {
    float pressure = bmp.readPressure(); // Read pressure in Pascals (Pa)

    // Ensure pressure stays within expected range
    if (pressure < 95000) pressure = 95000;
    if (pressure > 105000) pressure = 105000;

    // Map pressure from 95000-105000 Pa to 0-1000
    float mappedPressure = (pressure - 95000) * (1000.0 / (105000 - 95000));

    Serial.print("Raw Pressure (Pa): ");
    Serial.print(pressure);
    Serial.print(" | Mapped Pressure: ");
    Serial.println(mappedPressure);

    // Take multiple samples for a stable gas sensor reading
    int gasValue = 0;
    for (int i = 0; i < NUM_SAMPLES; i++) {
        gasValue += analogRead(MQ_SENSOR);
        delay(10); // Small delay between readings
    }
    gasValue /= NUM_SAMPLES; // Get the average value

    Serial.print("Gas Sensor Value (Avg): ");
    Serial.print(gasValue);

    if (gasValue > 750) {
        digitalWrite(LED_PIN, HIGH);  // Turn ON LED
        Serial.println(" | Gas Detected!"); // Indicate gas detection
    } else {
        digitalWrite(LED_PIN, LOW);   // Turn OFF LED
        Serial.println(" | No Gas Detected.");
    }

    delay(1000);
}
