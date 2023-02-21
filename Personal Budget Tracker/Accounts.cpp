/* Adam Scibelli*/

#include "Accounts.h"
#include <iostream>
#include <fstream>   //file input/output
#include <string>   
#include <sstream>   //stringstream
#include <vector> 
#include <algorithm>  //for getting unique categories.
#include <iomanip> //for setprecision in calcCurrCashFlow()
using namespace std;

Accounts::transaction::transaction(string dt, string nm, string cat, float amnt, string typ, string acntType) {  //constructor for transaction struct.
	date = dt;
	name = nm;
	category = cat;
	amount = amnt;
	type = typ;
	accountType = acntType;
}


Accounts::Accounts(float beg, string fileName) {  //constructor for Accounts class.  parameters are beginning cash flow and master account .csv file name.
	BEG_CASH_FLOW = beg;
	accountFileName = fileName;

	/*Code to parse .csv file into vector taken from,
	Code Morsels. (2022). How to Parse a CSV File in C++ [Video]. YouTube. https://www.youtube.com/watch?v=NFvxA-57LLA*/

	ifstream inputFile;      //creates input file pointer.
	inputFile.open(accountFileName); //opens file.
	if (inputFile.is_open()) {   //if file opens. check else statement for error message.
		string line = "";
		inputFile.clear();
		while (getline(inputFile, line)) {  //keeps getting row after row until end of file.
			string date;   //temp variables to pass into the transaction struct.
			string name;
			string category;
			double amount = 0;
			string type;
			string accountType;
			string tempString;  //use for converting the amount from string to double

			stringstream inputString(line);   //stores the first line in stringstream object.
			getline(inputString, date, ',');  //puts the first word into the date variable.
			int countBackSlash = 0;        //make sure a date only has two backslashes.
			if (date.length() < 8 || date.length() > 10) { //All dates should be 8 to 10 characters.
				cerr << "There is a date with an invalid number of characters or no characters in the date column in the .csv file." << endl;  //it's an error.
				exit(1);
			}
			if (date.length() == 8) {   //date of length 8
				for (int i = 0; i < date.length(); i++) {
					if (isdigit((int)date[i]) == false && date[i] != '/') {   //check everything is either a digit or a '/'
						cerr << "Check the date column for an invalid date format in the .csv file." << endl;  //it's an error.
						exit(1);
					}
					if (date[i] == '/') {
						countBackSlash++;
					}
					if (countBackSlash > 2) {  //Can only be two backslashes
						cerr << "Check the date column in the .csv file for more than two backslashes in a date." << endl;  //it's an error.
						exit(1);
					}
					if ((i == 1 && date[i] != '/') || (i == 3 && date[i] != '/')) {  //for a date of length 8 the '/' need to be in the 2nd and 4th position.
						cerr << "Check the date column for an invalid date format in the .csv file." << endl;  //it's an error.
						exit(1);
					}
					if (i == 0 && date[i] == '0') {    //make sure the first digit isn't 0.
						cerr << "Check the date column for a preceding 0 in the .csv file." << endl;  //it's an error.
						exit(1);
					}
				}
			}
			else if (date.length() == 10) {  //date of length 10.
				for (int i = 0; i < date.length(); i++) {
					if (isdigit((int)date[i]) == false && date[i] != '/') {
						cerr << "Check the date column for an invalid date format in the .csv file." << endl;  //it's an error.
						exit(1);
					}
					if (date[i] == '/') {
						countBackSlash++;
					}
					if (countBackSlash > 2) {
						cerr << "Check the date column in the .csv file for more than two backslashes in a date." << endl;  //it's an error.
						exit(1);
					}
					if ((i == 2 && date[i] != '/') || (i == 5 && date[i] != '/')) {   //backslashes need to be in the 3rd and 6th position.
						cerr << "Check the date column for an invalid date format in the .csv file." << endl;  //it's an error.
						exit(1);
					}
					if (i == 0 && date[i] == '0') {           //check that the first digit isn't 0.
						cerr << "Check the date column for a preceding 0 in the .csv file." << endl;  //it's an error.
						exit(1);
					}
					if (date.substr(0, 2) != "10" && date.substr(0, 2) != "11" && date.substr(0, 2) != "12") {   //month has to be 10-12.
						cerr << "Check the date column in the .csv file for a month greater than 12." << endl;  //it's an error.
						exit(1);
					}
				}
			}
			else if (date.length() == 9) {
				for (int i = 0; i < date.length(); i++) {
					if (isdigit((int)date[i]) == false && date[i] != '/') {
						cerr << "Check the date column for an invalid date format in the .csv file." << endl;  //it's an error.
						exit(1);
					}
					if (date[i] == '/') {
						countBackSlash++;
					}
					if (countBackSlash > 2) {
						cerr << "Check the date column in the .csv file for more than two backslashes in a date." << endl;  //it's an error.
						exit(1);
					}
					if (i == 1 && date[i] != '/') {     //if the backslash is not in the second position (months 10-12).
						if ((date[i + 1] != '/') || (date[i + 3] != '/')) {  //the backslashes must be in the 3rd and 5th positions.
							cerr << "Check the date column for an invalid date format in the .csv file." << endl;  //it's an error.
							exit(1);
						}
						if (date.substr(0, 2) != "10" && date.substr(0, 2) != "11" && date.substr(0, 2) != "12") {  //months must be 10-12.
							cerr << "Check the date column in the .csv file for a month greater than 12." << endl;  //it's an error.
							exit(1);
						}
					}
					if (i == 1 && date[i] == '/') {    //if the backslash is in the second position
						if (date[i + 3] != '/') {     //the other must be in the fifth.
							cerr << "Check the date column for an invalid date format in the .csv file." << endl;  //it's an error.
							exit(1);
						}
					}
					if (i == 0 && date[i] == '0') {   //first digit can't be 0.
						cerr << "Check the date column for a preceding 0 in the .csv file." << endl;  //it's an error.
						exit(1);
					}
				}
			}

			getline(inputString, name, ',');
			if (name == "") {     //check for empty cell.
				cerr << "Check name column in .csv file for empty cell." << endl;  //it's an error.
				exit(1);
			}
			else if (name.length() > 35) {         //can't have a name longer than 35 characters for printing output neatly.
				cerr << "A transaction name is too long. They can be no longer than 35 characters." << endl;
				exit(1);
			}

			getline(inputString, category, ',');
			if (category == "") {     //check for empty cell.
				cerr << "Check category column in .csv file for empty cell." << endl;  //it's an error.
				exit(1);
			}
			else if (category.length() > 20) {    //can't have a category longer than 20 characters for printing output neatly.
				cerr << "A category name is too long. They can be no longer than 20 characters." << endl;
				exit(1);
			}
			else if (category[0] < 'A' || category[0] > 'Z') {   //make sure categories start with capital letter so unique() keeps them sorted. Lower case ASCII letters have higher values than capitals.
				cerr << "Category names must start with a capital letter. Please check master account .csv file." << endl;
				exit(1);
			}

			getline(inputString, tempString, ',');  //uses tempString to get the string first.
			int countperiod = 0;             //error checking for amount column in .csv file.
			int counthyphen = 1;             //error checking for amount column in .csv file.
			/*the next for loop checks the amount column for invalid data*/
			for (int i = 0; i < tempString.length(); i++) {
				if (isdigit((int)tempString[i]) == false && tempString[i] != '.' && tempString[i] != '-') {  //if character is non digit and not a '.' or '-'.
					cerr << "Check amount column in masterAccount .csv file for invalid entries." << endl;  //it's an error.
					exit(1);
				}
				if (tempString[i] == '.') {  //should only have 1 '.' in amount data.
					countperiod++;
				}
				if (tempString[i] == '-' && i > 0) {  //can have a hyphen becuase of negative numbers, but it should always be the first character.
					counthyphen++;
				}
				if (countperiod > 1 || counthyphen > 1) {  //if either of these is greater than 1 there is an error.
					cerr << "Check amount column in masterAccount .csv file for invalid entries." << endl;
					exit(1);
				}
				if (i == 0 && tempString[i] == '0') {   //all transactions should have some value and not be 0.
					cerr << "Can't have an amount of 0." << endl;
					exit(1);
				}
			}
			if (tempString == "") {     //check for empty cell.
				cerr << "Check amount column in .csv file for empty cell." << endl;  //it's an error.
				exit(1);
			}
			amount = atof(tempString.c_str());      //uses atof to take string tempString and return a double into amount.

			getline(inputString, type, ',');
			for (int i = 0; i < type.length(); i++) {
				if (type[i] != 'I' && type[i] != 'E') {  //'I' and 'E' are the only valid entries for this column
					cerr << "Check type column in .csv file for invalid entries." << endl;  //it's an error.
					exit(1);
				}
			}
			if (type == "I" && amount < 0) {  //income should positive.
				cerr << "There is a negative value labelled as income." << endl;  //it's an error.
				exit(1);
			}
			if (type == "E" && amount > 0) {  //expenses should be negative.
				cerr << "There is a positive value labelled as an expense." << endl;  //it's an error.
				exit(1);
			}
			if (type == "") {     //check for empty cell.
				cerr << "Check type column in .csv file for empty cell." << endl;  //it's an error.
				exit(1);
			}

			getline(inputString, accountType, ',');
			if (accountType != "Checking" && accountType != "Credit") {  //these are the only two valid options
				cerr << "Check account type column in .csv file for invalid entries or blank cells." << endl;  //it's an error.
				exit(1);

			}

			transaction row(date, name, category, (float)amount, type, accountType);  //pass temp variables into transaction struct.

			masterAccount.push_back(row);  //pushes back the struct row into the masterAccount vector.

			line = "";  // resets the line to empty string for next row.
		} //end while getline
	} //end if file open
	else {
		cerr << "masterAccount file failed to open." << endl;  //file couldn't open.
		exit(1);  //exit on purpose.
	}
	inputFile.close();  //close file	

	/*The uniqueCategories vector is used in the historical transactions sub menu and Budget class.*/
	vector<string> categories(masterAccount.size());   //set up a temp vector of string to read categories into.
	for (int i = 0; i < masterAccount.size(); i++) {
		categories[i] = masterAccount[i].category;    //copy categories from .csv file into temp vector.
	}
	vector<string>::iterator ip;
	sort(categories.begin(), categories.end());     //sort by category.
	ip = unique(categories.begin(), categories.begin() + categories.size());  //get unique values.
	categories.resize(distance(categories.begin(), ip));    //resize vector for unique values only.
	for (int j = 0; j < categories.size(); j++) {
		uniqueCategories.push_back(categories[j]);    //copy new temp vector into uniqueCategories vector.
	}

	/*The set monthYrs is used in the getMonthlyByCat() and getMonthEndCashFlows() functions */
	pair<int, int> pairs;
	for (int i = 0; i < masterAccount.size(); i++) {  //make pairs of all month and year combos.
		pairs = make_pair(stoi(masterAccount[i].date.substr(masterAccount[i].date.length() - 2)), stoi(masterAccount[i].date.substr(0, 2))); //year is the key and month is the value.
		monthYrs.insert(pairs);      //inserts pairs into a set so it will store only unique combos of month and year.
	}

}


