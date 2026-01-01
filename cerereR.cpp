#include "cerereR.h"
#include "service.h"
#include<iomanip>
void citire(cerereR& cr,std::string linie,std::ostream& dev, bool optiune)
{
            std::stringstream ss(linie);
            if (linie.empty()) {
            if (optiune) dev << linie << "\n";
            throw std::invalid_argument("linie goala");
            }
            std::string id;
            std::getline(ss,id,',');
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
            std::string time;
            std::string extra;
            std::getline(ss,extra,',');
            std::getline(ss,time,',');
            std::string complexitate;
            std::getline(ss,complexitate,',');
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

            // Holds date-time components
            tm timeParts = {};
            timeParts.tm_isdst = -1;
            // Parse string
            strptime(time.c_str(), formatPattern, &timeParts);  

            // Convert to timestamp for easy use
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

void cerereR::afisare(std::ostream& dev)const
{
    dev << id << " ";

    if (!e) {
        dev << "[electrocasnic null]\n";
        return; // sau throw
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
        return; // sau throw
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
        //std::cout<<itMarca->first;
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
        //cr.afisare(dev);
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
        //std::cout<<mma.marca<<" "<<mma.model<<std::endl;
    
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