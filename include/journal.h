#pragma once

#ifdef BUILD_LIBRARY
#define JOURNAL_API __attribute__((visibility("default")))
#else
#define JOURNAL_API
#endif

#include <string>
#include <fstream>
#include <ctime>
#include <iomanip>

enum JOURNAL_API t_priority{
    Low,
    Medium,
    High
};

class JOURNAL_API Journal{
public:
    Journal(const std::string& path, t_priority min_priority);
    ~Journal();

    void change_min_priority(t_priority min_priority);

    void add_message(const std::string& message, t_priority priority);
private:
    std::string path_;
    t_priority min_priority_;
    std::fstream file_;
};

