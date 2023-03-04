#include "SavingFor.h"
#include <sstream>
#include <iostream>
#include <fstream>   //file input/output
#include <algorithm>
#include <iomanip> 
using namespace std;

SavingFor::savingItems::savingItems(string itm, float cst, int pr) {   //sets savingItems struct field
	item = itm;
	cost = cst;
	priority = pr;
}


SavingFor::SavingFor(string filename) {     //stores the saving .csv filename.
	savingFileName = filename;


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

void SavingFor::getItems() {     //display a report of items saving for.
	cout << "Here's a list of items you are saving for listed by priority." << endl;
	cout << endl;
	cout << left << setw(17) << "Priority" << setw(25) << "Item" << "Cost" << endl;
	for (int i = 0; i < objects.size(); i++) {
		cout << left << setw(17) << i + 1 << setw(25) << objects[i].item << "$" << objects[i].cost << endl;       //print out items for user.
	}
	cout << endl;
}

void SavingFor::editItems(string item) {                //Can change the cost or priority of an item.
	cin.ignore(numeric_limits<streamsize>::max(), '\n');   //discards input buffer from previous menu in case float was entered.
	int answer = 0;
	cout << "Enter 1 to change the cost of " << item << "." << endl;
	cout << "Enter 2 to change the priority of " << item << "." << endl;
	cin >> answer;
	while (cin.fail() || answer < 1 || answer > 2) {  //input can't be non integer characters or integers not 1-2.
		cout << "Sorry, that is not valid input. Please try again. " << endl;
		cin.clear();   //clears error if users enters a string.
		cin.ignore(256, '\n');    //ignores up to 256 characters in the last user input.
		cin >> answer;   //takes new input.
	}
	switch (answer) {
	case 1: {
		cin.ignore(numeric_limits<streamsize>::max(), '\n');   //discards input buffer from previous menu in case float was entered.
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
				cout << "The cost for " << item << " has been set to $" << objects[i].cost << "." << endl;
			}
		}
		writeToFile(objects, savingFileName);  //update file with new amount.
		cout << endl;
		break;
	} //end case 1.
	case 2: {
		cin.ignore(numeric_limits<streamsize>::max(), '\n');   //discards input buffer from previous menu in case float was entered.
		int priority = 0;
		int difference = 0;                   //used to determine if new priority is higher or lower than previous.
		int size = objects.back().priority;   //size of vector equals the highest priority.
		cout << "Enter a new priority for " << item << "." << endl;
		cin >> priority;
		while (cin.fail() || priority < 1 || priority > objects.size()) {  //input can't be non integer characters or integers less than 1 or greater than the vector size.
			cout << "Sorry, that is not valid input. Valid input are integers in the range of 1-" << objects.size() + 1 << ". Please try again. " << endl;
			cin.clear();   //clears error if users enters a string.
			cin.ignore(256, '\n');    //ignores up to 256 characters in the last user input.
			cin >> priority;   //takes new input.
		}
		for (int i = 0; i < objects.size(); i++) { 
			if (objects[i].item == item && objects[i].priority == priority) {      //if priority already equals the priority user wants.
				cout << "The priority for " << item << " is already " << priority << "." << endl;
				cout << endl;
				return;
			}
			else if (objects[i].item == item) {
				if (priority - objects[i].priority < 0) {   //if making the priority of existing item a higher priority.
					difference = -1;
				}
				else {                       //or if moving it down the list of priorities.
					difference = 1;
				}
				objects[i].priority = priority;  //set the new priority
				cout << "The priority for " << item << " has been set to " << objects[i].priority << "." << endl;
			}
		}
		sort(objects.begin(), objects.end());         //sort vector by priority.
		if (difference < 0) {                        //if item going up the list.
			for (int i = 0; i < objects.size(); i++) {
				if (objects[i].priority == priority && objects[i].item != item) {   //if two items have the same priority now.
					objects[i].priority += 1;       //increase the priority of the already existing item at that priority so it goes further down the list by one.
				}
			}
			sort(objects.begin(), objects.end());  //resort vector by adjusted priorities.
			for (int i = size - 1; i > priority; i--) {  //renumber the priorities on the list lower than the priority user entered.
				objects[i].priority = i + 1;
			}
		}
		else if (difference > 0) {        //else if item is going down the list.
			for (int i = 0; i < objects.size(); i++) {
				if (i < priority - 2) {          //renumber priorities of items that have moved up the list.
					objects[i].priority = i + 1;
				}
				if (i == priority - 1) {   //if two items have the same priority now.
					objects[i].priority -= 1;       //decrease the priority of the item lower in the list so it moves before the item that had its priority changed.
					sort(objects.begin(), objects.end());  //resort vector by adjusted priorities.
				}
			}
		}
		writeToFile(objects, savingFileName);  //update file with new priorities.
		cout << endl;
		break;
	} //end case 2
	} //end switch
}

