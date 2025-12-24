#include<iostream>
#pragma once
#include<string>

class angajat
{
    protected:
    const int ID;
    std::string nume, prenume;
    std::string CNP;
    std::string data_A;
    std::string oras_D;
    public:
    angajat(int id,
            std::string numele,
            std::string prenumele,
            std::string cnp,
            std::string dataAngajarii,
            std::string oras)
        : ID(id),
          nume(std::move(numele)),
          prenume(std::move(prenumele)),
          CNP(std::move(cnp)),
          data_A(std::move(dataAngajarii)),
          oras_D(std::move(oras)) 
          {
            std::cout<<numele;
            if(nume.size()<3 || nume.size()>30)
            {
                throw std::invalid_argument("Numele trebuie sa aiba intre 3 si 30 de caractere");
            }
            if(prenume.size()<3 || prenume.size()>30)
            {
                throw std::invalid_argument("Numele trebuie sa aiba intre 3 si 30 de caractere");
            }
            std::string control="279146358279";
            int suma=0;
            for(int i=0;i<12;i++)
            {
                suma=suma+((control[i]-'0')*(CNP[i]-'0'));
            }
                suma=suma%11;

            if(CNP[12]-'0'!=suma)
            {
                throw std::invalid_argument("CNP invalid");
            }
          }
    int getTimp_lucrat() const;
    int Prima_transport() const;
    int getID(){return ID;}
    std::string getNume(){return nume;}
    std::string setNume(std::string Nume){nume=Nume;}
    std::string getPrenume(){return prenume;}
    std::string getCNP(){return CNP;}
    std::string getdata_A(){return data_A;}
    std::string getOras_D(){return oras_D;}
    virtual double calcS() const = 0;
    virtual ~angajat()= default;

};