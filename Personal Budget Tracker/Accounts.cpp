/*Adam Scibelli-3/19/2023
SDEV 435-Final Project-Personal Budget Tracker
This is the implementation of the Accounts class.
This reads in the transactions in the master .csv file and contains
functions for calculating the current cash flow and showing reports of historical transactions.*/

#include "Accounts.h"
#include <iostream>
#include <fstream>   //file input/output 
#include <sstream>   //stringstream, reading in .csv file
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


Accounts::Accounts(float beg, string fileName, string date) {  //constructor for Accounts class.  parameters are beginning cash flow, master account .csv file name, and today's date.
	BEG_CASH_FLOW = beg;
	accountFileName = fileName;
	currentDate = date;

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
			getline(inputString, date, ',');  //gets the string up to the comma and puts into the date variable.
			if (date.length() < 8 || date.length() > 10) { //All dates should be 8 to 10 characters.
				cerr << "There is a date with an invalid number of characters or no characters in the date column in the .csv file." << endl;  //it's an error.
				exit(1);
			}
			int countForwardSlash = 0;
			for (int i = 0; i < date.length(); i++) {
				if (isdigit((int)date[i]) == false && date[i] != '/') {   //check everything is either a digit or a '/'.
					cerr << "Check the date column in the .csv file for an invalid character." << endl;  //it's an error.
					exit(1);
				}
				if (date[i] == '/') {      //count forward slashes.
					countForwardSlash++;
				}
				if (countForwardSlash > 2){  //if more than two forward slashes, it's an error.
					cerr << "Check the date column in the .csv file for an extra forward slash." << endl;  //it's an error.
					exit(1);
				}
			}
			if (date.length() == 8) {   //date of length 8
				if (stoi(date.substr(0, 2)) < 1 || stoi(date.substr(0, 2)) > 9) {     //if month not 1-9 it's an error.
					cerr << "Check the date column in the .csv file for an invalid entry." << endl;  //it's an error.
					exit(1);
				}
				else if (date.substr(1, 1) != "/" || date.substr(3, 1) != "/") {     //forward slashes must be in the second and fourth position.
					cerr << "Check the date column in the .csv file for an invalid entry." << endl;  //it's an error.
					exit(1);
				}
				else if (stoi(date.substr(2, 1)) == 0) {     //days must be between 1 and 9.
					cerr << "Check the date column in the .csv file for a day of 0." << endl;  //it's an error.
					exit(1);
				}
				else if (stoi(date.substr(4, 4)) > stoi(currentDate.substr(currentDate.length() - 4))){   //year can't be greater than current year.
					cerr << "Check the date column in the .csv file for a an invalid year." << endl;  //it's an error.
					exit(1);
				}
			}  //end if date length 8
			else if (date.length() == 10) {  //date of length 10.
				if (stoi(date.substr(0, 2)) < 10 || stoi(date.substr(0, 2)) > 12) {     //if month not 10-12 it's an error.
					cerr << "Check the date column in the .csv file for an invalid entry." << endl;  //it's an error.
					exit(1);
				}
				else if (date.substr(2, 1) != "/" || date.substr(5, 1) != "/") {     //forward slashes must be in the third and sixth position.
					cerr << "Check the date column in the .csv file for an invalid entry." << endl;  //it's an error.
					exit(1);
				}
				else if (stoi(date.substr(3, 2)) > 31 || stoi(date.substr(3, 2)) < 10) {     //days must be between 10 and 31.
					cerr << "Check the date column in the .csv file for an invalid day." << endl;  //it's an error.
					exit(1);
				}
				else if (stoi(date.substr(6, 4)) > stoi(currentDate.substr(currentDate.length() - 4))) {   //year can't be greater than current year.
					cerr << "Check the date column in the .csv file for a an invalid year." << endl;  //it's an error.
					exit(1);
				}
			} //end if date length 10
			else if (date.length() == 9) {
				if (stoi(date.substr(0, 2)) < 1 || stoi(date.substr(0, 2)) > 12) {     //if month not 1-12 it's an error.
					cerr << "Check the date column in the .csv file for an invalid month." << endl;  //it's an error.
					exit(1);
				}
				else if (stoi(date.substr(0, 2)) < 10) {                        //months 1-9.
					if (date.substr(1, 1) != "/" || date.substr(4, 1) != "/") {     //forward slashes must be in the second and fifth position.
						cerr << "Check the date column in the .csv file for an invalid entry." << endl;  //it's an error.
						exit(1);
					}
					else if (stoi(date.substr(2, 2)) > 31 || stoi(date.substr(2, 2)) < 10) {     //days must be between 10 and 31.
						cerr << "Check the date column in the .csv file for an invalid day." << endl;  //it's an error.
						exit(1);
					}
				}
				else if (stoi(date.substr(0, 2)) > 9) {          //months 10-12
					if (date.substr(2, 1) != "/" || date.substr(4, 1) != "/") {     //forward slashes must be in the third and fifth position.
						cerr << "Check the date column in the .csv file for an invalid entry." << endl;  //it's an error.
						exit(1);
					}
					else if (stoi(date.substr(3, 1)) == 0) {     //days must be between 1 and 9, 0 is the only other option for this length.
						cerr << "Check the date column in the .csv file for a day of 0." << endl;  //it's an error.
						exit(1);
					}
					
				}
				if (stoi(date.substr(5, 4)) > stoi(currentDate.substr(currentDate.length() - 4))) {   //year can't be greater than current year.
					cerr << "Check the date column in the .csv file for a an invalid year." << endl;  //it's an error.
					exit(1);
				}
			}// end if date length 9
			
			getline(inputString, name, ',');
			if (name == "") {     //check for empty cell.
				cerr << "Check name column in .csv file for empty cells." << endl;  //it's an error.
				exit(1);
			}
			else if (name.length() > 35) {         //can't have a name longer than 35 characters for printing output neatly.
				cerr << "A transaction name is too long. They can be no longer than 35 characters." << endl;
				exit(1);
			}

			getline(inputString, category, ',');
			if (category == "") {     //check for empty cell.
				cerr << "Check category column in .csv file for empty cells." << endl;  //it's an error.
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
				else if (tempString[i] == '.') {  //should only have 1 '.' in amount data.
					countperiod++;
				}
				else if (tempString[i] == '-' && i > 0) {  //can have a hyphen becuase of negative numbers, but it should always be the first character.
					counthyphen++;
				}
				else if (countperiod > 1 || counthyphen > 1) {  //if either of these is greater than 1 there is an error.
					cerr << "Check amount column in masterAccount .csv file for invalid entries." << endl;
					exit(1);
				}
				else if (i == 0 && tempString[i] == '0') {   //all transactions should have some value and not be 0.
					cerr << "Can't have an amount of 0." << endl;
					exit(1);
				}
			}
			if (tempString == "") {     //check for empty cell.
				cerr << "Check amount column in .csv file for empty cells." << endl;  //it's an error.
				exit(1);
			}
			amount = atof(tempString.c_str());      //uses atof to take string tempString and return a double into amount.

			getline(inputString, type, ',');
			if (type != "I" && type != "E") {  //'I' and 'E' are the only valid entries for this column
					cerr << "Check type column in .csv file for invalid entries or blank cells." << endl;  //it's an error.
					exit(1);
				}
			else if (type == "I" && amount < 0) {  //income should positive.
				cerr << "There is a negative value labelled as income." << endl;  //it's an error.
				exit(1);
			}
			else if (type == "E" && amount > 0) {  //expenses should be negative.
				cerr << "There is a positive value labelled as an expense." << endl;  //it's an error.
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
		monthYrs.insert(pairs);      //inserts pairs into a set, it will store only unique combos of year and month.
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
	int data = 0;              //to see if that month and year are in the file
	for (int i = 0; i < masterAccount.size(); i++) {  //loop through the master file.
		if (stoi(masterAccount[i].date.substr(0, 2)) == mo && stoi(masterAccount[i].date.substr(masterAccount[i].date.length() - 2)) == yr) {
			data++;   //if the month and year match there is data there.
			break;
		}
	}
	if (data == 0) {  //no data.  return function.
		cout << "There is no data in the file for that month and year." << endl;
		cout << endl;
		return;
	}
	cout << "Here are all the transactions for " << mo << "/" << yr << "." << endl;  //else print out transactions.
	cout << endl;
	cout << left << setw(13) << "Date" << setw(40) << "Transaction" << setw(30) << "Category" << setw(20) << "Amount" << setw(8) << "Type" << "Account" << endl;
	for (int i = 0; i < masterAccount.size(); i++) {    //loop through the whole master file. print out every transaction if month and year match.
		if (stoi(masterAccount[i].date.substr(0, 2)) == mo && stoi(masterAccount[i].date.substr(masterAccount[i].date.length() - 2)) == yr) {
			cout << left << setw(13) << masterAccount[i].date << setw(40) << masterAccount[i].name << setw(30) << masterAccount[i].category <<
				"$" << setw(19) << masterAccount[i].amount << setw(8) << masterAccount[i].type << masterAccount[i].accountType << endl;
		}
	}
	cout << endl;
}

void Accounts::getCatAllTime(string cat) {     // gets all transactions in a specific category all time.
	cout << "These are your all time transactions for " << cat << "." << endl;
	cout << endl;
	cout << left << setw(13) << "Date" << setw(40) << "Transaction" << setw(30) << "Category" << setw(20) << "Amount" << setw(8) << "Type" << "Account" << endl;
	for (int i = 0; i < masterAccount.size(); i++) {    
		if (masterAccount[i].category == cat) {     //print out if there's a match.
			cout << left << setw(13) << masterAccount[i].date << setw(40) << masterAccount[i].name << setw(30) << masterAccount[i].category <<
				"$" << setw(19) << masterAccount[i].amount << setw(8) << masterAccount[i].type << masterAccount[i].accountType << endl;
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

	set<pair<int, int>>::iterator it;       //iterate through the set of month/years.
	for (it = monthYrs.begin(); it != monthYrs.end(); ++it) {  //for each unique month year combo
		for (int j = 0; j < masterAccount.size(); j++) {   //loop through entire masterAccount file for matches.
			/*if matches category and unique month and year*/
			if (masterAccount[j].category == cat && stoi(masterAccount[j].date.substr(0, 2)) == it->second && stoi(masterAccount[j].date.substr(masterAccount[j].date.length() - 2)) == it->first) {
				monthlyTotal += masterAccount[j].amount;   //add the amounts. resets to 0 for each new month/year combo outside of loop below.
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
	float monthEndTotals = 0;            //Add all month end values for an overall average.   
	cout << "These are your month end cash flow values." << endl;
	cout << endl;
	cout << left << setw(16) << "Month/Year" << "Cash Flow" << endl;

	set<pair<int, int>>::iterator it;   //going to loop through the unique set of months and years.
	set<pair<int, int>>::iterator itt = monthYrs.end();  //store in iterator for the current month and year.
	--itt;                              //decrement iterator because still not at month end yet for current month and don't want to print that.
	for (it = monthYrs.begin(); it != itt; ++it) {          //loop through unique months and years until previous month.
		for (int j = 0; j < masterAccount.size(); j++) {   //loop through entire masterAccount file.
			/*if matches unique month and year*/
			if (stoi(masterAccount[j].date.substr(0, 2)) == it->second && stoi(masterAccount[j].date.substr(masterAccount[j].date.length() - 2)) == it->first) {
				monthEnd += masterAccount[j].amount;   //add the amounts.
			}
		}
		cout << left << it->second << "/" << it->first << '\t' << '\t' << "$" << monthEnd << endl;   //output the monthly total.
		monthEndTotals += monthEnd;   //add each monthly total for a grand total for to calculate average monthly total.
	}
	cout << "Your average month end cash flow value is $" << monthEndTotals / (monthYrs.size()-1) << "." << endl;     //output monthly average.
	cout << endl;
}

