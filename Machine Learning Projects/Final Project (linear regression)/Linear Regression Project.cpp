/*
 * Linear Regression Project.cpp
 *
 *  Created on: May 7, 2016
 *      Author: Sam Pehr sep73
 */

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <cmath>

using namespace std;

const int DATA_SIZE = 53;
const int k = 5;
const int CUT_VALUE = ceil((DATA_SIZE * 1.0)/k);
const double LEARNING_RATE = 0.0005;

struct City {
	string name;
	double income;
	string income_division;
	double Owner_occupied;
	double Owner_occupied_total;
	double occupants_per_room;
	double occupants_per_room_total;
	double houses;
	double mobile_homes;
	double boat;
	double house_total;
	double house_1_year_ago;
	double house_1_year_ago_total;
	double less_than_high_school_m;
	double less_than_high_school_f;
	double high_school_m;
	double high_school_f;
	double bachelor_m;
	double bachelor_f;
	double school_total;
};

City data[DATA_SIZE];

void inputData();
double Hypothesis (double, double, double, double, double, double, double, double, double, double, City);
double Cost(double, double, double, double, double, double, double, double, double, double, int);
City Gradient(double, double, double, double, double, double, double, double, double, double, int);

int main() {
	inputData();


	bool convergence = false;
	double cost;
	double previous_cost = 0;
	int count = 0;

	for(int k_count = 0; k_count < k; k_count++){

		double thetaIntercept  = 1;
		double thetaOwner_occupied = 1;
		double thetaOccupants_per_room = 1;
		double thetaHouses = 1;
		double thetaMobile_homes = 1;
		double thetaBoat = 1;
		double thetaHouse_1_year_ago = 1;
		double thetaLess_than_high_school_m = 1;
		double thetaHigh_school_m = 1;
		double thetaBachelor_m = 1;

		bool convergence = false;
		double cost;
		double previous_cost = 0;
		int count = 0;


		while (convergence == false) {
			cost = Cost(thetaIntercept,  thetaOwner_occupied, thetaOccupants_per_room,
					 thetaHouses,  thetaMobile_homes,  thetaBoat,
					 thetaHouse_1_year_ago, thetaLess_than_high_school_m,
					 thetaHigh_school_m,   thetaBachelor_m, k_count);
			cost = cost / DATA_SIZE;
			if(abs(cost - previous_cost) < .0001) {
				convergence = true;
				cout << "fucking did it" << endl;
			}
			else {
				count++;

				City newGradients = Gradient(thetaIntercept, thetaOwner_occupied, thetaOccupants_per_room, thetaHouses,
						  thetaMobile_homes, thetaBoat, thetaHouse_1_year_ago, thetaLess_than_high_school_m,
						  thetaHigh_school_m, thetaBachelor_m, k_count);

				thetaIntercept += newGradients.income;
				thetaOwner_occupied += newGradients.Owner_occupied;
				thetaOccupants_per_room  += newGradients.occupants_per_room;
				thetaHouses  += newGradients.houses;
				thetaMobile_homes  += newGradients.mobile_homes;
				thetaBoat  += newGradients.boat;
				thetaHouse_1_year_ago  += newGradients.house_1_year_ago;
				thetaLess_than_high_school_m  += newGradients.less_than_high_school_m;
				thetaHigh_school_m  += newGradients.high_school_m;
				thetaBachelor_m  += newGradients.bachelor_m;

				if(count > 300000) {
					convergence = true;
				}

				previous_cost = cost;
			}

		}

		cout << endl << endl << "section used as test data: " << k_count << endl;
		cout << "intercept weight: " << thetaIntercept << endl;
		cout << "Owner_occupied weight: " << thetaOwner_occupied << endl;
		cout << "Occupants_per_room weight: " << thetaOccupants_per_room << endl;
		cout << "houses weight: " << thetaHouses << endl;
		cout << "mobile_homes weight: " << thetaMobile_homes << endl;
		cout << "boat weight: " << thetaBoat << endl;
		cout << "house_1_year_ago weight: " << thetaHouse_1_year_ago << endl;
		cout << "less_than_high_school weight: " << thetaLess_than_high_school_m << endl;
		cout << "high_school weight: " << thetaHigh_school_m << endl;
		cout << "bachelor weight: " << thetaBachelor_m  << endl << endl;

		int outside_count = 0;
		int correct = 0;
		for(int count = k_count*CUT_VALUE; count < (k_count+1)*CUT_VALUE; count++) {
			double hypo = Hypothesis(thetaIntercept,  thetaOwner_occupied, thetaOccupants_per_room,
						 	 thetaHouses,  thetaMobile_homes,  thetaBoat,
						 	 thetaHouse_1_year_ago, thetaLess_than_high_school_m,
						 	 thetaHigh_school_m, thetaBachelor_m, data[count]);
			string income_division;

			if(hypo < 10000) {
				income_division = "Less than $10,000";
			}
			else if (hypo < 14999) {
				income_division = "$10,000 to $14,999";
			}
			else if (hypo < 19999) {
				income_division = "$15,000 to $19,999";
			}
			else if (hypo < 24999) {
				income_division = "$20,000 to $24,999";
			}
			else if (hypo < 29999) {
				income_division = "$25,000 to $29,999";
			}
			else if (hypo < 34999) {
				income_division = "$30,000 to $34,999";
			}
			else if (hypo < 39999) {
				income_division = "$35,000 to $39,999";
			}
			else if (hypo < 44999) {
				income_division = "$40,000 to $44,999";
			}
			else if (hypo < 49999) {
				income_division = "$45,000 to $49,999";
			}
			else if (hypo < 59999) {
				income_division = "$50,000 to $59,999";
			}
			else if (hypo < 74999) {
				income_division = "$60,000 to $74,999";
			}
			else if (hypo < 99999) {
				income_division = "$75,000 to $99,999";
			}
			else if (hypo < 124999) {
				income_division = "$100,000 to $124,999";
			}
			else if (hypo < 149999) {
				income_division = "$125,000 to $149,999";
			}
			else if (hypo < 199999) {
				income_division = "$150,000 to $199,999";
			}
			else if (hypo >= 200000) {
				income_division = "$200,000 or more";
			}
			else {
				cout << "how the hell did you end up here" << endl;
				assert(0);
			}

			if(data[count].income_division.compare(income_division) == 0) {
				correct++;
			}

			cout << "test data #" << outside_count+1 << " predicted result: " << hypo << " classified as: " <<
					income_division << " actual result: " << data[count].income << " " << data[count].income_division << endl;

			outside_count++;
			if(count+1 >= DATA_SIZE) {
				count = k*CUT_VALUE;
			}
		}

		double accuracy = (correct*1.0)/(outside_count-1);
		cout << "accuracy: " << accuracy << endl;
	}
	return 0;
}

