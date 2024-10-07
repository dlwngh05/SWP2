
// Arduino pin assignment
#define PIN_LED  9
#define PIN_TRIG 12   // sonar sensor TRIGGER
#define PIN_ECHO 13   // sonar sensor ECHO

// configurable parameters
#define SND_VEL 346.0     // sound velocity at 24 celsius degree (unit: m/sec)
#define INTERVAL 25      // sampling interval (unit: msec)
#define PULSE_DURATION 10 // ultra-sound Pulse Duration (unit: usec)
#define _DIST_MIN 100.0   // minimum distance to be measured (unit: mm)
#define _DIST_MAX 300.0   // maximum distance to be measured (unit: mm)

#define TIMEOUT ((INTERVAL / 2) * 1000.0) // maximum echo waiting time (unit: usec)
#define SCALE (0.001 * 0.5 * SND_VEL) // coefficent to convert duration to distance

unsigned long last_sampling_time = 0;   // unit: msec, 변수 초기화

void setup() {
  // initialize GPIO pins
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);  // sonar TRIGGER
  pinMode(PIN_ECHO, INPUT);   // sonar ECHO
  digitalWrite(PIN_TRIG, LOW);  // turn-off Sonar 
  
  // initialize serial port
  Serial.begin(57600);

  // Initialize last_sampling_time
  last_sampling_time = millis(); // 처음 실행 시 현재 시간을 저장
}

void loop() {
  float distance;

  // wait until next sampling time.
  if (millis() - last_sampling_time < INTERVAL)
    return;

  distance = USS_measure(PIN_TRIG, PIN_ECHO); // read distance

  if (distance == 0.0 || distance > _DIST_MAX) {
      distance = _DIST_MAX + 10.0;  // Set Higher Value
      analogWrite(PIN_LED, 255);    // LED OFF
  } else if (distance < _DIST_MIN) {
      distance = _DIST_MIN - 10.0;  // Set Lower Value
      analogWrite(PIN_LED, 255);    // LED OFF
  } else {
      int led_brightness = calculateLEDBrightness(distance);  // Calculate brightness
      analogWrite(PIN_LED, led_brightness); // Adjust brightness
  }

  // output the distance to the serial port
  Serial.print("Min:");        Serial.print(_DIST_MIN);
  Serial.print(",distance:");  Serial.print(distance);
  Serial.print(",Max:");       Serial.print(_DIST_MAX);
  Serial.println("");
  
  // update last sampling time
  last_sampling_time = millis();
}

// get a distance reading from USS. return value is in millimeter.
float USS_measure(int TRIG, int ECHO)
{
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);
  
  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE; // unit: mm
}

// Calculate LED brightness based on distance
int calculateLEDBrightness(float distance) {
  int brightness;

  if (distance <= 100.0) {
    // 100mm 이하에서 LED OFF
    brightness = 255; // LED OFF
  } else if (distance <= 150.0) {
    // 100mm에서 150mm까지 밝기 증가 (최대 밝기의 50%)
    brightness = map(distance, 100, 150, 255, 127); 
  } else if (distance <= 200.0) {
    // 150mm에서 200mm까지 밝기 증가 (최대 밝기)
    brightness = map(distance, 150, 200, 127, 0); // 밝기 최댓값으로 증가
  } else if (distance <= 250.0) {
    // 200mm에서 250mm까지 밝기 감소 (50% 유지)
    brightness = 127; // 50% 밝기 유지
  } else if (distance <= 300.0) {
    // 250mm에서 300mm까지 밝기 감소 (LED OFF)
    brightness = map(distance, 250, 300, 127, 255); // 점점 감소
  } else {
    // 300mm 이상일 때 LED OFF
    brightness = 255; // LED OFF
  }

  return constrain(brightness, 0, 255); // 밝기 값 제한 (0: 최대 밝기, 255: 꺼짐)
}
