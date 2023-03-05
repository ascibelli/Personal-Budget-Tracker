/*Adam Scibelli-3//2023
SDEV 435-Final Project-Personal Budget Tracker
This is the SavingFor class structure that is implemented in SavingFor.cpp.*/

#pragma once
#include "Budget.h"
using namespace std;

class SavingFor
{
private:
	struct savingItems {  //to store information about items being saved up for.
		string item;      
		float cost;       //price of the item.
		int priority;    //which item user wants the most.
		bool operator < (const savingItems& rhs) const {  //sorts the objects vector by priority when sort() used.
			return priority < rhs.priority;
		}
		savingItems(string itm, float cst, int pr);  //constructor for savingItems struct.
	};
	string savingFileName;    //.csv filename for storing the items to be saved for.
public:
	vector<savingItems> objects;     //vector to hold items saving for.
	SavingFor(string filename);       //constructor sets savingFileName.
	void getItems();                  //display a report of items saving for.
	void editItems(string item);      //edit existing items.
	void addItems();                  //function to add an object to be saved for.
	void deleteItems(string item);     //function to delete an object to be saved for.
	void projectedAcquisition(Budget budg, float cashFlow, struct tm time); //reports on when item can be acquired or how much monthly can be paid towards the item based on budget settings.
	template <typename T>          //template function to write any type of vector to columns and rows in a .csv file.          
	void writeToFile(vector <T> vec, string filename);
};



