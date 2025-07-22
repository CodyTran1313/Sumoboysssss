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
int RIGHT_F = 12; // Pin to move motor forwards (IN1)
int RIGHT_R = 11; // Pin to move motor backwards (IN2)

int LEFT_SPEED = 9; // Speed pin, ranges from 0 to 255 (ENB)
int LEFT_F = 8; // Pin to move motor forwards (IN3)
int LEFT_R = 7; // Pin to move motor backwards (IN4)

// TODO: Define other constants to be used in your sumobot
#define MAX_SPEED 255

//Maximum distance robot can be, 999 for now until I'm told
#define ROBOT_RANGE 999

//Time it takes to drive around the circle
#define Circle_time 1000

#define WAITING 0
#define SEARCHING 1
#define ATTACKING 2
#define SPEEDOFSENSOR 0.0340

// TODO: Initialise more global variables to be used
int currentState = SEARCHING;

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// Setup Function /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void setup() {

    pinMode(trigPin1, OUTPUT);
    pinMode(echoPin1, INPUT);

  //Setup serial communication at 9600 baudrate to allow testing for
  // input/output of the sumobot
    Serial.begin(9600);
}

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// Loop Function /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// This function is where all your logic will go. The provided template uses the 
// 'states model' discussed in week 5's build session.
void loop() {
    //Stay in system state WAITING for 5 seconds
    if (currentState == WAITING) {
        Serial.println("Waiting 5 seconds before starting");
        delay(5000);
        currentState = SEARCHING;
    }

    //OK SO IF WE'RE PULLING THAT CIRCLE DRIVING BS
    double distanceDetected = getDistance(trigPin1, echoPin1);
    if (distanceDetected <= ROBOT_RANGE) {
        //seen as we're driving in a circle regardless, we can just note down the position here
        Serial.println("enemy robot found");
    }

    //Turning left so we're ready to drive
    while (checkBorder(IRPin) != 1) {
        stationaryTurnLeft(MAX_SPEED);
    }

    //Now I have no fucking clue how to drive around the edge of the circle but I'll just wing it here
    double startTime = millis();
    while (millis() <= startTime + Circle_time) {
        if (checkBorder(IRPin) == 1) {
            stationaryTurnRight(MAX_SPEED);
        } else {
            driveForwards(MAX_SPEED);
        }
    }

    //At this point we should (theoretically) be at the other side of the circle, so here we turn around
    stationaryTurnRight(MAX_SPEED);
    delay(100);
    stop();

    //Now we need to start scanning for the robot

    if (checkBorder(IRPin)) {
        Serial.println("Front White");
        driveBackwards(MAX_SPEED);
        delay(100);
        turnRight(MAX_SPEED);
        delay(50);
    } else if (checkBorder(sideIRPin)) {
        Serial.println("Side White");
        // ?? WHICH SIDE YOU FUCK, WRITE COMMENTS
        stationaryTurnRight(MAX_SPEED);
        delay(50);
        driveForwards(MAX_SPEED);
        delay(50);
    } else {
        // A switch statement allows us to compare a given variable to multiple
        // cases.
        switch (currentState) {
        case SEARCHING:
            //Obtaining the ditance
            double distanceDetected = getDistance(trigPin1, echoPin1);
            Serial.println("Distance detected: " + String(distanceDetected) + " cm");

            //If/else conditionals based on detection
            if (distanceDetected <= ROBOT_RANGE) {
                currentState = ATTACKING;
            } else if(checkBorder(IRPin) != 1) {
                // ^^^ pretty sure this won't work because idfk where the IR pin is,
                // but basically it's meant to be, until it reaches the border turn right
                // alternatively we should probably do it based on degrees

                //Turn right first because it's likely the robot will be on our right (as we loop around the left)
                stationaryTurnRight(MAX_SPEED);
            } else {
                stationaryTurnLeft(MAX_SPEED);
            }
        case ATTACKING:
            // If it finds bot, ram at it
            if (getDistance(trigPin1, echoPin1) <= ROBOT_RANGE) {
                driveForwards(MAX_SPEED);
            } else {
                currentState = SEARCHING;
            }
        case WAITING:
            //literally 0 reason to have this case
            currentState = SEARCHING;
        default:
            // This is for if the currentState is neither SEARCHING or ATTACKING
            driveForwards(MAX_SPEED);
            break;
        }
        // What other states would you need? kys whoever wrote this

        delay(250); // Small delay for stability
    }
    Serial.println("End of loop.");
    // The bot will run this code if the IR detects white
    //what code ?
    // What movement should the bot do in this situation?
    //declare victory
    victorySpins();

    
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


//Stop function
void stop() {
    Serial.println("STOP");

	digitalWrite(LEFT_F, LOW);
	digitalWrite(LEFT_R, LOW);
	digitalWrite(RIGHT_F, LOW);
	digitalWrite(RIGHT_R, LOW);
}

//Victory burnout function
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


// Don't forget to ask questions on the DISCORD if you need any help!

void startRight(){  
    Serial.println("Doing the starting right turn")
	analogWrite(LEFT_SPEED, MAX_SPEED); 
    \\ proll do a different speed than max_speed to change turning radius
    \\but will need to actually run to check
	
    \\ power the left wheel, don't power the right to curve right
    digitalWrite(LEFT_F, HIGH);
	digitalWrite(LEFT_R, LOW);

    \\ once the edge is reached (side ir detects white), follow the curve of the ring until some point

    \\ start searching
}




