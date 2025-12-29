#include "electrocasnice.h"
#include<ctime>
#include<memory>
#include<sstream>
#include "icreareElectrocasnice.h"
#include <queue>
#pragma once

class cerereR
{
    int id;
    std::unique_ptr<electrocasnice> e;
    time_t timestamp;
    int complexitate;
    int durata;
    double pret;
    public:
    cerereR()=default;
    cerereR(std::unique_ptr<electrocasnice> E, 
            time_t Timestamp,
            int Complexitate, int Id
        ):id(Id),
        e{std::move(E)},
        timestamp{Timestamp},
        complexitate{Complexitate}
        {
            if(complexitate>5 || complexitate<0)
            {
                throw std::invalid_argument("complexitate neconforma");
            }
            if(complexitate==0)
            {
                durata=0;
                pret=0;
            }
            else
            {
                durata=complexitate*e->getVechime(timestamp);
                pret=durata*e->getPret();
            }
        }
        void afisare(std::ostream& dev)const;
        time_t getTime()const{return timestamp;}
        std::string getMarca()const{return e->getMarca();}
        std::string getTip()const{return e->getTip();}
};

void citire(cerereR&, std::string );
struct compare {
    bool operator()(const cerereR& a, const cerereR& b) const {
        return a.getTime() > b.getTime();
    }
};

using PQ = std::priority_queue<cerereR, std::vector<cerereR>, compare>;

