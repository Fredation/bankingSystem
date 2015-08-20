#include "stdafx.h"
#include "Account.h"

#include <iostream>
#include <fstream>
#include <cctype>
#include <iomanip>
#include <string>

using namespace std;

void Account::create_account(){
	cout << "Enter the account number: ";
	cin >> acno;
	cout << "Enter the name of the account holder: ";
	cin >> name;
	cout << "Enter the type (C/S): ";
	cin >> type;
	cout << "Enter the initial amount: ";
	cin >> deposite;
	cout << "Account created..." << endl;
}

void Account::report(){
	cout << acno << setw(10) << " " << name << setw(10) << " " << type << setw(6) << " " << deposite << endl;
}

void Account::modify(){
	cout << "Enter the account details: " << endl;
	cout << "Account number: " << acno << endl;
	cout << "Account holder name: ";
	cin>>name;
	cout << "Type of account (C/S): ";
	cin >> type;
	cout << "Balance amount: ";
	cin >> deposite;
}

void Account::show_account(){
	cout << "Account number: " << acno << endl;
	cout << "Holder name: " << name << endl;
	cout << "Accout type: " << type << endl;
	cout << "Balance amount: " << deposite << endl;
}

void Account::dep(int amt){
	deposite += amt;
}
void Account::draw(int amt){
	deposite -= amt;
}


int Account::acnumber(){
	return acno;
}

int Account::getDeposit(){
	return deposite;
}

string Account::getType(){
	return type;
}

