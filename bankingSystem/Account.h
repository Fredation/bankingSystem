#ifndef Account_H
#define Account_H

#include <string>

using namespace std;

class Account{
	int acno;
	int deposite;
	string name;
	string type;

public:
	void create_account();
	void report();
	void modify();
	void show_account();
	void dep(int amt);
	void draw(int amt);
	int getDeposit();
	string getType();
	int acnumber();
};
#endif