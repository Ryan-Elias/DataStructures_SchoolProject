#include "DrillingRecord.h"
#include <iomanip>
#include "DrillingRecord.h"



//copied from Driller 0.4 (general format for output matches the previous Drillers)

std::ostream& operator<<(std::ostream& os, const DrillingRecord& record) {
	os << record.strings[0] << ";" << record.strings[1] << ";";

	//for loop prints the first 15 numbers, the additional one is done to omit the trailing semicolon.
	for (int i = 0; i < 15; ++i) {
		os << std::fixed << std::setprecision(2) << std::showpoint << record.nums[i] << ";";
	}
	//ommit endline here and add when calling the function in main to maintain consistency.
	os << record.nums[15];

	return os;
}

//default constructor
DrillingRecord::DrillingRecord() {

	//initializes all nums to 0.0
	for (unsigned int i = 0; i < 16; ++i) {
		this->nums[i] = 0.0;
	}

	//initializes strings to "" (null)
	this->strings[0] = "";
	this->strings[1] = "";
}

//the following must throw exceptions if attempts are made to add too many values

//puts num in array, increments counter
void DrillingRecord::addNum(double num) {

	//by calling at the beginning, the exception will only be thrown if an additional call to addNum is made after the array is full. 
	if (numCtr >= MAX_NUMS) {
		throw new ExceptionIndexOutOfRange();
	}

	//numCtr starts at zero, so I will use it as the index to add the number then increment. 
	this->nums[numCtr] = num;
	++numCtr;
}

//addString works the same as addNum
void DrillingRecord::addString(std::string string) {

	if (strCtr >= MAX_STRINGS) {
		throw new ExceptionIndexOutOfRange();
	}

	this->strings[strCtr] = string;
	++strCtr;
}


//the following must throw index out of bounds exceptions if the index is too large. The index cannot be negative because it is an unsigned int.

double DrillingRecord::getNum(unsigned int index) const {

	// if number is greater than or equal MAX_NUMS, throw exception. otherwise, return number at index.
	if (index >= MAX_NUMS) {
		throw new ExceptionIndexOutOfRange();
	}
	return this->nums[index];
}

//getString will work the same as getNum
std::string DrillingRecord::getString(unsigned int index) const {
	if (index >= MAX_STRINGS) {
		throw new ExceptionIndexOutOfRange();
	}

	return this->strings[index];
}

//for setNum and setString, checks to make sure index is valid, if so, changes the item at that index to the specified value.
void DrillingRecord::setNum(double num, unsigned int index) {
	if (index >= MAX_NUMS) {
		throw new ExceptionIndexOutOfRange();
	}

	this->nums[index] = num;
}

void DrillingRecord::setString(std::string string, unsigned int index) {
	if (index >= MAX_STRINGS) {
		throw new ExceptionIndexOutOfRange();
	}

	this->strings[index] = string;
}