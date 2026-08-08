#ifndef STUDENTAS_H_DEFINED
#define STUDENTAS_H_DEFINED

#include <string>
#include <vector>
#include <iostream>
#include <numeric>
#include <algorithm>
#include <iomanip>
#include <stdexcept>
#include <charconv>

// -------------------------------------------------------
// class studentas
//
// Saugo vieno studento duomenis ir skaičiuoja galutinius balus.
//
// v1.2: realizuota pilna "Rule of Five" taisyklė —
//   1. Destruktorius
//   2. Kopijavimo konstruktorius
//   3. Kopijavimo priskyrimo operatorius
//   4. Perkėlimo (move) konstruktorius
//   5. Perkėlimo (move) priskyrimo operatorius
//
// Move operacijos pažymėtos noexcept — be to std::vector
// perskirstymo metu naudotų kopijavimą, ne perkėlimą.
// -------------------------------------------------------

class studentas {
private:
    std::string vardas_;
    std::string pavarde_;
    std::vector<int> nd_;
    int egzaminas_;
    double galutinisVid_;
    double galutinisMed_;

    // Pagalbiniai statiniai metodai
    static double vidurkis(const std::vector<int>& nd);
    static double mediana(const std::vector<int>& nd);

    // Greita patikra be jokio std::string kūrimo
    static inline bool pazymysTinkamas(int p) {
        return p >= minPazymys && p <= maxPazymys;
    }

public:
    // ---------------------------------------------------
    // Konstantos
    // ---------------------------------------------------
    static constexpr int minPazymys = 1;
    static constexpr int maxPazymys = 10;
    static constexpr double namuDarbaiSvoris = 0.4;
    static constexpr double egzaminasSvoris = 0.6;

    // Skaitiklis gyvuojantiems objektams — naudojamas testuose,
    // kad būtų galima patikrinti, ar destruktorius tikrai kviečiamas.
    static int gyvuObjektu;

    // ---------------------------------------------------
    // 1. Konstruktoriai
    // ---------------------------------------------------
    studentas();                                        // numatytasis
    studentas(const std::string& vardas,                // pilnas
        const std::string& pavarde,
        const std::vector<int>& nd,
        int egzaminas);
    explicit studentas(std::istream& is);               // iš srauto

    // ---------------------------------------------------
    // 2. RULE OF FIVE
    // ---------------------------------------------------
    ~studentas();                                       // destruktorius
    studentas(const studentas& other);                  // kopijavimo konstruktorius
    studentas& operator=(const studentas& other);       // kopijavimo priskyrimas
    studentas(studentas&& other) noexcept;              // perkėlimo konstruktorius
    studentas& operator=(studentas&& other) noexcept;   // perkėlimo priskyrimas

    // ---------------------------------------------------
    // 3. Get'eriai (inline, be kopijų)
    // ---------------------------------------------------
    inline const std::string& vardas() const { return vardas_; }
    inline const std::string& pavarde() const { return pavarde_; }
    inline const std::vector<int>& nd() const { return nd_; }
    inline int egzaminas() const { return egzaminas_; }
    inline double galutinisVid() const { return galutinisVid_; }
    inline double galutinisMed() const { return galutinisMed_; }
    inline bool tuscias() const { return vardas_.empty() && pavarde_.empty(); }

    // ---------------------------------------------------
    // 4. Set'eriai (su validacija)
    // ---------------------------------------------------
    void setVardas(const std::string& v);
    void setPavarde(const std::string& p);
    void setNd(const std::vector<int>& nd);
    void setEgzaminas(int e);
    void addPazymys(int p);
    void isvalyk();                     // atlaisvina duomenis

    // ---------------------------------------------------
    // 5. Skaičiavimo metodai
    // ---------------------------------------------------
    void calculateGalutinis();

    // ---------------------------------------------------
    // 6. Įvesties metodai
    // ---------------------------------------------------
    // readStudentas — bendras variantas per std::istream (interaktyvi įvestis)
    std::istream& readStudentas(std::istream& is, int ndCount = 0, int lineNumber = 0);

    // parseFromLine — greitas parsinimas failo skaitymui (std::from_chars)
    void parseFromLine(const std::string& line, int ndCount, int lineNumber);

    // ---------------------------------------------------
    // 7. Išvesties metodai
    // ---------------------------------------------------
    // appendListTo — prideda suformatuotą eilutę į bendrą buferį (std::to_chars)
    void appendListTo(std::string& out) const;

    // ---------------------------------------------------
    // 8. Perdengti operatoriai
    // ---------------------------------------------------
    friend std::istream& operator>>(std::istream& is, studentas& s);
    friend std::ostream& operator<<(std::ostream& os, const studentas& s);

    // Lyginimo operatoriai — naudingi būsimiems klasės naudotojams
    // (std::find, std::unique, std::set, asociatyvūs konteineriai)
    bool operator==(const studentas& other) const;
    bool operator!=(const studentas& other) const;
    bool operator<(const studentas& other) const;   // pagal pavardę, tada vardą
    bool operator>(const studentas& other) const;

    // Prieiga prie namų darbo pažymio pagal indeksą
    int operator[](size_t i) const;

    // Konvertavimas į bool — ar objektas turi duomenų
    explicit operator bool() const;
};

// ---------------------------------------------------
// Ne-nariai lyginimo funkcijos (rikiavimui)
// ---------------------------------------------------
bool comparePagalVarda(const studentas& s1, const studentas& s2);
bool comparePagalPavarde(const studentas& s1, const studentas& s2);
bool comparePagalEgzamina(const studentas& s1, const studentas& s2);
bool comparePagalVidurki(const studentas& s1, const studentas& s2);
bool comparePagalMediana(const studentas& s1, const studentas& s2);

#endif