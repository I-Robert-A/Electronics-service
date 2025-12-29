#include "service.h"
void service::adaugareMarca(
    const std::string& tip,
    const std::string& marca,
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>>& posReparatii
)
{
    auto itTip = posReparatii.find(tip);
    if (itTip == posReparatii.end()) {
       throw std::invalid_argument("tip nonexistent");
    }
    else
    {
        auto& marci=itTip->second;
        if (marci.find(marca) == marci.end()) {
        marci[marca] = std::vector<std::string>{};
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
        //std::cout<<itTip->first;
       throw std::invalid_argument("tip nonexistent");
    }
    else
    {
         auto& marci = itTip->second;
        auto& modele = marci[marca]; 

    if (std::find(modele.begin(), modele.end(), model) != modele.end()) {
        throw std::invalid_argument("model deja existent");
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
       throw std::invalid_argument("tip nonexistent");
    }
    else
    {
        auto itMarca=itTip->second.find(marca);
        if(itMarca==itTip->second.end())
        {
            throw std::invalid_argument("marca nonexistent");
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
       throw std::invalid_argument("tip nonexistent");
    }
    else
    {
        auto itMarca=itTip->second.find(marca);
        auto& marci=itTip->second;
        if(itMarca==itTip->second.end())
        {
            throw std::invalid_argument("marca nonexistent");
        }
        else{
            auto& modele = itMarca->second;
            auto itModel = std::find(modele.begin(), modele.end(), model);
            if (itModel == modele.end()) {
                throw std::invalid_argument("model nonexistent");
            }
            else
                modele.erase(itModel);
        }
    }
}

void service::verificareCerere(std::ostream& dev,cerereR& cr, PQ& cereri)
{
    std::string marca=cr.getMarca();
    std::string tip=cr.getTip();
    int ok=1;
    auto itTip = posReparatii.find(tip);
    if(itTip!=posReparatii.end())
    {
        auto itMarca=itTip->second.find(marca);
        if(itMarca!=itTip->second.end())
        {
            ok=0;
        }
    }
    if(ok==1)
    {
        cr.afisare(dev);
    }
    else
    {
        cereri.push(std::move(cr));
    }
}
void service::citireMarci(std::istream& dev)
{
    std::string linie;
    while(std::getline(dev,linie))
    {
        std::string tip;
        std::string marca;
        std::string modele;
        std::stringstream ss(linie);
        std::getline(ss,tip,',');
        std::getline(ss,marca,',');
        std::getline(ss,modele,',');
        std::stringstream ss2(modele);
        std::string model;
        while(std::getline(ss2,model,';'))
        {
            adaugareModel(tip,marca,model,posReparatii);
        } 
    }
}