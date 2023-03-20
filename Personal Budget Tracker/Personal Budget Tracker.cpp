/*Adam Scibelli-3/19/2023
SDEV 435-Final Project-Personal Budget Tracker
This is the main file that instantiates the Accounts, Budget, and SavingFor classes.
It prints out the user's current cash flow, expected month end cash flow, and notifications 
regarding categories they have gone over budget in, as well as contains the menu interface.*/

#include "Accounts.h"
#include "Budget.h"
#include "SavingFor.h"
#include <iostream>
#include <iomanip> 
using namespace std;

int main()
{
	time_t rawtime;   //for use with getting the current day and time.
	struct tm timeinfo;
	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);
	char buffer[70];
	strftime(buffer, 70, "%m/%d/%Y", &timeinfo);  //formats today's date into string buffer.

	/*instantiate three classes*/
	Accounts master((float)100, "MasterAccounts.csv", buffer); //pass in beginning cash flow value(100) before pasting transactions into master .csv file.
	Budget budget("Budget.csv", buffer, master);
	SavingFor saving("Saving.csv");

	cout << "Hello, today is " << buffer << ". " << endl;
	/*only print out two decimal places because it's currency.*/
	cout << "Your current cash flow is $" << fixed << setprecision(2) << master.calcCurrCashFlow() << "." << endl;
	cout << "Your projected month end cash flow value based on your spending so far this month and your remaining budget allocations is $"
		<< fixed << setprecision(2) << budget.calcEndMoCashFlow(master.calcCurrCashFlow(), master) << "." << endl; //print out expected month end cash flow.
	budget.budgetAlerts();                   //print out alerts for over budget categories if there are any.
	cout << endl;


	while (1) {  //menu runs in an infinite loop.
		int option = 0;     //user enters a number from the menu based on what they want to do.
		cout << "Welcome to the main menu." << endl;
		cout << "What would you like to do?" << endl;
		cout << "Enter 1 to see historical transactions and balances." << endl;
		cout << "Enter 2 to creat/change a budget plan and track your budget." << endl;
		cout << "Enter 3 to plan for future purchases." << endl;
		cout << "Enter 4 to quit the program." << endl;
		cin >> option;          //reads input.
		while (cin.fail() || option < 1 || option > 4) {  //input can't be non integer characters or integers not 1-4.
			cout << "Sorry, that is not valid input. Please try again. " << endl;
			cin.clear();        //clears error if users enters a string.
			cin.ignore(256, '\n');  //ignores up to 256 characters in the last user input.
			cin >> option;   //takes new input.
		}
		switch (option) {
		case 1: {
			int answer = 0;
			do {   //stay in this menu on a loop until user presses 5 to go back or 6 to quit.
				cin.ignore(numeric_limits<streamsize>::max(), '\n');   //discards input buffer from previous menu in case a float (1.2) was entered in error.
				cout << "How would you like to view your history?" << endl;
				cout << "Enter 1 to see all transactions in specific month." << endl;
				cout << "Enter 2 to see all transactions in a specific category all time." << endl;
				cout << "Enter 3 to see all monthly totals in a specific category." << endl;
				cout << "Enter 4 to see all your month end cash flow values." << endl;
				cout << "Enter 5 to go back." << endl;        //goes back to main menu.
				cout << "Enter 6 to quit the program" << endl; //terminates program.
				cin >> answer;
				while (cin.fail() || answer < 1 || answer > 6) {  //input can't be non integer characters or integers not 1-6.
					cout << "Sorry, that is not valid input. Please try again. " << endl;
					cin.clear();   //clears error if users enters a string.
					cin.ignore(256, '\n');    //ignores up to 256 characters in the last user input.
					cin >> answer;   //takes new input.
				}
				switch (answer) {
				case 1: {
					cin.ignore(numeric_limits<streamsize>::max(), '\n');   //discards input buffer from previous menu in case a float(1.2) was entered.
					int month = 0;
					int year = 0;
					cout << "Enter the month as a number from 1-12 where 1 is January and 12 is December. " << endl;    //get month.
					cin >> month;
					while (cin.fail() || month < 1 || month > 12) {  //input can't be non integer characters or integers not 1-12.
						cout << "Sorry, that is not valid input. Please try again. " << endl;
						cin.clear();   //clears error if users enters a string.
						cin.ignore(256, '\n');    //ignores up to 256 characters in the last user input.
						cin >> month;   //takes new input.
					}
					cin.ignore(numeric_limits<streamsize>::max(), '\n');   //discards input buffer from previous menu in case a float(1.2) was entered.
					cout << "Enter the year as a number from 0-99.  For example, 2000 should be entered as 0 and 2099 should be entered as 99. " << endl; 
					cin >> year;
					while (cin.fail() || year < 0 || year > 99) {  //input can't be non integer characters or integers not 0-99.
						cout << "Sorry, that is not valid input. Please try again. " << endl;
						cin.clear();   //clears error if users enters a string.
						cin.ignore(256, '\n');    //ignores up to 256 characters in the last user input.
						cin >> year;   //takes new input.
					}
					cout << endl;
					master.getAllTransMo(month, year);  //get all transactions in a month.
					break;
				} //end case 1
				case 2: {
					cin.ignore(numeric_limits<streamsize>::max(), '\n');   //discards input buffer from previous menu in case float(2.5) was entered.
					int category = 0;
					cout << "Here's a list of categories in the file. Enter the corresponding number of the category you want to view. " << endl;
					cout << endl;
					cout << left << setw(17) << "Menu Option" << setw(25) << "Category" << endl;
					for (int i = 0; i < master.uniqueCategories.size(); i++) {
						cout << left << setw(17) << i + 1 << master.uniqueCategories[i] << endl;  //print out unique categories for user with menu option.
					}
					cin >> category;
					while (cin.fail() || category < 1 || category > master.uniqueCategories.size()) { //option has to be within size of uniqueCategories.
						cout << "Sorry, that is not valid input.  Please try again. " << endl;
						cin.clear();   //clears error if users enters a string.
						cin.ignore(256, '\n');    //ignores up to 256 characters in the last user input.
						cin >> category;   //takes new input.
					}
					cout << endl;
					master.getCatAllTime(master.uniqueCategories[category - 1]);  //pass in unique category as a string and get all transactions for it.
					break;
				} //end case 2
				case 3: {
					cin.ignore(numeric_limits<streamsize>::max(), '\n');   //discards input buffer from previous menu in case float(3.6) was entered.
					int category = 0;
					cout << "Here's a list of categories in the file. Enter the corresponding number of the category you want to view. " << endl;
					cout << endl;
					cout << left << setw(17) << "Menu Option" << setw(25) << "Category" << endl;
					for (int i = 0; i < master.uniqueCategories.size(); i++) {
						cout << left << setw(17) << i + 1 << master.uniqueCategories[i] << endl;   //print out unique categories for user with menu options.
					}
					cin >> category;
					while (cin.fail() || category < 1 || category > master.uniqueCategories.size()) {
						cout << "Sorry, that is not valid input.  Please try again. " << endl;
						cin.clear();   //clears error if users enters a string.
						cin.ignore(256, '\n');    //ignores up to 256 characters in the last user input.
						cin >> category;   //takes new input.
					}
					cout << endl;
					master.getMonthlyByCat(master.uniqueCategories[category - 1]); //pass unique category as a string and get the monthly totals for it.
					break;
				}  //end case 3
				case 4: {
					cout << endl;
					master.getMonthEndCashFlows();  //get list of month end cash flows.
					break;
				} //end case 4
				case 6:
					exit(0);     //terminates program.
				} //end switch
			} while (answer != 5);
			cin.ignore(numeric_limits<streamsize>::max(), '\n');   //discards input buffer from previous menu in case a float (5.5) was entered in error.
			break;     //breaks out so program stays in main menu loop.
		} //end case 1
		case 2: {
			int answer = 0;
			do {   //stay in this menu on a loop until user presses 5 to go back or 6 to quit.
				cin.ignore(numeric_limits<streamsize>::max(), '\n');   //discards input buffer from previous menu in case float (4.5) was entered.
				cout << "Here you can set monthly budgeted amounts for your categories, compare your current actual spending compared to your budgeted amounts, and calculate how much per month you can save based on your budgeted amounts." << endl;
				cout << "Enter 1 to view budgeted amounts." << endl;
				cout << "Enter 2 to set budget amounts." << endl;
				cout << "Enter 3 to compare actual to budget." << endl;
				cout << "Enter 4 to see how much your budget plan should save you each month and your actual month end projected savings." << endl;
				cout << "Enter 5 to go back." << endl;        //goes back to main menu.
				cout << "Enter 6 to quit the program" << endl; //terminates program.
				cin >> answer;
				while (cin.fail() || answer < 1 || answer > 6) {  //input can't be non integer characters or integers not 1-6.
					cout << "Sorry, that is not valid input. Please try again. " << endl;
					cin.clear();   //clears error if users enters a string.
					cin.ignore(256, '\n');    //ignores up to 256 characters in the last user input.
					cin >> answer;   //takes new input.
				}
				switch (answer) {
				case 1: {
					cout << "Here's a list of your categories and budgeted amounts." << endl;
					cout << endl;
					cout << left << setw(25) << "Category" << setw(15) << "Type" << "Budget Amount" << endl;
					for (int i = 0; i < budget.vec.size(); i++) {
						cout << left << setw(25) << budget.vec[i].category << setw(15) << budget.vec[i].type << "$" << budget.vec[i].amount << endl; //print out unique categories for user.
					}
					cout << endl;
					break;
				} //end case 1
				case 2: {
					cin.ignore(numeric_limits<streamsize>::max(), '\n');   //discards input buffer from previous menu in case float(2.2) was entered.
					int category = 0;
					cout << "Here's a list of your categories and budgeted amounts. Enter the corresponding number of the category you want to set a budget amount for under Menu Options." << endl;
					cout << endl;
					cout << left << setw(17) << "Menu Options" << setw(25) << "Category" << setw(15) << "Type" << "Budget Amount" << endl;
					for (int i = 0; i < budget.vec.size(); i++) {
						cout << left << setw(17) << i + 1 << setw(25) << budget.vec[i].category << setw(15) << budget.vec[i].type << "$" << budget.vec[i].amount << endl; //print out unique categories for user.
					}
					cin >> category;
					while (cin.fail() || category < 1 || category > budget.vec.size()) {  //user can only enter integers in the range of the size of the vector.
						cout << "Sorry, that is not valid input.  Please try again. " << endl;
						cin.clear();   //clears error if users enters a string.
						cin.ignore(256, '\n');    //ignores up to 256 characters in the last user input.
						cin >> category;   //takes new input.
					}
					budget.setAmount(budget.vec[category - 1].category, master);  //pass in the category user entered.
					break;
				}//end case 2
				case 3: {
					cout << "Here's a list of your current monthly totals per category compared to your budgeted amounts." << endl;
					cout << endl;
					budget.paidVersusBudget(master);         //compare actual to budgeted for each category.
					break;
				}//end case 3
				case 4: {
					budget.calcMonthlySavings();            //month end savings projections.
					break;
				}//end case 4
				case 6:
					exit(0);     //terminates program.
				} //end switch
			} while (answer != 5);
			cin.ignore(numeric_limits<streamsize>::max(), '\n');   //discards input buffer from previous menu in case a float (5.5) was entered in error.
			break;
		} //end case 2
		case 3: {
			int answer = 0;
			do {   //stay in this menu on a loop until user presses 6 to go back or 7 to quit.
				cin.ignore(numeric_limits<streamsize>::max(), '\n');   //discards input buffer from previous menu in case float(3.5) was entered.
				cout << "Here you can view and edit items that you want to save up for and see how long it will take to acquire them or pay them off." << endl;
				cout << "Enter 1 to view items." << endl;
				cout << "Enter 2 to edit an item." << endl;
				cout << "Enter 3 to add an item." << endl;
				cout << "Enter 4 to remove an item." << endl;
				cout << "Enter 5 to show timelines for acquiring items." << endl;
				cout << "Enter 6 to go back." << endl;  //goes back to last menu.
				cout << "Enter 7 to quit the program" << endl; //terminates program.
				cin >> answer;
				while (cin.fail() || answer < 1 || answer > 7) {  //input can't be non integer characters or integers not 1-7.
					cout << "Sorry, that is not valid input. Please try again. " << endl;
					cin.clear();   //clears error if users enters a string.
					cin.ignore(256, '\n');    //ignores up to 256 characters in the last user input.
					cin >> answer;   //takes new input.
				}
				switch (answer) {
				case 1: {
					if (saving.objects.size() < 1) {      //if no items entered.
						cout << "You have not entered any items to save for yet." << endl;
						cout << endl;
						break;
					}
					saving.getItems();   //print list of items.
					break;
				} //end case 1
				case 2: {
					if (saving.objects.size() < 1) {  //if no items entered.
						cout << "You have not entered any items to save for yet." << endl;
						cout << endl;
						break;
					}
					int item = 0;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');   //discards input buffer from previous menu in case a float was entered.
					cout << "Here's a list of items you are saving for listed by priority. Enter the corresponding number under Priority that you want to edit. " << endl;
					cout << endl;
					cout << left << setw(17) << "Priority" << setw(25) << "Item" << "Cost" << endl;
					for (int i = 0; i < saving.objects.size(); i++) {
						cout << left << setw(17) << i + 1 << setw(25) << saving.objects[i].item << "$" << saving.objects[i].cost << endl; //print out items for user.
					}
					cin >> item;
					while (cin.fail() || item < 1 || item > saving.objects.size()) {    //input has to be integers within size of vector.
						cout << "Sorry, that is not valid input.  Please try again. " << endl;
						cin.clear();   //clears error if users enters a string.
						cin.ignore(256, '\n');    //ignores up to 256 characters in the last user input.
						cin >> item;   //takes new input.
					}
					saving.editItems(saving.objects[item - 1].item);  //pass the item as a string.
					break;
				}//end case 2
				case 3: {
					saving.addItems();
					break;
				} //end case 3
				case 4: {
					if (saving.objects.size() < 1) {   //if no items entered.
						cout << "You have not entered any items to save for yet." << endl;
						cout << endl;
						break;
					}
					cin.ignore(numeric_limits<streamsize>::max(), '\n');   //discards input buffer from previous menu.
					int item = 0;
					cout << "Here's a list of items you are saving for listed by priority. Enter the corresponding number under Priority that you want to delete. " << endl;
					cout << endl;
					cout << left << setw(17) << "Priority" << setw(25) << "Item" << "Cost" << endl;
					for (int i = 0; i < saving.objects.size(); i++) {
						cout << left << setw(17) << i + 1 << setw(25) << saving.objects[i].item << "$" << saving.objects[i].cost << endl;       //print out items for user.
					}
					cin >> item;
					while (cin.fail() || item < 1 || item > saving.objects.size()) {
						cout << "Sorry, that is not valid input.  Please try again. " << endl;
						cin.clear();   //clears error if users enters a string.
						cin.ignore(256, '\n');    //ignores up to 256 characters in the last user input.
						cin >> item;   //takes new input.
					}
					saving.deleteItems(saving.objects[item - 1].item);   //pass in the item as a string.
					break;
				} //end case 4
				case 5: {
					if (saving.objects.size() < 1) {    //if no items entered.
						cout << "You have not entered any items to save for yet." << endl;
						cout << endl;
						break;
					}
					saving.projectedAcquisition(budget, budget.calcEndMoCashFlow(master.calcCurrCashFlow(), master), timeinfo);
					break;
				}//end case 5
				case 7:
					exit(0);     //terminates program.
				} //end switch
			} while (answer != 6);
			cin.ignore(numeric_limits<streamsize>::max(), '\n');   //discards input buffer from previous menu in case a float (6.5) was entered in error.
			break;
		} //end case 3
		case 4:
			exit(0);        //terminates the program.
		} //end case 4
	} //end while
} //end main

