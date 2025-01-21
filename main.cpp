#include "journal.h"
#include <iostream>
#include <string>
#include <cctype>
#include <thread>
#include <mutex>
#include <future>

#define ASCII_ZERO 48

//Макросы для команд
#define CHANGE_PRIORITY 1
#define EXIT 2
#define NO_COMMAND 0

std::mutex logMutex;

//Выдача приоритета сообщению (указанного или по умолчанию)
int get_priority(std::string& message);

//Обработчик команд
int command_handler(std::string const& message);

//Смена приоритета по умолчанию
void change_priority(Journal& journal, t_priority& min_priority);

//Добавление сообщения в журнал
void add_message(std::string message, Journal& journal, t_priority min_priority);

int main(int argc, char* argv[]) {
    if (argc < 3){
        std::cout << "Usage: " << argv[0] << " <log_file> <default_priority>\n";
        return 1;
    }

    std::string path = argv[1];
    unsigned int min;
    try {
        min = std::stoi(std::string(argv[2]));
    } catch (const std::exception& e) {
        std::cerr << "Error: Invalid default priority\n";
        return 1;
    }

    t_priority min_priority = (min >= 3) ? High : t_priority(min);
    Journal* journal;
    try {
        journal = new Journal(path, min_priority);
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    int flag = 1;
    while (flag) {
        std::cout << "Enter message or command:" << std::endl;

        // Считываем сообщение с приоритетом
        std::string message;
        std::getline(std::cin, message);

        //Проверям на команды
        switch (command_handler(message)){
            case CHANGE_PRIORITY:
                change_priority(*journal,min_priority);
                break;
            case EXIT:
                flag = 0;
                break;
            case NO_COMMAND:
                (void) std::async(std::launch::async, add_message,
                        message, std::ref(*journal), min_priority);
                break;
            default:
                std::cout << "Unknown command" << std::endl;
        }
    }

    return 0;
}

int get_priority(std::string& message){
    if (std::isdigit(message.back())){
        int priority = message.back()-ASCII_ZERO;
        message.pop_back();
        return priority;
    } else {
        return 10;
    }
}

int command_handler(std::string const& message){
    if (message[0] != '-'){
        return NO_COMMAND;
    }
    if (message == "-exit"){
        return EXIT;
    }
    if (message == "-change"){
        return CHANGE_PRIORITY;
    }
    return -1;
}

void change_priority(Journal& journal, t_priority& min_priority){
    unsigned int min;
    std::cout << "Enter new priority:" << std::endl;
    std::cin >> min;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    min_priority = (min >= 3) ? High : t_priority(min);
    journal.change_min_priority(min_priority);
}

void add_message(std::string message, Journal& journal, t_priority min_priority){
    //Предотвращение преждевременной записи нового сообщения в журнал
    std::lock_guard<std::mutex> guard(logMutex);

    // Выделяем приоритет, если есть
    unsigned int priority = get_priority(message);
    t_priority p = (priority == 10) ? min_priority :
                   (priority >= 3) ? High : t_priority(priority);

    journal.add_message(message, p);
}