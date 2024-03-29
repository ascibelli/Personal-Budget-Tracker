/*Adam Scibelli-3/19/2023
SDEV 435-Final Project-Personal Budget Tracker
This is the Accounts class structure that is implemented in Accounts.cpp.*/

#pragma once
#include <string>
#include <vector>
#include <set>
using namespace std;

class Accounts
{
private:
	float BEG_CASH_FLOW;    //This is the user's cash flow before pasting in transactions and using the program for the first time.
	struct transaction {  //models a transaction and the .csv masterAccounts file.
		string date;     //date of transaction.
		string name;     //name of transaction.
		string category; //classification of transaction.
		float amount;   //amount of transaction.
		string type;     // income=I, expense=E.
		string accountType;  //checking/credit/savings.
		transaction(string dt, string nm, string cat, float amnt, string typ, string acntType);  //constructor for transaction to set values.
	};
	struct Date{        //date struct used to make sure the .csv file is sorted from oldest to newest.
		int day;
		int	month;
		int	year;
		Date(int d, int m, int y);    //constructor for Date truct.
	};
	vector<Date> dateArray1;     //will compare two vectors of dates, one sorted in the code, the other as it is taken in from the file to see if they match.
	vector<Date> dateArray2;    
	string accountFileName;  //the .csv file name.
	float currCashFlow;     //the result of beginning cash flow +/- all transactions in the .csv masterAccount file.
	string currentDate;
public:
	set<pair<int, int>> monthYrs;      //for storing month year combinations in the .csv file.
	vector<transaction> masterAccount;  //vector of type transaction to push transactions from .csv file into.
	vector<string> uniqueCategories;    //stores unique values for the categories field in the .csv file.
	Accounts(float beg, string fileName, string date);  //constructor for Accounts class.  Takes beginning cash flow, .csv file name, and current date.
	float calcCurrCashFlow();   //calculates the current cash flow value.
	void getAllTransMo(int mo, int yr);    //gets all transactions in a specific month and year.
	void getCatAllTime(string cat); //gets all transactions in a specific category all time.
	void getMonthlyByCat(string cat); //gets monthly totals in a specific category all time.
	void getMonthEndCashFlows();  //calculates month end cash flow values all time.
};

