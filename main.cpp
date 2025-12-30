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

service* service::instance=nullptr;

int main() {

        std::ifstream fin("cereri.csv");
        std::ifstream fin2("angajati.csv");
        std::ofstream fout("cereriInvalide.csv"); 
        std::ifstream fin3("marci.csv");
        std::string linie;
        service s;
        citireAngajat(fin2,s);
        s.citireMarci(fin3);
        s.afisareAngajati();
        struct less
        {
            bool operator()(const cerereR& a, const cerereR& b)const
            {
                return a.getTime()<b.getTime();
            }
        };
        std::set<cerereR,less> cereriAsteptare;
        EmployeeData d; 
        std::unordered_map<int, tehnician*> tehnicieniOcupati;
        auto teh=s.getPtrteh();
        auto rec=s.getPtrrec();
        std::sort(teh.begin(), teh.end(),[](tehnician* a, tehnician*b)
        {
            return a->getLucrare()<b->getLucrare();
        });
       // for (auto& x : teh)
         //   x->afisare(std::cout);
        PQ cereri;
while (std::getline(fin, linie)) {
    cerereR cr;
    if(!linie.empty())
    {
        citire(cr, linie,fout);
        if(rec.size()>1)
            {
                std::mt19937 gen(std::random_device{}());
                std::uniform_int_distribution<> dist(0, rec.size() - 1);
                receptioner* r = rec[dist(gen)];
                r->addID(cr.getID());
            }
        else {
        if (!rec.empty()) {
            (*rec.begin())->addID(cr.getID());
        } else {
            std::cerr << "Eroare: rec este gol, nu pot face addID\n";
        }
    }
        s.verificareCerere(fout,cr,cereri);
    }
}
int ok=0;
while(!cereri.empty())
{
    ok=0;
    cerereR cr=std::move(cereri.top());

    std::string marca=cr.getMarca();
    std::string tip=cr.getTip();
    for(auto& it: teh)
    {
        
        //it->afisare(std::cout);
        //std::cout<<it->getStatus()<<" "<<it->getID()<<" ";
        if(it->getStatus()<2)
        {
            auto itT=it->getRepara().find(tip);
            //std::cout<<it->getRepara().begin()->first<<" "<<tip<<std::endl;
            if(itT!=it->getRepara().end())
            {

                auto itM=itT->second.find(marca);
                if(itM!=itT->second.end())
                {   
                    ok=1;
                    it->setpret(0.05*cr.getPret());
                    it->setIDC(cr.getID(),it->getStatus());
                    it->setdurataC(cr.getDurata(),it->getStatus());
                    std::cout<<"tehnicianul cu ID "<<it->getID()<<" a primit cererea cu ID: "<<cr.getID()<<std::endl;
                    if(it->getStatus()==0)
                    {
                        tehnicieniOcupati[it->getID()]=it;
                    }
                    it->setStatus(1); 
                    std::sort(teh.begin(), teh.end(),[](tehnician* a, tehnician*b)
                    {
                            return a->getLucrare()<b->getLucrare();
                    }); 
                    break;
                }
            }
        }
    }
    if(ok==0)
    {
        cereriAsteptare.insert(cr);
    }
    cereri.pop();
}
int timp=1;
while(!cereriAsteptare.empty() || !tehnicieniOcupati.empty())
{
    std::cout<<"timp: "<<timp<<std::endl;
    for(auto it = tehnicieniOcupati.begin(); it != tehnicieniOcupati.end();)
        {
            for(int i=0;i<2;i++)
            {
                it->second->setdurataC(it->second->getdurataC(i)-1,i);
                if(it->second->getdurataC(i)==0 && it->second->getIDC(i)!=0)
                {
                    std::cout<<"tehnicianul "<<it->first<<"a terminat cererea "<<it->second->getIDC(i)<<std::endl;
                    it->second->setIDC(0,i);
                    it->second->setStatus(-1);
                    for (auto itC = cereriAsteptare.begin(); itC != cereriAsteptare.end(); ++itC)
                        {
                            auto itT=it->second->getRepara().find(itC->getTip());
                            if(itT!=it->second->getRepara().end())
                            {
                                auto itM=itT->second.find(itC->getMarca());
                                if(itM!=itT->second.end())
                                {
                                        it->second->setpret(0.05*(itC->getPret()));
                                        it->second->setIDC(itC->getID(),i);
                                        it->second->setdurataC(itC->getDurata(),i);
                                        std::cout<<"tehnicianul "<<it->second->getID()<<"a primit cererea cu ID: "<<itC->getID()<<std::endl;
                                        if(it->second->getStatus()==0)
                                        {
                                            tehnicieniOcupati[it->second->getID()]=it->second;
                                        }
                                        it->second->setStatus(1); 
                                        cereriAsteptare.erase(itC);
                                        break;
                                }
                            }
                        }
                }
                else if(it->second->getdurataC(i)!=0 && it->second->getIDC(i)!=0)
                {
                    std::cout<<"tehnicianul "<<it->first<<" proceseaza cererea "<<it->second->getIDC(i)<<
                    " si mai are "<<it->second->getdurataC(i)<<" unitati de timp"<<std::endl;
                }
            }
            if (it->second->getStatus() == 0)
                it = tehnicieniOcupati.erase(it);
            else
                ++it;
        }
        std::cout<<"cereri asteptare: ";
        for (auto itC = cereriAsteptare.begin(); itC != cereriAsteptare.end(); ++itC)
        {
            std::cout<<itC->getID()<<" ";
        }
        std::cout<<std::endl;
timp++;
 std::this_thread::sleep_for(std::chrono::seconds(1));
}

    return 0;
}

