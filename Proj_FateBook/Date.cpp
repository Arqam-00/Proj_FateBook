#include "Date.h"

Date::Date() : Day(1), Month(1), Year(1970) {}
Date::Date(int D, int M, int Y) : Day(D), Month(M), Year(Y) {}
bool Date::IsValid()
{
    if (Month < 1 || Month > 12 || Day < 1 || Year < 1) return false;
    int DaysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    bool Leap = (Year % 4 == 0 && (Year % 100 != 0 || Year % 400 == 0));
    if (Leap) DaysInMonth[1] = 29;

    return Day <= DaysInMonth[Month - 1];
}
Date Date::NextDate()
{
    Date D = *this;

    int DaysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    bool Leap = (Year % 4 == 0 && (Year % 100 != 0 || Year % 400 == 0));
    if (Leap) DaysInMonth[1] = 29;
    D.Day++;
    if (D.Day > DaysInMonth[D.Month - 1])
    {

        D.Day = 1;
        D.Month++;
        if (D.Month > 12)
        {
            D.Month = 1;
            D.Year++;
        }
    }
    return D;
}
time_t Date::ToTimeT()
{
    tm Time = {};
    Time.tm_mday = Day;
    Time.tm_mon = Month - 1;
    Time.tm_year = Year - 1900;
    return mktime(&Time);
}
Date Date::FromTimeT(time_t T)
{
    tm* TimeStruct = localtime(&T);
    return Date(TimeStruct->tm_mday, TimeStruct->tm_mon + 1, TimeStruct->tm_year + 1900);
}
