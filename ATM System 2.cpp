
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <fstream>

using namespace std;

enum enWhatToDo { enQuickWithdraw = 1, enNormalWithdraw = 2, enDeposit = 3, enCheckBalance = 4, enLogout = 5 };

struct stClient
{
	string AccountNumber;
	string PinCode;
	string ClientName;
	string Phone;
	double AccountBalance;
};

const string ClientsFile = "Clients.txt";
stClient CurrentClient;

void ShowMainMenu();
bool CheckBalance(vector<stClient>&, stClient&);
void ShowQuickWithdrawScreen();
void ShowNormalWithdrawScreen();
void Login();

double ReadDepositAmount()
{
	double DepositAmount;
	do {

		cout << "Enter a Positive Deposit Amount? ";
		cin >> DepositAmount;

	} while (DepositAmount <= 0);

	return DepositAmount;
}

void ResetScreen()
{
	system("cls");
	system("color 0F");
}

string ConvertRecordToLine(stClient Client, string Separator = "#//#")
{
	string RecordLine = "";

	RecordLine += Client.AccountNumber + Separator;
	RecordLine += Client.PinCode + Separator;
	RecordLine += Client.ClientName + Separator;
	RecordLine += Client.Phone + Separator;
	RecordLine += to_string(Client.AccountBalance);

	return RecordLine;
}

void SaveClientsDataToFile(vector<stClient>& vClients, string FileName)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);

	if (MyFile.is_open())
	{
		string Line;

		for (stClient& Client : vClients)
		{
			Line = ConvertRecordToLine(Client);
			MyFile << Line << endl;
		}

		MyFile.close();
	}
}

vector<string> SplitString(string S1, string Delimiter)
{
	vector<string> vString;
	short Pos = 0;
	string sWord;

	while ((Pos = S1.find(Delimiter)) != std::string::npos)
	{
		sWord = S1.substr(0, Pos);

		if (sWord != "")
		{
			vString.push_back(sWord);
		}

		S1.erase(0, Pos + Delimiter.length());
	}

	if (S1 != "")
	{
		vString.push_back(S1);
	}

	return vString;
}

stClient ConvertLineToRecord(string Line, string Separator = "#//#")
{
	vector<string> vClient = SplitString(Line, Separator);
	stClient Client;

	Client.AccountNumber = vClient[0];
	Client.PinCode = vClient[1];
	Client.ClientName = vClient[2];
	Client.Phone = vClient[3];
	Client.AccountBalance = stod(vClient[4]);

	return Client;
}

vector<stClient> LoadClientsDataFromFile(string FileName)
{
	fstream MyFile;
	MyFile.open(FileName, ios::in);

	vector<stClient> vClients;
	string Line;
	stClient Client;

	if (MyFile.is_open())
	{
		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecord(Line);
			vClients.push_back(Client);
		}

		MyFile.close();
	}

	return vClients;
}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, vector<stClient>& vClients, double Amount)
{
	char PerformTransaction;

	cout << "\nAre you sure you want to perform this transaction? Y/N? ";
	cin >> PerformTransaction;

	if (toupper(PerformTransaction) == 'Y')
	{
		for (stClient& Client : vClients)
		{
			if (Client.AccountNumber == AccountNumber)
			{
				Client.AccountBalance += Amount;
				SaveClientsDataToFile(vClients, ClientsFile);
				cout << "\nDone Successfully. New Balance is : " << Client.AccountBalance << endl;
				return true;
			}
		}
	}

	return false;
}

void PerformDeposit()
{
	double DepositAmount = ReadDepositAmount();

	vector<stClient> vClients = LoadClientsDataFromFile(ClientsFile);
	DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, vClients, DepositAmount);
	CurrentClient.AccountBalance += DepositAmount;
}

void ShowDepositScreen()
{
	ResetScreen();

	cout << "==========================================\n";
	cout << "\t\tDeposit Screen\n";
	cout << "==========================================\n";

	PerformDeposit();
}

int ReadWithdrawAmount()
{
	int WithdrawAmount;

	do
	{
		cout << "\nEnter an amount multiple of 5's ? ";
		cin >> WithdrawAmount;

	} while (WithdrawAmount % 5 != 0);

	return WithdrawAmount;
}

void PerformNormalWithdraw()
{
	int WithdrawAmount = ReadWithdrawAmount();

	if (WithdrawAmount > CurrentClient.AccountBalance)
	{
		cout << "\nThe Amount Exceeds Your Balance, Make Another Choice." << endl;
		cout << "Press any key to continue.....";
		system("pause>0");
		ShowNormalWithdrawScreen();
		return;
	}

	vector<stClient> vClients = LoadClientsDataFromFile(ClientsFile);
	DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, vClients, -1 * double(WithdrawAmount));
	CurrentClient.AccountBalance -= WithdrawAmount;
}

void ShowNormalWithdrawScreen()
{
	ResetScreen();

	cout << "==========================================\n";
	cout << "\tNormal Withdraw Screen\n";
	cout << "==========================================\n";

	PerformNormalWithdraw();
}

