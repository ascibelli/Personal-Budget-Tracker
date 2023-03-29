/*Adam Scibelli-3/19/2023
SDEV 435-Final Project-Personal Budget Tracker
This is the implementation of the Budget class.
This allows the user to set budgeted amounts for their transaction categories in the
master .csv file, calculates the projected month end cash flow and monthly savings, and 
alerts the user when they go over budget in a category.*/


#include "Budget.h"
#include <iostream>
#include <fstream>   //file input/output 
#include <sstream>   //stringstream, read in .csv file
#include <iomanip>
using namespace std;


Budget::budgeting::budgeting(string cat, float amnt, string typ) {   //sets the budgeting struct fields.
	category = cat;
	amount = amnt;
	type = typ;
}


Budget::Budget(string fileName, string date, Accounts master) {   //stores the budgeting .csv filename and current date.
	budgetFileName = fileName;
	currentDate = date;

	for (int i = 0; i < master.uniqueCategories.size(); i++) {    //get transaction type for each unique category from master .csv file
		string transType = "";                      //temp variable to get transaction type(I/E)
		for (int j = 0; j < master.masterAccount.size(); j++) {    //bring in its type from the masterAccount .csv file.
			if (master.uniqueCategories[i] == master.masterAccount[j].category) {
				transType = master.masterAccount[j].type;
				break;          //as soon as a type gets pulled in from a category can break out of the loop.
			}
		}
		budgeting fill(master.uniqueCategories[i], (float)0.00, transType); //fill the budget vector with unique categories and with default $0.00 values and the type.
		vec.push_back(fill);
	}

	/*Code to parse .csv file into vector taken from,
	Code Morsels. (2022). How to Parse a CSV File in C++ [Video]. YouTube. https://www.youtube.com/watch?v=NFvxA-57LLA*/

	ifstream inputFile;      //creates input file pointer.
	inputFile.open(budgetFileName); //opens file.
	if (inputFile.is_open()) {   //if file opens. check else statement for error message.
		string line = "";
		inputFile.clear();
		while (getline(inputFile, line)) {  //keeps getting row after row until end of file.
			string category;   //temp variables to pass into the budgeting struct.
			double amount = 0;
			string tempString;  //use for converting the amount from string to double
			string type;

			stringstream inputString(line);   //stores the first line in stringstream object.
			getline(inputString, category, ',');  //puts the first word into the category variable.

			getline(inputString, tempString, ',');  //uses tempString to get the string first.
			amount = atof(tempString.c_str());      //uses atof to take string tempString and return a double into amount.


			for (int i = 0; i < vec.size(); i++) {    //fill out the budget vector with stored budget amounts.
				if (category == vec[i].category) {   //if category is saved in the Budget .csv file
					vec[i].amount = (float)amount;   //bring in its saved amount to write over $0.00 default values.
				}
			}

			line = "";  // resets the line to empty string for next row.
		} //end while getline
	} //end if file open
	else {
		cerr << "budget file failed to open." << endl;  //file couldn't open.
		exit(1);  //exit on purpose.
	}
	inputFile.close();  //close file
	
}


