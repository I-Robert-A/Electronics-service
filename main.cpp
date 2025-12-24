#include <iostream>
#include <exception>
#include "iAngajat.h"

int main() {
    try {
        EmployeeData d{
            1, "Popa", "Iona", "5050603170065",
            "10/03/2031", "Cluj",
            {"TV"}, {"LG"}, {}
        };

        std::unique_ptr<iAngajat> ms = std::make_unique<Angajat_tehnician>();
        auto doc = ms->creeazaAngajat(d);

        std::cout << doc->calcS() << "\n";
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Eroare: " << e.what() << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Eroare generala: " << e.what() << "\n";
    }

    return 0;
}
