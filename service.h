#include "lucruAngajati.h"
#include "lucruElectrocasnice.h"
class service{
    private:
    std::vector<std::unique_ptr<angajat>> angajati;
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>> posReparatii;
    static service *instance;

    public:
    service()=default;
    static service* getInstance(){
        if(!instance)
            instance=new service();
        return instance;
    }
    service(const service&)=delete;
    service& operator=(const service&)=delete;
};