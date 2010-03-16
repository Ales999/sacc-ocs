//  namepair.h
#ifndef NAME_PAIR_H
#define NAME_PAIR_H

#include <string>

class NamePair {
 private:
	std::string _name;
	std::string _value;
	NamePair *_next;
	// Неявное копирование запрещено. 
	 NamePair(NamePair & Source);	// нет реализации!
	 NamePair & operator=(NamePair & Source);	// нет реализации!

 public:
	 NamePair();		// нет реализации!
	 NamePair(const std::string & initName, const std::string & initValue);
	 virtual ~ NamePair();
	//
	 std::string GetName() {
		return _name;
	};
	std::string GetValue() {
		return _value;
	};
	void SetNext(NamePair * next) {
		_next = next;
	};
	NamePair *GetNext() {
		return _next;
	};
};

class NamePairList {
 private:
	NamePair * _head;
	NamePair *_last;
	// Неявное копирование запрещено. 
	 NamePairList(NamePairList & Source);	// нет реализации!
	 NamePairList & operator=(NamePairList & Source);	// нет реализации!
 public:
	 NamePairList();
	~NamePairList();
	//
	const NamePair *AddPair(const std::string & name,
				const std::string & value);
	// 
	bool GetValue(const std::string & name, std::string * buf);
};
#endif				/*namepair.h */