float Accounts::calcCurrCashFlow() {   //calculates the current cash flow value.
	float beg = BEG_CASH_FLOW;     //store the beginning cash flow value.
	for (int i = 0; i < masterAccount.size(); i++) {   //iterate through the vector of transactions
		beg += masterAccount[i].amount;    //add the amount to the beginning cash flow.
	}
	currCashFlow = beg;         //current cash flow is now equal to beg.
	return currCashFlow;
}


void Accounts::getAllTransMo(int mo, int yr) {  //gets all transactions in a specific month and year.
	int data = 0;              //see if that month and year are in the file
	for (int i = 0; i < masterAccount.size(); i++) {
		if (stoi(masterAccount[i].date.substr(0, 2)) == mo && stoi(masterAccount[i].date.substr(masterAccount[i].date.length() - 2)) == yr) { //if month and year match.
			data++;   //there is data there.
		}
	}
	if (data == 0) {  //no data.
		cout << "There is no data in the file for that month and year." << endl;
		cout << endl;
		return;
	}
	cout << "Here are all the transactions for " << mo << "/" << yr << "." << endl;
	cout << endl;
	cout << left << setw(13) << "Date" << setw(40) << "Transaction" << setw(25) << "Category" << "Amount" << '\t' << '\t' << setw(8) << "Type" << "Account" << endl;
	for (int i = 0; i < masterAccount.size(); i++) {    //print out if there's a match.
		if (stoi(masterAccount[i].date.substr(0, 2)) == mo && stoi(masterAccount[i].date.substr(masterAccount[i].date.length() - 2)) == yr) {
			cout << left << setw(13) << masterAccount[i].date << setw(40) << masterAccount[i].name << setw(25) << masterAccount[i].category <<
				"$" << masterAccount[i].amount << '\t' << '\t' << setw(8) << masterAccount[i].type << masterAccount[i].accountType << endl;
		}
	}
	cout << endl;
}

