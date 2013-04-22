/* HC-SR04 Sensor controlling DC motor speed.
 
 code and circuits from
 http://oomlout.com/a/products/ardx/circ-03/ for basic motor connection
 https://gist.github.com/flakas/3294829 for PING sensor code
 
 I'm experimenting with different ways of controlling motor speed with an arduino 
 for use in a robot project.  In this case I'm using an HC-SR04 distance sensor to control 
 the motor speed.  The circ-03 link above shows the motor being powered by a digital in so, to
 control speed I'm using the PIN 9 on the arduino as a PWM to control the transistor that turns the 
 motor on and off.  This is giving me the speed control I require.  Under normal circumstances, the motor would 
 be powered by an external power source.
 
 This example code is in the public domain.
 */

// Declaring the pins being used
const int motorPin = 9;
const int trigPin = 10;
const int echoPin = 11;

void setup()
{
  pinMode(motorPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // This is for debugging only...
  Serial.begin(9600);
}

void loop()
{
  // cause the sensor to return a distance
  long distance = ping();
  
  // This is for debugging only...
  Serial.println(distance);

  // based on the distance from the sensor, write a voltage
  // 0-5V to pin 9.  This is what drives the PWM and controls the speed.
  // 0v = 0% duty cycle, 2.5v = 50%, 5v = 100% duty cycle
  if(distance <= 1)
    analogWrite(motorPin, pwmVoltage(5));
  else if(distance > 1 && distance <= 2)
    analogWrite(motorPin, pwmVoltage(2.5));
  else
    analogWrite(motorPin, pwmVoltage(0));
}

float pwmVoltage(float volts)
{
  // the pwm takes a value from 0 - 255.  Since our board has a 5v digital out
  // 255 / 5 = 51 so we multiply incoming voltages by 51.
  return volts * 51;
}

long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long ping()
{
  // from the second link up top: I factored the code out of the loop() function 
  // that was responsible for polling the sensor for distance
  // into a function that returns a distance. This makes the code in the loop function 
  // a little more readable.
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = 0;
  duration = pulseIn(echoPin, HIGH);

  return microsecondsToInches(duration);
}











