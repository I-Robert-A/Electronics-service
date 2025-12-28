#include <iostream>
#include <exception>
#include "lucruAngajati.h"
#include "service.h"
#include<fstream>
#include<sstream>
#include "frigider.h"
#include "TV.h"
#include "masinaSpalat.h"

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
        std::string linie;
        service s;

        EmployeeData d;
        std::map<std::string, std::set<std::string>> Repara;
        while(std::getline(fin2, linie))
        {
            std::stringstream ss(linie);
            std::string post;
            std::getline(ss,post,',');
            std::string nume;
            std::getline(ss,nume,',');
            std::string prenume;
            std::getline(ss,prenume,',');
            std::string cnp;
            std::getline(ss,cnp,',');
            std::string time;
            std::getline(ss,time,',');
            std::string oras;
            std::getline(ss,oras,',');
            std::string reparatii;
            std::getline(ss,reparatii,',');
            std::string Lucrari;
            std::getline(ss,Lucrari,',');
            std::string iduri;
            std::getline(ss,iduri,','); 
                   
            std::stringstream ss2(reparatii);
            std::string tipMarca;
            std::string marca, tip;
            while(std::getline(ss2,tipMarca,' '))
            {
                std::stringstream ss3(tipMarca);
                std::getline(ss3,tip,';');
                std::getline(ss3,marca,';');
                Repara[tip].insert(marca);
            }
            std::stringstream ss4(iduri);
            std::string id;
            std::vector <int>idI;
            while(std::getline(ss4,id,';'))
            {
                idI.push_back(std::stoi(id));
            }
            int lucrariI;
            if(!(Lucrari.empty()))
                lucrariI=std::stoi(Lucrari);
            d.Post=post;
            d.Nume=nume;
            d.Prenume=prenume;
            d.CNP=cnp;
            d.data_A=time;
            d.IDuri=idI;
            d.lucrari=lucrariI;
            d.oras_D=oras;
            d.repara=Repara;
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
            if(post=="tehnician")
            {
                s.angajareTehnician(d);
            }
            else if(post=="receptioner")
            {
                s.angajareReceptioner(d);
            }
            else
            {
                s.angajareSupervizor(d);
            }
        }    
        s.concediere(2);
        s.afisareAngajati();
        auto tehnicieni=s.getPtrteh();
        std::sort(tehnicieni.begin(), tehnicieni.end(),[](tehnician* a, tehnician*b)
        {
            return a->getLucrare()<b->getLucrare();
        });
        for (auto& x : tehnicieni)
            x->afisare(std::cout);
        /*while(std::getline(fin, linie))
        {
            std::stringstream ss(linie);
            std::string id;
            std::getline(ss,id,',');
            std::string marca;
            std::getline(ss,marca,',');
            std::string model;
            std::getline(ss,model,',');
            std::string an;
            std::getline(ss,an,',');
            std::string pret;
            std::getline(ss,pret,',');
            std::string time;
            std::getline(ss,time,',');
            std::string complexitate;
            std::getline(ss,complexitate,',');
            int anul=std::stoi(an);
            double pretul=std::stod(pret);   
            std::unique_ptr<electrocasnice> e=std::make_unique<electrocasnice>(marca,model,anul,pretul); 
            std::cout<<complexitate;       
            const char* formatPattern = "%d/%m/%Y %H:%M:%S";  

            // Holds date-time components
            tm timeParts = {};
            timeParts.tm_isdst = -1;
            // Parse string
            strptime(time.c_str(), formatPattern, &timeParts);  

            // Convert to timestamp for easy use
            time_t timestamp = mktime(&timeParts);
            std::cout<<id<<" "<<marca<<" "<<model<<" "<<an<<" "<<pret<<" "<<ctime(&timestamp)<<" "<<complexitate<<std::endl;  
            
        }*/
    return 0;
}

