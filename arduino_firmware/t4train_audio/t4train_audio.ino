/*
This code gets 20 samples from the x vector of the Arduino,
then sends the 20 samples to T4Train.
*/

#include <Arduino_LSM9DS1.h>
#include <PDM.h>


int delimeter_length = 4;
uint8_t delimiter[] = { 0xde, 0xad, 0xbe, 0xef };
#define CAPTURE_SIZE 512 

static const char channels = 1; // use mono audio signals
static const int frequency = 16000;
short sampleBuffer[CAPTURE_SIZE];
volatile int samplesRead;

void setup() {

  Serial.begin(9600);
  while (!Serial);
  PDM.onReceive(onPDMdata);

  if (!PDM.begin(channels, frequency)) {
    Serial.println("Failed to initialize PDM!");
    while (1)
      ;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  // get_samples();
  // send_samples();
  if (samplesRead) {
    for (int i = 0; i < samplesRead; i++) {
      Serial.println(sampleBuffer[i]);
    }
    samplesRead = 0;
  }

  send_samples();
}

void send_samples() {
  sampleBuffer[CAPTURE_SIZE] = 0;
  sampleBuffer[CAPTURE_SIZE] = 1;

  // Send the delimiter
  Serial.write(delimiter, sizeof(delimiter));

  Serial.write((uint8_t *)sampleBuffer, sizeof(sampleBuffer));
}

void onPDMdata() {
  // query the number of bytes available
  int bytesAvailable = PDM.available();

  // read into the sample buffer
  PDM.read(sampleBuffer, bytesAvailable);

  // 16-bit, 2 bytes per sample
  samplesRead = bytesAvailable / 2;
}




