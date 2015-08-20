#ifndef functions_H
#define functions_H

#include "stdafx.h"
#include "Account.h"

void write_account();
void display_all();
void modify_account(int num);
void delete_account(int num);
void display_sp(int num);
void deposit_withdraw(int num, int option);
std::string checksumFile();
bool is_file_locked();
void removeReg();
#endif