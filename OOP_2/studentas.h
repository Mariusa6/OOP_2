#ifndef STUDENTAS_H_DEFINED
#define STUDENTAS_H_DEFINED

#include "zmogus.h"

#include <string>
#include <vector>
#include <iostream>
#include <numeric>
#include <algorithm>
#include <iomanip>
#include <stdexcept>
#include <charconv>

// -------------------------------------------------------
// class studentas : public zmogus
//
// IŠVESTINĖ (derived) klasė iš abstrakčios bazinės klasės zmogus.
//
// Paveldi:
//   - vardas_, pavarde_ (protected laukai)
//   - vardas(), pavarde(), pilnasVardas() get'erius
//   - setVardas(), setPavarde() set'erius
//   - virtualų destruktorių
//
// Realizuoja (override) visus grynai virtualius metodus:
//   - galutinis()  → galutinisVid_
//   - print()      → suformatuota eilutė su balais
//   - read()       → nuskaitymas iš srauto
//   - tipas()      → "Studentas"
//
// Prideda savo duomenis: namų darbų pažymius, egzaminą,
// du galutinius balus (pagal vidurkį ir pagal medianą).
//
// Išlaiko pilną v1.2 RULE OF FIVE realizaciją — visos penkios
// funkcijos kviečia atitinkamas bazinės klasės funkcijas.
// -------------------------------------------------------

class studentas : public zmogus {
private:
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

    // Gyvuojančių studentų skaitiklis (atskiras nuo zmogus::gyvuZmoniu)
    static int gyvuStudentu;

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
    // 2. RULE OF FIVE (paveldėta iš v1.2)
    // ---------------------------------------------------
    ~studentas() override;                              // destruktorius
    studentas(const studentas& other);                  // kopijavimo konstruktorius
    studentas& operator=(const studentas& other);       // kopijavimo priskyrimas
    studentas(studentas&& other) noexcept;              // perkėlimo konstruktorius
    studentas& operator=(studentas&& other) noexcept;   // perkėlimo priskyrimas

    // ---------------------------------------------------
    // 3. Grynai virtualių metodų REALIZACIJA (override)
    // ---------------------------------------------------
    double galutinis() const override;
    void print(std::ostream& os) const override;
    std::istream& read(std::istream& is) override;
    std::string tipas() const override;

    // ---------------------------------------------------
    // 4. Studentui specifiniai get'eriai
    // ---------------------------------------------------
    inline const std::vector<int>& nd() const { return nd_; }
    inline int egzaminas() const { return egzaminas_; }
    inline double galutinisVid() const { return galutinisVid_; }
    inline double galutinisMed() const { return galutinisMed_; }
    inline bool tuscias() const { return vardas_.empty() && pavarde_.empty(); }

    // ---------------------------------------------------
    // 5. Set'eriai (su validacija)
    // ---------------------------------------------------
    void setNd(const std::vector<int>& nd);
    void setEgzaminas(int e);
    void addPazymys(int p);
    void isvalyk();

    // ---------------------------------------------------
    // 6. Skaičiavimo metodai
    // ---------------------------------------------------
    void calculateGalutinis();

    // ---------------------------------------------------
    // 7. Įvesties metodai
    // ---------------------------------------------------
    // readStudentas — bendras variantas per std::istream
    std::istream& readStudentas(std::istream& is, int ndCount = 0, int lineNumber = 0);

    // parseFromLine — greitas parsinimas failo skaitymui (std::from_chars)
    void parseFromLine(const std::string& line, int ndCount, int lineNumber);

    // ---------------------------------------------------
    // 8. Išvesties metodai
    // ---------------------------------------------------
    // appendListTo — prideda suformatuotą eilutę į buferį (std::to_chars)
    void appendListTo(std::string& out) const;

    // ---------------------------------------------------
    // 9. Perdengti operatoriai
    // ---------------------------------------------------
    bool operator==(const studentas& other) const;
    bool operator!=(const studentas& other) const;
    bool operator<(const studentas& other) const;   // pagal pavardę, tada vardą
    bool operator>(const studentas& other) const;
    int operator[](size_t i) const;                 // ND pažymys pagal indeksą
    explicit operator bool() const;                 // ar turi duomenų
};

// ---------------------------------------------------
// Ne-nariai lyginimo funkcijos (rikiavimui)
// ---------------------------------------------------
bool comparePagalVarda(const studentas& s1, const studentas& s2);
bool comparePagalPavarde(const studentas& s1, const studentas& s2);
bool comparePagalEgzamina(const studentas& s1, const studentas& s2);
bool comparePagalVidurki(const studentas& s1, const studentas& s2);
bool comparePagalMediana(const studentas& s1, const studentas& s2);

#endif // STUDENTAS_H_DEFINED