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
    std::shared_ptr<electrocasnice> e;
    time_t timestamp;
    int complexitate;
    int durata;
    double pret;
    public:
    cerereR()=default;
    cerereR(std::shared_ptr<electrocasnice> E, 
            time_t Timestamp,
            int Complexitate, int Id
        ):id(Id),
        e{E},
        timestamp{Timestamp},
        complexitate{Complexitate}
        {
            if(complexitate>5 || complexitate<0)
            {
                throw std::invalid_argument("complexitate neconforma la id: " + std::to_string(id));

            }
            else if(complexitate==0)
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
    cerereR(const cerereR& cr):id{cr.id},e{cr.e},timestamp{cr.timestamp},
                               complexitate{cr.complexitate},durata{cr.durata},pret{cr.pret}{}

        void afisare(std::ostream& dev)const;
        void afisareCSV(std::ostream& dev)const;
        time_t getTime()const{return timestamp;}
        std::string getMarca() const {
        if (!e) throw std::runtime_error("cerereR: electrocasnice null");
        return e->getMarca();
        }
        std::string getTip()const{return e->getTip();}
        double getPret()const{return pret;}
        int getDurata()const{return durata;}
        int getID()const{return id;}
        std::string getModel()const{return e->getModel();}
};

void citire(cerereR&, std::string ,std::ostream& ,bool);
struct compare {
    bool operator()(const cerereR& a, const cerereR& b) const {
        return a.getTime() > b.getTime();
    }
};

using PQ = std::priority_queue<cerereR, std::vector<cerereR>, compare>;

void afisareReparate(std::ostream& ,std::istream&);
void afiseazaInvalide(std::istream& dev,std::ostream& devO);