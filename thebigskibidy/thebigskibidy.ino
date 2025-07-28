////////////////////////////////////////////////////////////////////////////////
// This is the basic Sumobots scaffold code authored by the workshops and
// projects team for participants in MTRNSoc's 2024 sumobot competition.
////////////////////////////////////////////////////////////////////////////////

// TODO: Define ultrasonic and infrared sensor pins
// REFER TO ARDUINO BASICS WORKSHOP ON HOW TO CODE SENSORS

// Ultrasonic sensor pins
// CHANGE THIS
#define echoPin1 3
#define trigPin1 4

// Infrared sensor pins
// CHANGE THIS
#define IRPin 6
#define sideIRPin 5

// TODO: Define constants/variables for motors (workshop 4)
int RIGHT_SPEED = 10; // Speed pin, ranges from 0 to 255 (ENA)
int RIGHT_F = 7; // Pin to move motor forwards (IN1)
int RIGHT_R = 8; // Pin to move motor backwards (IN2)

int LEFT_SPEED = 9; // Speed pin, ranges from 0 to 255 (ENB)
int LEFT_F = 11; // Pin to move motor forwards (IN3)
int LEFT_R = 12; // Pin to move motor backwards (IN4)

int RIGHT_CHECK = 1; //Checking right first

// TODO: Define other constants to be used in your sumobot
#define MAX_SPEED 255
#define PARTIAL_SPEED 84
#define SP_SPEED 55
#define BEGINNING_SPEED 65

//Maximum distance robot can be, 999 for now until I'm told
#define ROBOT_RANGE 40

//Time it takes to drive around the circle
#define Circle_time 3400

#define WAITING 0
#define SEARCHING 1
#define ATTACKING 2
#define SPEEDOFSENSOR 0.0340

//Time to turn 90 degrees
#define turn90 230

// TODO: Initialise more global variables to be used
int currentState = WAITING;

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// Setup Function /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void setup() {
    pinMode(trigPin1, OUTPUT);
    pinMode(echoPin1, INPUT);

  //Setup serial communication at 9600 baudrate to allow testing for
  // input/output of the sumobot
    Serial.begin(9600);

  //TEST IF ONE TIME WORKS
  //Stay in system state WAITING for 5 seconds
    if (currentState == WAITING) {
        Serial.println("Waiting 5 seconds before starting");
        delay(2000); //REMEMBER TO CHANGE BACK TO 5 SECS
        currentState = SEARCHING;
    }
    //Get close but not over the edge
    driveForwards(PARTIAL_SPEED);
    delay(50);
    //Drive forwards to the edge of the circle
    while (checkBorder(IRPin) != 1) {
        driveForwards(BEGINNING_SPEED);
    }

    //Turn hard right, should theoretically be 90 degrees
    stationaryTurnRight(MAX_SPEED);
    delay(turn90);
    stop();

    //Drives around the circumference of the circle until it's in the appropriate position
    double startTime = millis();
    while (millis() <= startTime + Circle_time) {
        turnRightSlight(MAX_SPEED, PARTIAL_SPEED);
    }

    //At this point we should be at the edge of the cirlce, turn right 90 degrees so we're facing the centre
    stationaryTurnRight(MAX_SPEED);
    delay(turn90); // TODO
    stop();
}

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// Loop Function /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// This function is where all your logic will go. The provided template uses the 
// 'states model' discussed in week 5's build session.
void loop() {
    //Now we need to start scanning for the robot

    // if (checkBorder(IRPin)) {
    //     Serial.println("Front White");
    //     driveBackwards(MAX_SPEED);
    //     delay(100);
    //     stationaryTurnRight(MAX_SPEED);
    //     delay(50);
    // } else if (checkBorder(sideIRPin)) {
    //     Serial.println("Side White");
    //     stationaryTurnRight(MAX_SPEED);
    //     delay(50);
    //     driveForwards(MAX_SPEED);
    //     delay(50);
    // } 
    
    // else {
        // A switch statement allows us to compare a given variable to multiple
        // cases.
        switch (currentState) {
            case SEARCHING:
            searching:
                int i = 0;
                while (currentState == SEARCHING) {
                    double distanceDetected = getDistance(trigPin1, echoPin1);
                    Serial.println("Distance detected: " + String(distanceDetected) + " cm -- " + String(i));
                    //If/else conditionals based on detection
                    if (distanceDetected <= ROBOT_RANGE && distanceDetected != 0) {
                        // if (i <= 5) {
                        //     stationaryTurnRight(PARTIAL_SPEED);
                        //     delay(5);
                        // } else {
                        //     stationaryTurnLeft(PARTIAL_SPEED);
                        //     delay(5);
                        // }
                        currentState = ATTACKING;
                    } else if (i < 10) {
                        //Turn right first because it's likely the robot will be on our right (as we loop around the left)
                        stationaryTurnRight(SP_SPEED); // change
                        i++;
                    } else {
                        stationaryTurnLeft(SP_SPEED);
                        if (i >= 26) {
                            i = -10;
                        }
                        i++;
                    }
                    delay(100);
                }

            case ATTACKING:
                // If it finds bot, ram at it
                while (getDistance(trigPin1, echoPin1) <= ROBOT_RANGE ) {
                    driveForwards(MAX_SPEED);
                } 
                currentState = SEARCHING;
                goto searching;
            default:
                // This is for if the currentState is neither SEARCHING or ATTACKING
                driveForwards(MAX_SPEED);
                break;
            }

        delay(250); // Small delay for stability
    // }
    Serial.println("End of loop.");
    // The bot will run this code if the IR detects white
    //what code ?
    // What movement should the bot do in this situation?
    //declare victory
}

