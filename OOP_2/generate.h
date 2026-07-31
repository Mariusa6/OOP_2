#ifndef GENERATE_H_DEFINED
#define GENERATE_H_DEFINED

#include "main.h"
#include <vector>   // vector
#include <string>   // string
#include <random>   // mt19937, random_device

// -------------------------------------------------------
// Template: generateStudentai
// Generuoja studentus su atsitiktiniais vardais, pavardėmis ir pažymiais.
// -------------------------------------------------------
template<typename Container>
Container generateStudentai(int n)
{
    Container studentai;
    for (int i = 0; i < n; i++)
    {
        studentas s;
        s.vardas = "VardasNR" + std::to_string(i + 1);
        s.pavarde = "PavardeNR" + std::to_string(i + 1);
        int m = NUMBER_OF_PAZYMYS;
        s.namuDarbai = generatePazymiai(m);
        s.egzaminas = generatePazymiai(1)[0];
        studentai.push_back(s);
    }
    return studentai;
}

// -------------------------------------------------------
// Template: generateOnlyPazymiai
// Generuoja tik atsitiktinius pažymius studentams.
// -------------------------------------------------------
template<typename Container>
Container generateOnlyPazymiai(int n)
{
    Container studentai;
    studentas s;
    for (int i = 0; i < n; i++)
    {
        s.vardas = enterName(i + 1);
        s.pavarde = enterSurname(i + 1);
        int m = NUMBER_OF_PAZYMYS;
        s.namuDarbai = generatePazymiai(m);
        s.egzaminas = generatePazymiai(1)[0];
        studentai.push_back(s);
    }
    return studentai;
}

#endif // GENERATE_H_DEFINED