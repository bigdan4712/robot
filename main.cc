#include <iostream>
#include <unistd.h>
#include <time.h>
#include <ev3dev.h>

using namespace std;

int choose(int n, int k){

        int
        answer = 1,
        d;

    // first check: k > n?
    if(k > n)
        return 0;

    // trickery: if k > n/2  then its less arithmetic  to use n-k instead of k

    if (k > n/2)
        k = n - k;
    // alternate mult and div

    for (d=1;d<=k;d++)
        answer = (answer * (n-d+1)) / d;

    return answer;

}

int main(void) {

	int count = 0;
	int bar_count = 0;
	int edge = 0;
	int total = 0;
	int memory[1000];
	int answer = 0;
	int n = 8;
	int k = 3;


	ev3Setup();
	ev3MotorReset(MOTOR_A);
	ev3MotorReset(MOTOR_B);
	setInputMode();

	ev3ColorSetMode(SENSOR_1,0);
	ev3MotorSetPower(MOTOR_A, -78);
	ev3MotorSetPower(MOTOR_B, -80);

	cout << "Code 39?  (1 for yes, 0 for no): " ;
	cin >> answer;

	if(answer == 1) {


		while (!escapeButton.isPressed()) {

			//if first iteration of loop, sleep for 3 seconds and then turn the motors on

			if(count == 0) {
				sleep(3);
				ev3MotorRun(MOTOR_A);
				ev3MotorRun(MOTOR_B);
			}

			//if the count is greater than 20,
			//then the bars are white and make it a 1
			if(ev3ColorReadReflected(SENSOR_1) > 20) memory[count] = 1;
			else memory[count] = 0;

			//if the count is >= 2, and there is a change in values between 1 and 0, then
			//an edge has been found

			//this algorithm works by by detecting wide vs narrow bars.  for code 39 to work,
			//there must be exactly 3 wide bars in the code.  it takes advantage of the
			//" n choose k" combinatoric function to detect the numbers.  if exactly
			// 3 wide bars are detected, it is a valid code.  if not, the code is invalid.
			if((count >= 2) && (memory[count] != memory[count-1])) {
				edge++;

				if(bar_count > 40) { //detects a wide bar, decrements k
					total = total + choose(n,k);
					k--;
				}
				//reset bar count for next wide or narrow, decrement n everytime a change is 					//detected.  this is for the choose function.
				bar_count = 0;
				n--;
			}
			//if robot has passed 10th edge, stop the motors and output detected number
			if(edge > 9) {
				ev3MotorStop(MOTOR_A, MOTOR_FINISH_HOLD);
				ev3MotorStop(MOTOR_B, MOTOR_FINISH_HOLD);

				if(k != 0) {
					cout << "Invalid code read.  Try again." << endl;

					resetInputMode();
					return 0;
				}

				cout << "The total is: " << total << endl;

				resetInputMode();
				return 0;
			}

			count++;
			bar_count++;
		}
	}

	else {	// binary representation of bar code
		while (!escapeButton.isPressed()) {

		//if first iteration of loop, sleep for 3 seconds and then turn the motors on

			if(count == 0) {
				sleep(3);
				ev3MotorRun(MOTOR_A);
				ev3MotorRun(MOTOR_B);
			}

			//if the count is greater than 20,
			//then the bars are white and make it a 1
			if(ev3ColorReadReflected(SENSOR_1) > 20) memory[count] =1;
			else memory[count] =0;

			//if the count is >= 2, and there is a change in values between 1 and 0, then
			//an edge has been found.  this algorithm works best when binary values are read
			//left to right.  double the current total everytime an edge is found.
			//a wide bar represents a 1 and short ones represent a 0.  when a wide bar is
			//detected, increment the total by one.

			if((count >= 2) && (memory[count] != memory[count-1])) {
				total = total * 2;
				edge++;
				//if the bar is wide, add one to the total			
				if(bar_count > 40) total++;
				//whether the bar is wide or narrow, bar_counts gets reset to 0
				bar_count = 0;
			}
			//if robot has passed the final edge of the barcode, stop the motors, output
			//the read number, and exit the program.
			if(edge > 9) {
				ev3MotorStop(MOTOR_A, MOTOR_FINISH_HOLD);
				ev3MotorStop(MOTOR_B, MOTOR_FINISH_HOLD);

				cout << "The total is: " << total << endl;

				resetInputMode();
				return 0;
			}

			count++;
			bar_count++;
		
			}
		}
	}
}
