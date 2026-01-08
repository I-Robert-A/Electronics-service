#include "electrocasnice.h"
#include<unordered_map>
#include<algorithm>
#pragma once
#pragma once
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