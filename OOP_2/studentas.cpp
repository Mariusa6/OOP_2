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

double studentas::getGalutinisVid() const {
	if (nd_.empty()) return 0.0;
	double ndVid = std::accumulate(nd_.begin(), nd_.end(), 0.0) / nd_.size();
	return ndVid * namuDarbaiSvoris + egzaminas_ * egzaminasSvoris;
}

double studentas::getGalutinisMed() const {
	if (nd_.empty()) return 0.0;
	std::vector<int> ndCopy = nd_;
	std::sort(ndCopy.begin(), ndCopy.end());
	double ndMed;
	size_t size = ndCopy.size();
	if (size % 2 == 0) {
		ndMed = (ndCopy[size / 2 - 1] + ndCopy[size / 2]) / 2.0;
	}
	else {
		ndMed = ndCopy[size / 2];
	}
	return ndMed * namuDarbaiSvoris + egzaminas_ * egzaminasSvoris;
}