short ReadQuickWithdrawOption()
{
	short Option;
	
	do {

		cout << "Choose what to withdraw from [1] to [9]? ";
		cin >> Option;

	} while (Option < 1 || Option > 9);

	return Option;
}

double GetWithdrawAmount(short Option)
{
	switch (Option)
	{
	case 1:
		return 20;

	case 2:
		return 50;

	case 3:
		return 100;

	case 4:
		return 200;

	case 5:
		return 400;

	case 6:
		return 600;

	case 7:
		return 800;

	case 8:
		return 1000;
	}
}

void PerformQuickWithdrawOption(short Option)
{
	if (Option == 9)
	{
		ShowMainMenu();
		return;
	}

	double WithdrawAmount = GetWithdrawAmount(Option);

	if (WithdrawAmount > CurrentClient.AccountBalance)
	{
		cout << "\nThe Amount Exceeds Your Balance, Make Another Choice." << endl;
		cout << "Press any key to continue.....";
		system("pause>0");
		ShowQuickWithdrawScreen();
		return;
	}

	vector<stClient> vClients = LoadClientsDataFromFile(ClientsFile);
	DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, vClients, -1 * WithdrawAmount);
	CurrentClient.AccountBalance -= WithdrawAmount;
}

void ShowQuickWithdrawScreen()
{
	ResetScreen();

	cout << "==========================================\n";
	cout << "\tQuick Withdraw Screen\n";
	cout << "==========================================\n";
	cout << "\t[1] 20  \t[2] 50\n";
	cout << "\t[3] 100 \t[4] 200\n";
	cout << "\t[5] 400 \t[6] 600\n";
	cout << "\t[7] 800 \t[8] 1000\n";
	cout << "\t[9] Exit\n";
	cout << "==========================================\n";
	cout << "your Balance is : " << CurrentClient.AccountBalance << endl;

	PerformQuickWithdrawOption(ReadQuickWithdrawOption());
}

void ShowCheckBalanceScreen()
{
	ResetScreen();

	cout << "==========================================\n";
	cout << "\tCheck Balance Screen\n";
	cout << "==========================================\n";
	
	cout << "Your Balance is : " << CurrentClient.AccountBalance << endl;
}

void GoBackToMainMenu()
{
	cout << "\nPress any key to go back to main Menu......." << endl;
	system("pause>0");
	ShowMainMenu();
}

void PerformMainMenuOption(enWhatToDo WhatToDo)
{
	switch (WhatToDo)
	{
	case enWhatToDo::enQuickWithdraw:
		ShowQuickWithdrawScreen();
		GoBackToMainMenu();
		break;

	case enWhatToDo::enNormalWithdraw:
		ShowNormalWithdrawScreen();
		GoBackToMainMenu();
		break;

	case enWhatToDo::enDeposit:
		ShowDepositScreen();
		GoBackToMainMenu();
		break;

	case enWhatToDo::enCheckBalance:
		ShowCheckBalanceScreen();
		GoBackToMainMenu();
		break;

	case enWhatToDo::enLogout:
		Login();
		break;
	}
}

short ReadMainMenuOption()
{
	short Option;
	cout << "Choose what do you want to do? [1] to [5]? ";
	cin >> Option;

	return Option;
}

void ShowMainMenu()
{
	ResetScreen();

	cout << "==========================================\n";
	cout << "\tATM Main Menu Screen\n";
	cout << "==========================================\n";
	cout << "\t[1] Quick Withdraw." << endl;
	cout << "\t[2] Normal Withdraw." << endl;
	cout << "\t[3] Deposit." << endl;
	cout << "\t[4] Check Balance." << endl;
	cout << "\t[5] Logout." << endl;
	cout << "==========================================\n";

	PerformMainMenuOption((enWhatToDo)ReadMainMenuOption());
}

bool FindClientByAccountNumberAndPinCode(string AccountNumber, string PinCode, stClient& Client)
{
	vector<stClient> vClients = LoadClientsDataFromFile(ClientsFile);

	for (stClient& C : vClients)
	{
		if (C.AccountNumber == AccountNumber && C.PinCode == PinCode)
		{
			Client = C;
			return true;
		}
	}

	return false;
}

bool LoadClientInfo(string AccountNumber, string PinCode)
{
	if (FindClientByAccountNumberAndPinCode(AccountNumber, PinCode, CurrentClient))
		return true;
	else
		return false;
}

void Login()
{
	bool LoginFailed = false;
	string AccountNumber, PinCode;

	do {
		ResetScreen();

		cout << "==========================================\n";
		cout << "\t\tLogin Screen\n";
		cout << "==========================================\n";

		if (LoginFailed)
		{
			cout << "Invalid Account Number/PinCode!" << endl;
		}

		cout << "Enter Account Number? ";
		cin >> AccountNumber;

		cout << "Enter PinCode? ";
		cin >> PinCode;

		LoginFailed = !LoadClientInfo(AccountNumber, PinCode);

	} while (LoginFailed);

	ShowMainMenu();
}

int main()
{

	Login();

	return 0;
}