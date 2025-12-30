#include "angajat.h"
#include <vector>
#include <ctime>
#include <map>
#include<set>
#include<array>
#pragma once


class tehnician : public angajat
{
private:
    std::map<std::string, std::set<std::string>> repara;
    double pretR = 0.0;
    int lucrari=0;
    std::array<int,2> IDC={0,0};
    std::array<int,2> durataC={0,0}; 
public:
    tehnician(std::string Post,
              std::string Nume, std::string Prenume,
              std::string cnp,
              std::string data,
              std::string oras, std::map<std::string, std::set<std::string>> Repara, int Lucrari): 
              angajat(Post,Nume,Prenume,cnp,data,oras),repara{Repara},lucrari{Lucrari}
              {
              }
              
    void setpret(double pret) { pretR = pretR + pret; }
    double calcS() const override
    {
        return 4000 + pretR + Prima_transport() + 200 * (getTimp_lucrat() / 3);
    }
    void afisare(std::ostream& dev)const override
    {
        dev<<getID()<<" "<<post<<" "<<nume<<" "<<prenume<<" "<<CNP<<" "<<data_A<<" "<<oras_D<<" "<<lucrari<<std::endl;
        dev<<"tipuri si marci: ";
        for (auto it = repara.begin(); it != repara.end(); ++it) {
            const std::string& tip = it->first;
            const std::set<std::string>& marci = it->second;

            dev << tip << ": ";
            for (const auto& marca : marci) {
                dev << marca << " ";
            }
            dev << "\n";
        }


        dev<<std::endl;
        dev<<"salariu: "<<calcS()<<std::endl;
    }
    void addLucrare(){lucrari++;}
    int getLucrare()const{return lucrari;}
    const std::map<std::string, std::set<std::string>>& getRepara()const{return repara;}
    int getIDC(int i){return IDC[i];}
    int getdurataC(int i){return durataC[i];}
    void setIDC(int idc, int i){IDC[i]=idc;}
    void setdurataC(int duratac, int i){durataC[i]=duratac;}
};

class supervizor : public angajat
{
public:
supervizor(   std::string Post,
              std::string Nume, std::string Prenume,
              std::string cnp,
              std::string data,
              std::string oras): angajat(Post,Nume,Prenume,cnp,data,oras)
              {
              }    
double calcS() const override
    {
        return 4000 + 800 + Prima_transport() + 200 * (getTimp_lucrat() / 3);
    }
    void afisare(std::ostream& dev)const override
    {
        dev<<getID()<<" "<<post<<" "<<nume<<" "<<prenume<<" "<<CNP<<" "<<data_A<<" "<<oras_D<<" "<<std::endl;
        dev<<"salariu: "<<calcS()<<std::endl;
    }
};

class receptioner : public angajat
{
private:
    std::vector<int> IDuri;

public:
receptioner(std::string Post,std::string Nume, std::string Prenume,
              std::string cnp,
              std::string data,
              std::string oras, std::vector<int> iduri): angajat(Post,Nume,Prenume,cnp,data,oras), IDuri{iduri}
              {
              }
    double calcS() const override
    {
        return 4000 + Prima_transport() + 200 * (getTimp_lucrat() / 3);
    }
    void afisare(std::ostream& dev)const override
    {
        dev<<getID()<<post<<" "<<nume<<" "<<prenume<<" "<<CNP<<" "<<data_A<<" "<<oras_D<<" "<<std::endl;
        dev<<"iduri: ";
        auto it1 = IDuri.begin();

        for (; it1 != IDuri.end(); ++it1)
        {
            dev << *it1 << " ";
        }

        dev<<std::endl;
        dev<<"salariu: "<<calcS()<<std::endl;
    }
    void addID(int IDA){IDuri.push_back(IDA);}
};