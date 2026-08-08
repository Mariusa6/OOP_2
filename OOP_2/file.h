#ifndef FILE_H_DEFINED
#define FILE_H_DEFINED

#include "main.h"
#undef max
#include <iostream> // cout, cin
#include <fstream>  // ifstream, ofstream
#include <sstream>  // istringstream
#include <vector>   // vector
#include <string>   // string
#include <iomanip>  // setprecision, setw
#include <limits>   // numeric_limits
#include <stdexcept>// runtime_error
#include <chrono>  // high_resolution_clock

// -------------------------------------------------------
// Template: readStudentaiFromFile<Container>
// Nuskaito studentus iš .txt failo į bet kokį konteinerį
// (vector, list, deque) naudojant push_back.
// Failo formatas: antraštė su ND1..NDn ir Egzaminas stulpeliais.
// -------------------------------------------------------
template<typename Container>
Container readStudentaiFromFile(const std::string& filename)
{
    Container studentai;
    std::ifstream file(filename);

    if (!file.is_open())
        throw std::runtime_error("Nepavyko atidaryti failo: " + filename);

    std::string line;

    if (!std::getline(file, line))
        throw std::runtime_error("Failas tuščias arba sugadintas: " + filename);

    // Apskaičiuojame ND stulpelių skaičių iš antraštės
    std::istringstream headerStream(line);
    std::string token;
    int ndCount = 0;
    while (headerStream >> token)
        if (token.find("ND") != std::string::npos) ndCount++;

    int lineNumber = 1;
    double tStream = 0, tParse = 0, tPush = 0;

    while (std::getline(file, line)) {
        lineNumber++;
        if (line.empty()) continue;

        std::istringstream ss(line);
        studentas s;
        s.readStudentas(ss, ndCount, lineNumber);
        studentai.push_back(std::move(s));
    }

    return studentai;
}

// -------------------------------------------------------
// Template: writeStudentaiToFile
// Išveda studentus į .txt failą su galutiniais balais.
// Formatavimą atlieka klasės operator<<.
// -------------------------------------------------------
template<typename Container>
void writeStudentaiToFile(const Container& studentai, const std::string& filename)
{
    std::ofstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Nepavyko sukurti failo: " + filename);

    file << u8"Vardas              Pavardė             Galutinis (Vid.)    Galutinis (Med.)\n";
    file << u8"----------------------------------------------------------------------------\n";

    for (const auto& s : studentai)
        file << s << "\n";

    file.close();
}

// -------------------------------------------------------
// Template: writeStudentaiListToFile
// Išveda studentus į .txt failą su visais namų darbų pažymiais.
// Šis formatas suderinamas su readStudentaiFromFile skaitymui.
// -------------------------------------------------------
template<typename Container>
void writeStudentaiListToFile(const Container& studentai, const std::string& filename)
{
    std::ofstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Nepavyko sukurti failo: " + filename);

    file << std::left << std::setw(25) << u8"Vardas"
        << std::left << std::setw(25) << u8"Pavardė";

    for (int i = 1; i <= NUMBER_OF_PAZYMYS; ++i)
    {
        file << std::right << std::setw(10) << ("ND" + std::to_string(i));
    }

    file << std::right << std::setw(10) << u8"Egzaminas" << "\n";

    for (const auto& s : studentai)
    {
        file << std::left << std::setw(25) << s.vardas()
            << std::left << std::setw(25) << s.pavarde();

        for (const auto& nd : s.nd())
        {
            file << std::right << std::setw(10) << nd;
        }

        file << std::right << std::setw(10) << s.egzaminas() << "\n";
    }
    file.close();
}

#endif // FILE_H_DEFINED