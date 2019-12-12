#include "DrillingRecordComparator.h"


DrillingRecordComparator::DrillingRecordComparator(unsigned int column) {

	this->column = column;
}



//valid numbers are 0-17, if else, throws exception elsewhere. 
//0 is the date
//1 is the time
//2-17 are the numeric values
// returns -1 if item1 < item2, 0 if item1 == item2, +1 if item1 > item2
int DrillingRecordComparator::compare(const DrillingRecord& item1, const DrillingRecord& item2) const {

	if (column < 2) {
		if (item1.getString(column) < item2.getString(column)) { //less than
			return -1;
		}
		else if (item1.getString(column) > item2.getString(column)) { //greater than
			return 1;
		}
		else { //is equal
			return 0;
		}
	}

	//because column is 0-17 and num is 0-15, then the actual value being compared would be num[index - 2]
	else  {
		if (item1.getNum(column - 2) < item2.getNum(column - 2)) {
			return -1;
		}
		else if (item1.getNum(column - 2) > item2.getNum(column - 2)) {
			return 1;
		}
		else {
			return 0;
		}
	}
}