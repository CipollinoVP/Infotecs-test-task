#include "journal.h"

Journal::Journal(const std::string &path, t_priority min_priority) {
    path_ = path;
    file_.open(path, std::ios::in | std::ios::out | std::ios::app);
    if (!file_.is_open()){
        throw std::runtime_error("File error");
    }
    min_priority_ = min_priority;
}

void Journal::change_min_priority(t_priority min_priority){
    min_priority_ = min_priority;
}

void Journal::add_message(const std::string& message, t_priority priority) {
    if (priority < min_priority_){
        return;
    }

    file_.seekp(0, std::ios::end);

    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
    auto time_str = std::put_time(&tm, "%c %Z");

    std::string s_priority;
    switch (priority){
        case Low:
            s_priority = "Low";
            break;
        case Medium:
            s_priority = "Medium";
            break;
        case High:
            s_priority = "High";
            break;
    }

    file_ << time_str << " " << s_priority << " " << message << std::endl;
}

Journal::~Journal() {
    file_.close();
}
