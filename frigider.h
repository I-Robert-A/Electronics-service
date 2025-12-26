#include "electrocasnice.h"

class frigider: public electrocasnice
{
    bool congelator;
    public:
    frigider(
            std::string Marca,
            std::string Model,
            int An,
            double Pret,
            bool Congelator     
        ):electrocasnice(Marca,Model,An,Pret),congelator{Congelator}{}
    void afisare(std::ostream& dev)
    {
        electrocasnice::afisare(dev);
        dev<<congelator<<std::endl;
    }
    };