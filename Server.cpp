#include "Server.h"

void Server::createSocket()
{
#ifdef _WIN32
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "Ошибка инициализации Winsock!" << endl;
        exit(1);
    }

    sockert_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (sockert_file_descriptor == INVALID_SOCKET) {
        cout << "Не удалось создать сокет!" << endl;
        exit(1);
    }
    else {
        cout << "Создание сокета прошло успешно!" << endl;
    }
#else
    sockert_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (sockert_file_descriptor == -1) {
        cout << "Не удалось создать сокет!" << endl;
        exit(1);
    }
    else {
        cout << "Создание сокета прошло успешно!" << endl;
    }
#endif
}

void Server::bindSocket()
{
    serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_family = AF_INET;
    bind_status = bind(sockert_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));
    if (bind_status == -1) {
        cout << "Ошибка привязки сокета!" << endl;
        exit(1);
    }
    else {
        cout << "Привязка сокета прошла успешно!" << endl;
    }
}

void Server::closeSocket()
{
#ifdef _WIN32
    closesocket(sockert_file_descriptor);
    WSACleanup();
#else
    close(sockert_file_descriptor);
#endif 
}

void Server::dataReceivingServer()
{
    connection_status = listen(sockert_file_descriptor, 5);

    if (connection_status == -1) {
        cout << "Сокет не может прослушивать новые соединения!" << endl;
        exit(1);
    }
    else {
        cout << "Сервер ожидает нового соединения: " << endl;
    }

    length = sizeof(client);
    connection = accept(sockert_file_descriptor, (struct sockaddr*)&client, &length);

    if (connection == -1) {
        cout << "Сервер не может принять данные от клиента!" << endl;
        exit(1);
    }
    else {
        cout << "Клиент присоединился!" << endl;
    }
}

char* Server::readData()
{
    cout << " >> Ожидание данных!" << endl;

#ifdef _WIN32
    memset(message, '\0', MESSAGE_LENGTH);

    recv(connection, message, sizeof(message), 0);
#else
    bzero(message, MESSAGE_LENGTH);

    read(connection, message, sizeof(message));
#endif

    cout << "Данные получены от клиента! << " << message << endl;
    return message;
}

void Server::writeData(string serverMessage)
{
#ifdef _WIN32
    memset(message, '\0', MESSAGE_LENGTH);
    strcpy_s(message, serverMessage.c_str());

    int bytesSent = send(connection, message, sizeof(message), 0);

    if (bytesSent >= 0) {
        cout << " >> Данные отправлены клиенту! >> " << message << endl;
    }
#else
    bzero(message, MESSAGE_LENGTH);
    strcpy_s(message, serverMessage.c_str());

    ssize_t bytes = write(connection, message, sizeof(message));

    if (bytes >= 0) {
        cout << " >> Данные отправлены клиенту! >> " << message << endl;
    }
#endif
}