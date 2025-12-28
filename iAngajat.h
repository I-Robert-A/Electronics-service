#include "iMeserie.h"
#include <memory>
#pragma once
struct EmployeeData {
    std::string Post,Nume, Prenume, CNP, data_A, oras_D;

    // specifice (pot fi goale dacă nu se aplică)
    std::map<std::string, std::set<std::string>> repara;   // doar tehnician
    int lucrari;
    std::vector<int> IDuri;       // doar receptioner (ID-uri cereri)
};

class iAngajat
{
    public:
    virtual std::unique_ptr<angajat> creeazaAngajat(EmployeeData)=0;
};

class Angajat_tehnician: public iAngajat
{
    public:
    std::unique_ptr<angajat> creeazaAngajat(EmployeeData d) override{
        //angajat::setID(1);
        return std::make_unique<tehnician>(d.Post,d.Nume,d.Prenume,d.CNP,d.data_A,d.oras_D,d.repara,d.lucrari);
    }
};
class Angajat_supervizor: public iAngajat
{
    public:
    std::unique_ptr<angajat> creeazaAngajat(EmployeeData d) override{
        //angajat::setID(1);
        return std::make_unique<supervizor>(d.Post,d.Nume,d.Prenume,d.CNP,d.data_A,d.oras_D);
    }
};
class Angajat_receptioner: public iAngajat
{
    public:
    std::unique_ptr<angajat> creeazaAngajat(EmployeeData d) override{
        //angajat::setID(1);
        return std::make_unique<receptioner>(d.Post,d.Nume,d.Prenume,d.CNP,d.data_A,d.oras_D,d.IDuri);
    }
};