void Accounts::getCatAllTime(string cat) {     // gets all transactions in a specific category all time.
	cout << "These are your all time transactions for " << cat << "." << endl;
	cout << endl;
	cout << left << setw(13) << "Date" << setw(40) << "Transaction" << setw(25) << "Category" << "Amount" << '\t' << '\t' << setw(8) << "Type" << "Account" << endl;
	for (int i = 0; i < masterAccount.size(); i++) {    //print out if there's a match.
		if (masterAccount[i].category == cat) {
			cout << left << setw(13) << masterAccount[i].date << setw(40) << masterAccount[i].name << setw(25) << masterAccount[i].category <<
				"$" << masterAccount[i].amount << '\t' << '\t' << setw(8) << masterAccount[i].type << masterAccount[i].accountType << endl;
		}
	}
	cout << endl;
}

void Accounts::getMonthlyByCat(string cat) {   //gets monthly totals in a specific category all time.
	float monthlyTotal = 0;       //for each month's total.
	float allTimeTotal = 0;      //for average monthly total.
	cout << "These are your monthly totals for " << cat << "." << endl;
	cout << endl;
	cout << left << setw(16) << "Month/Year" << "Total" << endl;

	set<pair<int, int>>::iterator it;
	for (it = monthYrs.begin(); it != monthYrs.end(); ++it) {  //for each unique month year combo
		for (int j = 0; j < masterAccount.size(); j++) {   //loop through entire masterAccount file for matches.
			/*if matches category and unique month and year*/
			if (masterAccount[j].category == cat && stoi(masterAccount[j].date.substr(0, 2)) == it->second && stoi(masterAccount[j].date.substr(masterAccount[j].date.length() - 2)) == it->first) {
				monthlyTotal += masterAccount[j].amount;   //add the amounts. resets to 0 for each new month/year combo.
				allTimeTotal += masterAccount[j].amount;   //keep adding all the amounts for an overall average.
			}
		}
		cout << left << it->second << "/" << it->first << '\t' << '\t' << "$" << monthlyTotal << endl;   //output the monthly total.
		monthlyTotal = 0;     //set back to 0 for next month/year combo.
	}
	cout << "Your monthly average for " << cat << " is $" << allTimeTotal / monthYrs.size() << "." << endl;     //output monthly average.
	cout << endl;
}

