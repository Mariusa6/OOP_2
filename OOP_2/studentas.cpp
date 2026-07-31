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