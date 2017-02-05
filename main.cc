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

	timespec tv;
	int count = 0;
	int bar_count = 0;
	int edge = 0;
	int total = 0;
	int memory[1000];
	char answer = '';
	int n = 8;
	int k = 3;
	bool code39 = false;


	ev3Setup();
	ev3MotorReset(MOTOR_A);
	ev3MotorReset(MOTOR_B);
	setInputMode();

	ev3ColorSetMode(SENSOR_1,0);
	ev3MotorSetPower(MOTOR_A, -78);
	ev3MotorSetPower(MOTOR_B, -80);

	cout << "Code 39?  (y or n): " ;
	cin >> answer;

	if(answer == 'Y' || 'y')
		code39 = true;

	if(code39) {


		while (!escapeButton.isPressed()) {

			//if first iteration of loop, sleep for 3 seconds and then turn the motors on

			if(count == 0) {
				sleep(3);
				ev3MotorRun(MOTOR_A);
				ev3MotorRun(MOTOR_B);
			}

			//sensor values fall between 5 and 90, therefore if the count is greater than 20, a color transition has occured
			//thus, it is an edge.
			if(ev3ColorReadReflected(SENSOR_1) > 20) memory[count] =1;
			else memory[count] =0;

			//if the count is >= 2, and there is a change in values between 1 and 0, then
			//an edge has been found

			if((count >= 2) && (memory[count] != memory[count-1])) {
				total = total * 2;  //mathematical equivalent of reading binary left to right
				edge++;
				if(bar_count > 40) {
					total = total + choose(n,k);
					k--;
					bar_count = 0;
				}
					//reset the bar count to zero for the next time to detect a 1 or 0
			}
			//if robot has passed the final edge of the barcode, stop the motors, and then drop out of loop
			if(edge > 9) {
				ev3MotorStop(MOTOR_A, MOTOR_FINISH_HOLD);
				ev3MotorStop(MOTOR_B, MOTOR_FINISH_HOLD);
				break;
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

		//sensor values fall between 5 and 90, therefore if the count is greater than 20, a color transition has occured
		//thus, it is an edge.
		if(ev3ColorReadReflected(SENSOR_1) > 20) memory[count] =1;
		else memory[count] =0;

		//if the count is >= 2, and there is a change in values between 1 and 0, then
		//an edge has been found

		if((count >= 2) && (memory[count] != memory[count-1])) {
			total = total * 2;  //mathematical equivalent of reading binary left to right
			edge++;
			if(bar_count > 40) total++; //if the bar wide, count it as a "1" mathematical equivalent of a 1.
			bar_count = 0;	//reset the bar count to zero for the next time to detect a 1 or 0
		}
		//if robot has passed the final edge of the barcode, stop the motors, and then drop out of loop
		if(edge > 9) {
			ev3MotorStop(MOTOR_A, MOTOR_FINISH_HOLD);
			ev3MotorStop(MOTOR_B, MOTOR_FINISH_HOLD);
			break;
		}

		count++;
		bar_count++;
		
		}
	}

	if(code39 && k != 0) {
		cout "Invalid Code!" << endl;
		return 0;
	}


	cout << "The number read is: " << total << endl;


  resetInputMode();

  return 0;
}
