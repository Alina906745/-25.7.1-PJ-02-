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
    cout << "(0)выход или (любую клавишу)повторить: ";
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

    cout << "логин: ";
    cin >> login;
    cout << "пароль: ";
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
        cout << "<<<ошибка: имя пользователя или пароль введены неверно>>>" << endl;
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

    cout << "логин: ";
    do {
        cin >> login;
    } while (login == "all");
    cout << "пароль: ";
    cin >> password;
    cout << "имя: ";
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
            cout << "<<<ошибка: логин пользователя занят>>>" << endl;
        }
        else if (isNameOrLogin == "name") {
            cout << "<<<ошибка: имя пользователя занято>>>" << endl;
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
    cout << "Вы вышли из чата." << endl;
    return "exit";
}


void Chat::showLoginMenu()
{
    char operation;
    string outMessage;
    _sendMessage = false;

    cout << endl;
    cout << "-------------------------" << endl;
    cout << "|Добро пожаловать в чат!|" << endl;
    cout << "-------------------------" << endl;
    cout << "Войти - 1" << endl;
    cout << "Регистрация - 2" << endl;
    cout << "Выход - 0" << endl;
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
        cout << "1 или 2..." << endl;
        break;
    }
}


void Chat::showUserMenu()
{
    char operation;
    string outMessage;
    _sendMessage = false;

    cout << endl;
    cout << "Привет, " << _currentUserName << endl;
    cout << "___________________________" << endl;
    cout << "Меню:" << endl;
    cout << "Показать сообщения     - 1" << endl;
    cout << "Написать сообщение     - 2" << endl;
    cout << "Показать пользователей - 3" << endl;
    cout << "Сменить пользователя   - 4" << endl;
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
        cout << "неизвестный выбор " << endl;
        _currentMenu = "";
        break;
    }
}


string Chat::addMessage()
{
    string from, to, text, outMessage;
    _sendMessage = true;

    from = _currentUserName;

    cout << "Для (name или all): ";
    cin >> to;
    cout << "Текст: ";
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
        cout << "<<<ошибка: не удалось передать сообщение пользователю ";
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

    cout << "----------чат-----------" << endl;

    if (_vectorFromMessage[1] == "empty") {
        cout << "В данный момент в чате нет сообщений" << endl;
    }
    else {
        int i;
        for (i = 1; i < _vectorFromMessage.size() - 1; i += 3) {
            cout << "Сообщение от " << _vectorFromMessage[i];
            cout << " для " << _vectorFromMessage[i + 1] << endl;
            cout << "текст: " << _vectorFromMessage[i + 2] << endl;
        }
    }

    cout << "------------------------" << endl;

    _currentMenu = "";
}


void Chat::showUsers()
{
    _sendMessage = false;

    cout << "------Пользователи------" << endl;

    int i;
    for (i = 1; i < _vectorFromMessage.size() - 1; i++) {
        cout << _vectorFromMessage[i] << endl;
    }

    cout << "------------------------" << endl;

    _currentMenu = "";
}