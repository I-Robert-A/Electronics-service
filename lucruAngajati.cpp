#include "service.h"

void service::angajareTehnician(const EmployeeData& d)
{
    try
    {
        if(std::stoi(d.CNP.substr(1,2))<=10 || std::stoi(d.CNP.substr(1,2))>26)
            {
                std::unique_ptr<iAngajat> ms = std::make_unique<Angajat_tehnician>();
                auto doc = ms->creeazaAngajat(d);
                auto* raw = dynamic_cast<tehnician*>(doc.get());
                angajati.push_back(std::move(doc)); 
                if (!raw) 
                {
                throw std::runtime_error("Angajatul creat nu este tehnician");
                }

                PtrTehnician.push_back(raw);
                //raw->afisare(std::cout);
            }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}
void service::angajareReceptioner(const EmployeeData &d)
{
    try
    {
        if(std::stoi(d.CNP.substr(1,2))<=10 || std::stoi(d.CNP.substr(1,2))>26)
            {
                std::unique_ptr<iAngajat> ms = std::make_unique<Angajat_receptioner>();
                auto doc = ms->creeazaAngajat(d);
                auto* raw = dynamic_cast<receptioner*>(doc.get());
                angajati.push_back(std::move(doc)); 
                if (!raw) 
                {
                throw std::runtime_error("Angajatul creat nu este receptioner");
                }

                PtrReceptioner.push_back(raw);
           

            }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}
void service::angajareSupervizor(const EmployeeData& d)
{
    try
    {
        if(std::stoi(d.CNP.substr(1,2))<=10 || std::stoi(d.CNP.substr(1,2))>26)
            {
                std::unique_ptr<iAngajat> ms = std::make_unique<Angajat_supervizor>();
                auto doc = ms->creeazaAngajat(d);
                auto* raw = dynamic_cast<supervizor*>(doc.get());
                angajati.push_back(std::move(doc)); 
                if (!raw) 
                {
                throw std::runtime_error("Angajatul creat nu este supervizor");
                }

                PtrSupervizor.push_back(raw);
           

            }           

            }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

void service::concediere(int ID)
{
    auto it=std::find_if(angajati.begin(),angajati.end(),[ID](const std::unique_ptr<angajat>& a)
    {
        return a->getID()==ID;
    });
    angajat* victim = it->get();

    // scoate din listele de pointeri (dacă e acolo)
    auto rm = [victim](auto& vec){
        vec.erase(std::remove(vec.begin(), vec.end(), victim), vec.end());
    };

    rm(PtrTehnician);
    rm(PtrReceptioner);
    rm(PtrSupervizor);

    // acum îl scoți din evidența principală -> obiectul se distruge aici
    angajati.erase(it);

}

void service::modificaNume(std::unique_ptr<angajat>& a,std::string NumeNou)
{
    a->setNume(NumeNou);
}

void service::afisareAngajati()const
{
    for(auto& i: angajati)
    {
        if((*i).getStatus())
            (*i).afisare(std::cout);
    }
}