#include "Chat.h"
#include <iostream>
#include <ranges>
#include <string_view>
#include <cstring>
#include "Sha1.h"
#include <string>

using namespace std;
string Chat::chatProcessing()
{
    string outMessage;

    while (!_sendMessage) {
        if (!_isCurrentUser) {
            if (_currentMenu == "") {
                showLoginMenu();
            }
            else if (_currentMenu == "loginUser") {
                outMessage = loginUser();
            }
            else if (_currentMenu == "registerUser") {
                outMessage = registerUser();
            }
            else if (_currentMenu == "exitChat") {
                outMessage = exitChat();
            }
        }
        else {
            if (_currentMenu == "") {
                showUserMenu();
            }
            else if (_currentMenu == "getChat") {
                outMessage = getChat();
            }
            else if (_currentMenu == "addMessage") {
                outMessage = addMessage();
            }
            else if (_currentMenu == "getUsers") {
                outMessage = getUsers();
            }
        }
    }

    return outMessage;
}


void Chat::messageProcessing(string serverMessage)
{
    _vectorFromMessage = messageToVector(serverMessage, ":");

    if (_vectorFromMessage[0] == "loginUser") {
        checkLogin();
    }
    else if (_vectorFromMessage[0] == "registerUser") {
        checkRegister();
    }
    else if (_vectorFromMessage[0] == "showChat") {
        showChat();
    }
    else if (_vectorFromMessage[0] == "addMessage") {
        checkAddMessage();
    }
    else if (_vectorFromMessage[0] == "showUsers") {
        showUsers();
    }
}


vector<string> Chat::messageToVector(string message, string delimiter)
{
    vector<string> resultVector;

    if (!message.empty()) {
        int start = 0;
        do {
            int idx = message.find(delimiter, start);
            if (idx == string::npos) {
                break;
            }

            int length = idx - start;
            resultVector.push_back(message.substr(start, length));
            start += (length + delimiter.size());
        } while (true);

        resultVector.push_back(message.substr(start));
    }

    return resultVector;
}


void Chat::backMenu(string currentMenu)
{
    char operation;
    cout << "(0)����� ��� (����� �������)���������: ";
    cin >> operation;
    if (operation == '0') {
        _currentMenu = "";
    }
    else {
        _currentMenu = currentMenu;
    }
}


string Chat::loginUser()
{
    string login, password, outMessage;
    _sendMessage = true;

    cout << "�����: ";
    cin >> login;
    cout << "������: ";
    cin >> password;

    const char* passwordChar = password.c_str();

    uint* passHex = sha1(passwordChar, strlen(passwordChar));

    outMessage = "loginUser:" + login + ":" + to_string(*passHex);

    return outMessage;
}


void Chat::checkLogin()
{
    char operation;
    string checkUser;
    checkUser = _vectorFromMessage[1];
    _sendMessage = false;

    if (checkUser == "error") {
        _isCurrentUser = false;
        cout << "<<<������: ��� ������������ ��� ������ ������� �������>>>" << endl;
        backMenu("loginUser");
    }
    else if (checkUser == "ok") {
        _isCurrentUser = true;
        _currentMenu = "";
        _currentUserName = _vectorFromMessage[2];
    }
}


string Chat::registerUser()
{
    string login, password, name, outMessage;
    _sendMessage = true;

    cout << "�����: ";
    do {
        cin >> login;
    } while (login == "all");
    cout << "������: ";
    cin >> password;
    cout << "���: ";
    do {
        cin >> name;
    } while (login == "all");

    const char* passwordChar = password.c_str();

    uint* passHex = sha1(passwordChar, strlen(passwordChar));

    outMessage = "registerUser:" + login + ":" + to_string(*passHex) + ":" + name;

    return outMessage;
}


void Chat::checkRegister()
{
    char operation;
    string checkUser, isNameOrLogin;
    checkUser = _vectorFromMessage[1];
    isNameOrLogin = _vectorFromMessage[2];
    _sendMessage = false;

    if (checkUser == "error") {
        _isCurrentUser = false;
        if (isNameOrLogin == "login") {
            cout << "<<<������: ����� ������������ �����>>>" << endl;
        }
        else if (isNameOrLogin == "name") {
            cout << "<<<������: ��� ������������ ������>>>" << endl;
        }
        backMenu("registerUser");
    }
    else if (checkUser == "ok") {
        _isCurrentUser = true;
        _currentMenu = "";
        _currentUserName = _vectorFromMessage[2];
    }
}