void Budget::setAmount(string cat, Accounts master) {      //set a budget amount for a category.
	float monthlyTotal = 0;       //for each month's total.
	float allTimeTotal = 0;      //for average monthly total.
	set<pair<int, int>>::iterator it;       //iterate through the set of month/years.
	for (it = master.monthYrs.begin(); it != master.monthYrs.end(); ++it) {  //for each unique month year combo
		for (int j = 0; j < master.masterAccount.size(); j++) {   //loop through entire masterAccount file for matches.
			/*if matches category and unique month and year*/
			if (master.masterAccount[j].category == cat && stoi(master.masterAccount[j].date.substr(0, 2)) == it->second && stoi(master.masterAccount[j].date.substr(master.masterAccount[j].date.length() - 2)) == it->first) {
				monthlyTotal += master.masterAccount[j].amount;   //add the amounts. resets to 0 for each new month/year combo outside of loop below.
				allTimeTotal += master.masterAccount[j].amount;   //keep adding all the amounts for an overall average.
			}
		}
		monthlyTotal = 0;     //set back to 0 for next month/year combo.
	}
	cin.ignore(numeric_limits<streamsize>::max(), '\n');   //discards input buffer from previous menu in case a float was entered.
	string choice = "";
	/*User can choose to set budget based on monthly average, or can set other amount manually*/
	cout << "Enter Y to apply your monthly average for " << cat << " of $" << allTimeTotal/master.monthYrs.size() << " as your budgeted amount, or enter N to manually set your own amount. " << endl;
	cin >> choice;
	while (choice != "Y" && choice != "N") {       //check for valid input, Y or N.
		cout << "Sorry, that is not valid input.  Please enter Y or N. " << endl;
		cin >> choice;
	}
	if (choice == "Y") {     //Update budgeted amount with monthly average.
		for (int i = 0; i < vec.size(); i++) {  //update vector with new amount
			if (vec[i].category == cat && vec[i].type == "E") {
				vec[i].amount = -abs(allTimeTotal/master.monthYrs.size());  //if it's an expense category, make it negative.  The user can enter a positive or negative number.
				cout << "Budget amount for " << cat << " has been set to $" << vec[i].amount << "." << endl;
			}
			else if (vec[i].category == cat && vec[i].type == "I") {
				vec[i].amount = abs(allTimeTotal / master.monthYrs.size());  //if it's an income category, make it positive.  The user can enter a positive or negative number.
				cout << "Budget amount for " << cat << " has been set to $" << vec[i].amount << "." << endl;
			}
		}
	}
	else if (choice == "N") {  //user chooses any amount.
		float amount = 0;
		cout << "Enter a new monthly budget amount for " << cat << ". " << endl;
		cin >> amount;
		while (cin.fail()) {
			cout << "Sorry, that is not valid input.  Please try again. " << endl;
			cin.clear();   //clears error if users enters a string.
			cin.ignore(256, '\n');    //ignores up to 256 characters in the last user input.
			cin >> amount;   //takes new input.
		}
		for (int i = 0; i < vec.size(); i++) {  //update vector with new amount
			if (vec[i].category == cat && vec[i].type == "E") {
				vec[i].amount = -abs(amount);  //if it's an expense category, make it negative.  The user can enter a positive or negative number.
				cout << "Budget amount for " << cat << " has been set to $" << vec[i].amount << "." << endl;
			}
			else if (vec[i].category == cat && vec[i].type == "I") {
				vec[i].amount = abs(amount);  //if it's an income category, make it positive.  The user can enter a positive or negative number.
				cout << "Budget amount for " << cat << " has been set to $" << vec[i].amount << "." << endl;
			}
		}
	}
	/*Reset the overBudget vector to account for new amount entered.*/
	overBudget.clear();            //empty vector.
	float monthlyAmount = 0;                //get each categories monthly total for this month.
	for (int i = 0; i < vec.size(); i++) {
		for (int j = 0; j < master.masterAccount.size(); j++) {   //for each unique category for the current month and year, if the category, month, and year in the master file match add the totals.
			if (vec[i].category == master.masterAccount[j].category && stoi(master.masterAccount[j].date.substr(0, 2)) == stoi(currentDate.substr(0, 2)) && stoi(master.masterAccount[j].date.substr(master.masterAccount[j].date.length() - 2)) == stoi(currentDate.substr(currentDate.length() - 2))) {
				monthlyAmount += master.masterAccount[j].amount;
			}
		}
		if (vec[i].type == "I" && monthlyAmount - vec[i].amount > 0) {       //if the difference is more than 0 for income, great, earned more than expected.
			budgeting fill(vec[i].category, monthlyAmount - vec[i].amount, vec[i].type);  //fill the overBudget vector for that category by the difference for the amount.
			overBudget.push_back(fill);
		}
		else if (vec[i].type == "E" && monthlyAmount - vec[i].amount < 0) {      //if the difference is less than 0 for expenses, warning, spent more than expected.
			budgeting fill(vec[i].category, abs(monthlyAmount - vec[i].amount), vec[i].type);  //fill the overBudget vector.
			overBudget.push_back(fill);
		}
		monthlyAmount = 0;
	}

	writeToFile(vec, budgetFileName);  //update file with new amount.

	cout << endl;
}



