//потоки
#include <thread>
//блокировка
#include<mutex> 
#include<locale>
#include <fstream>
#include <sstream>
#include<iostream>
#include<string.h>
//exit
#include<unistd.h>
#include<stdlib.h>
//содержит функции для работы с сетевыми адресами, такие как htons.
#include<arpa/inet.h>
//определяет символы, начинающиеся с " AF_ " для различных видов сетей и для работы сокетов
#include<sys/socket.h>
//функции для работы с сетевыми именами и адресами.
#include<netdb.h>
//содержащую определения для работы с интернет-адресами и портами.
#include<netinet/in.h>

#define SERVER_IP "127.0.0.1"
#define NUMBER_PORT 1610
#define SYMBOL '~'
#define MAX_SIZE_BUF 1024

using namespace std;
//узел
struct Node {
    //колонки
    string tableColon;
    Node *next;
};


// связь колонок и таблиц
struct TablColonNode{
    //табл
    string table;
    // указываем на 1 колонку
    Node* fColon;
    TablColonNode* next;
};

//Файл json
struct configuration{
    string name;
    int tuples_limit;
    TablColonNode* Storage;
};

//Индексы строк
struct IndexPrintColon{
    string indexFromStrok;
    IndexPrintColon* next;
};

//Для селекта
struct PrintColon{
    string fromTabl;
    int fromColon;
    IndexPrintColon* strochki;
    PrintColon* next;
};

void whithFile(configuration& cond);
void NewFile(configuration cond);
void Insert(string comand, configuration cond);
PrintColon* Select(string comand, string condName);
string CutToFrom(string command);
string CutAfterWhere(string command);
string StrPrint(string CondName, PrintColon* chosen);
void Where(string CondName, PrintColon* chosen, string comand, string adComand);
void StrDel(string CondName, PrintColon* chosen);
void LockTable(string CondName);
void UnlockTable(string CondName);
bool isLocked(string CondName);
//Server
string Commands(string comand, configuration cond);
bool ExitOrNO(const char* meaning);
void ErrorOrNo(int meaning);
void sendAMessage(int system, char* buffer, configuration cond);
void AcceptTheMessage(int system, char* buffer);
void Communication_With_Client(int server, configuration cond);
