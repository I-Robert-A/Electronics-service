#include "cerereR.h"

void citire(cerereR& cr,std::string linie,std::ostream& dev)
{
            std::stringstream ss(linie);
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
                dev<<linie<<std::endl;
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

    dev << ctime(&timestamp) << " " << complexitate << "\n";
} 