void Budget::paidVersusBudget(Accounts master) {      //will display actual expenditures versus budgeted amounts.
	float monthlyTotal = 0;
	cout << left << setw(25) << "Category" << setw(20) << "Actual Amount" << setw(20) << "Budgeted Amount" << "Current Status" << endl;
	for (int i = 0; i < vec.size(); i++) {
		for (int j = 0; j < master.masterAccount.size(); j++) {   //for each unique category for the current month and year, if the category, month, and year in the master file match add the totals.
			if (vec[i].category == master.masterAccount[j].category && stoi(master.masterAccount[j].date.substr(0, 2)) == stoi(currentDate.substr(0, 2)) && stoi(master.masterAccount[j].date.substr(master.masterAccount[j].date.length() - 2)) == stoi(currentDate.substr(currentDate.length() - 2))) {
				monthlyTotal += master.masterAccount[j].amount;
			}
		}
		cout << left << setw(25) << vec[i].category << "$" << setw(19) << monthlyTotal << "$" << setw(19) << vec[i].amount; //print out actual and budgeted amounts.

		if (vec[i].type == "E" && monthlyTotal - vec[i].amount < 0 || vec[i].type == "I" && monthlyTotal - vec[i].amount > 0) {  //if difference is negative for expense or positive for income it's over budget.
			cout << "Over Budget by $" << abs(monthlyTotal - vec[i].amount) << "." << endl;
		}
		else if (vec[i].type == "E" && monthlyTotal - vec[i].amount > 0 || vec[i].type == "I" && monthlyTotal - vec[i].amount < 0) {  //if difference is positive for expense or negative for income it's under budget.
			cout << "Under Budget by $" << abs(monthlyTotal - vec[i].amount) << "." << endl;
		}
		else if (monthlyTotal - vec[i].amount == 0) {  //difference is 0.
			cout << "Equal to budget. No difference." << endl;
		}
		monthlyTotal = 0;
	}
	cout << endl;
}


void Budget::calcMonthlySavings() {   //calculates an expected month end savings amount based on the budget if it is followed and and adjusted month end savings based on actual expenditures.

	cout << "Your budgeted monthly savings per month is $" << calcBudgetedMoSavings() << "." << endl;

	cout << "Your actual expected monthly savings this month is $" << calcExpectedMoSaving() << "." << endl;
	cout << endl;
}


