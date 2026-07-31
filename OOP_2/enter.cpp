#include "enter.h"

studentas enterStudentas(int n)
{
    studentas ivedamas;
    std::cout << '#' << n << " studentas:\n";
    ivedamas.vardas = enterName(n);
    ivedamas.pavarde = enterSurname(n);
    ivedamas.namuDarbai = enterPazymiai(n, enterNumberOfPazymys(n));
    ivedamas.egzaminas = enterEgzaminas(n);
    return ivedamas;
}

std::string enterName(int n)
{
    std::string v;

    std::cout << u8"Iveskite studento #" << n << u8" vardą: ";
    while (!(std::cin >> v) || !isAllLetters(v))
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << u8"Naudokite raides. Iveskite studento varda: ";
    }

    return v;
}

std::string enterSurname(int n)
{
    std::string v;

    std::cout << u8"Iveskite studento #" << n << u8" pavardę: ";
    while (!(std::cin >> v) || !isAllLetters(v))
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << u8"Naudokite raides.\n";
        std::cout << u8"Iveskite studento #" << n << u8" pavardę: ";
    }

    return v;
}

int enterNumberOfPazymys(int n)
{
    int tmp{};

    std::cout << u8"Iveskite kiek #" << n << u8" studentas turėjo namų darbų: ";
    while (!(std::cin >> tmp) || tmp < 0)
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << u8"Iveskite neneigiamą skaičių.\n";
        std::cout << u8"Iveskite kiek #" << n << u8" studentas turėjo namų darbų: ";
    }

    return tmp;
}

int enterPazymys(int n)
{
    int tmp{};

    std::cout << u8"" << n << u8" pažymys: ";
    while (!(std::cin >> tmp) || tmp < minPazymys || tmp > maxPazymys)
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << u8"Iveskite skaičių nuo " << minPazymys << u8" iki " << maxPazymys << u8".\n";
        std::cout << n << u8" pažymys: ";
    }

    return tmp;
}

std::vector<int> enterPazymiai(int n, int m) // n - studento numeris, m - pažymių skaičius
{
    std::vector<int> tmp;
    std::cout << u8"Iveskite #" << n << u8" studento namų darbų pažymius.\n";
    for (int i{ 0 }; i < m; i++)
    {
        tmp.push_back(enterPazymys(i + 1));
    }
    return tmp;
}

int enterEgzaminas(int n)
{
    int tmp{};

    std::cout << u8"Iveskite kiek #" << n << u8" studentas gavo iš egzamino.\n";
    while (!(std::cin >> tmp) || tmp < minPazymys || tmp > maxPazymys)
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << u8"Iveskite skaičių nuo " << minPazymys << u8" iki " << maxPazymys << u8".\n";
        std::cout << u8"Iveskite kiek #" << n << u8" studentas gavo iš egzamino.\n";
    }

    return tmp;
}

bool isAllLetters(const std::string& input) {
    return std::all_of(input.begin(), input.end(), ::isalpha);
}