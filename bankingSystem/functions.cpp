#include "stdafx.h"
#include "functions.h"

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <iomanip>
#include <windows.h>

#pragma comment (lib, "Advapi32.lib")
#include <openssl/sha.h> 

void write_account(){
	Account ac;
	ac.create_account();

	errno = 0;

	ofstream File;
	try{
		File.open("account.dat", ios::binary|ios::app);
		File.exceptions (std::ios::badbit | std::ios::failbit);
		File.write(reinterpret_cast<char *> (&ac), sizeof(Account));
	} catch(exception &e){
		if (errno){
			//cerr << "*Error: " << e.what() << "account.dat" << " - " << strerror(errno) << endl;
		}
	}

	File.close();
}

void display_all(){
	Account ac;

	ifstream inFile;
	inFile.open("account.dat", ios::binary);
	
	if(!inFile){
		cout << "File could not be open!!!" << endl;
		return;
	}
	
	while ( inFile.read(reinterpret_cast<char *> (&ac), sizeof(Account))){
		ac.report();
	}

	inFile.close();
}

void modify_account(int num){
	Account ac;
	bool found = false;
	fstream File;
	File.open("account.dat", ios::binary|ios::in|ios::out);
	
	if(!File){
		cout << "File could not be open!!!" << endl;
		return;
	}

	while(!File.eof() && found==false){
		File.read(reinterpret_cast<char *> (&ac), sizeof(Account));
		if ( ac.acnumber() ==  num){
			ac.modify();
			int pos = (-1)*static_cast<int>(sizeof(Account));
			File.seekp(pos, ios::cur);
			File.write(reinterpret_cast<char *> (&ac), sizeof(Account));
			cout << "Record updated!";
			found=true;
		}
	}
	File.close();
	if(found == false){
		cout << "Record not found!" << endl;
	}
}

void delete_account(int num){
	Account ac;
	ifstream inFile;
	ofstream outFile;

	inFile.open("account.dat", ios::binary);
	if(!inFile){
		cout << "Could not open file!!!" << endl;
		return;
	}
	outFile.open("temp.dat", ios::binary);
	while(inFile.read(reinterpret_cast<char *>(&ac), sizeof(Account))){
		if(ac.acnumber() != num){
			outFile.write(reinterpret_cast<char *> (&ac), sizeof(Account));
		}
	}

	inFile.close();
	outFile.close();
	remove("account.dat");
	rename("temp.dat", "account.dat");
	cout << "Record deleted!" << endl;
}

void display_sp(int num){
	Account ac;
	bool found = false;
	ifstream inFile;

	inFile.open("account.dat", ios::binary);
	if (! inFile){
		cout << "Could not open file!!!" << endl;
		return;
	}

	cout << "BALANCE DETAILS" << endl;

	while(inFile.read(reinterpret_cast<char *>(&ac), sizeof(Account))){
		if(ac.acnumber() == num){
			ac.show_account();
			found = true;
		}
	}

	if (found == false){
		cout << "Account number does not exist" << endl;
	}
}

void deposit_withdraw(int num, int option){
	Account ac;
	bool found = false;
	int amt;
	fstream File;
	File.open("account.dat", ios::binary|ios::in|ios::out);
	
	if(!File){
		cout << "File could not be open!!!" << endl;
		return;
	}

	while(!File.eof() && found==false){
		File.read(reinterpret_cast<char *> (&ac), sizeof(Account));
		if (ac.acnumber() ==  num){
			ac.show_account();

			if(option == 1){
				cout << "TO DEPOSITE AMOUNT" << endl;
				cout << "Enter the amount to be deposited: ";
				cin >> amt;
				ac.dep(amt);
			}

			if(option == 2){
				cout << "TO WITHDRAW AMOUNT" << endl;
				cout << "Enter the amount to be withdraw: ";
				cin >> amt;
				int balance = ac.getDeposit() - amt;
				if ((balance < 500 && ac.getType() == "S") || (balance < 1000 && ac.getType() == "C"))
					cout << "Insufficience balance" << endl;
				else
					ac.draw(amt);
			}

			int pos = (-1)*static_cast<int>(sizeof(ac));
			File.seekp(pos, ios::cur);
			File.write(reinterpret_cast<char *> (&ac), sizeof(Account));
			cout << "Record updated!" << endl;
			found=true;
		}


	}
}

std::string checksumFile() {
		unsigned char hash[SHA256_DIGEST_LENGTH];
		char *filename="account.dat";
		int i;
		FILE *inFile = fopen (filename, "rb");
		SHA256_CTX sha256;
		int bytes;
		unsigned char data[1024];

		if (inFile == NULL) {
			printf ("%s can't be opened.\n", filename);
			return "";
		}

		SHA256_Init(&sha256);
		
		while ((bytes = fread (data, 1, 1024, inFile)) != 0)
			SHA256_Update(&sha256, data, bytes);

		SHA256_Final(hash, &sha256);

		std::stringstream ss;  
		for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)  
		{  
			ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];  
		}  
       
		fclose (inFile);

		return ss.str();
}

bool is_file_locked(){
	bool lock = false;

	HKEY hKey;
    LPCTSTR sk = TEXT("SOFTWARE\\BankingAccounts");

	LONG nError = RegOpenKeyEx(HKEY_LOCAL_MACHINE, sk, 0, KEY_ALL_ACCESS, &hKey);

    if (nError==ERROR_FILE_NOT_FOUND) {
    	cout << "Creating registry key: " << sk << endl;
		nError = RegCreateKeyEx(HKEY_LOCAL_MACHINE, sk, NULL, NULL, REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL, &hKey, NULL);
	} else {
		char valueReg[1024];
	    DWORD valueReg_length = 1024;
		RegGetValueA (hKey, NULL, "SHA", RRF_RT_REG_SZ, NULL, (void*) valueReg, &valueReg_length);
		if (checksumFile() == valueReg){
			lock = true;
			cout << "File is locked" << endl;
		}
	}

	if (nError)
		cout << "Error: " << nError << " Could not find or create " << sk << endl;

	LONG setRes = RegSetValueExA (hKey, "SHA", 0, REG_SZ,
		(LPBYTE)checksumFile().c_str(), strlen(checksumFile().c_str())*sizeof(char));

    if (setRes != ERROR_SUCCESS) {
       	printf("Error writing to Registry.");
    }

    LONG closeOut = RegCloseKey(hKey);

    if (closeOut != ERROR_SUCCESS) {
       	printf("Error closing key.");
    }

	return lock;
}

void removeReg(){
	HKEY hKey;
    LPCTSTR sk = TEXT("SOFTWARE\\BankingAccounts");

	LONG nError = RegOpenKeyEx(HKEY_LOCAL_MACHINE, sk, 0, KEY_ALL_ACCESS, &hKey);

    if (nError==ERROR_FILE_NOT_FOUND) {
    	cout << "Could not find registry: " << "SOFTWARE\\BankingAccounts" << endl;
	}

	LONG setRes = RegSetValueExA (hKey, "SHA", 0, REG_SZ, (LPBYTE)"", 0);

    if (setRes != ERROR_SUCCESS) {
       	printf("Error writing to Registry.");
    }

    LONG closeOut = RegCloseKey(hKey);

    if (closeOut != ERROR_SUCCESS) {
       	printf("Error closing key.");
    }
}