void Accounts::getMonthEndCashFlows() {    //calculates month end cash flow values all time.
	float monthEnd = BEG_CASH_FLOW;      //start at beginning cash flow.
	float monthEndTotals = 0;            //Add all monthe end values for an overall average.   
	cout << "These are your month end cash flow values." << endl;
	cout << endl;
	cout << left << setw(16) << "Month/Year" << "Cash Flow" << endl;

	set<pair<int, int>>::iterator it;   //going to loop through the unique set of months and years.
	for (it = monthYrs.begin(); it != monthYrs.end(); ++it) {          //loop through unique months and years.
		for (int j = 0; j < masterAccount.size(); j++) {   //loop through entire masterAccount file.
			/*if matches unique month and year*/
			if (stoi(masterAccount[j].date.substr(0, 2)) == it->second && stoi(masterAccount[j].date.substr(masterAccount[j].date.length() - 2)) == it->first) {
				monthEnd += masterAccount[j].amount;   //add the amounts.
			}
		}
		cout << left << it->second << "/" << it->first << '\t' << '\t' << "$" << monthEnd << endl;   //output the monthly total.
		monthEndTotals += monthEnd;   //add each monthly total for the average monthly total.
	}
	cout << "Your average month end cash flow value is $" << monthEndTotals / monthYrs.size() << "." << endl;     //output monthly average.
	cout << endl;
}

