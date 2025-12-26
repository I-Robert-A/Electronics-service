#include "iAngajat.h"
#include<algorithm>
#pragma once
std::unique_ptr<angajat> angajare(EmployeeData d, std::string tip, std::vector<std::unique_ptr<angajat>>& angajati);


void concediere(int ID, std::vector<std::unique_ptr<angajat>>& angajati);

void modificaNume(std::unique_ptr<angajat>& a,std::string NumeNou);