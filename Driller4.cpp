#include <iostream>
#include "Comparator.h"
#include "ResizableArray.h"
#include "DrillingRecord.h"
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
//#include "OULinkedList.h"
#include "OULink.h"
#include "OULinkedListEnumerator.h"
#include "DrillingRecordComparator.h"
#include "Sorter.h"
#include "Search.h"
#include "Enumerator.h"
#include "Exceptions.h"
//#include "HashTable.h"
#include "HashTableEnumerator.h"
#include "Hasher.h"
#include "DrillingRecordHasher.h"
#include "AVLTree.h"
//#include "AVLTreeEnumerator.h"
//#include "AVLTreeOrder.h"


using namespace std;



vector<string> parseLine(string lineData) {
	vector<string> data;
	istringstream inSS;
	inSS.str(lineData);
	string tempData = "";

	for (unsigned int i = 0; i < 18; ++i) {
		getline(inSS, tempData, ',');
		data.push_back(tempData);
	}
	inSS.clear();
	tempData = "";
	return data;
}

DrillingRecord VectorToRecord(vector<string> data) {
	DrillingRecord temp;
	temp.addString(data[0]);
	temp.addString(data[1]);
	for (unsigned int i = 2; i < data.size(); ++i) {
		temp.addNum(stod(data[i]));
	}
	return temp;
}

bool NumericDataCheck(vector<string> record) {

	for (unsigned long i = 2; i < 18; ++i) {
		if (stod(record[i]) <= 0) {
			return false;
		}
	}
	return true;
}

bool arrayTimeCheck(string time, ResizableArray<DrillingRecord>& array) { //could also search array for matching time.
	for (unsigned int i = 0; i < array.getSize(); ++i) {
		if (time == array.get(i).getString(1)) {
			return false;
		}
	}
	return true;
}

bool listTimeCheck(string time, AVLTree<DrillingRecord>* tree) {
	DrillingRecord temp;
	temp.setString(time, 1);
	bool passes = tree->contains(temp); 
	return passes;
}

bool DateCheck(string curDate, string testDate) {
	//cout << record << endl << endl << date << endl; //TEST
	if (curDate != testDate) { //this line throws an exception bad_alloc on line 5 of mixed.csv
		return false;
	}
	return true;
}


//instead of returning a linkedlist, all files need to return or take a AVLTree.
AVLTree<DrillingRecord>* readFile(vector<unsigned long>& tallies, DrillingRecordComparator* comp, string* testDate) {
	string userInput = "placeholder";
	ifstream infile;

	AVLTree<DrillingRecord>* tree = new AVLTree<DrillingRecord>(comp);

	unsigned long validLines = 0;
	unsigned long lineNumber = 0;
	string data = "";
	string date = "placeholder";


	while (!userInput.empty()) {

		cout << "Enter data file name: ";
		getline(cin, userInput);
		if (userInput.empty()) {
			break;
		}

		infile.open(userInput);
		if (infile.fail()) {
			cout << "File is not available." << endl;
			infile.close();
			continue;
		}

		//add data to the linked list

		//first line	
		getline(infile, data);	//skip first line of non-data
		getline(infile, data); // line of first actual data
		vector<string> dataLine = parseLine(data);
		lineNumber = 0;



		//first record in file adds date to be tested
		date = dataLine[0];

		//if testDate does not equal the placeholder string, compares testDate to the date and closes the file if it does not match.
		if (*testDate != "placeholder") {
			if (date != *testDate) {
				cout << "Date mismatch; file closed." << endl;
				infile.close();
				break;
			}
		}
		//if testDate is equal to the placeholder date, sets current date to testDate.
		else {
			*testDate = date;
		}


		++lineNumber;

		//test numeric data
		bool dataCheck = NumericDataCheck(dataLine);

		if (!dataCheck) {
			cout << "Invalid floating-point data at line " << lineNumber << "." << endl;
		}
		else {
			tree->insert(VectorToRecord(dataLine));
			++validLines;
		}

		dataLine.clear();
		//loop the rest of the data from the file
		while (getline(infile, data)) {
			dataLine.clear();
			vector<string> dataLine = parseLine(data);
			++lineNumber;

			if (!DateCheck(dataLine[0], date)) {
				cout << "Non-matching date stamp " << dataLine[0] << " at line " << lineNumber << "." << endl;
				continue;
			}

			if (listTimeCheck(dataLine[1], tree)) {
				cout << "Duplicate timestamp " << dataLine[1] << " at line " << lineNumber << "." << endl;
				continue;
			}

			if (!NumericDataCheck(dataLine)) {
				cout << "Invalid floating-point data at line " << lineNumber << "." << endl;
				continue;
			}

			//if this point is reached all tests have passed.
			tree->insert(VectorToRecord(dataLine));
			//if validLines if ever equal to list.size when this next breakpoint is reached, then that lineNumber is the source of my error.
			validLines++;
		}


		tallies[0] += lineNumber;
		tallies[1] += validLines;
		infile.close();
		if (tree->getSize() > 0) {
			return tree;
		}
		//if nothing is valid, then error message will be output and the loop will continue (might not be correct if the goal is to identify files that have nothing that resembles record data)
		if (validLines == 0) {
			cout << "No valid records found." << endl;
		}
	}


	return tree;
}

