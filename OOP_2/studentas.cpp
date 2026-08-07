#include "studentas.h"

// -------------------------------------------------------
// Privatūs pagalbiniai metodai
// -------------------------------------------------------

double studentas::vidurkis(const std::vector<int>& nd) {
	if (nd.empty())
		return 0.0;
	double sum = std::accumulate(nd.begin(), nd.end(), 0.0);
	return sum / nd.size();
}

double studentas::mediana(const std::vector<int>& nd) {
	if (nd.empty())
		return 0.0;
	std::vector<int> sortedNd = nd;
	std::sort(sortedNd.begin(), sortedNd.end());
	size_t size = sortedNd.size();
	if (size % 2 == 0)
		return (sortedNd[size / 2 - 1] + sortedNd[size / 2]) / 2.0;
	else
		return sortedNd[size / 2];
}

void studentas::validatePazymys(int p, const std::string& kontekstas) {
	if (p < minPazymys || p > maxPazymys) {
		throw std::runtime_error(kontekstas + " pažymys už ribų ("
			+ std::to_string(minPazymys) + "-"
			+ std::to_string(maxPazymys) + "): " + std::to_string(p));
	}
}

// -------------------------------------------------------
// Konstruktoriai
// -------------------------------------------------------

studentas::studentas(const std::string& vardas,
	const std::string& pavarde,
	const std::vector<int>& nd,
	int egzaminas)
	: vardas_(vardas), pavarde_(pavarde), nd_(nd), egzaminas_(egzaminas),
	galutinisVid_(0.0), galutinisMed_(0.0)
{
	if (vardas_.empty())
		throw std::invalid_argument("Vardas negali būti tuščias");
	if (pavarde_.empty())
		throw std::invalid_argument("Pavardė negali būti tuščia");

	for (int p : nd_)
		validatePazymys(p, "Namų darbo");
	validatePazymys(egzaminas_, "Egzamino");
}

studentas::studentas(std::istream& is)
	: vardas_(), pavarde_(),
	nd_(), egzaminas_(0),
	galutinisVid_(0.0), galutinisMed_(0.0)
{
	readStudentas(is);
}

// -------------------------------------------------------
// Skaičiavimas
// -------------------------------------------------------

void studentas::calculateGalutinis() {
	galutinisVid_ = namuDarbaiSvoris * vidurkis(nd_) + egzaminasSvoris * egzaminas_;
	galutinisMed_ = namuDarbaiSvoris * mediana(nd_) + egzaminasSvoris * egzaminas_;
}

// -------------------------------------------------------
// Įvestis iš srauto
// -------------------------------------------------------

std::istream& studentas::readStudentas(std::istream& is, int ndCount, int lineNumber) {
	const std::string vieta = (lineNumber > 0)
		? "Eilutėje " + std::to_string(lineNumber)
		: std::string("Įvestyje");

	if (!(is >> vardas_ >> pavarde_))
		throw std::runtime_error(vieta + " trūksta vardo arba pavardės.");

	nd_.clear();

	if (ndCount > 0) {
		// Žinomas namų darbų skaičius (failo antraštė nurodo ND stulpelius)
		nd_.reserve(ndCount);
		for (int i = 0; i < ndCount; ++i) {
			int p;
			if (!(is >> p))
				throw std::runtime_error(vieta + " trūksta namų darbo pažymio.");
			validatePazymys(p, vieta + " namų darbo");
			nd_.push_back(p);
		}
		if (!(is >> egzaminas_))
			throw std::runtime_error(vieta + " trūksta egzamino pažymio.");
	}
	else {
		// Nežinomas skaičius — skaitome iki galo, paskutinis yra egzaminas
		int p;
		while (is >> p) {
			validatePazymys(p, vieta + " namų darbo");
			nd_.push_back(p);
		}
		if (nd_.empty())
			throw std::runtime_error(vieta + " trūksta egzamino pažymio.");

		egzaminas_ = nd_.back();
		nd_.pop_back();
		is.clear();   // eilutės pabaiga nėra klaida šiame režime
	}

	validatePazymys(egzaminas_, vieta + " egzamino");
	return is;
}

// -------------------------------------------------------
// friend operatoriai
// -------------------------------------------------------

std::istream& operator>>(std::istream& is, studentas& s) {
	return s.readStudentas(is);
}

std::ostream& operator<<(std::ostream& os, const studentas& s) {
	os << std::left << std::setw(20) << s.vardas_
		<< std::left << std::setw(20) << s.pavarde_
		<< std::fixed << std::setprecision(2)
		<< std::left << std::setw(20) << s.galutinisVid_
		<< std::left << std::setw(20) << s.galutinisMed_;
	return os;
}

// -------------------------------------------------------
// Ne-nariai lyginimo funkcijos
// -------------------------------------------------------

bool comparePagalVarda(const studentas& s1, const studentas& s2) {
	return s1.vardas() < s2.vardas();
}

bool comparePagalPavarde(const studentas& s1, const studentas& s2) {
	return s1.pavarde() < s2.pavarde();
}

bool comparePagalEgzamina(const studentas& s1, const studentas& s2) {
	return s1.egzaminas() > s2.egzaminas();
}

bool comparePagalVidurki(const studentas& s1, const studentas& s2) {
	return s1.galutinisVid() > s2.galutinisVid();
}

bool comparePagalMediana(const studentas& s1, const studentas& s2) {
	return s1.galutinisMed() > s2.galutinisMed();
}