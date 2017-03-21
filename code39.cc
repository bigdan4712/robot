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
	int total_count = 0;
	int total = 0;
	int memory[100] = {0};
	int bar_array[10];
	int answer = 0;
	int n = 8;
	int k = 3;


	ev3Setup();
	ev3MotorReset(MOTOR_A);
	ev3MotorReset(MOTOR_B);
	setInputMode();

	ev3ColorSetMode(SENSOR_1,0);
	ev3MotorSetPower(MOTOR_A,-100);
	ev3MotorSetPower(MOTOR_B,-100);

	sleep(1);
	ev3MotorRun(MOTOR_A);
	ev3MotorRun(MOTOR_B);

	while (!escapeButton.isPressed()) {
		
		total_count++;

		//if the sensor count is greater than 20,
		//then the bars are white and make it a 1
		if(count%100 == 0) {
			count = 0;
		}
			 
		cout << ev3ColorReadReflected(SENSOR_1) << endl;
		if(ev3ColorReadReflected(SENSOR_1) > 40) memory[count] = 1;
		else memory[count] = 0;

		//if the count is >= 2, and there is a change in values between 1 and 0, then
		//an edge has been found

		//this algorithm works by by detecting wide vs narrow bars.  for code 39 to work,
		//there must be exactly 3 wide bars in the code.  it takes advantage of the
		//" n choose k" combinatoric function to detect the numbers.  if exactly
		// 3 wide bars are detected, it is a valid code.  if not, the code is invalid.
		
		if((count >= 1) && (memory[count] != memory[count-1])) {
			bar_array[edge] = bar_count;
			edge++;
			cout << "BAR COUNT: " << bar_count << " EDGE COUNT: " << edge << endl;
			bar_count = 0;
		}
		//if robot has passed 10th edge, stop the motors and output detected number
		if(edge > 9) {
			ev3MotorStop(MOTOR_A, MOTOR_FINISH_HOLD);
			ev3MotorStop(MOTOR_B, MOTOR_FINISH_HOLD);
			break;
		}
		count++;
		bar_count++;
	}

	cout << total_count << endl;
	for(int i = 1; i<edge; i++) {
		

		if(bar_array[i] > total_count/8) {
			total = total + choose(n,k);
			k--;
		}
		n--;
	}

	if(k!=0) { 
		cout << "INVALID CODE: TRY AGAIN" << endl;
		return 0;
	}

	cout << "The number read is: " << total << endl;

	resetInputMode();
	return 0;
}


		
			

