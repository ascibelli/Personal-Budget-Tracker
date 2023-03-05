/*Adam Scibelli-3//2023
SDEV 435-Final Project-Personal Budget Tracker
This is the Budget class structure that is implemented in Budget.cpp.*/

#pragma once
#include "Accounts.h" 

using namespace std;

class Budget
{
private:
	struct budgeting {  //budget by category and amount.
		string category;     //income/expense category.
		float amount;       //budgeted amount.
		string type;        //I=income, E=expense.
		budgeting(string cat, float amnt, string typ);  //constructor for budgeting struct.
	};
	string budgetFileName;    //.csv filename for saving the budget settings.
	string currentDate;     //holds the current date.
	vector<budgeting> overBudget; //vector of categories that have gone over expected budget for alerts.
public:
	vector<budgeting> vec;    //vector of budgeting struct.
	Budget(string fileName, string date, Accounts master);   //constructor for Budget class. 
	void setAmount(string cat);    //set a budget amount for a category.
	void paidVersusBudget(Accounts master);      //will display actual expenditures versus budgeted amounts.
	void calcMonthlySavings();    //calculates a projected month end savings amount based on the budget and if it is followed.
	float calcEndMoCashFlow(float currentCashFlow, Accounts master);     //calculates the expected month end cash flow based on actual expenditures and remaining budgeted amounts.
	void budgetAlerts();       //alerts the user when they are over budget on categories.
	template <typename T>  //template function to write any type of vector to columns and rows in a .csv file.          
	void writeToFile(vector <T> vec, string filename);
	float calcBudgetedMoSavings();      //calculates monthly savings if budget is followed exactly.
	float calcExpectedMoSaving();    //calculates expected monthly savings based on adjustments for overbudget transactions.
};


