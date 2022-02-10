const int sensorPin = A0;

const int threshold = 25;
const long sampleDuration = 20000; // microseconds
const long cutoffDuration = 100000; // microseconds

/**
 * The cutoff period is used to filter out reflections of the drum head.
 * When a trigger happens, a cutoff timer is started, and any inputs in 
 * the cutoff period are ignored UNLESS they are of a greater amplitude.
 * This still allows us to catch things like flams
 */
bool inCutoffPeriod = false;
int cutoffPeak = 0; // Value of the trigger when cutoff period started
long cutoffStart = 0;  // micros() when the cutoff period started

void setup() {
  Serial.begin(115200);
}


/**
 * Reads an analog input for the given amount of time and returns
 * the highest value in that period.
 */
int getSample(int inputPin, long durationMicros) {
  long start = micros();
  int value = 0;
  int peak = 0;
  while (micros() - start < durationMicros) {
    // Catch instances of micros() overflowing back to 0
    if (micros() < start) {
      break;
    }
    value = analogRead(inputPin);
    if (value > peak) {
      peak = value;
    }
  }
  return peak;
}



void loop() {

  int sensorReading = getSample(sensorPin, sampleDuration);

  /**
   * If enough time has passed, exit the cutoff period
   */
  if (micros() - cutoffStart > cutoffDuration) {
    inCutoffPeriod = false;
  }


  /**
   * Catch cases of micros() overflowing to 0.
   * FIXME: Handle this better (what if cutoffStart was only a few microseconds from the max value?)
   */
  if (micros() < cutoffStart) {
    inCutoffPeriod = false;
  }

  /**
   * We want to trigger if the sensor reading is over the threshold (obviously) AND we're either not in a cutoff
   * period, or the sensor reading is higher than the value that trigger the cutoff (probably not a reflection)
   */
  if (sensorReading > threshold && (!inCutoffPeriod || sensorReading > cutoffPeak)) {
    Serial.println(0);
    Serial.println(0);
    Serial.println(0);
    Serial.println(0);

    Serial.println(sensorReading);
    Serial.println(0);
    Serial.println(0);
    Serial.println(0);
    Serial.println(0);

    cutoffPeak = sensorReading;
    cutoffStart = micros();
    inCutoffPeriod = true;
  }
}
