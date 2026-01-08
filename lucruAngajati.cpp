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
            }
            else
            {
                throw std::invalid_argument("angajatul nu are varsta necesara");
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
            else
            {
                throw std::invalid_argument("angajatul nu are varsta necesara");
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
            else
            {
                throw std::invalid_argument("angajatul nu are varsta necesara");
            }


            }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

bool service::concediere(int ID)
{
    auto it=std::find_if(angajati.begin(),angajati.end(),[ID](const std::unique_ptr<angajat>& a)
    {
        return a->getID()==ID;
    });
    if (it == angajati.end()) 
    {
        std::cout << "nu exista angajatul asta" << std::endl;
        return false;
    }
    angajat* victim = it->get();

    auto rm = [victim](auto& vec){
        vec.erase(std::remove(vec.begin(), vec.end(), victim), vec.end());
    };

    rm(PtrTehnician);
    rm(PtrReceptioner);
    rm(PtrSupervizor);

    angajati.erase(it);
    return true;
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
    int index=0;
    std::string linie;
        while(std::getline(dev, linie))
        {
            index++;
            if(linie.empty()) continue;
            try
            {
            std::map<std::string, std::set<std::string>> Repara;
            d.lucrari = 0;
            d.pretR = 0.0;
            std::stringstream ss(linie);
            std::string post, nume,prenume,cnp,time,oras;
            if(!std::getline(ss,post,',') || !std::getline(ss,nume,',') || !std::getline(ss,prenume,',')
            || !std::getline(ss,cnp,',') || !std::getline(ss,time,',') || !std::getline(ss,oras,','))
            {
                throw std::runtime_error("Format linie invalid: coloane lipsa pe liniaA: "+std::to_string(index));
            }
            std::string reparatii;
            std::getline(ss,reparatii,',');  
            std::string lucrari;
            std::getline(ss,lucrari,',');   
            std::string pretR;
            std::getline(ss,pretR,',');    
            std::string tipMarca;
            std::string marca, tip;
            if (post != "tehnician" && post != "receptioner" && post != "supervizor") {
                throw std::runtime_error("Post necunoscut: " + post);
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
            
                int lucrariI=0;
                double PretR=0.0;
            if(post=="tehnician")
            {
                if(!reparatii.empty()){
                    std::stringstream ss2(reparatii);
                    while(std::getline(ss2,tipMarca,'|'))
                    {
                        std::stringstream ss3(tipMarca);
                        std::getline(ss3,tip,';');
                        std::getline(ss3,marca,';');
                        Repara[tip].insert(marca);
                    }
                    if (tip.empty() || marca.empty()) {
                        std::cerr << "nu e tip sau marca"<< "\n";
                        continue; 
                        }
                }
                if(!lucrari.empty())
                    lucrariI=std::stoi(lucrari);
                if(!pretR.empty())
                    PretR=std::stod(pretR);
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
            d.pretR=PretR;
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
            catch(const std::exception& e)
            {
                std::cerr << e.what()<<"pe liniaA: "<<index << '\n';
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