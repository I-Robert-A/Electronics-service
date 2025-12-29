#include "electrocasnice.h"

class TV: public electrocasnice
{
    double diagonala;
    public:
    TV(
            std::string Tip,
            std::string Marca,
            std::string Model,
            int An,
            double Pret,
            double Diagonala     
        ):electrocasnice(Tip,Marca,Model,An,Pret),diagonala{Diagonala}{}
    void afisare(std::ostream& dev) const override
    {
        electrocasnice::afisare(dev);
        dev<<diagonala<<std::endl;
    }
    };

    class masinaSpalat: public electrocasnice
{
    double greutate;
    public:
    masinaSpalat(
            std::string Tip,
            std::string Marca,
            std::string Model,
            int An,
            double Pret,
            double Greutate     
        ):electrocasnice(Tip,Marca,Model,An,Pret),greutate{Greutate}{}
    void afisare(std::ostream& dev) const override
    {
        electrocasnice::afisare(dev);
        dev<<greutate<<std::endl;
    }
    };

    class frigider: public electrocasnice
{
    int congelator;
    public:
    frigider(
            std::string Tip,
            std::string Marca,
            std::string Model,
            int An,
            double Pret,
            int Congelator     
        ):electrocasnice(Tip,Marca,Model,An,Pret),congelator{Congelator}{}
    void afisare(std::ostream& dev) const override
    {
        electrocasnice::afisare(dev);
        dev<<congelator<<std::endl;
    }
    };