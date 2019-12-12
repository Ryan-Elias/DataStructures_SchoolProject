#include "DrillingRecordHasher.h"


//returns the key given by the item (the ASCII values of the timestamp)

unsigned long DrillingRecordHasher::hash(const DrillingRecord& item) const
{
	unsigned long aValue = 0;
	//each timestamp has 8 characters in it, the validity of the timestamp will be assessed before this is used.
	for (unsigned int i = 0; i < item.getString(1).length(); ++i) {
		aValue += item.getString(1)[i]; //i'm hoping this will simply return the casted character. 
	}
	return aValue;
}
