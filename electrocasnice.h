#include<iostream>
#include<string>
#include<vector>
#include<ctime>
#pragma once

class electrocasnice
{
    protected:
    std::string tip;
    std::string marca;
    std::string model;
    int an;
    double pret;

    public:
   electrocasnice()=default;
    electrocasnice(std::string Tip,std::string Marca, std::string Model, int An, double Pret):tip{Tip},marca{Marca}, model{Model}, an{An}, pret{Pret}{}
    virtual void afisare(std::ostream& dev)const{dev<<tip<<" "<<marca<<" "<<model<<" "<<an<<" "<<pret<<" ";}
    virtual ~electrocasnice()=default;   
    double getPret(){return pret;}
    std::string getMarca(){return marca;}
    std::string getTip(){return tip;}

    int getVechime(time_t timestamp)
    {
        struct tm datetime;
        time_t now;
        time_t fabricatie;
        now = time(NULL);
        datetime = *localtime(&now);
        datetime.tm_year = an-1900;
        datetime.tm_mon = 1-1;
        datetime.tm_mday = 1;
        datetime.tm_hour = 0; datetime.tm_min = 0; datetime.tm_sec = 0;
        datetime.tm_isdst = -1;
        fabricatie = mktime(&datetime);
        double diffSec = difftime(timestamp, fabricatie); 
        int years = static_cast<int>(diffSec / 31536000.0);
        return years;
    }                                                            
};