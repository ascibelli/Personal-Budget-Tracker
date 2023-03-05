/*Adam Scibelli-3//2023
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
	float BEG_CASH_FLOW;    //This is the user's cash flow before using the program for the first time.
	struct transaction {  //models a transaction and the .csv masterAccounts file.
		string date;     //date of transaction.
		string name;     //name of transaction.
		string category; //type of transaction.
		float amount;   //amount of transaction.
		string type;     // income=I, expense=E.
		string accountType;  //checking/credit/savings.
		transaction(string dt, string nm, string cat, float amnt, string typ, string acntType);  //constructor for transaction to set values.
	};
	string accountFileName;  //the .csv file name.
	float currCashFlow;     //the result of beginning cash flow +/- all transactions in the .csv masterAccount file.
	set<pair<int, int>> monthYrs;      //for storing month year combinations in the .csv file.
	string currentDate;
public:
	vector<transaction> masterAccount;  //vector of type transaction to push transactions from .csv file into.
	vector<string> uniqueCategories;    //stores unique values for the categories field in the .csv file.
	Accounts(float beg, string fileName, string date);  //constructor for Accounts class.  Takes beginning cash flow and .csv file name.
	float calcCurrCashFlow();   //calculates the current cash flow value.
	void getAllTransMo(int mo, int yr);    //gets all transactions in a specific month and year.
	void getCatAllTime(string cat); //gets all transactions in a specific category all time.
	void getMonthlyByCat(string cat); //gets monthly totals in a specific category all time.
	void getMonthEndCashFlows();  //calculates month end cash flow values all time.
};