string Chat::exitChat()
{
    _sendMessage = true;
    cout << "�� ����� �� ����." << endl;
    return "exit";
}


void Chat::showLoginMenu()
{
    char operation;
    string outMessage;
    _sendMessage = false;

    cout << endl;
    cout << "-------------------------" << endl;
    cout << "|����� ���������� � ���!|" << endl;
    cout << "-------------------------" << endl;
    cout << "����� - 1" << endl;
    cout << "����������� - 2" << endl;
    cout << "����� - 0" << endl;
    cout << ">> ";
    cin >> operation;

    switch (operation)
    {
    case '1':
        _currentMenu = "loginUser";
        break;

    case '2':
        _currentMenu = "registerUser";
        break;

    case '0':
        _currentMenu = "exitChat";
        break;

    default:
        cout << "1 ��� 2..." << endl;
        break;
    }
}


void Chat::showUserMenu()
{
    char operation;
    string outMessage;
    _sendMessage = false;

    cout << endl;
    cout << "������, " << _currentUserName << endl;
    cout << "___________________________" << endl;
    cout << "����:" << endl;
    cout << "�������� ���������     - 1" << endl;
    cout << "�������� ���������     - 2" << endl;
    cout << "�������� ������������� - 3" << endl;
    cout << "������� ������������   - 4" << endl;
    cout << "__________________________" << endl;
    cout << ">> ";
    cin >> operation;

    switch (operation)
    {
    case '1':
        _currentMenu = "getChat";
        break;

    case '2':
        _currentMenu = "addMessage";
        break;

    case '3':
        _currentMenu = "getUsers";
        break;

    case '4':
        _isCurrentUser = false;
        _currentMenu = "";
        break;

    default:
        cout << "����������� ����� " << endl;
        _currentMenu = "";
        break;
    }
}


string Chat::addMessage()
{
    string from, to, text, outMessage;
    _sendMessage = true;

    from = _currentUserName;

    cout << "��� (name ��� all): ";
    cin >> to;
    cout << "�����: ";
    cin.ignore();
    getline(cin, text);

    outMessage = "addMessage:" + from + ":" + to + ":" + text;

    return outMessage;
}


void Chat::checkAddMessage()
{
    _sendMessage = false;
    string checkMessage;
    checkMessage = _vectorFromMessage[1];

    if (checkMessage == "error") {
        _currentMenu = "";
        cout << "<<<������: �� ������� �������� ��������� ������������ ";
        cout << _vectorFromMessage[2] << ">>>" << endl;
    }
    else if (checkMessage == "ok") {
        _currentMenu = "";
    }
}


string Chat::getUsers()
{
    string outMessage;
    _sendMessage = true;

    outMessage = "showUsers:" + _currentUserName;

    return outMessage;
}


string Chat::getChat()
{
    string outMessage;
    _sendMessage = true;

    outMessage = "showChat:" + _currentUserName + ":showChat";

    return outMessage;
}


void Chat::showChat()
{
    string from, to;
    _sendMessage = false;

    cout << "----------���-----------" << endl;

    if (_vectorFromMessage[1] == "empty") {
        cout << "� ������ ������ � ���� ��� ���������" << endl;
    }
    else {
        int i;
        for (i = 1; i < _vectorFromMessage.size() - 1; i += 3) {
            cout << "��������� �� " << _vectorFromMessage[i];
            cout << " ��� " << _vectorFromMessage[i + 1] << endl;
            cout << "�����: " << _vectorFromMessage[i + 2] << endl;
        }
    }

    cout << "------------------------" << endl;

    _currentMenu = "";
}


void Chat::showUsers()
{
    _sendMessage = false;

    cout << "------������������------" << endl;

    int i;
    for (i = 1; i < _vectorFromMessage.size() - 1; i++) {
        cout << _vectorFromMessage[i] << endl;
    }

    cout << "------------------------" << endl;

    _currentMenu = "";
}