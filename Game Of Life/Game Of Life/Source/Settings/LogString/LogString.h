#pragma once

#include <string>

class LogString
{
    public:
        LogString();
        void Clear();
        bool IsEmpty() const;

        LogString& operator <<(const char* text);
        LogString& operator <<(const std::string& text);
        operator const char*() const;

    private:
        std::string log;
};
