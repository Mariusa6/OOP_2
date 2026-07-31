#include "studentas.h"

studentas::studentas(std::istream& is) {
	readStudentas(is);
}

std::istream& studentas::readStudentas(std::istream& is) {
	is >> vardas_ >> pavarde_;
	std::vector<int> nd;
	int pazymys;
	while (is >> pazymys) {
		nd.push_back(pazymys);
	}
	if (!nd.empty()) {
		egzaminas_ = nd.back();
		nd.pop_back();
		nd_ = nd;
	}
	is.clear();
	return is;
}

double studentas::galutinis(double (*func) (const std::vector<int>&)) const {
	return func(nd_) * namuDarbaiSvoris + egzaminas_ * egzaminasSvoris;
}

bool comparePagalVarda(const studentas& s1, const studentas& s2) {
	return s1.vardas() < s2.vardas();
}

bool comparePagalPavarde(const studentas& s1, const studentas& s2) {
	return s1.pavarde() < s2.pavarde();
}

bool comparePagalEgzamina(const studentas& s1, const studentas& s2) {
	return s1.egzaminas() < s2.egzaminas();
}

bool comparePagalgalutini(const studentas& s1, const studentas& s2) {
	return s1.galutinis() < s2.galutinis();
}