double Hypothesis (double thetaIntercept, double thetaOwner_occupied, double thetaOccupants_per_room, double thetaHouses,
		double thetaMobile_homes, double thetaBoat, double thetaHouse_1_year_ago, double thetaLess_than_high_school_m,
		double thetaHigh_school_m, double thetaBachelor_m, City values) {

	return thetaIntercept + thetaOwner_occupied*values.Owner_occupied + thetaOccupants_per_room*values.occupants_per_room +
			thetaHouses*values.houses + thetaMobile_homes*values.mobile_homes + thetaBoat*values.boat +
			thetaHouse_1_year_ago*values.house_1_year_ago + thetaLess_than_high_school_m*values.less_than_high_school_m +
			thetaHigh_school_m*values.high_school_m + thetaBachelor_m*values.bachelor_m;
}

double Cost(double thetaIntercept, double thetaOwner_occupied, double thetaOccupants_per_room, double thetaHouses,
		double thetaMobile_homes, double thetaBoat, double thetaHouse_1_year_ago, double thetaLess_than_high_school_m,
		double thetaHigh_school_m, double thetaBachelor_m, int k_count){
	double cost = 0;
	for (int cut_count = 0; cut_count < k; cut_count++) {

		if (cut_count != k_count) {
			for(int count = cut_count*CUT_VALUE; count < (cut_count+1)*CUT_VALUE; count++) {
				double hypo = Hypothesis(thetaIntercept,  thetaOwner_occupied,  thetaOccupants_per_room, thetaHouses,
						thetaMobile_homes,  thetaBoat,
						 thetaHouse_1_year_ago, thetaLess_than_high_school_m, thetaHigh_school_m, thetaBachelor_m, data[count]);
				cost += pow(hypo - data[count].income, 2);
				if(count+1 >= DATA_SIZE) {
					count = k*CUT_VALUE;
				}
			}
		}

	}
	cost = cost / 2;
	return cost;
}

City Gradient(double thetaIntercept, double thetaOwner_occupied, double thetaOccupants_per_room, double thetaHouses,
		double thetaMobile_homes, double thetaBoat, double thetaHouse_1_year_ago, double thetaLess_than_high_school_m,
		double thetaHigh_school_m, double thetaBachelor_m, int k_count){

	double interceptGradient = 0;
	double Owner_occupiedGradient = 0;
	double occupants_per_roomGradient = 0;
	double housesGradient = 0;
	double mobile_homesGradient = 0;
	double boatGradient = 0;
	double house_1_year_agoGradient = 0;
	double less_than_high_school_mGradient = 0;
	double high_school_mGradient = 0;
	double bachelor_mGradient = 0;

	for (int cut_count = 0; cut_count < k; cut_count++) {
		if (cut_count != k_count) {
			for (int count = cut_count*CUT_VALUE; count < (cut_count+1)*CUT_VALUE; count++) {
				 double hypo = Hypothesis(thetaIntercept,  thetaOwner_occupied, thetaOccupants_per_room,
						 thetaHouses,  thetaMobile_homes,  thetaBoat,
						 thetaHouse_1_year_ago, thetaLess_than_high_school_m,
						 thetaHigh_school_m, thetaBachelor_m,
						 data[count]);

				interceptGradient = interceptGradient + (data[count].income - hypo);
				Owner_occupiedGradient = Owner_occupiedGradient + (data[count].income - hypo) * data[count].Owner_occupied;
				occupants_per_roomGradient = occupants_per_roomGradient + (data[count].income - hypo) * data[count].occupants_per_room;
				housesGradient = housesGradient + (data[count].income - hypo) * data[count].houses;
				mobile_homesGradient = mobile_homesGradient + (data[count].income - hypo) * data[count].mobile_homes;
				boatGradient = boatGradient + (data[count].income - hypo) * data[count].boat;
				house_1_year_agoGradient = house_1_year_agoGradient + (data[count].income - hypo) * data[count].house_1_year_ago;
				less_than_high_school_mGradient = less_than_high_school_mGradient + (data[count].income - hypo) * data[count].less_than_high_school_m;
				high_school_mGradient = high_school_mGradient + (data[count].income - hypo) * data[count].high_school_m;
				bachelor_mGradient = bachelor_mGradient + (data[count].income - hypo) * data[count].bachelor_m;

				if(count+1 >= DATA_SIZE) {
					count = (k+1)*CUT_VALUE;
				}
			}
		}

	}

	interceptGradient = interceptGradient * LEARNING_RATE;
	Owner_occupiedGradient = Owner_occupiedGradient * LEARNING_RATE;
	occupants_per_roomGradient = occupants_per_roomGradient * LEARNING_RATE;
	housesGradient = housesGradient * LEARNING_RATE;
	mobile_homesGradient = mobile_homesGradient * LEARNING_RATE;
	boatGradient = boatGradient * LEARNING_RATE;
	house_1_year_agoGradient = house_1_year_agoGradient * LEARNING_RATE;
	less_than_high_school_mGradient = less_than_high_school_mGradient * LEARNING_RATE;
	high_school_mGradient = high_school_mGradient * LEARNING_RATE;
	bachelor_mGradient = bachelor_mGradient * LEARNING_RATE;

	City gradients;
	gradients.income = interceptGradient;//using income to store intercept gradient, because income has no gradient
	gradients.Owner_occupied = Owner_occupiedGradient;
	gradients.occupants_per_room = occupants_per_roomGradient;
	gradients.houses = housesGradient;
	gradients.mobile_homes = mobile_homesGradient;
	gradients.boat = boatGradient;
	gradients.house_1_year_ago = house_1_year_agoGradient;
	gradients.less_than_high_school_m = less_than_high_school_mGradient;
	gradients.high_school_m = high_school_mGradient;
	gradients.bachelor_m = bachelor_mGradient;

	return gradients;
}

