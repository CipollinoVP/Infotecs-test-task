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

int get_priority(std::string& message);

int command_handler(std::string const& message);

void change_priority(Journal& journal, t_priority& min_priority);

void add_message(std::string message, Journal& journal, t_priority min_priority);

int main(int argc, char* argv[]) {
    std::string path = argv[1];
    unsigned int min = std::stoi(std::string(argv[2]));

    t_priority min_priority = (min >= 3) ? High : t_priority(min);
    Journal journal(path, min_priority);

    int flag = 1;
    std::cin.ignore();

    while (flag) {
        // Считываем сообщение с приоритетом
        std::string message;
        std::getline(std::cin, message);

        //Проверям на команды
        switch (command_handler(message)){
            case CHANGE_PRIORITY:
                change_priority(journal,min_priority);
                break;
            case EXIT:
                flag = 0;
                break;
            case NO_COMMAND:
                (void) std::async(std::launch::async, add_message,
                        message, std::ref(journal), min_priority);
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