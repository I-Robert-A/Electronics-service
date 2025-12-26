#include "angajat.h"
#include <vector>
#include <ctime>
#pragma once


class tehnician : public angajat
{
private:
    std::vector<std::string> tipuriR;
    std::vector<std::string> marciR;
    double pretR = 0.0;

public:
    tehnician(std::string Nume, std::string Prenume,
              std::string cnp,
              std::string data,
              std::string oras, std::vector<std::string> tipuri,
              std::vector<std::string> marci): angajat(Nume,Prenume,cnp,data,oras), tipuriR{tipuri}, marciR{marci}
              {
              }
              
    void setpret(double pret) { pretR = pretR + pret; }
    double calcS() const override
    {
        return 4000 + pretR + Prima_transport() + 200 * (getTimp_lucrat() / 3);
    }
    void afisare(std::ostream& dev)const override
    {
        dev<<ID<<" "<<nume<<" "<<prenume<<" "<<CNP<<" "<<data_A<<" "<<oras_D<<" "<<std::endl;
        dev<<"tipuri si marci: ";
        auto it1 = tipuriR.begin();
        auto it2 = marciR.begin();

        for (; it1 != tipuriR.end() && it2 != marciR.end(); ++it1, ++it2)
        {
            dev << *it1 << " " << *it2;
        }

        dev<<std::endl;
        dev<<"salariu: "<<calcS()<<std::endl;
    }
};

class supervizor : public angajat
{
public:
supervizor(   std::string Nume, std::string Prenume,
              std::string cnp,
              std::string data,
              std::string oras): angajat(Nume,Prenume,cnp,data,oras)
              {
              }    
double calcS() const override
    {
        return 4000 + 800 + Prima_transport() + 200 * (getTimp_lucrat() / 3);
    }
    void afisare(std::ostream& dev)const override
    {
        dev<<ID<<" "<<nume<<" "<<prenume<<" "<<CNP<<" "<<data_A<<" "<<oras_D<<" "<<std::endl;
        dev<<"salariu: "<<calcS()<<std::endl;
    }
};

class receptioner : public angajat
{
private:
    std::vector<int> IDuri;

public:
receptioner(std::string Nume, std::string Prenume,
              std::string cnp,
              std::string data,
              std::string oras, std::vector<int> iduri): angajat(Nume,Prenume,cnp,data,oras), IDuri{iduri}
              {
              }
    double calcS() const override
    {
        return 4000 + Prima_transport() + 200 * (getTimp_lucrat() / 3);
    }
    void afisare(std::ostream& dev)const override
    {
        dev<<ID<<" "<<nume<<" "<<prenume<<" "<<CNP<<" "<<data_A<<" "<<oras_D<<" "<<std::endl;
        dev<<"iduri: ";
        auto it1 = IDuri.begin();

        for (; it1 != IDuri.end(); ++it1)
        {
            dev << *it1 << " ";
        }

        dev<<std::endl;
        dev<<"salariu: "<<calcS()<<std::endl;
    }
};