float Budget::calcEndMoCashFlow(float currentCashFlow, Accounts master) {    //calculates the expected month end cash flow based on actual expenditures and remaining budgeted amounts.
	float monthlyTotal = 0;                //get each categories monthly total for this month.
	float adjustedAmounts = 0;             //keep track of differences between actual expenditures versus budgeted.
	float projectedCashFlow = 0;           //final projected cash flow for the end of the month.
	for (int i = 0; i < vec.size(); i++) {
		for (int j = 0; j < master.masterAccount.size(); j++) {   //for each unique category for the current month and year, if the category, month, and year in the master file match add the totals.
			if (vec[i].category == master.masterAccount[j].category && stoi(master.masterAccount[j].date.substr(0, 2)) == stoi(currentDate.substr(0, 2)) && stoi(master.masterAccount[j].date.substr(master.masterAccount[j].date.length() - 2)) == stoi(currentDate.substr(currentDate.length() - 2))) {
				monthlyTotal += master.masterAccount[j].amount;
			}
		}
		if (vec[i].type == "I" && monthlyTotal - vec[i].amount < 0) {      //if actual versus budgeted for income is less than 0, more income is expected.
			adjustedAmounts += abs(monthlyTotal - vec[i].amount);          //add the difference to the adjusted amounts.
		}
		else if (vec[i].type == "E" && monthlyTotal - vec[i].amount > 0) { //if actual versus budgeted for expenses are more than 0, more spending is expected.
			adjustedAmounts -= monthlyTotal - vec[i].amount;               //add the difference to the adjusted amounts.
		}
		else if (vec[i].type == "I" && monthlyTotal - vec[i].amount > 0) {       //if the difference is more than 0 for income, great, earned more than expected.
			budgeting fill(vec[i].category, monthlyTotal - vec[i].amount, vec[i].type);  //fill the overudget vector for that category by the difference for the amount.
			overBudget.push_back(fill);
		}
		else if (vec[i].type == "E" && monthlyTotal - vec[i].amount < 0) {      //if the difference is less than 0 for expenses, warning, spent more than expected.
			budgeting fill(vec[i].category, abs(monthlyTotal - vec[i].amount), vec[i].type);  //fill the overBudget vector.
			overBudget.push_back(fill);
		}
		monthlyTotal = 0;
	}
	projectedCashFlow = currentCashFlow + adjustedAmounts;            //the final month end expected cash flow is the current cash flow plus the budget adjustments.
	return projectedCashFlow;
}


void Budget::budgetAlerts() {        //alerts the user when they are over budget on categories.
	if (overBudget.size() == 0) {        //if nothing overbudget don't do anything.
		return;
	}
	for (int i = 0; i < overBudget.size(); i++) {
		if (overBudget[i].type == "I") {             //if over budget on income, that's good.
			cout << "Congratulations, you have earned $" << overBudget[i].amount << " more than expected for " << overBudget[i].category << ". " << endl;
		}
	}
	for (int i = 0; i < overBudget.size(); i++) {
			if (overBudget[i].type == "E") {      //else if category is an expense, not good, overspent.
				cout << "WARNING: you have spent $" << overBudget[i].amount << " more than expected for " << overBudget[i].category << "." << endl;
			}
	}
	cout << endl;
}

template <typename T>
void Budget::writeToFile(vector <T> vec, string filename) {       //template function to write any type of vector to columns and rows in a .csv file.   
	ofstream readOut;   //read budget vector back to .csv file
	readOut.open(filename, ios::out);
	if (readOut.is_open()) {
		for (int i = 0; i < vec.size(); i++) {
			readOut << vec[i].category << "," << vec[i].amount << "\n";
		}
		readOut.close();   //close file
	}
	else {
		cerr << "Could not open budget .csv file." << endl;
		exit(1);
	}
}

float Budget::calcBudgetedMoSavings(){                 //calculates monthly savings if budget is followed exactly.
	float budgetedMonthlySavings = 0;
	for (int i = 0; i < vec.size(); i++) {
		budgetedMonthlySavings += vec[i].amount;          //simply add all the budgeted amounts.
	}
	return budgetedMonthlySavings;
}


float Budget::calcExpectedMoSaving() {                //calculates expected monthly savings based on adjustments for overbudget transactions.
	float expectedMonthlySavings = calcBudgetedMoSavings();    //start with budgeted savings
	for (int i = 0; i < overBudget.size(); i++) {        //adjust the month end savings by transactions that have gone over budget.
		if (overBudget[i].type == "I") {
			expectedMonthlySavings += overBudget[i].amount;      //if it's income, add the amount over budget.
		}
		else {
			expectedMonthlySavings -= overBudget[i].amount;    //else it's an expense, subtract amounts that have gone over budget.
		}
	}
	return expectedMonthlySavings;
}