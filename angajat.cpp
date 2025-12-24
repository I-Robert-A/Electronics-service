#include "angajat.h"
int angajat::getTimp_lucrat() const
    {
        int day = std::stoi(data_A.substr(0,2));
        int month = std::stoi(data_A.substr(3,2));
        int year = std::stoi(data_A.substr(6,4));
        time_t now;
        time_t angajare;
        struct tm datetime;
        now = time(NULL);
        datetime = *localtime(&now);
        datetime.tm_year = year-1900;
        datetime.tm_mon = month-1;
        datetime.tm_mday = day;
        datetime.tm_hour = 0; datetime.tm_min = 0; datetime.tm_sec = 0;
        datetime.tm_isdst = -1;
        angajare = mktime(&datetime);
        std::cout << asctime(&datetime);
        int diff = difftime(angajare, now);
        return diff/31536000;
    }
    int angajat::Prima_transport() const
    {
        if(oras_D!="Bucuresti" && oras_D!="bucuresti")
            return 400;
        else
            return 0;
    }