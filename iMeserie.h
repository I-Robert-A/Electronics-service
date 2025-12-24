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
    tehnician(const int id,
              std::string Nume, std::string Prenume,
              std::string cnp,
              std::string data,
              std::string oras, std::vector<std::string> tipuri,
              std::vector<std::string> marci): angajat(id,Nume,Prenume,cnp,data,oras), tipuriR{tipuri}, marciR{marci}
              {
              }
              
    void setpret(double pret) { pretR = pretR + pret; }
    double calcS() const override
    {
        return 4000 + pretR + Prima_transport() + 200 * (getTimp_lucrat() / 3);
    }
};

class supervizor : public angajat
{
public:
supervizor(const int id,
              std::string Nume, std::string Prenume,
              std::string cnp,
              std::string data,
              std::string oras): angajat(id,Nume,Prenume,cnp,data,oras)
              {
              }    
double calcS() const override
    {
        return 4000 + 800 + Prima_transport() + 200 * (getTimp_lucrat() / 3);
    }
};

class receptioner : public angajat
{
private:
    std::vector<int> IDuri;

public:
receptioner(const int id,
              std::string Nume, std::string Prenume,
              std::string cnp,
              std::string data,
              std::string oras, std::vector<int> iduri): angajat(id,Nume,Prenume,cnp,data,oras), IDuri{iduri}
              {
              }
    double calcS() const override
    {
        return 4000 + Prima_transport() + 200 * (getTimp_lucrat() / 3);
    }
};