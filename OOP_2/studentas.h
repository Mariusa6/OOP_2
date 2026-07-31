#ifndef STUDENTAS_H_DEFINED
#define STUDENTAS_H_DEFINED

#include <string>
#include <vector>
#include <iostream>

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

	// Setter'iai
	std::istream& readStudentas(std::istream& is);	// skaito studento duomenis

};

#endif