AVLTree<DrillingRecord>* mergeTrees(AVLTree<DrillingRecord>* mainTree, AVLTree<DrillingRecord>* newTree) {

	//I think a preorder enumerator will be ideal for merging trees.
	AVLTreeEnumerator<DrillingRecord> e = newTree->enumerator(AVLTreeOrder::inorder);

	while (e.hasNext()) {
		if (mainTree->replace(e.peek())) {
			e.next();
		}
		else {
			mainTree->insert(e.next());
		}
	}

	delete newTree;
	return mainTree;
}

//prints to standard out
void printList(OULinkedList<DrillingRecord>* list, vector<unsigned long>& tallies) {


	OULinkedListEnumerator<DrillingRecord> e = list->enumerator();

	//prompt user
	string userInput = "placeholder";

	while (!userInput.empty()) {
		cout << "Enter output file name: ";
		getline(cin, userInput);
		if (userInput.empty()) {
			continue;
		}

		ofstream outFile;
		outFile.open(userInput);
		if (outFile.fail()) {
			cout << "File is not available." << endl;
			outFile.close();
			continue;
		}
		else {
			//PRINT TO FILE
			while (e.hasNext()) {
				outFile << e.next() << endl;
			}
		}
		outFile << "Data lines read: " << tallies[0] << "; Valid Drilling records read: " << tallies[1] << "; Drilling records in memory: " << list->getSize() << endl;
			return;
		}

	//PRINTS TO STANDARD OUT (IF NO FILE SPECIFIED)
	//print first
	if (list->getSize() > 0) {
		while (e.hasNext()) {
			cout << e.next() << endl;
		}
	}
	cout << "Data lines read: " << tallies[0] << "; Valid drilling records: " << tallies[1] << "; Drilling records in memory: " << list->getSize() << endl;
	return;
}

void printArray(ResizableArray<DrillingRecord>* array, vector<unsigned long>& tallies) {

//prompt user
string userInput = "placeholder";

while (!userInput.empty()) {
	cout << "Enter output file name: ";
	getline(cin, userInput);
	if (userInput.empty()) {
		continue;
	}

	ofstream outFile;
	outFile.open(userInput);
	if (outFile.fail()) {
		cout << "File is not available." << endl;
		outFile.close();
		continue;
	}
	else {
		//PRINT TO FILE
		for (unsigned long i = 0; i < array->getSize(); ++i) {
			outFile << array->get(i) << endl;
		}
		outFile << "Data lines read: " << tallies[0] << "; Valid Drilling records read: " << tallies[1] << "; Drilling records in memory: " << array->getSize() << endl;
		return;
	}
}

//PRINTS TO STANDARD OUT (IF NO FILE SPECIFIED)
//print first
for (unsigned long i = 0; i < array->getSize(); ++i) {
	cout << array->get(i) << endl;
}
cout << "Data lines read: " << tallies[0] << "; Valid Drilling records read: " << tallies[1] << "; Drilling records in memory: " << array->getSize() << endl;
return;
}


