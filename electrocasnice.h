#include<iostream>
#include<string>
#include<vector>
#pragma once

class electrocasnice
{
    protected:
    std::string marca;
    std::string model;
    int an;
    double pret;
    public:
    electrocasnice(std::string Marca, std::string Model, int An, double Pret):marca{Marca}, model{Model}, an{An}, pret{Pret}{}
    virtual void afisare(std::ostream& dev)const{dev<<marca<<" "<<model<<" "<<an<<" "<<pret;}
    virtual ~electrocasnice();                                                                    
};