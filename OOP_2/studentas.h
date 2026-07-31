#ifndef STUDENTAS_H_DEFINED
#define STUDENTAS_H_DEFINED

#include <string>
#include <vector>

class studentas {
private:
	std::string vardas_;
	std::string pavarde_;
	std::vector<int> nd_;
	int egzaminas_;
	double galutinisVid_;
	double galutinisMed_;
};

#endif