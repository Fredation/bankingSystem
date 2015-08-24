// bankingSystem.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "functions.h"
#include <Windows.h>
#include <iostream>

using namespace std;

int main(int argc, char * argv[])
{
	char* sha;
	char ch;
    int num;

	for(;;){
		if ( is_file_locked()){
			cout << "Another user trying to modified accounts!!! Please, try again!" << endl;
			cin.ignore();
			cin.get();
		} else {
			break;
		}
	}

    do
    {
        system("cls");
        cout<<"\n\n\n\tMAIN MENU";
        cout<<"\n\n\t01. NEW ACCOUNT";
        cout<<"\n\n\t02. DEPOSIT AMOUNT";
        cout<<"\n\n\t03. WITHDRAW AMOUNT";
        cout<<"\n\n\t04. BALANCE ENQUIRY";
        cout<<"\n\n\t05. ALL ACCOUNT HOLDER LIST";
        cout<<"\n\n\t06. CLOSE AN ACCOUNT";
        cout<<"\n\n\t07. MODIFY AN ACCOUNT";
        cout<<"\n\n\t08. EXIT";
        cout<<"\n\n\tSelect Your Option (1-8) ";
        cin>>ch;

        system("cls");
        
		switch(ch)
        {
        case '1':
            write_account();
            break;
        case '2':
            cout<<"\n\n\tEnter The account No. : "; cin>>num;
            deposit_withdraw(num, 1);
            break;
        case '3':
            cout<<"\n\n\tEnter The account No. : "; cin>>num;
            deposit_withdraw(num, 2);
            break;
        case '4':
            cout<<"\n\n\tEnter The account No. : "; cin>>num;
            display_sp(num);
            break;
        case '5':
            display_all();
            break;
        case '6':
            cout<<"\n\n\tEnter The account No. : "; cin>>num;
            delete_account(num);
            break;
         case '7':
            cout<<"\n\n\tEnter The account No. : "; cin>>num;
            modify_account(num);
            break;
         case '8':
            cout<<"\n\n\tThanks for using bank managemnt system";
            break;
         default :cout<<"\a";
        }

		std::string str = checksumFile();
		sha = new char[str.size() + 1];
		std::copy(str.begin(), str.end(), sha);
		sha[str.size()] = '\0'; // don't forget the terminating 0

		updateReg(sha);

        cin.ignore();
        cin.get();
    }while(ch!='8');

	updateReg("");

	//free the sha after finished using it
	delete[] sha;
    return 0;
}