////////////////////////////////////////////////////////////////////////////////
/////////////////////////// Sensor Functions ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/*  Function: Get Distance
/   parameters: trigPin, EchoPin
/   returns: distance
/   summary: uses ultrasonic sensor to get distance of object in front
*/
double getDistance(int trigPin, int echoPin) {
    double distance, duration;
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = (duration * SPEEDOFSENSOR) / 2; // distance's unit is cm and duration is in ms 
    Serial.println("Distance detected: " + String(distance) + " cm");
    return distance;
}

// IMPORTANT NOTE:
// How will you prevent your sumobot from detecting objects outside of the ring?

/*  Function: Check Border
/   parameters: IRPin
/   returns: 1 if white is detected, 0 if not detected
/   summary: uses IR sensor to detect edge of arena
*/
int checkBorder(int irSensorPin) {
    int statusSensor = digitalRead(irSensorPin);
    if (statusSensor == HIGH) {
        Serial.println("Detected 1");
        return 1;
    } else {
        return 0;
    }
}

// What if you have more than 1 ultrasonic or infrared sensor? Do you need more
// reading functions or can the same ones be used?

////////////////////////////////////////////////////////////////////////////////
////////////////////////////// Move Functions //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/*  Function: Drive forward
/   parameters: none
/   returns: none
/   summary: this function drives sumobot forward
*/
void driveForwards(int speed)
{
Serial.println("Driving forward");
    analogWrite(LEFT_SPEED, speed);
    analogWrite(RIGHT_SPEED, speed);

    digitalWrite(LEFT_F, HIGH);
    digitalWrite(LEFT_R, LOW);
    digitalWrite(RIGHT_F, HIGH);
    digitalWrite(RIGHT_R, LOW);
}
// ADDITIONAL: How can we change the above function to all the sumobot to move
// forward at a variable speed? HINT: Modify the analogWrite functions

/*  Function: Drive backwards
/   parameters: none
/   returns: none
/   summary: this function drives sumobot backwards
*/
void driveBackwards(int speed)
{
    Serial.println("Driving backwards");
    analogWrite(LEFT_SPEED, speed);
    analogWrite(RIGHT_SPEED, speed);

    digitalWrite(LEFT_F, LOW);
    digitalWrite(LEFT_R, HIGH);
    digitalWrite(RIGHT_F, LOW);
    digitalWrite(RIGHT_R, HIGH);
}

/*  Function: Turn left
/   parameters: none
/   returns: none
/   summary: this function turns sumobot to the left
*/

//Turn left in place
void stationaryTurnLeft(int speed)
{
    Serial.println("Turning left");
    analogWrite(LEFT_SPEED, speed);
    analogWrite(RIGHT_SPEED, speed);

    digitalWrite(LEFT_F, LOW);
    digitalWrite(LEFT_R, HIGH);
    digitalWrite(RIGHT_F, HIGH);
    digitalWrite(RIGHT_R, LOW);
}

//Turn left with an arc
void turnLeft(int speed)
{
    Serial.println("Moving left");
    analogWrite(LEFT_SPEED, speed);
    analogWrite(RIGHT_SPEED, speed);

    digitalWrite(LEFT_F, LOW);
    digitalWrite(LEFT_R, LOW);
    digitalWrite(RIGHT_F, HIGH);
    digitalWrite(RIGHT_R, LOW);
}


// What other movement functions might we need?
// TODO: Create some of your own movement functions.
//TURN RIGHT Function
void stationaryTurnRight(int speed)
{
    Serial.println("Turning right");
    analogWrite(LEFT_SPEED, speed);
    analogWrite(RIGHT_SPEED, speed);

    digitalWrite(LEFT_F, HIGH);
    digitalWrite(LEFT_R, LOW);
    digitalWrite(RIGHT_F, LOW);
    digitalWrite(RIGHT_R, HIGH);
}

void turnRight(int speed)
{
    Serial.println("Moving right");
    analogWrite(LEFT_SPEED, speed);
    analogWrite(RIGHT_SPEED, speed);

    digitalWrite(LEFT_F, HIGH);
    digitalWrite(LEFT_R, LOW);
    digitalWrite(RIGHT_F, LOW);
    digitalWrite(RIGHT_R, LOW);
}

//Turn right slightly
void turnRightSlight(int speed, int halfSpeed)
{
    Serial.println("Moving right");
    analogWrite(LEFT_SPEED, halfSpeed);
    analogWrite(RIGHT_SPEED, speed);

    digitalWrite(LEFT_F, HIGH);
    digitalWrite(LEFT_R, LOW);
    digitalWrite(RIGHT_F, HIGH);
    digitalWrite(RIGHT_R, LOW);
}


//Stop function
void stop() {
    Serial.println("STOP");

    digitalWrite(LEFT_F, LOW);
    digitalWrite(LEFT_R, LOW);
    digitalWrite(RIGHT_F, LOW);
    digitalWrite(RIGHT_R, LOW);
}

//Victory spin function
void victorySpins() {
    Serial.println("Get Clapped Nerds!");
    digitalWrite(LEFT_F, LOW);
    digitalWrite(LEFT_R, HIGH);
    digitalWrite(RIGHT_F, HIGH);
    digitalWrite(RIGHT_R, LOW);
    delay(2000);
    digitalWrite(LEFT_F, HIGH);
    digitalWrite(LEFT_R, LOW);
    digitalWrite(RIGHT_F, LOW);
    digitalWrite(RIGHT_R, HIGH);
    delay(2000);
}
