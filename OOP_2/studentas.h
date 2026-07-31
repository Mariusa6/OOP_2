#ifndef STUDENTAS_H_DEFINED
#define STUDENTAS_H_DEFINED

#include <string>
#include <vector>
#include <iostream>
#include <numeric>
#include <algorithm>
#include <iomanip>
#include "calculate.h"

// -------------------------------------------------------
// class studentas
// Saugo vieno studento duomenis ir skaičiuoja galutinius balus.
// -------------------------------------------------------


class studentas {
private:
	std::string vardas_;
	std::string pavarde_;
	std::vector<int> nd_;
	int egzaminas_;

public:
	// Konstantos
	static constexpr double namuDarbaiSvoris = 0.4;
	static constexpr double egzaminasSvoris = 0.6;

	// Konstruktoriai
	studentas() :	vardas_(""),			// default
					pavarde_(""),
					nd_(),
					egzaminas_(0) {}
	studentas(std::istream& is);			// pilnas - kreipiasi į readStudentas

	// Destruktorius
	~studentas() {}

	// Getter'iai
	inline std::string vardas() const { return vardas_; }
	inline std::string pavarde() const { return pavarde_; }
	inline std::vector<int> nd() const { return nd_; }
	inline int egzaminas() const { return egzaminas_; }
	inline double galutinis(double (*func) (const std::vector<int>&) = mediana) const;

	// Setter'iai
	std::istream& readStudentas(std::istream& is);	// skaito studento duomenis

	// friend funkcijos

	friend std::istream& operator>>(std::istream& is, studentas& s) {
		return s.readStudentas(is);
	}

	friend std::ostream& operator<<(std::ostream& os, const studentas& s) {
		os << std::left << std::setw(20) << s.vardas_
			<< std::left << std::setw(20) << s.pavarde_
			<< std::left << std::setw(10) << s.galutinis();
		return os;
	}
};

bool comparePagalVarda(const studentas& s1, const studentas& s2);
bool comparePagalPavarde(const studentas& s1, const studentas& s2);
bool comparePagalEgzamina(const studentas& s1, const studentas& s2);
bool comparePagalgalutini(const studentas& s1, const studentas& s2);

#endif