void SavingFor::addItems() {   //function to add an object to be saved for.
	cin.ignore(numeric_limits<streamsize>::max(), '\n');   //discards input buffer from previous menu.
	string itemName;
	float cost = 0;
	int priority = 0;
	cout << "Enter a name for the item. " << endl;
	getline(cin, itemName, '\n');
	while (1) {
		/*find_if code taken from JL Borges. (2015, July 9). Finding a member in a struct. Cplusplus.com. http://www.cplusplus.com/forum/beginner/169178*/
		const auto p = std::find_if(objects.begin(), objects.end(),        
			[itemName](const savingItems& a) { return a.item == itemName; });   //search the objects vector to see if the item name is already there.
		if (p != objects.end()) {
			cout << "That item name already exists in the file. Please choose another name. " << endl;  //can't enter an item already listed.
			getline(cin, itemName, '\n');
		}	
		else {
			break;
		}
	}
	cout << "Enter the cost for the item. " << endl;
	cin >> cost;
	while (cin.fail()) {
		cout << "Sorry, that is not valid input.  Please try again. " << endl;
		cin.clear();   //clears error if users enters a string.
		cin.ignore(256, '\n');    //ignores up to 256 characters in the last user input.
		cin >> cost;   //takes new input.
	}
	cin.ignore(numeric_limits<streamsize>::max(), '\n');   //discards input buffer from previous menu.
	cout << "Enter the priority for the item. " << endl;
	cin >> priority;
	while (cin.fail() || priority < 1 || priority > objects.size() + 1) {  //input can't be non integer characters or integers less than 1 or greater than the vector size +1.
		cout << "Sorry, that is not valid input. Valid input are integers in the range of 1-" << objects.size() + 1 << ". Please try again. " << endl;
		cin.clear();   //clears error if users enters a string.
		cin.ignore(256, '\n');    //ignores up to 256 characters in the last user input.
		cin >> priority;   //takes new input.
	}
	savingItems fill(itemName, abs(cost), priority);    //create a new item to fill in the vector.
	objects.push_back(fill);                       //push the new item into the vector.
	sort(objects.begin(), objects.end());         //sort vector by priority.
	for (int i = 0; i < objects.size(); i++) {
		if (objects[i].priority == priority && objects[i].item != itemName) {   //if two items have the same priority now.
			objects[i].priority += 1;       //increase the priority of the older item by one.
		}
		if (i > priority) {  //increase the priorities of the rest of the items in the list by one.
			objects[i].priority++;
		}
	}
	sort(objects.begin(), objects.end());  //resort vector by adjusted priorities.
	writeToFile(objects, savingFileName);  //update file with new amount.
	cout << itemName << " has been saved." << endl;
	cout << endl;
}


void SavingFor::deleteItems(string item) {                //function to delete an object to be saved for.
	vector<savingItems>::iterator it;                     //create iterator to erase from vector.
	for (it = objects.begin(); it != objects.end(); ++it) {   //loop through the vector.
		if (item == it->item) {                            //if the item matches.
			int i = it->priority-1;                       //save the position in the vector.
			objects.erase(it);                            //erase the item.
			for (int j = i; j < objects.size(); j++) {   //starting from saved position, loop through the rest of vector.
				objects[j].priority--;                   //shift the priority of remaining elements down one.
			}
			break;
		}
	}
	writeToFile(objects, savingFileName);  //update file with new items.
	cout << item << " has been removed." << endl;
	cout << endl;
}



void SavingFor::projectedAcquisition(Budget budg, float expectedMoEndCashFlow, struct tm time) { //reports on when items can be purchased or paid for.
	float remainingAmount = 0;
	int month = 1 + time.tm_mon;
	int year = 1900 + time.tm_year;
	cout << "Here's a list of items you are saving for listed by priority and a timeline of when you can aquire them based on your cash flow and budget." << endl;
	cout << endl;
	cout << left << setw(10) << "Priority" << setw(25) << "Item" << setw(15) << "Cost" << setw(23) << "Month End Cash Flow" << setw(17) << "Remaining Cost" <<
		setw(25) << "Monthly Savings/Payment" << "Acquisition Date" << endl;
	
	for (int i = 0; i < objects.size(); i++) {
		cout << left << setw(10) << objects[i].priority << setw(25) << objects[i].item <<
			"$" << setw(14) << objects[i].cost << "$" << setw(22) << expectedMoEndCashFlow;   //print out the priority, item name, cost, and this month's expected month end cash flow.
		if (expectedMoEndCashFlow >= objects[i].cost) {               //can afford this month if expected month end cash flow is greater than or equal to cost.
			remainingAmount = 0;                                 //there is no remaining amount to be paid if can cover the entire cost this month.
		}
		else if (expectedMoEndCashFlow < objects[i].cost) {
			remainingAmount = objects[i].cost - expectedMoEndCashFlow;     //else calculate the remaining amount.
		}
		cout << "$" << setw(16) << remainingAmount;                    
		if (remainingAmount == 0) {                       
			expectedMoEndCashFlow -= objects[i].cost;     //adjust the cash flow. Subtract the cost of the item.
			cout << setw(25) << "-";                      //there is no monthly payment or savings going toward the item if can cover the whole cost this month.
			cout << month << "/" << year << endl;        //if can cover the whole cost, the acquisition date is the current month and year.
		}
		else if (remainingAmount > 0) {
			expectedMoEndCashFlow = 0;                   //don't have any more cash flow to apply to items.
			cout << "$" << setw(24) << budg.calcBudgetedMoSavings();   //otherwise apply the monthly savings amount in the budget plan as a payment.
			while (remainingAmount > 0) {
				remainingAmount -= budg.calcBudgetedMoSavings();     //apply the budgeted monthly savings to the item until cost is covered.
				if (month % 12 == 0) {                    //if month is December.
					month = 1;                  //make new month January.
					year++;                     //and add one to the year.
				}
				else if (month % 12 > 0) {     //if month not December.
					month++;                   //add one to the month.
				}
			}
			expectedMoEndCashFlow = abs(remainingAmount);  //item has been payed off and this is what's left over from the monthly savings for the last month.
			cout << month << "/" << year << endl;
		}
	}
	cout << endl;
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