//this will just be the thing that removes all elements of toRemove from main, then return main.
//deletes toRemove and returns main.
AVLTree<DrillingRecord>* purge(AVLTree<DrillingRecord>* main, AVLTree<DrillingRecord>* toRemove) {

	AVLTreeEnumerator<DrillingRecord> e = toRemove->enumerator(AVLTreeOrder::inorder);

	while (e.hasNext()) {
		main->remove(e.next()); 
	}
	delete toRemove;
	return main;
}

//this function will create a new ResizableArray using data from the linked list.
ResizableArray<DrillingRecord>* TreeToArray(AVLTree<DrillingRecord>* mainTree) {

	ResizableArray<DrillingRecord>* temporary = new ResizableArray<DrillingRecord>();
	AVLTreeEnumerator<DrillingRecord> e = mainTree->enumerator(AVLTreeOrder::inorder);

	while (e.hasNext()) {
		temporary->add(e.next());
	}
	return temporary;
}

//returns sortedColumn
int SortArray(ResizableArray<DrillingRecord>& array, unsigned int currentSort) {

	string userInput;


	//user prompt
	cout << "Enter sort field (0-17): ";
	getline(cin, userInput);

	//if the user response is greater than 17, exit function. userInput cannot be less than 0 because long is unsigned.
	//if the user response cannot be deciphered into a number, exits the function (I hope)
	if ((unsigned int)(stoi(userInput)) > 17) {
		return currentSort;
	}
	unsigned int column = (unsigned int)(stoi(userInput));
	DrillingRecordComparator* comp = new DrillingRecordComparator(column);
	Sorter<DrillingRecord>().sort(array, *comp);
	return column;
}

void linearSearch(ResizableArray<DrillingRecord>* array, DrillingRecord& item, DrillingRecordComparator& comp) {

	long total = 0;

	for (unsigned long i = 0; i < array->getSize(); ++i) {
		if (comp.compare(array->get(i), item) == 0) {
			cout << array->get(i) << endl;
			total++;
		}
	}


	cout << "Drilling records found: " << total << "." << endl;
} 



void SearchArray(ResizableArray<DrillingRecord>* array, unsigned int sortedColumn, HashTable<DrillingRecord>* table) {
	string userInput;
	DrillingRecord* toSearch = new DrillingRecord();
	cout << "Enter search field (0-17): ";
	getline(cin, userInput);
	

	//this will throw exceptoin if userInput cannot be converted to int
	if ((unsigned int)(stoi(userInput)) > 17) {
		return;
	}

	unsigned int column = (unsigned int)(stoi(userInput));
	DrillingRecordComparator* e = new DrillingRecordComparator(column);
	//if numeric column
	if (column > 1) {
		cout << "Enter positive field value: ";
		getline(cin, userInput);

		if (userInput.empty()) {
			return;
		}

		//will throw exception if userInput cannot be converted to a double
		double value = stod(userInput);
		if (value < 0) {
			return;
		}

		toSearch->setNum(value, column - 2);
	}
	//if string column
	else {
		cout << "Enter exact text on which to search: ";
		getline(cin, userInput);

		if (userInput.empty()) {
			return;
		}

		toSearch->setString(userInput, column);
	}

	//determine if the column is the time stamp, if so, search using the table, otherwise determine if the column is sorted, if not, linear search.
	if (column == 1) {
		try {
			std::cout << table->find(*toSearch) << std::endl;
			std::cout << "Drilling records found: 1." << std::endl;
		}
		catch (ExceptionHashTableAccess*) {
			std::cout << "Drilling records found: 0." << std::endl;
			return;
		}


	}
	else if (column == sortedColumn) {
		long firstIndex = binarySearch(*toSearch, *array, *e);
		unsigned long total = 0;


		if (firstIndex < 0) {
			std::cout << "Drilling records found: 0." << std::endl;
		}
		else {
			long index = firstIndex;
			while (e->compare(array->get(index), *toSearch) == 0) {
				std::cout << array->get(index) << std::endl;
				index++;
				total++;
			}
			std::cout << "Drilling records found: " << total << "." << std::endl;
		}

	}
	else {
		linearSearch(array, *toSearch, *e);
	}



}

