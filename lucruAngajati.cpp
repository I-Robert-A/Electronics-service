#include "service.h"
#include <sstream>
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

void service::afisareAngajati(std::ostream& dev)const
{
    for(auto& i: angajati)
    {
        (*i).afisare(dev);
    }
}
void service::afisareAngajatiCSV(std::ostream& dev)const
{
    for(auto& i: angajati)
    {
        (*i).afisareCSV(dev);
    }
}

void citireAngajat(std::istream& dev,service& s)
{
    EmployeeData d;
    std::string linie;
        while(std::getline(dev, linie))
        {
            std::map<std::string, std::set<std::string>> Repara;
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
            std::string extra;
            std::getline(ss,extra,',');       
            std::stringstream ss2(reparatii);
            std::string tipMarca;
            std::string marca, tip;
            while(std::getline(ss2,tipMarca,'|'))
            {
                std::stringstream ss3(tipMarca);
                std::getline(ss3,tip,';');
                std::getline(ss3,marca,';');
                Repara[tip].insert(marca);
            }
            std::vector <int>idI;
            if(post=="receptioner")
            {
                if(!reparatii.empty())
                {std::stringstream ss4(reparatii);
                    std::string id;
                    while(std::getline(ss4,id,';'))
                    {
                        idI.push_back(std::stoi(id));
                    }
                }
            }
            
                int lucrariI;
            if(post=="tehnician")
            {
                if(!extra.empty())
                    lucrariI=std::stoi(extra);
            }
            d.Post=post;
            d.Nume=nume;
            d.Prenume=prenume;
            d.CNP=cnp;
            d.data_A=time;
            d.IDuri=idI;
            d.lucrari=lucrariI;
            d.oras_D=oras;
            d.repara=Repara;
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
}

void service::modificaNume(int id, const std::string& numeNou)
{
    for (auto& a : angajati) {
        if (a && a->getID() == id) {
            a->setNume(numeNou);
            return;
        }
    }
    throw std::runtime_error("Nu exista angajat cu ID-ul dat.");
}