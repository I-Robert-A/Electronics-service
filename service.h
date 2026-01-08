#include "lucruAngajati.h"
#include "lucruElectrocasnice.h"
#include <queue>
#include "cerereR.h"
#pragma once

class service{
    private:
    std::vector<std::unique_ptr<angajat>> angajati;
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>> posReparatii;
    struct Cmp {
    bool operator()(const tehnician* a, const tehnician* b) const {
        return a->getLucrare() > b->getLucrare();
    }
};

    std::vector<tehnician*> PtrTehnician;
    std::vector<supervizor*> PtrSupervizor;
    std::vector<receptioner*> PtrReceptioner;
    static service *instance;

    public:
    service()
{
    posReparatii.emplace("TV",
        std::unordered_map<std::string, std::vector<std::string>>{});
    posReparatii.emplace("frigider",
        std::unordered_map<std::string, std::vector<std::string>>{});
    posReparatii.emplace("masina de spalat",
        std::unordered_map<std::string, std::vector<std::string>>{});
}

    static service* getInstance(){
        if(!instance)
            instance=new service();
        return instance;
    }
    service(const service&)=delete;
    service& operator=(const service&)=delete;
    void init(std::vector<std::unique_ptr<angajat>> &Angajati,
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>> &PosReparatii)
    {
        angajati = std::move(Angajati);
        posReparatii = std::move(PosReparatii);
    }

    void adaugareMarca(
    const std::string& tip,
    const std::string& marca,
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>>& posReparatii
) ;

void adaugareModel(
    const std::string& tip,
    const std::string& marca,
    const std::string& model,
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>>& posReparatii
);

  void stergereMarca(
    const std::string& tip,
    const std::string& marca,
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>>& posReparatii
) ;
void stergereModel(
    const std::string& tip,
    const std::string& marca,
    const std::string& model,
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>>& posReparatii
);
void angajareTehnician(const EmployeeData& d);
void angajareReceptioner(const EmployeeData& d);
void angajareSupervizor(const EmployeeData& d);


bool concediere(int ID);

void modificaNume(std::unique_ptr<angajat>& a,std::string NumeNou);

void afisareAngajati(std::ostream& dev)const;
void afisareAngajatiCSV(std::ostream& dev)const;


auto getPtrteh(){return PtrTehnician;}
auto getPtrrec(){return PtrReceptioner;}
auto getPtrsup(){return PtrSupervizor;}
const std::vector<std::unique_ptr<angajat>>& getAngajati() const { return angajati; }
void verificareCerere(std::ostream& dev,cerereR& cr,PQ&);
void citireMarci(std::istream& dev);
auto& getPosReparatii() { return posReparatii; }
const auto& getPosReparatii() const { return posReparatii; }
void modificaNume(int id, const std::string& numeNou);
void inregistrareMM(std::ostream& dev);
};
void citireAngajat(std::istream& dev,service& s);
void prelucrareCereri(PQ& cereri,std::ostream& fout2, std::vector<tehnician*>& teh, std::ostream& fout3);

