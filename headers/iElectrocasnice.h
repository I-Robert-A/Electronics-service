#include "electrocasnice.h"

class TV: public electrocasnice
{
    double diagonala=0.0;
    public:
    TV()=default;
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
        dev<<diagonala;
    }
    void afisareCSV(std::ostream& dev) const override
    {
        electrocasnice::afisareCSV(dev);
        dev<<diagonala<<",";
    }
    };

    class masinaSpalat: public electrocasnice
{
    double greutate;
    public:
    masinaSpalat()=default;
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
        dev<<greutate;
    }
    void afisareCSV(std::ostream& dev) const override
    {
        electrocasnice::afisareCSV(dev);
        dev<<greutate<<",";
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
        dev<<congelator;
    }
    void afisareCSV(std::ostream& dev) const override
    {
        electrocasnice::afisareCSV(dev);
        dev<<congelator<<",";
    }
    };

    class necunoscut: public electrocasnice
{
    double ceva;
    public:
    necunoscut()=default;
    necunoscut(
            std::string Tip,
            std::string Marca,
            std::string Model,
            int An,
            double Pret,
            double Ceva     
        ):electrocasnice(Tip,Marca,Model,An,Pret),ceva{Ceva}{}
    void afisare(std::ostream& dev) const override
    {
        electrocasnice::afisare(dev);
        dev<<ceva;
    }
    void afisareCSV(std::ostream& dev) const override
    {
        electrocasnice::afisareCSV(dev);
        dev<<ceva<<",";
    }
    };
