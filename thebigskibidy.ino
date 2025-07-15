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

// hi

// Infrared sensor pins
// CHANGE THIS
#define IRPin 6

// TODO: Define constants/variables for motors (workshop 4)
// CHANGE THESE
int RIGHT_SPEED = 10; // Speed pin, ranges from 0 to 255 (ENA)
int RIGHT_F = 11; // Pin to move motor forwards (IN1)
int RIGHT_R = 12; // Pin to move motor backwards (IN2)

// CHANGE THESE
int LEFT_SPEED = 9; // Speed pin, ranges from 0 to 255 (ENB)
int LEFT_F = 7; // Pin to move motor forwards (IN3)
int LEFT_R = 8; // Pin to move motor backwards (IN4)

// TODO: Define other constants to be used in your sumobot
#define MAX_SPEED 255

#define WAITING 0
#define SEARCHING 1
#define ATTACKING 2

// TODO: Initialise more global variables to be used
int currentState = WAITING;

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// Setup Function /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
 
void setup() {
  // TODO: Set pinmodes for every pin defined above - you will have to determine
  // whether the pins will be input or output (or you could look at the workshop
  // slides)
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
    // Stay in system state WAITING for 5 seconds
    // if (currentState == WAITING) {
    //     Serial.println("Waiting 5 seconds before starting");
    //     delay(5000);
    //     currentState = SEARCHING;
    // }
    driveForwards();

    // If the IR doesnt detect white, the bot wil only run this while loop
    // if (checkBorder(IRPin) == 0) {
    //     // A switch statement allows us to compare a given variable to multiple
    //     // cases.
    //     switch (currentState) {
    //     case SEARCHING:
    //         // TODO: Add code to search for another bot
    //         // (e.g., rotate or move in a pattern)

    //         // For now, if you have wired up your bot correctly, this should be
    //         // printing out the distance being read from teh
    //         double distanceDetected = getDistance(trigPin1, echoPin1);

    //         // Use println statements to ensure that you code is working properly
    //         // String([value]) converts a value to a string, and you can add strings
    //         // to each other to combine them into one.
    //         Serial.println("Distance detected: " + String(distanceDetected) + " cm");
            
    //         // if another bot is found, what should the new currentState be?
            
    //     case ATTACKING:
    //         // TODO: Add code to move forward aggressively towards the
    //         // detected bot
            

    //         // If the other bot is lost, what should the new currentState be?
            
    //     default:
    //         // This is for if the currentState is neither SEARCHING or ATTACKING
            
    //         break;
    //     }
    //     // What other states would you need?

    //     delay(250); // Small delay for stability
    // }

    // The bot will run this code if the IR detects white

    // What movement should the bot do in this situation?
    
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////// More Functions //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/*  Function: Example Function
/   parameters: x, y
/   returns: distance
/   summary: this is an example function
*/
int exampleFunction(int x, int y) {
    int distance = x + y; // logic inside the function
    return distance; // this returns to where the function was called
}

/*  Function: Example Function 2 (Addition function)
/   parameters: x, y
/   returns: true if the sum of x and y is greater than 10, false if not
*/
bool add(int x, int y) {
	int sum = x + y;
	if (sum > 10) { // Use if statements to check for conditions
		return true;
	} else {
		return false;
	}
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
    distance = (duration * 0.0340) / 2;
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
void driveForwards()
{
  Serial.println("Driving forward");
	analogWrite(LEFT_SPEED, MAX_SPEED);
	analogWrite(RIGHT_SPEED, MAX_SPEED);

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
void driveBackwards()
{
    Serial.println("Driving backwards");
	analogWrite(LEFT_SPEED, MAX_SPEED);
	analogWrite(RIGHT_SPEED, MAX_SPEED);

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

void turnLeft()
{
    Serial.println("Moving left");
	analogWrite(LEFT_SPEED, MAX_SPEED);
	analogWrite(RIGHT_SPEED, MAX_SPEED);

	digitalWrite(LEFT_F, LOW);
	digitalWrite(LEFT_R, HIGH);
	digitalWrite(RIGHT_F, HIGH);
	digitalWrite(RIGHT_R, LOW);
}


// What other movement functions might we need?
// TODO: Create some of your own movement functions.




// Don't forget to ask questions on the DISCORD if you need any help!