void printTable(HashTable<DrillingRecord>* table, vector<unsigned long> tallies) {

	HashTableEnumerator<DrillingRecord>* tempEnum;

	DrillingRecordHasher hash = DrillingRecordHasher();
	unsigned long count = 0;


	//prompt user
	string userInput = "placeholder";

	while (!userInput.empty()) {
		cout << "Enter output file name: ";
		getline(cin, userInput);
		if (userInput.empty()) {
			continue;
		}

		ofstream outFile;
		outFile.open(userInput);
		if (outFile.fail()) {
			cout << "File is not available." << endl;
			outFile.close();
			continue;
		}
		else {
			//print to file
			for (unsigned long i = 0; i < table->getBaseCapacity(); ++i) {

				tempEnum = new HashTableEnumerator<DrillingRecord>(table);

				while (tempEnum->hasNext()) {

					if (hash.hash(tempEnum->peek()) % table->getBaseCapacity() == i) {
						if (count == 0) {
							outFile << i << ": " << tempEnum->next() << endl;
							++count;
						}
						else {
							outFile << "OVERFLOW: " << tempEnum->next() << endl;
							++count;
						}
					}
					else {
						tempEnum->next();
					}
				}
				if (count > 0) {
					outFile << endl;
				}
				count = 0;
				delete tempEnum;

			}

			outFile << "Base Capacity: " << table->getBaseCapacity() << "; Total Capacity: " << table->getTotalCapacity() << "; Load Factor: " << table->getLoadFactor() << endl;
			outFile << "Data lines read: " << tallies[0] << "; Valid Drilling records read: " << tallies[1] << "; Drilling records in memory: " << table->getSize() << endl;
			return;
		}
	}

	//print to standard out if not out to file
	for (unsigned long i = 0; i < table->getBaseCapacity(); ++i) {

		tempEnum = new HashTableEnumerator<DrillingRecord>(table);

		while (tempEnum->hasNext()) {

			if (hash.hash(tempEnum->peek()) % table->getBaseCapacity() == i) {
				if (count == 0) {
					cout << i << ": " << tempEnum->next() << endl;
					++count;
				}
				else {
					cout << "OVERFLOW: " << tempEnum->next() << endl;
					++count;
				}
			}
			else {
				tempEnum->next();
			}
		}
		if (count > 0) {
			cout << endl;
		}
		count = 0;
		delete tempEnum;

	}
	cout << "Base Capacity: " << table->getBaseCapacity() << "; Total Capacity: " << table->getTotalCapacity() << "; Load Factor: " << table->getLoadFactor() << endl;
	cout << "Data lines read: " << tallies[0] << "; Valid Drilling records read: " << tallies[1] << "; Drilling records in memory: " << table->getSize() << endl;
	return;

}

HashTable<DrillingRecord>* TreeToTable(AVLTree<DrillingRecord>* mainTree, DrillingRecordComparator* comp, unsigned long size) {
	HashTable<DrillingRecord>* tempTable = new HashTable<DrillingRecord> (comp, new DrillingRecordHasher, size);
	AVLTreeEnumerator<DrillingRecord> e = mainTree->enumerator(AVLTreeOrder::inorder);

	while (e.hasNext()) {
		tempTable->insert(e.next());
	}
	return tempTable;
}

