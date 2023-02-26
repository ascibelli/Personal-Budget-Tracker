#pragma once
#include "Budget.h"
#include <string>
#include <vector>
using namespace std;

class SavingFor
{
private:
	struct savingItems {  //to store information about items being saved up for.
		string item;
		float cost;       //price of the item.
		int priority;    //save for this item 1st, 2nd, etc.
		bool operator < (const savingItems& rhs) const {  //sorts the objects vector by priority when sort() used.
			return priority < rhs.priority;
		}
		savingItems(string itm, float cst, int pr);  //constructor for savingItems struct.
	};
	string savingFileName;    //.csv filename for storing the items to be saved for.
public:
	vector<savingItems> objects;
	SavingFor(string filename);       //constructor sets savingFileName and currentDate;
	void getItems();                          //display a report of items saving for.
	void editItems(string item);                   //edit existing items.
	void addItems();                    //function to add an object to be saved for.
	void deleteItems(string item);                //function to delete an object to be saved for.
	void projectedAcquisition(Budget budg, float cashFlow, struct tm time);               //reports on how when item can be acquired or how much monthly can be paid towards the item based on budget settings.
	template <typename T>          //template function to write any type of vector to columns and rows in a .csv file.          
	void writeToFile(vector <T> vec, string filename);
};



