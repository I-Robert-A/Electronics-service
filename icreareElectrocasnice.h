#include "iElectrocasnice.h"
#include <memory>
#pragma once
struct ElectronicData
{
    std::string tip;
    std::string marca;
    std::string model;
    int an;
    double pret;
    double Diagonala;
    double greutate;
    int congelator;
};

class icreareElectrocasnice
{
    public:
    virtual std::shared_ptr<electrocasnice> creeazaElectrocasnic(ElectronicData)=0;
    virtual ~icreareElectrocasnice()=default;
};

class Electrocasnic_frigider:public icreareElectrocasnice
{
    public:
    std::shared_ptr<electrocasnice> creeazaElectrocasnic(ElectronicData ed) override
    {
        return std::make_shared<frigider>(ed.tip,ed.marca,ed.model,ed.an,ed.pret,ed.congelator);
    }

};
class Electrocasnic_TV:public icreareElectrocasnice
{
    public:
    std::shared_ptr<electrocasnice> creeazaElectrocasnic(ElectronicData ed) override
    {
        return std::make_shared<TV>(ed.tip,ed.marca,ed.model,ed.an,ed.pret,ed.Diagonala);
    }

};
class Electrocasnic_MasSpalat:public icreareElectrocasnice
{
    public:
    std::shared_ptr<electrocasnice> creeazaElectrocasnic(ElectronicData ed) override
    {
        return std::make_shared<masinaSpalat>(ed.tip,ed.marca,ed.model,ed.an,ed.pret,ed.greutate);
    }

};