void inputData() {


	ifstream data_input;

	data_input.open("data.txt");
	string input;
	int next_semicolon;
	string input_slice;
	string input_remainder;
	string input_smaller_slice;
	int colon_location;

	City minCity;
	minCity.Owner_occupied = 2147483647;
	minCity.Owner_occupied_total = 2147483647;
	minCity.occupants_per_room = 2147483647;
	minCity.occupants_per_room_total = 2147483647;
	minCity.houses = 2147483647;
	minCity.mobile_homes = 2147483647;
	minCity.boat = 2147483647;
	minCity.house_total = 2147483647;
	minCity.house_1_year_ago = 2147483647;
	minCity.house_1_year_ago_total = 2147483647;
	minCity.less_than_high_school_m = 2147483647;
	minCity.less_than_high_school_f = 2147483647;
	minCity.high_school_m = 2147483647;
	minCity.high_school_f = 2147483647;
	minCity.bachelor_m = 2147483647;
	minCity.bachelor_f = 2147483647;
	minCity.school_total = 2147483647;

	City maxCity;
	maxCity.Owner_occupied = 0;
	maxCity.Owner_occupied_total = 0;
	maxCity.occupants_per_room = 0;
	maxCity.occupants_per_room_total = 0;
	maxCity.houses = 0;
	maxCity.mobile_homes = 0;
	maxCity.boat = 0;
	maxCity.house_total = 0;
	maxCity.house_1_year_ago = 0;
	maxCity.house_1_year_ago_total = 0;
	maxCity.less_than_high_school_m = 0;
	maxCity.less_than_high_school_f = 0;
	maxCity.high_school_m = 0;
	maxCity.high_school_f = 0;
	maxCity.bachelor_m = 0;
	maxCity.bachelor_f = 0;
	maxCity.school_total = 0;

	int count = 0;
	while(getline(data_input, input)) {

		input_remainder = input;
		next_semicolon = input_remainder.find(';');
		input_slice = input_remainder.substr(0, next_semicolon);

		data[count].name = input_slice;

		input_remainder = input_remainder.substr(next_semicolon+1);
		next_semicolon = input_remainder.find(';');
		input_slice = input_remainder.substr(0, next_semicolon);
		colon_location = input_slice.find(':');
		input_smaller_slice = input_slice.substr(0, colon_location);
		if(input_smaller_slice.compare("Mean income (dollars)") != 0) {
			cout << "ERROR: expected income field is not indicated by 'Owner occupied'" << endl;
			cout << "field: income" << endl;
			cout << "received input: " << input_smaller_slice << endl;
			cout << "city name: " << data[count].name << endl;
			cout << "received line: " << input << endl;
			assert(0);
		}
		else {
			input_smaller_slice = input_slice.substr(colon_location+1);

			for(int i=0; i<=input_smaller_slice.size(); i++) {  //Loop through the entire string
				if(input_smaller_slice[i]==',') { //Check for occurence of SEP_SYMB
					input_smaller_slice.erase(i,1); //If SEP_SYMB , remove that single character
				}
			}

			if(input_smaller_slice.find_last_not_of("1234567890") != std::string::npos) {
				cout << "ERROR: expected integer field has nonnumeric characters" << endl;
				cout << "field: income_division" << endl;
				cout << "received input: " << input_smaller_slice << endl;
				cout << "city name: " << data[count].name << endl;
				cout << "received line: " << input << endl;
				assert(0);
			}

			data[count].income = atof(input_smaller_slice.c_str());

			if(data[count].income < minCity.income) {
				minCity.income = data[count].income;
			}
			if(data[count].income > maxCity.income) {
				minCity.income = data[count].income;
			}
		}

		input_remainder = input_remainder.substr(next_semicolon+1);
		next_semicolon = input_remainder.find(';');
		input_slice = input_remainder.substr(0, next_semicolon);
		colon_location = input_slice.find(':');
		input_smaller_slice = input_slice.substr(0, colon_location);
		if(input_smaller_slice.compare("income range") != 0) {
			cout << "ERROR: expected income field is not indicated by 'income range'" << endl;
			cout << "field: income_division" << endl;
			cout << "received input: " << input_smaller_slice << endl;
			cout << "city name: " << data[count].name << endl;
			cout << "received line: " << input << endl;
			assert(0);
		}
		else {
			input_smaller_slice = input_slice.substr(colon_location+1);
			data[count].income_division = input_smaller_slice;
		}


		input_remainder = input_remainder.substr(next_semicolon+1);
		next_semicolon = input_remainder.find(';');
		input_slice = input_remainder.substr(0, next_semicolon);
		colon_location = input_slice.find(':');
		input_smaller_slice = input_slice.substr(0, colon_location);
		if(input_smaller_slice.compare("Owner occupied") != 0) {
			cout << "ERROR: expected income field is not indicated by 'Owner occupied'" << endl;
			cout << "field: Owner occupied" << endl;
			cout << "received input: " << input_smaller_slice << endl;
			cout << "city name: " << data[count].name << endl;
			cout << "received line: " << input << endl;
			assert(0);
		}
		else {
			input_smaller_slice = input_slice.substr(colon_location+1);

			for(int i=0; i<=input_smaller_slice.size(); i++) {  //Loop through the entire string
				if(input_smaller_slice[i]==',') { //Check for occurence of SEP_SYMB
					input_smaller_slice.erase(i,1); //If SEP_SYMB , remove that single character
				}
			}

			if(input_smaller_slice.find_last_not_of("1234567890") != std::string::npos) {
				cout << "ERROR: expected integer field has nonnumeric characters" << endl;
				cout << "field: Owner occupied" << endl;
				cout << "received input: " << input_smaller_slice << endl;
				cout << "city name: " << data[count].name << endl;
				cout << "received line: " << input << endl;
				assert(0);
			}

			data[count].Owner_occupied = atof(input_smaller_slice.c_str());

			if(data[count].Owner_occupied < minCity.Owner_occupied) {
				minCity.Owner_occupied = data[count].Owner_occupied;
			}
			if(data[count].Owner_occupied > maxCity.Owner_occupied) {
				minCity.Owner_occupied = data[count].Owner_occupied;
			}
		}

		input_remainder = input_remainder.substr(next_semicolon+1);
		next_semicolon = input_remainder.find(';');
		input_slice = input_remainder.substr(0, next_semicolon);
		colon_location = input_slice.find(':');
		input_smaller_slice = input_slice.substr(0, colon_location);
		if(input_smaller_slice.compare("Total") != 0) {
			cout << "ERROR: expected income field is not indicated by 'Total'" << endl;
			cout << "field: Owner_occupied_total" << endl;
			cout << "received input: " << input_smaller_slice << endl;
			cout << "city name: " << data[count].name << endl;
			cout << "received line: " << input << endl;
			assert(0);
		}
		else {
			input_smaller_slice = input_slice.substr(colon_location+1);

			for(int i=0; i<=input_smaller_slice.size(); i++) {  //Loop through the entire string
				if(input_smaller_slice[i]==',') { //Check for occurence of SEP_SYMB
					input_smaller_slice.erase(i,1); //If SEP_SYMB , remove that single character
				}
			}

			if(input_smaller_slice.find_first_not_of("1234567890") != string::npos) {
				cout << "ERROR: expected integer field has nonnumeric characters" << endl;
				cout << "field: Owner_occupied_total" << endl;
				cout << "received input: " << input_smaller_slice << endl;
				cout << "city name: " << data[count].name << endl;
				cout << "received line: " << input << endl;
				assert(0);
			}

			data[count].Owner_occupied_total = atof(input_smaller_slice.c_str());

			if(data[count].Owner_occupied_total < minCity.Owner_occupied_total) {
				minCity.Owner_occupied_total = data[count].Owner_occupied_total;
			}
			if(data[count].Owner_occupied_total > maxCity.Owner_occupied_total) {
				minCity.Owner_occupied_total = data[count].Owner_occupied_total;
			}
		}

		input_remainder = input_remainder.substr(next_semicolon+1);
		next_semicolon = input_remainder.find(';');
		input_slice = input_remainder.substr(0, next_semicolon);
		colon_location = input_slice.find(':');
		input_smaller_slice = input_slice.substr(0, colon_location);
		if(input_smaller_slice.compare("1.01 or more occupants per room") != 0) {
			cout << "ERROR: expected income field is not indicated by '1.01 or more occupants per room'" << endl;
			cout << "field: occupants_per_room" << endl;
			cout << "received input: " << input_smaller_slice << endl;
			cout << "city name: " << data[count].name << endl;
			cout << "received line: " << input << endl;
			assert(0);
		}
		else {
			input_smaller_slice = input_slice.substr(colon_location+1);

			for(int i=0; i<=input_smaller_slice.size(); i++) {  //Loop through the entire string
				if(input_smaller_slice[i]==',') { //Check for occurence of SEP_SYMB
					input_smaller_slice.erase(i,1); //If SEP_SYMB , remove that single character
				}
			}

			if(input_smaller_slice.find_first_not_of("1234567890") != string::npos) {
				cout << "ERROR: expected integer field has nonnumeric characters" << endl;
				cout << "field: occupants_per_room" << endl;
				cout << "received input: " << input_smaller_slice << endl;
				cout << "city name: " << data[count].name << endl;
				cout << "received line: " << input << endl;
				assert(0);
			}

			data[count].occupants_per_room = atof(input_smaller_slice.c_str());

			if(data[count].occupants_per_room < minCity.occupants_per_room) {
				minCity.occupants_per_room = data[count].occupants_per_room;
			}
			if(data[count].occupants_per_room > maxCity.occupants_per_room) {
				minCity.occupants_per_room = data[count].occupants_per_room;
			}
		}

		input_remainder = input_remainder.substr(next_semicolon+1);
		next_semicolon = input_remainder.find(';');
		input_slice = input_remainder.substr(0, next_semicolon);
		colon_location = input_slice.find(':');
		input_smaller_slice = input_slice.substr(0, colon_location);
		if(input_smaller_slice.compare("Total") != 0) {
			cout << "ERROR: expected income field is not indicated by 'Total'" << endl;
			cout << "field: occupants_per_room_total" << endl;
			cout << "received input: " << input_smaller_slice << endl;
			cout << "city name: " << data[count].name << endl;
			cout << "received line: " << input << endl;
			assert(0);
		}
		else {
			input_smaller_slice = input_slice.substr(colon_location+1);

			for(int i=0; i<=input_smaller_slice.size(); i++) {  //Loop through the entire string
				if(input_smaller_slice[i]==',') { //Check for occurence of SEP_SYMB
					input_smaller_slice.erase(i,1); //If SEP_SYMB , remove that single character
				}
			}

			if(input_smaller_slice.find_first_not_of("1234567890") != string::npos) {
				cout << "ERROR: expected integer field has nonnumeric characters" << endl;
				cout << "field: occupants_per_room_total" << endl;
				cout << "received input: " << input_smaller_slice << endl;
				cout << "city name: " << data[count].name << endl;
				cout << "received line: " << input << endl;
				assert(0);
			}

			data[count].occupants_per_room_total = atof(input_smaller_slice.c_str());

			if(data[count].occupants_per_room_total < minCity.occupants_per_room_total) {
				minCity.occupants_per_room_total = data[count].occupants_per_room_total;
			}
			if(data[count].occupants_per_room_total > maxCity.occupants_per_room_total) {
				minCity.occupants_per_room_total = data[count].occupants_per_room_total;
			}
		}

		input_remainder = input_remainder.substr(next_semicolon+1);
		next_semicolon = input_remainder.find(';');
		input_slice = input_remainder.substr(0, next_semicolon);
		colon_location = input_slice.find(':');
		input_smaller_slice = input_slice.substr(0, colon_location);
		if(input_smaller_slice.compare("1, detached") != 0) {
			cout << "ERROR: expected income field is not indicated by '1, detached'" << endl;
			cout << "field: houses" << endl;
			cout << "received input: " << input_smaller_slice << endl;
			cout << "city name: " << data[count].name << endl;
			cout << "received line: " << input << endl;
			assert(0);
		}
		else {
			input_smaller_slice = input_slice.substr(colon_location+1);

			for(int i=0; i<=input_smaller_slice.size(); i++) {  //Loop through the entire string
				if(input_smaller_slice[i]==',') { //Check for occurence of SEP_SYMB
					input_smaller_slice.erase(i,1); //If SEP_SYMB , remove that single character
				}
			}

			if(input_smaller_slice.find_first_not_of("1234567890") != string::npos) {
				cout << "ERROR: expected integer field has nonnumeric characters" << endl;
				cout << "field: houses" << endl;
				cout << "received input: " << input_smaller_slice << endl;
				cout << "city name: " << data[count].name << endl;
				cout << "received line: " << input << endl;
				assert(0);
			}

			data[count].houses = atof(input_smaller_slice.c_str());

			if(data[count].houses < minCity.houses) {
				minCity.houses = data[count].houses;
			}
			if(data[count].houses > maxCity.houses) {
				minCity.houses = data[count].houses;
			}
		}

		input_remainder = input_remainder.substr(next_semicolon+1);
		next_semicolon = input_remainder.find(';');
		input_slice = input_remainder.substr(0, next_semicolon);
		colon_location = input_slice.find(':');
		input_smaller_slice = input_slice.substr(0, colon_location);
		if(input_smaller_slice.compare("Mobile home") != 0) {
			cout << "ERROR: expected income field is not indicated by 'Mobile home'" << endl;
			cout << "field: mobile_homes" << endl;
			cout << "received input: " << input_smaller_slice << endl;
			cout << "city name: " << data[count].name << endl;
			cout << "received line: " << input << endl;
			assert(0);
		}
		else {
			input_smaller_slice = input_slice.substr(colon_location+1);

			for(int i=0; i<=input_smaller_slice.size(); i++) {  //Loop through the entire string
				if(input_smaller_slice[i]==',') { //Check for occurence of SEP_SYMB
					input_smaller_slice.erase(i,1); //If SEP_SYMB , remove that single character
				}
			}

			if(input_smaller_slice.find_first_not_of("1234567890") != string::npos) {
				cout << "ERROR: expected integer field has nonnumeric characters" << endl;
				cout << "field: mobile_homes" << endl;
				cout << "received input: " << input_smaller_slice << endl;
				cout << "city name: " << data[count].name << endl;
				cout << "received line: " << input << endl;
				assert(0);
			}

			data[count].mobile_homes = atof(input_smaller_slice.c_str());

			if(data[count].mobile_homes < minCity.mobile_homes) {
				minCity.mobile_homes = data[count].mobile_homes;
			}
			if(data[count].mobile_homes > maxCity.mobile_homes) {
				minCity.mobile_homes = data[count].mobile_homes;
			}
		}

		input_remainder = input_remainder.substr(next_semicolon+1);
		next_semicolon = input_remainder.find(';');
		input_slice = input_remainder.substr(0, next_semicolon);
		colon_location = input_slice.find(':');
		input_smaller_slice = input_slice.substr(0, colon_location);
		if(input_smaller_slice.compare("Boat, RV, van, etc.") != 0) {
			cout << "ERROR: expected income field is not indicated by 'Boat, RV, van, etc.'" << endl;
			cout << "field: boat" << endl;
			cout << "received input: " << input_smaller_slice << endl;
			cout << "city name: " << data[count].name << endl;
			cout << "received line: " << input << endl;
			assert(0);
		}
		else {
			input_smaller_slice = input_slice.substr(colon_location+1);

			for(int i=0; i<=input_smaller_slice.size(); i++) {  //Loop through the entire string
				if(input_smaller_slice[i]==',') { //Check for occurence of SEP_SYMB
					input_smaller_slice.erase(i,1); //If SEP_SYMB , remove that single character
				}
			}

			if(input_smaller_slice.find_first_not_of("1234567890") != string::npos) {
				cout << "ERROR: expected integer field has nonnumeric characters" << endl;
				cout << "field: boat" << endl;
				cout << "received input: " << input_smaller_slice << endl;
				cout << "city name: " << data[count].name << endl;
				cout << "received line: " << input << endl;
				assert(0);
			}

			data[count].boat = atof(input_smaller_slice.c_str());

			if(data[count].boat < minCity.boat) {
				minCity.boat = data[count].boat;
			}
			if(data[count].boat > maxCity.boat) {
				minCity.boat = data[count].boat;
			}
		}

		input_remainder = input_remainder.substr(next_semicolon+1);
		next_semicolon = input_remainder.find(';');
		input_slice = input_remainder.substr(0, next_semicolon);
		colon_location = input_slice.find(':');
		input_smaller_slice = input_slice.substr(0, colon_location);
		if(input_smaller_slice.compare("Total") != 0) {
			cout << "ERROR: expected income field is not indicated by 'Boat, RV, van, etc.'" << endl;
			cout << "field: house_total" << endl;
			cout << "received input: " << input_smaller_slice << endl;
			cout << "city name: " << data[count].name << endl;
			cout << "received line: " << input << endl;
			assert(0);
		}
		else {
			input_smaller_slice = input_slice.substr(colon_location+1);

			for(int i=0; i<=input_smaller_slice.size(); i++) {  //Loop through the entire string
				if(input_smaller_slice[i]==',') { //Check for occurence of SEP_SYMB
					input_smaller_slice.erase(i,1); //If SEP_SYMB , remove that single character
				}
			}

			if(input_smaller_slice.find_first_not_of("1234567890") != string::npos) {
				cout << "ERROR: expected integer field has nonnumeric characters" << endl;
				cout << "field: house_total" << endl;
				cout << "received input: " << input_smaller_slice << endl;
				cout << "city name: " << data[count].name << endl;
				cout << "received line: " << input << endl;
				assert(0);
			}

			data[count].house_total = atof(input_smaller_slice.c_str());

			if(data[count].house_total < minCity.house_total) {
				minCity.house_total = data[count].house_total;
			}
			if(data[count].house_total > maxCity.house_total) {
				minCity.house_total = data[count].house_total;
			}
		}

		input_remainder = input_remainder.substr(next_semicolon+1);
		next_semicolon = input_remainder.find(';');
		input_slice = input_remainder.substr(0, next_semicolon);
		colon_location = input_slice.find(':');
		input_smaller_slice = input_slice.substr(0, colon_location);
		if(input_smaller_slice.compare("Same house 1 year ago") != 0) {
			cout << "ERROR: expected income field is not indicated by 'Same house 1 year ago'" << endl;
			cout << "field: house_1_year_ago" << endl;
			cout << "received input: " << input_smaller_slice << endl;
			cout << "city name: " << data[count].name << endl;
			cout << "received line: " << input << endl;
			assert(0);
		}
		else {
			input_smaller_slice = input_slice.substr(colon_location+1);

			for(int i=0; i<=input_smaller_slice.size(); i++) {  //Loop through the entire string
				if(input_smaller_slice[i]==',') { //Check for occurence of SEP_SYMB
					input_smaller_slice.erase(i,1); //If SEP_SYMB , remove that single character
				}
			}

			if(input_smaller_slice.find_first_not_of("1234567890") != string::npos) {
				cout << "ERROR: expected integer field has nonnumeric characters" << endl;
				cout << "field: house_1_year_ago" << endl;
				cout << "received input: " << input_smaller_slice << endl;
				cout << "city name: " << data[count].name << endl;
				cout << "received line: " << input << endl;
				assert(0);
			}

			data[count].house_1_year_ago = atof(input_smaller_slice.c_str());

			if(data[count].house_1_year_ago < minCity.house_1_year_ago) {
				minCity.house_1_year_ago = data[count].house_1_year_ago;
			}
			if(data[count].house_1_year_ago > maxCity.house_1_year_ago) {
				minCity.house_1_year_ago = data[count].house_1_year_ago;
			}
		}

		input_remainder = input_remainder.substr(next_semicolon+1);
		next_semicolon = input_remainder.find(';');
		input_slice = input_remainder.substr(0, next_semicolon);
		colon_location = input_slice.find(':');
		input_smaller_slice = input_slice.substr(0, colon_location);
		if(input_smaller_slice.compare("Total") != 0) {
			cout << "ERROR: expected income field is not indicated by 'Total'" << endl;
			cout << "field: house_1_year_ago_total" << endl;
			cout << "received input: " << input_smaller_slice << endl;
			cout << "city name: " << data[count].name << endl;
			cout << "received line: " << input << endl;
			assert(0);
		}
		else {
			input_smaller_slice = input_slice.substr(colon_location+1);

			for(int i=0; i<=input_smaller_slice.size(); i++) {  //Loop through the entire string
				if(input_smaller_slice[i]==',') { //Check for occurence of SEP_SYMB
					input_smaller_slice.erase(i,1); //If SEP_SYMB , remove that single character
				}
			}

			if(input_smaller_slice.find_first_not_of("1234567890") != string::npos) {
				cout << "ERROR: expected integer field has nonnumeric characters" << endl;
				cout << "field: house_1_year_ago_total" << endl;
				cout << "received input: " << input_smaller_slice << endl;
				cout << "city name: " << data[count].name << endl;
				cout << "received line: " << input << endl;
				assert(0);
			}

			data[count].house_1_year_ago_total = atof(input_smaller_slice.c_str());

			if(data[count].house_1_year_ago_total < minCity.house_1_year_ago_total) {
				minCity.house_1_year_ago_total = data[count].house_1_year_ago_total;
			}
			if(data[count].house_1_year_ago_total > maxCity.house_1_year_ago_total) {
				minCity.house_1_year_ago_total = data[count].house_1_year_ago_total;
			}
		}

		input_remainder = input_remainder.substr(next_semicolon+1);
		next_semicolon = input_remainder.find(';');
		input_slice = input_remainder.substr(0, next_semicolon);
		colon_location = input_slice.find(':');
		input_smaller_slice = input_slice.substr(0, colon_location);
		if(input_smaller_slice.compare("Less than high school diploma") != 0) {
			cout << "ERROR: expected income field is not indicated by 'Less than high school diploma'" << endl;
			cout << "field: less_than_high_school_m" << endl;
			cout << "received input: " << input_smaller_slice << endl;
			cout << "city name: " << data[count].name << endl;
			cout << "received line: " << input << endl;
			assert(0);
		}
		else {
			input_smaller_slice = input_slice.substr(colon_location+1);

			for(int i=0; i<=input_smaller_slice.size(); i++) {  //Loop through the entire string
				if(input_smaller_slice[i]==',') { //Check for occurence of SEP_SYMB
					input_smaller_slice.erase(i,1); //If SEP_SYMB , remove that single character
				}
			}

			if(input_smaller_slice.find_first_not_of("1234567890") != string::npos) {
				cout << "ERROR: expected integer field has nonnumeric characters" << endl;
				cout << "field: less_than_high_school_m" << endl;
				cout << "received input: " << input_smaller_slice << endl;
				cout << "city name: " << data[count].name << endl;
				cout << "received line: " << input << endl;
				assert(0);
			}

			data[count].less_than_high_school_m = atof(input_smaller_slice.c_str());

			if(data[count].less_than_high_school_m < minCity.less_than_high_school_m) {
				minCity.less_than_high_school_m = data[count].less_than_high_school_m;
			}
			if(data[count].less_than_high_school_m > maxCity.less_than_high_school_m) {
				minCity.less_than_high_school_m = data[count].less_than_high_school_m;
			}
		}

		input_remainder = input_remainder.substr(next_semicolon+1);
		next_semicolon = input_remainder.find(';');
		input_slice = input_remainder.substr(0, next_semicolon);
		colon_location = input_slice.find(':');
		input_smaller_slice = input_slice.substr(0, colon_location);
		if(input_smaller_slice.compare("Less than high school diploma") != 0) {
			cout << "ERROR: expected income field is not indicated by 'Less than high school diploma'" << endl;
			cout << "field: less_than_high_school_f" << endl;
			cout << "received input: " << input_smaller_slice << endl;
			cout << "city name: " << data[count].name << endl;
			cout << "received line: " << input << endl;
			assert(0);
		}
		else {
			input_smaller_slice = input_slice.substr(colon_location+1);

			for(int i=0; i<=input_smaller_slice.size(); i++) {  //Loop through the entire string
				if(input_smaller_slice[i]==',') { //Check for occurence of SEP_SYMB
					input_smaller_slice.erase(i,1); //If SEP_SYMB , remove that single character
				}
			}

			if(input_smaller_slice.find_first_not_of("1234567890") != string::npos) {
				cout << "ERROR: expected integer field has nonnumeric characters" << endl;
				cout << "field: less_than_high_school_f" << endl;
				cout << "received input: " << input_smaller_slice << endl;
				cout << "city name: " << data[count].name << endl;
				cout << "received line: " << input << endl;
				assert(0);
			}

			data[count].less_than_high_school_f = atof(input_smaller_slice.c_str());

			if(data[count].less_than_high_school_f < minCity.less_than_high_school_f) {
				minCity.less_than_high_school_f = data[count].less_than_high_school_f;
			}
			if(data[count].less_than_high_school_f > maxCity.less_than_high_school_f) {
				minCity.less_than_high_school_f = data[count].less_than_high_school_f;
			}
		}

		input_remainder = input_remainder.substr(next_semicolon+1);
		next_semicolon = input_remainder.find(';');
		input_slice = input_remainder.substr(0, next_semicolon);
		colon_location = input_slice.find(':');
		input_smaller_slice = input_slice.substr(0, colon_location);
		if(input_smaller_slice.compare("High school graduate, GED, or alternative") != 0) {
			cout << "ERROR: expected income field is not indicated by 'High school graduate, GED, or alternative'" << endl;
			cout << "field: high_school_m" << endl;
			cout << "received input: " << input_smaller_slice << endl;
			cout << "city name: " << data[count].name << endl;
			cout << "received line: " << input << endl;
			assert(0);
		}
		else {
			input_smaller_slice = input_slice.substr(colon_location+1);

			for(int i=0; i<=input_smaller_slice.size(); i++) {  //Loop through the entire string
				if(input_smaller_slice[i]==',') { //Check for occurence of SEP_SYMB
					input_smaller_slice.erase(i,1); //If SEP_SYMB , remove that single character
				}
			}

			if(input_smaller_slice.find_first_not_of("1234567890") != string::npos) {
				cout << "ERROR: expected integer field has nonnumeric characters" << endl;
				cout << "field: high_school_m" << endl;
				cout << "received input: " << input_smaller_slice << endl;
				cout << "city name: " << data[count].name << endl;
				cout << "received line: " << input << endl;
				assert(0);
			}

			data[count].high_school_m = atof(input_smaller_slice.c_str());

			if(data[count].high_school_m < minCity.high_school_m) {
				minCity.high_school_m = data[count].high_school_m;
			}
			if(data[count].high_school_m > maxCity.high_school_m) {
				minCity.high_school_m = data[count].high_school_m;
			}
		}

		input_remainder = input_remainder.substr(next_semicolon+1);
		next_semicolon = input_remainder.find(';');
		input_slice = input_remainder.substr(0, next_semicolon);
		colon_location = input_slice.find(':');
		input_smaller_slice = input_slice.substr(0, colon_location);
		if(input_smaller_slice.compare("High school graduate, GED, or alternative") != 0) {
			cout << "ERROR: expected income field is not indicated by 'High school graduate, GED, or alternative'" << endl;
			cout << "field: high_school_f" << endl;
			cout << "received input: " << input_smaller_slice << endl;
			cout << "city name: " << data[count].name << endl;
			cout << "received line: " << input << endl;
			assert(0);
		}
		else {
			input_smaller_slice = input_slice.substr(colon_location+1);

			for(int i=0; i<=input_smaller_slice.size(); i++) {  //Loop through the entire string
				if(input_smaller_slice[i]==',') { //Check for occurence of SEP_SYMB
					input_smaller_slice.erase(i,1); //If SEP_SYMB , remove that single character
				}
			}

			if(input_smaller_slice.find_first_not_of("1234567890") != string::npos) {
				cout << "ERROR: expected integer field has nonnumeric characters" << endl;
				cout << "field: high_school_f" << endl;
				cout << "received input: " << input_smaller_slice << endl;
				cout << "city name: " << data[count].name << endl;
				cout << "received line: " << input << endl;
				assert(0);
			}

			data[count].high_school_f = atof(input_smaller_slice.c_str());

			if(data[count].high_school_f < minCity.high_school_f) {
				minCity.high_school_f = data[count].high_school_f;
			}
			if(data[count].high_school_f > maxCity.high_school_f) {
				minCity.high_school_f = data[count].high_school_f;
			}
		}

		input_remainder = input_remainder.substr(next_semicolon+1);
		next_semicolon = input_remainder.find(';');
		input_slice = input_remainder.substr(0, next_semicolon);
		colon_location = input_slice.find(':');
		input_smaller_slice = input_slice.substr(0, colon_location);
		if(input_smaller_slice.compare("Bachelor's degree or higher") != 0) {
			cout << "ERROR: expected income field is not indicated by 'Bachelor's degree or higher'" << endl;
			cout << "field: bachelor_m" << endl;
			cout << "received input: " << input_smaller_slice << endl;
			cout << "city name: " << data[count].name << endl;
			cout << "received line: " << input << endl;
			assert(0);
		}
		else {
			input_smaller_slice = input_slice.substr(colon_location+1);

			for(int i=0; i<=input_smaller_slice.size(); i++) {  //Loop through the entire string
				if(input_smaller_slice[i]==',') { //Check for occurence of SEP_SYMB
					input_smaller_slice.erase(i,1); //If SEP_SYMB , remove that single character
				}
			}

			if(input_smaller_slice.find_first_not_of("1234567890") != string::npos) {
				cout << "ERROR: expected integer field has nonnumeric characters" << endl;
				cout << "field: bachelor_m" << endl;
				cout << "received input: " << input_smaller_slice << endl;
				cout << "city name: " << data[count].name << endl;
				cout << "received line: " << input << endl;
				assert(0);
			}

			data[count].bachelor_m = atof(input_smaller_slice.c_str());

			if(data[count].bachelor_m < minCity.bachelor_m) {
				minCity.bachelor_m = data[count].bachelor_m;
			}
			if(data[count].bachelor_m > maxCity.bachelor_m) {
				minCity.bachelor_m = data[count].bachelor_m;
			}
		}

		input_remainder = input_remainder.substr(next_semicolon+1);
		next_semicolon = input_remainder.find(';');
		input_slice = input_remainder.substr(0, next_semicolon);
		colon_location = input_slice.find(':');
		input_smaller_slice = input_slice.substr(0, colon_location);
		if(input_smaller_slice.compare("Bachelor's degree or higher") != 0) {
			cout << "ERROR: expected income field is not indicated by 'Bachelor's degree or higher'" << endl;
			cout << "field: bachelor_f" << endl;
			cout << "received input: " << input_smaller_slice << endl;
			cout << "city name: " << data[count].name << endl;
			cout << "received line: " << input << endl;
			assert(0);
		}
		else {
			input_smaller_slice = input_slice.substr(colon_location+1);

			for(int i=0; i<=input_smaller_slice.size(); i++) {  //Loop through the entire string
				if(input_smaller_slice[i]==',') { //Check for occurence of SEP_SYMB
					input_smaller_slice.erase(i,1); //If SEP_SYMB , remove that single character
				}
			}

			if(input_smaller_slice.find_first_not_of("1234567890") != string::npos) {
				cout << "ERROR: expected integer field has nonnumeric characters" << endl;
				cout << "field: bachelor_f" << endl;
				cout << "received input: " << input_smaller_slice << endl;
				cout << "city name: " << data[count].name << endl;
				cout << "received line: " << input << endl;
				assert(0);
			}

			data[count].bachelor_f = atof(input_smaller_slice.c_str());

			if(data[count].bachelor_f < minCity.bachelor_f) {
				minCity.bachelor_f = data[count].bachelor_f;
			}
			if(data[count].bachelor_f > maxCity.bachelor_f) {
				minCity.bachelor_f = data[count].bachelor_f;
			}
		}

		input_remainder = input_remainder.substr(next_semicolon+1);
		next_semicolon = input_remainder.find(';');
		input_slice = input_remainder.substr(0, next_semicolon);
		colon_location = input_slice.find(':');
		input_smaller_slice = input_slice.substr(0, colon_location);
		if(input_smaller_slice.compare("Total") != 0) {
			cout << "ERROR: expected income field is not indicated by 'Total'" << endl;
			cout << "field: school_total" << endl;
			cout << "received input: " << input_smaller_slice << endl;
			cout << "city name: " << data[count].name << endl;
			cout << "received line: " << input << endl;
			assert(0);
		}
		else {
			input_smaller_slice = input_slice.substr(colon_location+1);

			for(int i=0; i<=input_smaller_slice.size(); i++) {  //Loop through the entire string
				if(input_smaller_slice[i]==',') { //Check for occurence of SEP_SYMB
					input_smaller_slice.erase(i,1); //If SEP_SYMB , remove that single character
				}
			}

			if(input_smaller_slice.find_first_not_of("1234567890") != string::npos) {
				cout << "ERROR: expected integer field has nonnumeric characters" << endl;
				cout << "field: school_total" << endl;
				cout << "received input: " << input_smaller_slice << endl;
				cout << "city name: " << data[count].name << endl;
				cout << "received line: " << input << endl;
				assert(0);
			}

			data[count].school_total = atof(input_smaller_slice.c_str());

			if(data[count].school_total < minCity.school_total) {
				minCity.school_total = data[count].school_total;
			}
			if(data[count].school_total > maxCity.school_total) {
				minCity.school_total = data[count].school_total;
			}
		}

		count++;
	}


	for(int count = 0; count < DATA_SIZE; count++){
		data[count].Owner_occupied = data[count].Owner_occupied / data[count].Owner_occupied_total;
		data[count].occupants_per_room = data[count].occupants_per_room / data[count].occupants_per_room_total;
		data[count].houses = data[count].houses / data[count].house_total;
		data[count].mobile_homes = data[count].mobile_homes / data[count].house_total;
		data[count].boat = data[count].boat / data[count].house_total;
		data[count].house_1_year_ago = data[count].house_1_year_ago / data[count].house_1_year_ago_total;
		data[count].less_than_high_school_m = (data[count].less_than_high_school_m + data[count].less_than_high_school_f) / data[count].school_total;
		data[count].high_school_m = (data[count].high_school_m + data[count].high_school_f) / data[count].school_total;
		data[count].bachelor_m = (data[count].bachelor_m+ data[count].bachelor_f) / data[count].school_total;
	}

	data_input.close();
}

