#include "../headers/service.h"
void service::adaugareMarca(
    const std::string& tip,
    const std::string& marca,
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>>& posReparatii
)
{
    auto itTip = posReparatii.find(tip);
    if (itTip == posReparatii.end()) {
       throw std::invalid_argument("tip nonexistent "+ tip);
    }
    else
    {
        auto& marci=itTip->second;
        if (marci.find(marca) == marci.end()) {
        marci[marca] = std::vector<std::string>{};
    }
    else
    {
        throw std::invalid_argument("marca deja existenta " + marca);
    }
    }

}

void service::adaugareModel(
    const std::string& tip,
    const std::string& marca,
    const std::string& model,
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>>& posReparatii
)
{
    auto itTip = posReparatii.find(tip);
    if (itTip == posReparatii.end()) {
       throw std::invalid_argument("tip nonexistent "+ tip);
    }
    else
    {
        auto& marci = itTip->second;
        auto& modele = marci[marca]; 

    if (std::find(modele.begin(), modele.end(), model) != modele.end()) {
        std::cout<<model<<" ";
        throw std::invalid_argument("model deja existent "+ model);
    }

    modele.push_back(model);
}
    }


  void service::stergereMarca(
    const std::string& tip,
    const std::string& marca,
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>>& posReparatii
) 
{
     auto itTip = posReparatii.find(tip);
    if (itTip == posReparatii.end()) {
       throw std::invalid_argument("tip nonexistent "+ tip);
    }
    else
    {
        auto itMarca=itTip->second.find(marca);
        if(itMarca==itTip->second.end())
        {
            throw std::invalid_argument("marca nonexistent "+ marca);
        }
        else{itTip->second.erase(marca);}
    }
}

void service::stergereModel(
    const std::string& tip,
    const std::string& marca,
    const std::string& model,
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>>& posReparatii
)
{
auto itTip = posReparatii.find(tip);
    if (itTip == posReparatii.end()) {
       throw std::invalid_argument("tip nonexistent "+ tip);
    }
    else
    {
        auto itMarca=itTip->second.find(marca);
        auto& marci=itTip->second;
        if(itMarca==itTip->second.end())
        {
            throw std::invalid_argument("marca nonexistent "+ marca);
        }
        else{
            auto& modele = itMarca->second;
            auto itModel = std::find(modele.begin(), modele.end(), model);
            if (itModel == modele.end()) {
                throw std::invalid_argument("model nonexistent "+ model);
            }
            else
                modele.erase(itModel);
        }
    }
}


void service::citireMarci(std::istream& dev)
{
    std::string linie;
    int index=0;
    while(std::getline(dev,linie))
    {
        index++;
        std::string tip,marca,modele;
        std::stringstream ss(linie);
        if(linie.empty()){continue;}
        if(!std::getline(ss,tip,',') || !std::getline(ss,marca,','))
        {
            throw std::invalid_argument("nu sunt toate coloanele completate pe liniaM "+ std::to_string(index));
        }
        std::getline(ss,modele,',');
        std::stringstream ss2(modele);
        std::string model;
        if(!modele.empty())
        {    
            while(std::getline(ss2,model,';'))
            {
                adaugareModel(tip,marca,model,posReparatii);
            }
        }
        else
        {
            adaugareMarca(tip,marca,posReparatii);
        } 
    }
}

void afisareReparate(std::ostream& dev,std::istream& devI)
{
    std::string linie;
    while(std::getline(devI,linie))
    {
        std::stringstream ss(linie);
        std::string ID;
        std::getline(ss,ID,',');
        std::getline(ss,ID,',');
        std::getline(ss,ID,',');
        std::string tip;
        std::getline(ss,tip,',');
        std::string marca;
        std::getline(ss,marca,',');
        std::string model;
        std::getline(ss,model,',');
        std::string an;
        std::getline(ss,an,',');
        std::string pret;
        std::getline(ss,pret,',');
        std::string extra;
        std::getline(ss,extra,',');
        dev<<ID<<" "<<tip<<" "<<marca<<" "<<model<<" "<<an<<" "<<pret<<" "<<extra<<std::endl;
    }
}

void service::inregistrareMM(std::ostream& dev)
{
    for(auto& it:posReparatii)
    {
        auto& itMarci=it.second;
        for(auto& itmarci:itMarci)
        {
            dev<<it.first<<","<<itmarci.first<<",";
            for(auto itModele=itmarci.second.begin();itModele!=itmarci.second.end();++itModele)
            {
                dev<<*itModele;
                if(std::next(itModele)!=itmarci.second.end())
                {
                    dev<<";";
                }
            }
            dev<<std::endl;
        }
    }
}