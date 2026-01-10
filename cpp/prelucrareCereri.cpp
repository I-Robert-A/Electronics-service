#include "../headers/service.h"
#include <thread>
#include <chrono>
#include<fstream>
#include<algorithm>


void prelucrareCereri(PQ& cereri,std::ostream& fout2, std::vector<tehnician*>& teh, std::ostream& fout3)
{
    struct less
        {
            bool operator()(const cerereR& a, const cerereR& b)const
            {
                return a.getTime()<b.getTime();
            }
        };
    std::set<cerereR,less> cereriAsteptare;
    std::unordered_map<int, tehnician*> tehnicieniOcupati;
    while(!cereri.empty())
    {
        int ok=0;
        cerereR cr=std::move(cereri.top());

        std::string marca=cr.getMarca();
        std::string tip=cr.getTip();
        if(cr.getDurata()==0)
        {
            std::cout<<"cererea "<<cr.getID()<<" este ireparabila"<<std::endl;
            cr.afisareCSV(fout2);
        }
        else
        {
            for(auto& it: teh)
            {
                
                if(it->getStatus()<=2)
                {
                    auto itT=it->getRepara().find(tip);
                    if(itT!=it->getRepara().end())
                    {

                        auto itM=itT->second.find(marca);
                        if(itM!=itT->second.end())
                        {   
                            ok=1;
                            it->setpret(0.02*cr.getPret());
                            it->setIDC(cr.getID(),it->getStatus());
                            it->setdurataC(cr.getDurata(),it->getStatus());
                            it->addLucrare(cr.getDurata());
                            std::cout<<"tehnicianul cu ID "<<it->getID()<<" a primit cererea cu ID: "<<cr.getID()<<std::endl;
                            fout3<<cr.getDurata()<<",";
                            fout3<<it->getID()<<",";
                            cr.afisareCSV(fout3);
                            fout3<<std::endl;
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
        }
        cereri.pop();
    }
int timp=1;
std::vector<cerereR> cereriAsteptareAlfabet;
for (auto itC = cereriAsteptare.begin(); itC != cereriAsteptare.end(); ++itC)
{
        cereriAsteptareAlfabet.push_back(*itC);
}
    std::sort(cereriAsteptareAlfabet.begin(),cereriAsteptareAlfabet.end(),[](const cerereR& a, const cerereR& b)
{
    if(a.getTip()!=b.getTip())
    {
        return a.getTip()<b.getTip();
    }
    else
    {
        if(a.getMarca()!=b.getMarca())
        {
            return a.getMarca()<b.getMarca();
        }
        else
        {
            return a.getModel()<b.getModel();
        }
    }
});
std::ofstream foutAS("../outputs/cereriAsteptare.csv");
for (auto itC = cereriAsteptareAlfabet.begin(); itC != cereriAsteptareAlfabet.end(); ++itC)
{
    itC->afisareCSV(foutAS);
    foutAS<<std::endl;
}
foutAS.close();
while(!cereriAsteptare.empty() || !tehnicieniOcupati.empty())
{
    std::cout<<"timp: "<<timp<<std::endl;
    for(auto it = tehnicieniOcupati.begin(); it != tehnicieniOcupati.end();)
        {
            for(int i=0;i<=2;i++)
            {
                it->second->setdurataC(it->second->getdurataC(i)-1,i);
                if(it->second->getdurataC(i)==0 && it->second->getIDC(i)!=0)
                {
                    std::cout<<"tehnicianul "<<it->first<<" a terminat cererea "<<it->second->getIDC(i)<<std::endl;
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
                                        it->second->setpret(0.02*(itC->getPret()));
                                        it->second->setIDC(itC->getID(),i);
                                        it->second->setdurataC(itC->getDurata(),i);
                                        it->second->addLucrare(itC->getDurata());
                                        std::cout<<"tehnicianul "<<it->second->getID()<<"a primit cererea cu ID: "<<itC->getID()<<std::endl;
                                        fout3<<itC->getDurata()<<",";
                                        fout3<<it->second->getID()<<",";
                                        itC->afisareCSV(fout3);
                                        fout3<<std::endl;
                                        if(it->second->getStatus()==0)
                                        {
                                            tehnicieniOcupati[it->second->getID()]=it->second;
                                        }
                                        it->second->setStatus(1); 
                                        cereriAsteptare.erase(itC);
                                        auto it = std::find_if(cereriAsteptareAlfabet.begin(),cereriAsteptareAlfabet.end(),
                                                               [&](const cerereR& a) {
                                                return a.getID() == itC->getID();
                                            }
                                            );

                                        cereriAsteptareAlfabet.erase(it);
                                        std::ofstream foutAS("../outputs/cereriAsteptare.csv");
                                        for (auto itC = cereriAsteptareAlfabet.begin(); itC != cereriAsteptareAlfabet.end(); ++itC)
                                        {
                                            itC->afisareCSV(foutAS);
                                            foutAS<<std::endl;

                                        }
                                        foutAS.close();
                                        break;
                                }
                            }
                        }
                }
                else if(it->second->getdurataC(i)>0 && it->second->getIDC(i)!=0)
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
}