#include "electrocasnice.h"
#pragma once
class masinaSpalat: public electrocasnice
{
    double greutate;
    public:
    masinaSpalat(
            std::string Marca,
            std::string Model,
            int An,
            double Pret,
            double Greutate     
        ):electrocasnice(Marca,Model,An,Pret),greutate{Greutate}{}
    void afisare(std::ostream& dev)
    {
        electrocasnice::afisare(dev);
        dev<<greutate<<std::endl;
    }
    };