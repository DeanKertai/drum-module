const int sensorPin = A0;

const int threshold = 20;
const long sampleDuration = 10000; // microseconds
const long cutoffDuration = 100000; // microseconds
float cutoffOverrideFactor = 1.2; // ie: 1.2 = must be 20% higher than peak to override cutoff period

/**
 * The cutoff period is used to filter out reflections of the drum head.
 * When a trigger happens, a cutoff timer is started, and any inputs in 
 * the cutoff period are ignored UNLESS they are of a greater amplitude.
 * This still allows us to catch things like flams
 */
bool inCutoffPeriod = false;
int cutoffPeak = 0; // Value of the trigger when cutoff period started
long cutoffStart = 0; // micros() when the cutoff period started

void setup() {
  Serial.begin(31250);
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


/**
 * Send a midi note over serial
 * FIXME:
 *    Add checks to make sure command is >= 128
 *    and data values are less <= 127
 */
void sendMidi(int command, int pitch, int velocity) {
  Serial.write(command);
  Serial.write(pitch);
  Serial.write(velocity);
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
   * If the sensor reading is higher than the value at which the cutoff period was started,
   * this probably isn't a reflection, and might be something like a flam, so override the cutoff
   */
  bool overrideCutoff = false;
  if (inCutoffPeriod) {
    if (sensorReading > cutoffPeak * cutoffOverrideFactor) {
      overrideCutoff = true;
    }
  }


  /**
   * We want to trigger if the sensor reading is over the threshold (obviously) AND we're either not in a cutoff
   * period, or the sensor reading is higher than the value that trigger the cutoff (probably not a reflection)
   */
  if (sensorReading > threshold && (!inCutoffPeriod || overrideCutoff)) {
    int velocity = ((float)sensorReading / 1024.0) * 127.0;
    
    sendMidi(0x90, 43, velocity);  // Note on, 43=G2=Tom1, 64=Velocity(0-127)
    sendMidi(0x80, 43, velocity);  // Note off, 43=G2=Tom1, 64=Velocity(0-127)

    cutoffPeak = sensorReading;
    cutoffStart = micros();
    inCutoffPeriod = true;
  }
}
