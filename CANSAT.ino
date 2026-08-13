// C++ code
const int red_led_pin = 10;
const int blue_led_pin = 9;
const int green_led_pin = 8;
const int buzzer_pin = 6;
const int potent_pin = A0;
unsigned long deploy_time = 0;

//altitudes
const int ascent_threshold = 100;
const int apogee_threshold = 850;
const int descent_threshold = 820;

int altitude = 0;

//state machine setup
enum States {
  ASCENT,
  APOGEE,
  DESCENT
};
States current_state = ASCENT;

bool deployed = false; //parachute status
bool hasAscended = false;

void setup()
{
  Serial.begin(9600);
  pinMode(buzzer_pin, OUTPUT);
  pinMode(red_led_pin, OUTPUT);
  pinMode(blue_led_pin, OUTPUT);
  pinMode(green_led_pin, OUTPUT);
}

void loop()
{
 //read potentiometer and convert its value to altitude
 int potent_value = analogRead(potent_pin);
 altitude = map(potent_value, 0, 1023, 0, 1000);
 Serial.print("Altitude : ");
 Serial.println(altitude);
 
 //ASCENT
 if (altitude <= ascent_threshold) {
 	Serial.println("Current State : ASCENT");
   	digitalWrite(10, HIGH);
    digitalWrite(9, LOW);
   	digitalWrite(8, LOW);
   	hasAscended = true;
    current_state = ASCENT;
   	noTone(buzzer_pin);
    deployed = false;
 }
 
 //APOGEE
 else if (altitude >= apogee_threshold) {
 	Serial.println("Current State : APOGEE");
    digitalWrite(10, LOW);
    digitalWrite(9, HIGH);
    digitalWrite(8, LOW);
    
    current_state = APOGEE;
   if (!deployed){
     deployed = true;
     Serial.println("Deployment State : HAS DEPLOYED");
     tone(buzzer_pin, 1200);
     deploy_time = millis();
   }
 }
  
 //DESCENT
 else if (altitude <= descent_threshold) {
 	Serial.println("Current State : DESCENT");
 	digitalWrite(10, LOW);
 	digitalWrite(9, LOW);
    digitalWrite(8, HIGH);
    current_state = DESCENT;
    noTone(buzzer_pin);
    deployed = false;
  }  
 //stop buzzer after 3 seconds from deployment
 if (deployed && millis() - deploy_time  > 1000) {
 	noTone(buzzer_pin);
 }
}
