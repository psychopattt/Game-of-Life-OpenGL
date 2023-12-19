#include "LogString.h"

LogString::LogString()
{
    log.reserve(1024);
}

void LogString::Clear()
{
    log.clear();
}

bool LogString::IsEmpty() const
{
    return log.empty();
}

LogString& LogString::operator<<(const char* text)
{
    log += text;
    return *this;
}

LogString& LogString::operator<<(const std::string& text)
{
    return *this << text.c_str();
}

LogString::operator const char*() const
{
    return log.c_str();
}