void printTree(AVLTreeEnumerator<DrillingRecord>* e, vector<unsigned long> tallies, AVLTree<DrillingRecord>* tree) {


	//prompt user
	string userInput = "placeholder";

	while (!userInput.empty()) {
		cout << "Enter output file name: ";
		getline(cin, userInput);
		if (userInput.empty()) {
			continue;
		}

		ofstream outFile;
		outFile.open(userInput);
		if (outFile.fail()) {
			cout << "File is not available." << endl;
			outFile.close();
			continue;
		}
		else {
			//PRINT TO FILE
			while (e->hasNext()) {
				outFile << e->next() << endl;
			}
		}
		outFile << "Data lines read: " << tallies[0] << "; Valid Drilling records read: " << tallies[1] << "; Drilling records in memory: " << tree->getSize() << endl;
		return;
	}

	//PRINTS TO STANDARD OUT (IF NO FILE SPECIFIED)
	//print first
	while (e->hasNext()) {
		cout << e->next() << endl;
	}
	cout << "Data lines read: " << tallies[0] << "; Valid drilling records: " << tallies[1] << "; Drilling records in memory: " << tree->getSize() << endl;
	return;
}

int main() {
	DrillingRecordComparator* timeComp = new DrillingRecordComparator(1);
	ResizableArray<DrillingRecord>* records = new ResizableArray<DrillingRecord>();
	AVLTree<DrillingRecord>* mainTree = new AVLTree<DrillingRecord>(timeComp);
	unsigned int column = 1;
	vector<unsigned long>* tallies = new vector<unsigned long>{ 0,0,0 };
	string* testDate = new string;
	*testDate = "placeholder";


	//begin UI

	//first read

	mainTree = readFile(*tallies, timeComp, testDate);
	if (mainTree->getSize() == 0) { 
		return 0;
	}
	records = TreeToArray(mainTree);
	HashTable<DrillingRecord>* mainTable = TreeToTable(mainTree, timeComp, mainTree->getSize());
	//if the size of the mainTree after the first read is 0, then the app closes.

	string userInput = "placeholder";

	//MENU
	while (userInput != "q") {
		cout << "Enter (o)utput, (s)ort, (f)ind, (m)erge, (p)urge, (h)ash table, (pre)order, (in)order, (post)order, or (q)uit: ";
		getline(cin, userInput);

		//Output
		if (userInput == "o") {
			printArray(records, *tallies);
			continue;
		}
		//Sort
		if (userInput == "s") {
			try {
				column = SortArray(*records, column);
				continue;
			}
			catch (exception invalid_argument) {
				continue;
			}
		}
		//Search
		if (userInput == "f") {
			try {
				SearchArray(records, column, mainTable);
				continue;
			}
			catch (exception invalid_argumnet) {
				continue;
			}
		}
		//Merge
		if (userInput == "m") {
			mainTree = mergeTrees(mainTree, readFile(*tallies, timeComp, testDate));
			records->clear();
			records = TreeToArray(mainTree);
			delete mainTable;
			mainTable = TreeToTable(mainTree, timeComp, mainTree->getSize());
			continue;
		}
		//Purge
		if (userInput == "p") {
			mainTree = purge(mainTree, readFile(*tallies, timeComp, testDate));
			records->clear();
			delete mainTable;
			if (mainTree->getSize() > 0) {
				records = TreeToArray(mainTree);
				mainTable = TreeToTable(mainTree, timeComp, mainTree->getSize());
			}
			continue;
		}
		//Hash Table
		if (userInput == "h") {
			printTable(mainTable, *tallies);
			continue;
		}
		//preorder
		if (userInput == "pre") {
			AVLTreeEnumerator<DrillingRecord> e = mainTree->enumerator(AVLTreeOrder::preorder);
			printTree(&e, *tallies, mainTree);
		}
		//inorder
		if (userInput == "in") {
			AVLTreeEnumerator<DrillingRecord> e = mainTree->enumerator(AVLTreeOrder::inorder);
			printTree(&e, *tallies, mainTree);
		}
		//postorder
		if (userInput == "post") {
			AVLTreeEnumerator<DrillingRecord> e = mainTree->enumerator(AVLTreeOrder::postorder);
			printTree(&e, *tallies, mainTree);

		}
		//Quit
		if (userInput == "q") {
			cout << "Thanks for using Driller." << endl;
			continue; 
		}
	}
	return 0;
}





