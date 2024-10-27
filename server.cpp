#include "header.h"

int main(){
    //хранения дескриптора сокета
    int client;
    //хранения дескриптора сокета, который будет использоваться для связи с клиентом
    int server;
    configuration cond;
    whithFile(cond);
    NewFile(cond);

    //Сокет — это виртуальная конструкция из IP-адреса и номера порта
    struct sockaddr_in server_address;
    
    //Создает сокет с использованием протокола TCP (SOCK_STREAM).
    client = socket(AF_INET, SOCK_STREAM, 0);
    ErrorOrNo(client);

    // Функция htons используется для преобразования номера порта в сетевой порядок байтов.
    server_address.sin_port = htons(NUMBER_PORT);

    // Устанавливает семейство адресов на AF_INET, что означает использование IPv4.
    server_address.sin_family =AF_INET;
    //Устанавливает IP-адрес сервера на INADDR_ANY
    server_address.sin_addr.s_addr=htons(INADDR_ANY);

    //Привязывает сокет к указанному адресу и порту.
    int ret = (bind(client, reinterpret_cast<struct sockaddr*>(&server_address),sizeof(server_address)));
     ErrorOrNo(ret);

    //Определяет размер структуры server_address.
    socklen_t size = sizeof(server_address);
    cout<<"SERVER: "<<"Listening client...\n";
    while(true){
        //Начинает прослушивать входящие подключения на сокете
        listen(client,1);

        //Принимает входящее подключение
        server = accept(client, reinterpret_cast<struct sockaddr*>(&server_address), &size);
        //Проверка
        if(server < 0){
            cout<<"Server error. Can not accepting client. \n";
        }

    
        thread t(Communication_With_Client, ref(server), ref(cond));
        t.detach();
    }
  
    return 0;
   
}

