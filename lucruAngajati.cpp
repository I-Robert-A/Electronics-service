#include "lucruAngajati.h"
std::unique_ptr<angajat> angajare(EmployeeData d, std::string tip, std::vector<std::unique_ptr<angajat>>& angajati)
{
    try
    { if(tip=="tehnician")
        {if(std::stoi(d.CNP.substr(1,2))<=10 || std::stoi(d.CNP.substr(1,2))>26)
            {std::unique_ptr<iAngajat> ms = std::make_unique<Angajat_tehnician>();
            auto doc = ms->creeazaAngajat(d);
            angajati.push_back(std::move(doc));  
            }
        }
    else if(tip=="receptioner")
    {
        if(std::stoi(d.CNP.substr(1,2))<=10 || std::stoi(d.CNP.substr(1,2))>26)
            {std::unique_ptr<iAngajat> ms = std::make_unique<Angajat_receptioner>();
            auto doc = ms->creeazaAngajat(d);
            angajati.push_back(std::move(doc));  
            }
    }    
    else if(tip=="supervizor")
    {
        if(std::stoi(d.CNP.substr(1,2))<=10 || std::stoi(d.CNP.substr(1,2))>26)
            {std::unique_ptr<iAngajat> ms = std::make_unique<Angajat_supervizor>();
            auto doc = ms->creeazaAngajat(d);
            angajati.push_back(std::move(doc));  
            }
    }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return nullptr;
}

void concediere(int ID, std::vector<std::unique_ptr<angajat>>& angajati)
{
    auto it=std::find_if(angajati.begin(),angajati.end(),[ID](const std::unique_ptr<angajat>& a)
    {
        return a->getID()==ID;
    });
if (it != angajati.end()) {
    angajati.erase(it);   
}

    angajat::setID(-1);
    std::cout<<"sters";
}

void modificaNume(std::unique_ptr<angajat>& a,std::string NumeNou)
{
    a->setNume(NumeNou);
}