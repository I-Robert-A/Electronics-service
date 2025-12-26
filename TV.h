#include "electrocasnice.h"

class TV: public electrocasnice
{
    double diagonala;
    public:
    TV(
            std::string Marca,
            std::string Model,
            int An,
            double Pret,
            double Diagonala     
        ):electrocasnice(Marca,Model,An,Pret),diagonala{Diagonala}{}
    void afisare(std::ostream& dev)
    {
        electrocasnice::afisare(dev);
        dev<<diagonala<<std::endl;
    }
    };