#include "SavingFor.h"
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>   //file input/output
#include <algorithm>
using namespace std;

SavingFor::savingItems::savingItems(string itm, float cst, int pr) {   //sets savingItems struct field
	item = itm;
	cost = cst;
	priority = pr;
}


SavingFor::SavingFor(string filename, string date) {     //stores the saving .csv filename and current date.
	savingFileName = filename;
	currentDate = date;

	/*Code to parse .csv file into vector taken from,
	Code Morsels. (2022). How to Parse a CSV File in C++ [Video]. YouTube. https://www.youtube.com/watch?v=NFvxA-57LLA*/

	ifstream inputFile;      //creates input file pointer.
	inputFile.open(savingFileName); //opens file.
	if (inputFile.is_open()) {   //if file opens. check else statement for error message.
		string line = "";
		inputFile.clear();
		while (getline(inputFile, line)) {  //keeps getting row after row until end of file.
			string item;   //temp variables to pass into the savingItems struct.
			double cost = 0;
			string tempString;  //use for converting the cost and priority from string to double and integer.
			int priority = 0;

			stringstream inputString(line);   //stores the first line in stringstream object.
			getline(inputString, item, ',');  //puts the first word into the item variable.
			if (item == "") {     //check for empty cell.
				break;       //the file is empty, move on.
			}

			getline(inputString, tempString, ',');  //uses tempString to store the string first.
			cost = atof(tempString.c_str());      //uses atof to take string tempString and return a double into cost.

			getline(inputString, tempString, ',');  //uses tempString to store the string first.
			priority = stoi(tempString.c_str());      //uses stoi to take string tempString and return an int into priority.

			savingItems row(item, (float)cost, priority);  //pass temp variables into savingItems struct.

			objects.push_back(row);         //fills the saving Objects vector.

			line = "";  // resets the line to empty string for next row.
		} //end while getline
	} //end if file open
	else {
		cerr << "saving file failed to open." << endl;  //file couldn't open.
		exit(1);  //exit on purpose.
	}
	inputFile.close();  //close file
}

void SavingFor::getItems() {                          //display a report of items saving for.
}

void SavingFor::editItems(string item) {
	cin.ignore(numeric_limits<streamsize>::max(), '\n');   //discards input buffer from previous menu.
	float cost = 0;
	cout << "Enter a new cost for " << item << "." << endl;
	cin >> cost;
	while (cin.fail()) {
		cout << "Sorry, that is not valid input.  Please try again. " << endl;
		cin.clear();   //clears error if users enters a string.
		cin.ignore(256, '\n');    //ignores up to 256 characters in the last user input.
		cin >> cost;   //takes new input.
	}
	for (int i = 0; i < objects.size(); i++) {  //update vector with new cost.
		if (objects[i].item == item) {
			objects[i].cost = abs(cost);  //The user can enter a positive or negative number and it turns into a positive.
		}
		cout << "The cost for " << item << " has been set to $" << objects[i].cost << "." << endl;
	}

	writeToFile(objects, savingFileName);  //update file with new amount.
	cout << endl;
}

void SavingFor::addItems() {   //function to add an object to be saved for.
	string itemName;
	float cost = 0;
	int priority = 0;
	cout << "Enter a name for the item. " << endl;
	cin >> itemName;
	cout << "Enter the cost for the item. " << endl;
	cin >> cost;
	while (cin.fail()) {
		cout << "Sorry, that is not valid input.  Please try again. " << endl;
		cin.clear();   //clears error if users enters a string.
		cin.ignore(256, '\n');    //ignores up to 256 characters in the last user input.
		cin >> cost;   //takes new input.
	}
	cout << "Enter the priority for the item. " << endl;
	cin >> priority;
	while (cin.fail() || priority < 1 || priority > objects.size() + 1) {  //input can't be non integer characters or integers less than 1 or greater than the vector size +1.
		cout << "Sorry, that is not valid input. Valid input are integers in the range of 1-" << objects.size() + 1 << ". Please try again. " << endl;
		cin.clear();   //clears error if users enters a string.
		cin.ignore(256, '\n');    //ignores up to 256 characters in the last user input.
		cin >> priority;   //takes new input.
	}
	savingItems fill(itemName, cost, priority);


	for (int i = 0; i < objects.size(); i++) {
		cout << objects[i].item << objects[i].cost << objects[i].priority << endl;
	}

	writeToFile(objects, savingFileName);  //update file with new amount.
	cout << endl;
}


void SavingFor::deleteItems() {                //function to delete an object to be saved for.
}



void SavingFor::projectedAcquisition() {               //reports on how much monthly can be paid towards the item based on budget and acquisition date.
}


template <typename T>
void SavingFor::writeToFile(vector <T> vec, string filename) {       //template function to write any type of vector to columns and rows in a .csv file.          
	ofstream readOut;   //read savingObjects vector back to .csv file
	readOut.open(filename, ios::out);
	if (readOut.is_open()) {
		for (int i = 0; i < vec.size(); i++) {
			readOut << vec[i].item << "," << vec[i].cost << "," << vec[i].priority << "\n";
		}
		readOut.close();   //close file
	}
	else {
		cerr << "Could not open saving .csv file." << endl;
		exit(1);
	}
}
