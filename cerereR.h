#include "electrocasnice.h"
#include<ctime>
#include<memory>

class cerereR
{
    int id;
    std::unique_ptr<electrocasnice> e;
    time_t timestamp;
    int complexitate;
    int durata;
    double pret;
    public:
    cerereR(std::unique_ptr<electrocasnice> E, 
            time_t Timestamp,
            int Complexitate, int Id
        ):id(Id),
        e{std::move(E)},
        timestamp{Timestamp},
        complexitate{Complexitate}
        {
            if(complexitate>5 || complexitate<0)
            {
                throw std::invalid_argument("complexitate neconforma");
            }
            if(complexitate==0)
            {
                durata=0;
                pret=0;
            }
            else
            {
                durata=complexitate*e->getVechime(timestamp);
                pret=durata*e->getPret();
            }
        }
};