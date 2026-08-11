#ifndef ZMOGUS_H_DEFINED
#define ZMOGUS_H_DEFINED

#include <string>
#include <iostream>
#include <stdexcept>

// -------------------------------------------------------
// class zmogus — ABSTRAKTI bazinė klasė
// Aprašo bendrus bet kokio žmogaus duomenis: vardą ir pavardę.
// -------------------------------------------------------

class zmogus {
protected:
    std::string vardas_;
    std::string pavarde_;

    // ---------------------------------------------------
    // Konstruktoriai — PROTECTED
    // Prieinami tik išvestinėms klasėms
    // ---------------------------------------------------
    zmogus();                                           // numatytasis
    zmogus(const std::string& vardas,                   // pilnas
        const std::string& pavarde);
    zmogus(const zmogus& other);                        // kopijavimo
    zmogus(zmogus&& other) noexcept;                    // perkėlimo

    zmogus& operator=(const zmogus& other);             // kopijavimo priskyrimas
    zmogus& operator=(zmogus&& other) noexcept;         // perkėlimo priskyrimas

public:
    // Gyvuojančių objektų skaitiklis — naudojamas testuose,
    // kad būtų galima patikrinti virtualaus destruktoriaus veikimą
    static int gyvuZmoniu;

    // ---------------------------------------------------
    // VIRTUALUS destruktorius — PUBLIC
    // ---------------------------------------------------
    virtual ~zmogus();

    // ---------------------------------------------------
    // Get'eriai — bendri visoms išvestinėms klasėms
    // ---------------------------------------------------
    inline const std::string& vardas() const { return vardas_; }
    inline const std::string& pavarde() const { return pavarde_; }
    inline std::string pilnasVardas() const { return vardas_ + " " + pavarde_; }

    // ---------------------------------------------------
    // Set'eriai su validacija
    // ---------------------------------------------------
    void setVardas(const std::string& v);
    void setPavarde(const std::string& p);

    // ---------------------------------------------------
    // GRYNAI VIRTUALŪS metodai
    // Kiekviena išvestinė klasė PRIVALO juos realizuoti
    // ---------------------------------------------------

    // Galutinis įvertinimas — kiekvienas žmogaus tipas
    // skaičiuoja jį savaip (studentas — pagal pažymius)
    virtual double galutinis() const = 0;

    // Išvedimas į srautą — realizuoja operator<<
    virtual void print(std::ostream& os) const = 0;

    // Nuskaitymas iš srauto — realizuoja operator>>
    virtual std::istream& read(std::istream& is) = 0;

    // Žmogaus tipo pavadinimas (diagnostikai, logams)
    virtual std::string tipas() const = 0;
};

// -------------------------------------------------------
// POLIMORFINIAI įvesties / išvesties operatoriai
// -------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const zmogus& z);
std::istream& operator>>(std::istream& is, zmogus& z);

#endif // ZMOGUS_H_DEFINED