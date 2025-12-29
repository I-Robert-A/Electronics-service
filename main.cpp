#include <iostream>
#include <exception>
#include "lucruAngajati.h"
#include "service.h"
#include<fstream>
#include<sstream>
#include "cerereR.h"
#include <queue>
#include <set>


service* service::instance=nullptr;

int main() {
     /*EmployeeData d{
            "Popa", "Iona", "5050603170065",
            "10/03/2031", "Cluj",
            {"TV"}, {"LG"}, {1,3}
     };
    std::vector<std::unique_ptr<angajat>> angajati;
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>> posReparatii;
    angajare(d,"tehnician",angajati);
    for (const auto& p : angajati) {      // iterare prin referință
    if (p) p->afisare(std::cout);     // p e unique_ptr, folosești ->
    }
    concediere(1,angajati);
    for (const auto& p : angajati) {      // iterare prin referință
    if (p) p->afisare(std::cout);     // p e unique_ptr, folosești ->
    }
    std::string tip="frigider";
    std::string tip2="TV";
    std::string marca="LG";
    std::string model="pinguin";
    std::string model2="pasare";
    adaugareModel(tip,marca,model,posReparatii);
    adaugareModel(tip,marca,model2,posReparatii);
    stergereMarca(tip,marca,posReparatii);
    adaugareMarca(tip2,"Samsung",posReparatii);

    for(auto it = posReparatii.begin(); it != posReparatii.end(); it++)
        {
            std::cout << it->first << ": ";
            for(auto it2=(*it).second.begin();it2!=(*it).second.end();it2++)
                {
                    std::cout<<it2->first<<"="<<" ";
                    for(auto it3:(*it2).second)
                    {
                        std::cout<<(it3)<<" ";
                    }
                }
        }*/

        std::ifstream fin("cereri.csv");
        std::ifstream fin2("angajati.csv");
        std::ofstream fout("cereriInvalide.csv"); 
        std::ifstream fin3("marci.csv");
        std::string linie;
        service s;
        struct compare
        {
            bool operator()(const cerereR& a, const cerereR& b)const
            {
                return a.getTime()>b.getTime();
            }
        };
        struct less
        {
            bool operator()(const cerereR& a, const cerereR& b)const
            {
                return a.getTime()<b.getTime();
            }
        };
        std::set<cerereR,less> cereriAsteptare;
        EmployeeData d;
        
            //std::cout<<d.Nume<<" "<<d.Prenume<<" "<<d.CNP<<" "<<d.data_A<<" "<<d.lucrari<<" "<<d.oras_D<<" ";
            for(auto i:d.IDuri)
            {
                //std::cout<<(i);
            }
            for(auto i:d.repara)
            {
                //std::cout<<i.first<<" ";
                for(auto y:i.second)
                {
                    //std::cout<<y<<" ";
                }
                //std::cout<<std::endl;
            }    
        citireAngajat(fin2,s);
        s.concediere(2);
        //s.afisareAngajati();
        auto tehnicieni=s.getPtrteh();
        std::sort(tehnicieni.begin(), tehnicieni.end(),[](tehnician* a, tehnician*b)
        {
            return a->getLucrare()<b->getLucrare();
        });
        for (auto& x : tehnicieni)
            //x->afisare(std::cout);
   std::string linie;
   s.citireMarci(fin3);
while (std::getline(fin, linie)) {
    cerereR cr;
    citire(cr, linie);
    PQ cereri;
    s.verificareCerere(fout,cr,cereri);
    auto temp(std::move(cereri));
    while (!temp.empty())
    {
        temp.top().afisare(std::cout);
        temp.pop();
    }
    //cr.afisare(std::cout);
}

    return 0;
}

