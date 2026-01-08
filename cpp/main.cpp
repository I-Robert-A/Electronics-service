#include <iostream>
#include <exception>
#include "lucruAngajati.h"
#include "service.h"
#include<fstream>
#include<sstream>
#include "cerereR.h"
#include <queue>
#include <set>
#include <array>
#include<random>
#include <thread>
#include <chrono>
#include "raporturi.h"

service* service::instance=nullptr;

int main() {

        std::ifstream fin("cereri.csv");
        std::ifstream fin2("angajati.csv");
        std::ofstream fout("cereriInvalide.csv"); 
        std::ifstream fin3("marci.csv");
        std::ofstream fout2("cereriIreparabile.csv");
        std::ofstream fout3("cereriReparate.csv");
        std::ifstream fincR("cereriReparate.csv");
        std::string linie;
        service s;
        citireAngajat(fin2,s);
        s.citireMarci(fin3);
        
        EmployeeData d; 
        auto teh=s.getPtrteh();
        auto rec=s.getPtrrec();
        auto sup=s.getPtrsup();
        if(teh.size()<3 || rec.size()<1 || sup.size()<1)
        {
            std::cout<<"Service ul nu are destui angajati";
            return 0;
        }
        std::sort(teh.begin(), teh.end(),[](tehnician* a, tehnician*b)
        {
            return a->getLucrare()<b->getLucrare();
        });
        PQ cereri;
while (std::getline(fin, linie)) {
    cerereR cr;
    if(!linie.empty())
    {
        try
        {
            citire(cr,linie,fout,true);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            continue;
        }
        if(rec.size()>1)
            {
                std::mt19937 gen(std::random_device{}());
                std::uniform_int_distribution<> dist(0, rec.size() - 1);
                receptioner* r = rec[dist(gen)];
                auto it=std::find(r->getIduri().begin(),r->getIduri().end(),cr.getID());
                if(it!=r->getIduri().end())
                    r->addID(cr.getID());
            }
        else {
        if (!rec.empty()) 
        {
            auto* r = *rec.begin();                 
            const auto& ids = r->getIduri();         

            auto it = std::find(ids.begin(), ids.end(), cr.getID());

            if (it == ids.end()) {
                r->addID(cr.getID());
            }
        }
        else {
            std::cerr << "Eroare: rec este gol, nu pot face addID\n";
        }
    }
        s.verificareCerere(fout,cr,cereri);
    }
}
prelucrareCereri(cereri,fout2,teh,fout3);
fin2.close();
std::ofstream foutA("angajati.csv");
s.afisareAngajatiCSV(foutA);
fin3.close();
std::ofstream foutM("marci.csv");
s.inregistrareMM(foutM);
fout3.close();
try
{
lunga(fincR,teh);
}
catch(const std::exception& e)
{
    std::cerr << e.what() << '\n';
}
fout.close();
std::ifstream finIn("cereriInvalide.csv");
afiseazaInvalide(finIn,std::cout);
fincR.close();
fincR.open("cereriReparate.csv");
        return 0;

}

