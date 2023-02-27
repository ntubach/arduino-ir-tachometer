/*
A small IR Tachometer

Project based off of walkthrough found here:
https://www.instructables.com/Arduino-Based-Optical-Tachometer/

Author: Niko Tubach
Class: EN.605.715.81
*/

// Pin used to trigger the digital interrupt
const uint32_t DIO_INT_PIN = 2;
// Pin for the IR LED
const uint32_t IR_LED_PIN = 13;
// The number of blades on prop, adjusts math for RPM
const uint32_t BLADE_NUM = 2;
// Transistory storage for number of ir breaks we see
volatile uint32_t prop_count = 0;
// Storage for time
uint32_t time = 0;
// Time elapse value for rpm count
uint32_t timeold = 0;
// Storage for the rpm seen
unsigned int rpm = 0;
// Debouncer helper
uint32_t last_interrupt_time = 0;

void setup()
{
  Serial.begin(9600);
  // Setup headers for data
  Serial.println("Time (s),RPM");
  // Attach our ISR to the Vout of the photo-receiver to trigger when it goes low
  attachInterrupt(digitalPinToInterrupt(DIO_INT_PIN), prop_count_isr, FALLING);
  // Turn on the IR LED permanently
  pinMode(IR_LED_PIN, OUTPUT);
  digitalWrite(IR_LED_PIN, HIGH);
}

void loop()
{
  // Update each second after readings
  delay(1000);

  // Prevent interrupts while we print out
  noInterrupts();

  // Calculate the RPM, adjust for blade count
  rpm = (60 / BLADE_NUM) * 1000 / (millis() - timeold) * prop_count;
  // Update time for next cycle
  timeold = millis();
  // Check time for output
  time = timeold / 1000;

  // Print out current time , rpm reading
  Serial.print(time);
  Serial.print(" , ");
  Serial.println(rpm);

  // Reset counters
  prop_count = 0;
  rpm = 0;

  // Re-enable Interrupts
  interrupts();
}

void prop_count_isr()
{
  // Update our counter
  // prop_count++;
  uint32_t interrupt_time = millis();

  // Debouncing idea from Kevin
  // If interrupts come faster than 1ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 1)
  {
    prop_count++;
  }

  last_interrupt_time = interrupt_time;
}