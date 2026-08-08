#ifndef STUDENTAS_H_DEFINED
#define STUDENTAS_H_DEFINED

#include <string>
#include <vector>
#include <iostream>
#include <numeric>
#include <algorithm>
#include <iomanip>
#include <stdexcept>

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
	double galutinisVid_;
	double galutinisMed_;

	// Pagalbinai statiniai metodai
	static double vidurkis(const std::vector<int>& nd);
	static double mediana(const std::vector<int>& nd);

	// Validacija
	static bool pazymysTinkamas(int p) { return p >= minPazymys && p <= maxPazymys; }

public:
	// Konstantos
	static constexpr int minPazymys = 1;
	static constexpr int maxPazymys = 10;
	static constexpr double namuDarbaiSvoris = 0.4;
	static constexpr double egzaminasSvoris = 0.6;

	// Konstruktoriai
	studentas() :	vardas_(""),			// default
					pavarde_(""),
					nd_(),
					egzaminas_(0),
					galutinisVid_(0.0),
					galutinisMed_(0.0) {}
	explicit studentas(std::istream& is);			// pilnas - kreipiasi į readStudentas
	studentas(const std::string& vardas, const std::string& pavarde, const std::vector<int>& nd, int egzaminas);

	// Getter'iai
	inline std::string vardas() const { return vardas_; }
	inline std::string pavarde() const { return pavarde_; }
	inline const std::vector<int>& nd() const { return nd_; }
	inline int egzaminas() const { return egzaminas_; }
	inline double galutinisVid() const { return galutinisVid_; }
	inline double galutinisMed() const { return galutinisMed_; }

	// Skaičiavimo metodai

	void calculateGalutinis();	// skaičiuoja galutinį balą pagal vidurkį ir medianą	

	// Setter'iai
	std::istream& readStudentas(std::istream& is, int ndCount = 0, int lineNumber = 0);	// skaito studento duomenis

	// friend funkcijos

	friend std::istream& operator>>(std::istream& is, studentas& s);

	friend std::ostream& operator<<(std::ostream& os, const studentas& s);
};

bool comparePagalVarda(const studentas& s1, const studentas& s2);
bool comparePagalPavarde(const studentas& s1, const studentas& s2);
bool comparePagalEgzamina(const studentas& s1, const studentas& s2);
bool comparePagalVidurki(const studentas& s1, const studentas& s2);
bool comparePagalMediana(const studentas& s1, const studentas& s2);

#endif