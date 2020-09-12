#include <LiquidCrystal_I2C.h>

// https://www.makerguides.com/character-i2c-lcd-arduino-tutorial/
// Download: https://gitlab.com/tandembyte/LCD_I2C/

// 16x2 LCD connections:
// VCC - 5V
// GNC - GND
// SDA - A4
// SCL - A5

// Hall sensor connections:
// digital out of hall sensor to D2 of arduino
int interruptPin = 2;

const int TRAINER_ROLLER_DIAMETER = 50; // mm
const int WHEEL_DIAMETER = 700; // mm
const float REDUCTION = (float) WHEEL_DIAMETER / (float) TRAINER_ROLLER_DIAMETER; // for every REDUCTION rotations of the trainer's roller the bike's wheel spins once
const float WHEEL_CIRCUMFERENCE = PI * WHEEL_DIAMETER / 1000.0 / 1000.0; // km

LiquidCrystal_I2C lcd(0x3F, 16, 2); // use I2C scanner to find the address
const char bell[8] PROGMEM = {B00100, B01110, B01110, B01110, B11111, B00000, B00100, B00000};

volatile unsigned long num_rotations = 0;
unsigned long last_compute_time = 0;
unsigned long last_num_rotations = 0;
float max_speed = 0; // km/h

const long SECOND = 1000;
const long MINUTE = 60 * SECOND;
const long HOUR = 60 * MINUTE;

void countRotations()
{
  num_rotations++;
}

void setup() {
  Serial.begin(9600);

  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), countRotations, FALLING); //normal output = 5V, when magnet is near, output = 0V.
  lcd.init();
  lcd.backlight();

  lcd.createChar(0, bell);
}

void loop() {
  unsigned long current_time = millis();
  unsigned long time_diff = current_time - last_compute_time;

  if (time_diff >= SECOND)
  {
    last_compute_time = current_time;
    // count the trainer's roller number of rotations and from there compute the bike's wheel number of rotations
    unsigned long rots_in_period = num_rotations - last_num_rotations;
    last_num_rotations = num_rotations;

    float bike_rotations = num_rotations / REDUCTION;
    float bike_rots_in_period = rots_in_period / REDUCTION;
    

    //lcd.clear(); //TODO: write the whole screen  in order to avoid this flicker
    lcd.home();
    //lcd.print("Rots ");
    //lcd.print(num_rotations);


    float distance = WHEEL_CIRCUMFERENCE * bike_rotations; // km
    float dist_per_time_unit = distance / current_time; // km/ms
    float avg_speed = dist_per_time_unit * HOUR; // km/h
    // try avg_spd = (dst*HOUR)/current_time;

    Serial.print("WHEEL_DIAMETER: ");
    Serial.println(WHEEL_DIAMETER);

    Serial.print("WHEEL_CIRCUMFERENCE: ");
    Serial.println(WHEEL_CIRCUMFERENCE);

    Serial.print("SECOND: ");
    Serial.println(SECOND);

    Serial.print("MINUTE: ");
    Serial.println(MINUTE);

    Serial.print("HOUR: ");
    Serial.println(HOUR);

    Serial.print("Total rotations: ");
    Serial.println(num_rotations);

    Serial.print("Total bike rotations: ");
    Serial.println(bike_rotations);

    Serial.print("Total Distance [km]: ");
    Serial.println(distance, 4);

    Serial.print("Average Speed [km/h]: ");
    Serial.println(avg_speed, 4);

    Serial.print("Dist per time unit [km/ms]: ");
    Serial.println(dist_per_time_unit, 4);

    Serial.print("Current time [s]: ");
    Serial.println((float)current_time / SECOND, 4);

    float instant_distance = WHEEL_CIRCUMFERENCE * bike_rots_in_period; // km

    //knowing that the update period is 1 second (time diff) I could transform this to: instant_distance * 60*60;
    float instant_speed = (instant_distance / time_diff) * HOUR; // km/h

    if(instant_speed > max_speed)
    {
        max_speed = instant_speed;
    }

    Serial.print("time diff: ");
    Serial.println(time_diff);

    Serial.print("rotations in period: ");
    Serial.println(rots_in_period);

    Serial.print("Bike rotations in period: ");
    Serial.println(bike_rots_in_period);

    Serial.print("Instant distance [km]: ");
    Serial.println(instant_distance, 4);

    Serial.print("Instant speed [km/h]: ");
    Serial.println(instant_speed, 4);
    
    Serial.print("Max speed [km/h]: ");
    Serial.println(max_speed, 4);

    Serial.println();
    
    lcd.print("A "); //2 chars used
    lcd.print(avg_speed, 2); //+5 = 7 chars used
    lcd.print(" M "); //+3 = 10 chars used
    lcd.print(max_speed, 2); //+5 = 15 chars used
    lcd.print("           "); // make sure left over charachters on the line are overwritten

    lcd.setCursor(0, 1); // col, line
    lcd.print("S "); 
    lcd.print(instant_speed, 2); //+5 = 7 chars used
    lcd.print(" D "); //+3 = 10 chars used
    lcd.print(distance, 2); //+5 or 6 = 15/16 chars used
    lcd.print("           "); // make sure left over charachters on the line are overwritten

    
    //lcd.write(0); //bell
  }
}
