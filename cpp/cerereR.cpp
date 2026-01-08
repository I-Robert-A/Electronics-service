#include "../headers/cerereR.h"
#include "../headers/service.h"
#include<iomanip>
void citire(cerereR& cr,std::string linie,std::ostream& dev, bool optiune)
{
            std::stringstream ss(linie);
            if (linie.empty()) {
            throw std::invalid_argument("linie goala");
            }
            std::string id,tip,marca,model,an,pret,time,extra,complexitate;
            if(!std::getline(ss,id,',') || !std::getline(ss,tip,',') || !std::getline(ss,marca,',') || !std::getline(ss,model,',')
            || !std::getline(ss,an,',') || !std::getline(ss,pret,',') || !std::getline(ss,extra,',') || !std::getline(ss,time,',')
            || !std::getline(ss,complexitate,','))
            {
                std::cout << "Eroare format: Numar insuficient de coloane in liniaC: " << id << "\n";
                return;
            }
            try
            {
                int anul=std::stoi(an);
            double pretul=std::stod(pret);   
            int ID=std::stoi(id);
            int Complexitate=std::stoi(complexitate);
            int Extra;
            ElectronicData ed;
            ed.an=anul;
            ed.marca=marca;
            ed.model=model;
            ed.pret=pretul;
            ed.tip=tip;
            const char* formatPattern = "%d/%m/%Y %H:%M:%S";  

            tm timeParts = {};
            timeParts.tm_isdst = -1;
            char* result = strptime(time.c_str(), formatPattern, &timeParts);
            if (result == nullptr || *result != '\0') {
             throw std::runtime_error("Format data invalid (" + time + ")");
            }  

            time_t timestamp = mktime(&timeParts);
            if(tip=="frigider")
            {
                std::unique_ptr<icreareElectrocasnice> ms = std::make_unique<Electrocasnic_frigider>();
                ed.congelator=std::stoi(extra);
                auto doc = ms->creeazaElectrocasnic(ed);
                cr = cerereR(std::move(doc), timestamp, Complexitate,ID);
            }
            else if(tip=="TV")
            {
                std::unique_ptr<icreareElectrocasnice> ms = std::make_unique<Electrocasnic_TV>();
                ed.Diagonala=std::stod(extra);
                auto doc = ms->creeazaElectrocasnic(ed);
                cr = cerereR(std::move(doc), timestamp, Complexitate,ID);
            }
            else if(tip=="masina de spalat")
            {
                std::unique_ptr<icreareElectrocasnice> ms = std::make_unique<Electrocasnic_MasSpalat>();
                ed.greutate=std::stod(extra);
                auto doc = ms->creeazaElectrocasnic(ed);
                cr = cerereR(std::move(doc), timestamp, Complexitate,ID);
            }
            else
            {
                if(optiune==true)
                { 
                    dev<<linie<<std::endl;
                    throw std::invalid_argument("tip electrocasnic nesuportat: " + tip);
                }
                else
                {
                    std::unique_ptr<icreareElectrocasnice> ms = std::make_unique<Electrocasnic_necunoscut>();
                    ed.ceva=std::stod(extra);
                    auto doc = ms->creeazaElectrocasnic(ed);
                    cr = cerereR(std::move(doc), timestamp, Complexitate,ID);
                }
            }
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() <<" pe liniaC"<<id<< '\n';
                return;
            }
        }

void cerereR::afisare(std::ostream& dev)const
{
    dev << id << " ";

    if (!e) {
        dev << "[electrocasnic null]\n";
        return; 
    }

    e->afisare(dev);

    
    std::string t = ctime(&timestamp);
    std::tm tm = *std::localtime(&timestamp);
    dev << std::put_time(&tm, "%d/%m/%Y %H:%M:%S")
        << "," << complexitate;
} 

void cerereR::afisareCSV(std::ostream& dev)const
{
    dev << id << ",";

    if (!e) {
        dev << "[electrocasnic null]\n";
        return; 
    }

    e->afisareCSV(dev);

    
    std::string t = ctime(&timestamp);
    std::tm tm = *std::localtime(&timestamp);
    dev << std::put_time(&tm, "%d/%m/%Y %H:%M:%S")
        << "," << complexitate;
}

void service::verificareCerere(std::ostream& dev,cerereR& cr, PQ& cereri)
{
    try
    {   
        std::string marca=cr.getMarca();
        std::string tip=cr.getTip();
    int ok=0;
    auto itTip = posReparatii.find(tip);
    if(itTip!=posReparatii.end())
    {
        auto itMarca=itTip->second.find(marca);
        if(itMarca!=itTip->second.end())
        {
            ok=1;
        }
    }
    
    if(ok==0)
    {
        cr.afisareCSV(dev);
        dev<<std::endl;
    }
    else
    {
        cereri.push(std::move(cr));
    }
    }
    catch(const std::exception& e)
    {
        std::cout<<cr.getID();
        std::cerr << e.what() << '\n';
    }
    
    
}

void afiseazaInvalide(std::istream& dev,std::ostream& devO)
{
    struct MMA
    {
        std::string marca;
        std::string model;
        int aparitii=1;
    }; 
    std::string linie;
    std::vector<MMA> marciModele;
    while(std::getline(dev,linie))
    {        
        cerereR cr;
        MMA mma;
        try
        {
            citire(cr,linie,devO,false);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        
        try
        {
        mma.marca=cr.getMarca();
        mma.model=cr.getModel();        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    
    auto it=std::find_if(marciModele.begin(),marciModele.end(),[&](const MMA& x)
{
    return (x.marca==mma.marca && x.model==mma.model);
});
    if(it!=marciModele.end())
    {
        (*it).aparitii++;
    }
    else
    {
        marciModele.push_back(mma);
    }
}
    std::sort(marciModele.begin(),marciModele.end(),[](const MMA& a, const MMA& b)
{
    return a.aparitii>b.aparitii;
});
for (const auto& x : marciModele) {
        devO << x.marca << "," << x.model << "," << x.aparitii << "\n";
    }
}