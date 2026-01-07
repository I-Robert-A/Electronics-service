#include "raporturi.h"
void salariu(const std::vector<std::unique_ptr<angajat>>& angajati)
{
    std::vector<const angajat*> v;
    for (const auto& p : angajati)
        v.push_back(p.get());

    std::sort(v.begin(), v.end(),[](const angajat* a, const angajat* b) {
            return a->calcS() > b->calcS();
        });

    if (v.size() > 3) v.resize(3);
    std::sort(v.begin(), v.end(),[](const angajat* a, const angajat* b) {
            if (a->getNume() != b->getNume())
                return a->getNume() < b->getNume();
            return a->getPrenume() < b->getPrenume();
        });
    for(const auto& p:v)
    {
        p->afisare(std::cout);
    }
}

void lunga(std::istream& dev, const std::vector<tehnician*>& tehnicieni)
{
    int max=0;
    int maxID=0;
    int id;
    int durata;
    std::string linie;
    while(std::getline(dev,linie))
    {
        if(!linie.empty())
        {
            std::stringstream ss(linie);        
            std::string Durata;
            std::getline(ss,Durata,',');
            std::string ID;
            std::getline(ss,ID,',');
            id=std::stoi(ID);
            durata=std::stoi(Durata);
            if(durata>max)
            {
                max=durata;
                maxID=id;
            }
        }
    }
        if (maxID == -1) 
        {
            throw std::invalid_argument("nu a reparat nimeni nimic");
        }
    for(const auto& it:tehnicieni)
    {
        if(maxID==it->getID())
        {
            it->afisare(std::cout);
        }
    }

}