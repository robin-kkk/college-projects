#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
using namespace std;

int dim;
int random_min, random_max;
float theta;
float sum;
vector<float> weights;
vector<vector<float> > input_x;
vector<pair<float, float> > output_x(4);

float AND_GATE_IN[2][4] = {
	{0.0, 0.0, 1.0, 1.0},
	{0.0, 1.0, 0.0, 1.0}
};

float AND_GATE_OUT[4] = { 0.0, 0.0, 0.0, 1.0 };

/* Get random value. */
float get_random_number(int min, int max) {
	static const double fraction = 1.0 / (RAND_MAX + 1.0); 
	return min + static_cast<float>((max - min + 1) * (rand() * fraction));
}


/* Initialize weight / theta value, or set x value if dimension > 2. */
void init() {
	theta = get_random_number(random_min, random_max);
	for (int i=0; i<dim; i++) {
		float w = get_random_number(random_min, random_max);
		weights.push_back(w);
	}
	for (int i=2; i<dim; i++) {
		vector<float> xx(4);
		for (int j=0; j<4; j++) {
			xx[j] = -1;
		}
		input_x.push_back(xx);
	}

	for (int i=0; i<4; i++) {
		output_x.push_back(make_pair(0.0, 0.0));
	}
}


/* Update weight and theta value, if wrong output. */
void update() {
	theta = get_random_number(random_min, random_max);
	for (int i=0; i<dim; i++) {
		weights[i] = get_random_number(random_min, random_max);
	}
}


/* It's a learning function for 1-layer perceptron. */
int learn(int input_order) {
	sum = 0.0;
	for (int i=2; i<dim; i++) {
		sum += weights[i] * input_x[i-2][input_order];
	}

	// sum + x1*w1 + x2*w2 = theta
	// x2*w2 = theta - sum - x1*w1
	float left = AND_GATE_IN[1][input_order] * weights[1];
	float right = theta - sum - AND_GATE_IN[0][input_order] * weights[0];

	output_x[input_order].first = left;
	output_x[input_order].second = right;
	return (left < right) ? 0 : 1;
}


/* Print every values to need in calculation. */
void print() {
	cout << "--------------------------------------------------------" << endl;
	cout << "Theta Value : " << theta << endl;
	cout << "Weights : ";
	for (int i=0; i<dim-1; i++) {
		cout << weights[i] << ", ";
	}
	cout << weights[dim-1] << endl;

	cout << endl;
	cout << "\n[Input Fixed Value]" << endl;
	cout << "x1 : 0.0, 0.0, 1.0, 1.0" << endl;
	cout << "x2 : 0.0, 1.0, 0.0, 1.0" << endl;
	for (int i=0; i<dim-2; i++) {
		cout << "x" << i+3 << ": ";
		for (int j=0; j<3; j++) {
			cout << input_x[i][j] << ", ";
		}
		cout << input_x[i][2] << endl;
	}

	cout << "\nx2*w2 = theta - sum - x1*w1" << endl;
	for (int i=0; i<4; i++) {
		cout << "[Output " << i+1 << "]" << endl;	
		cout << "left term : " << output_x[i].first << endl;
		cout << "right term : " << output_x[i].second << endl;
		cout << endl;
	}
	cout << "--------------------------------------------------------" << endl;
}

/* Function for x1 and x2. */
void print_function() {
	cout << "Function: x2 = ";
	float tmp = -(weights[0] / weights[1]);

	cout << tmp << " x1 ";

	tmp = (theta - sum) / weights[1];
	if (tmp >= 0) {
		cout << "+ ";
	}
	cout << tmp << endl;
}

int main() {
	int wrong_cnt = 0;
	long long try_cnt = 1;

	srand(static_cast<unsigned int>(time(0)));

	cout << "Input Dimension (> 2): ";
	cin >> dim;
	cout << "Input Random Range (min, max): ";
	cin >> random_min >> random_max;

	init();
	while (1) {
		wrong_cnt = 0;
		for (int order=0; order<4; order++) {
			int output = learn(order);
			if (output != AND_GATE_OUT[order]) {
				wrong_cnt++;
			}
			print_function();
		}

		cout << "Try Count: " << try_cnt << ", ";
		cout << "Wrong Count: " << wrong_cnt << endl;

		if (wrong_cnt == 0) {
			cout << "ALL PASS Above 4 function" << endl;
			print();
			break;
		}

		update();
		try_cnt++;
	}
	return 0;
}