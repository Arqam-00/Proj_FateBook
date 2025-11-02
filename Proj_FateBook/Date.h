#pragma once

#include <ctime>
using namespace std;

struct Date
{
    int Day;
    int Month;
    int Year;
    Date();
    Date(int D, int M, int Y);
    bool IsValid();
    Date NextDate();
};
