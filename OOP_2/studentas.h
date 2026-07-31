#ifndef STUDENTAS_H_DEFINED
#define STUDENTAS_H_DEFINED

#include <string>
#include <vector>
#include <iostream>
#include <numeric>
#include <algorithm>

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

public:
	// Konstantos
	static constexpr int minPazymys = 1;
	static constexpr int maxPazymys = 10;
	static constexpr double namuDarbaiSvoris = 0.4;
	static constexpr double egzaminasSvoris = 0.6;

	// Konstruktoriai
	studentas() :   vardas_(""),			// default
					pavarde_(""),
					nd_(),
					egzaminas_(0),
					galutinisVid_(0),
					galutinisMed_(0) {}
	studentas(std::istream& is);			// pilnas - kreipiasi į readStudentas

	// Destruktorius
	~studentas() {}

	// Getter'iai
	inline std::string getVardas() const { return vardas_; }
	inline std::string getPavarde() const { return pavarde_; }
	inline std::vector<int> getNamuDarbai() const { return nd_; }
	inline int getEgzaminas() const { return egzaminas_; }
	inline double getGalutinisVid() const;
	inline double getGalutinisMed() const;

	// Setter'iai
	std::istream& readStudentas(std::istream& is);	// skaito studento duomenis

};

#endif