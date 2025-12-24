#include "iMeserie.h"
#include <memory>
#pragma once
struct EmployeeData {
    int ID;
    std::string Nume, Prenume, CNP, data_A, oras_D;

    // specifice (pot fi goale dacă nu se aplică)
    std::vector<std::string> tipuriR;   // doar tehnician
    std::vector<std::string> marciR;    // doar tehnician

    std::vector<int> IDuri;       // doar receptioner (ID-uri cereri)
};

class iAngajat
{
    public:
    virtual std::unique_ptr<angajat> creeazaAngajat(const EmployeeData&)=0;
};

class Angajat_tehnician: public iAngajat
{
    public:
    std::unique_ptr<angajat> creeazaAngajat(const EmployeeData& d) override{
        return std::make_unique<tehnician>(d.ID, d.Nume,d.Prenume,d.CNP,d.data_A,d.oras_D,d.tipuriR,d.marciR);
    }
};
class Angajat_supervizor: public iAngajat
{
    public:
    std::unique_ptr<angajat> creeazaAngajat(const EmployeeData& d) override{
        return std::make_unique<supervizor>(d.ID, d.Nume,d.Prenume,d.CNP,d.data_A,d.oras_D);
    }
};
class Angajat_receptioner: public iAngajat
{
    public:
    std::unique_ptr<angajat> creeazaAngajat(const EmployeeData& d) override{
        return std::make_unique<receptioner>(d.ID, d.Nume,d.Prenume,d.CNP,d.data_A,d.oras_D,d.IDuri);
    }
};