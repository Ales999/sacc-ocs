// namepair.cpp
#include <iostream>
#include <cstring>
#include <string>
//#include <string.h>
//#include 

#include "namepair.h"

using namespace std;

// Конструктор
NamePair::NamePair(const string & initName,
		   const string & initValue):_name(initName), _value(initValue),
_next(NULL)
{
#ifdef DEBUG
	clog << "(C!)-Конструктор NamePair: " << _name << " <-> " <<
	    _value << endl;
#endif
}

NamePair::~NamePair()
{
	if (_next) {
		delete _next;
	}
#ifdef DEBUG
	clog <<
	    "(D!)-Вызван дефолтный деструктор NamePair"
	    << endl;
#endif
}

 NamePairList::NamePairList():_head(NULL), _last(NULL)
{
#ifdef DEBUG
	clog << "(C!)-Вызван конструктор NamePairList" << endl;
#endif

}

NamePairList::~NamePairList()
{
	if (_head)
		delete _head;
}

// AddPair
const NamePair *NamePairList::AddPair(const string & newName,
				      const string & newValue)
{
	NamePair *sp = new NamePair(newName, newValue);
	if (_head == NULL) {
		_head = sp;
		_last = sp;
	} else {
		_last->SetNext(sp);
		_last = sp;
	}
#ifdef DEBUG
	std::clog << "(A!)-Добавлена пара:\t" << newName <<
	    " <--> " << newValue << std::endl;
#endif
	return sp;
}

// GetValue
bool NamePairList::GetValue(const string & findName, string * buf)
{
	NamePair *temp = _head;
	(*buf) = "";
	if (temp != NULL) {
		do {
			if (findName.compare(temp->GetName()) == 0) {
				(*buf) = temp->GetValue();
#ifdef DEBUG
				std::clog << "(F!)-Найдена пара:\t"
				    << temp->GetName() << " <--> " << temp->
				    GetValue() << std::endl;
#endif
				return true;
			}
			temp = temp->GetNext();
		}
		while (temp != NULL);
	}
	return false;
}
