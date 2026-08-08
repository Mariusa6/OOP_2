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

// -------------------------------------------------------
// Vidinės pagalbinės funkcijos (tik šiam failui)
// -------------------------------------------------------

namespace {

	// Klaidos pranešimo pradžia — kuriama TIK metant išimtį
	inline std::string vietosTekstas(int lineNumber) {
		return (lineNumber > 0)
			? "Eilutėje " + std::to_string(lineNumber)
			: std::string("Įvestyje");
	}

	inline std::string ribuKlaida(int lineNumber, const char* ko, int p) {
		return vietosTekstas(lineNumber) + " " + ko + " pažymys už ribų ("
			+ std::to_string(studentas::minPazymys) + "-"
			+ std::to_string(studentas::maxPazymys) + "): "
			+ std::to_string(p);
	}

} // anoniminis namespace


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
		if (!pazymysTinkamas(p))
			throw std::runtime_error("Namų darbo pažymys už ribų: " + std::to_string(p));
	if (!pazymysTinkamas(egzaminas_))
		throw std::runtime_error("Egzamino pažymys už ribų: " + std::to_string(egzaminas_));
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
// parseFromLine — GREITAS parsinimas failo skaitymui
//
// Neturi std::istringstream (nekuriamas objektas kiekvienai eilutei)
// ir naudoja std::from_chars vietoj operator>> — be locale,
// be stream sentry, be būsenos tikrinimo.
// -------------------------------------------------------

void studentas::parseFromLine(const std::string& line, int ndCount, int lineNumber)
{
	const char* p = line.data();
	const char* end = p + line.size();

	auto skipSpace = [&]() {
		while (p < end && (*p == ' ' || *p == '\t' || *p == '\r')) ++p;
		};

	// --- Vardas ---
	skipSpace();
	const char* start = p;
	while (p < end && *p != ' ' && *p != '\t' && *p != '\r') ++p;
	if (start == p)
		throw std::runtime_error(vietosTekstas(lineNumber) + " trūksta vardo.");
	vardas_.assign(start, p - start);

	// --- Pavardė ---
	skipSpace();
	start = p;
	while (p < end && *p != ' ' && *p != '\t' && *p != '\r') ++p;
	if (start == p)
		throw std::runtime_error(vietosTekstas(lineNumber) + " trūksta pavardės.");
	pavarde_.assign(start, p - start);

	// --- Pažymiai ---
	auto readInt = [&](const char* ko) -> int {
		skipSpace();
		int v = 0;
		auto res = std::from_chars(p, end, v);
		if (res.ec != std::errc())
			throw std::runtime_error(vietosTekstas(lineNumber)
				+ " trūksta " + ko + " pažymio.");
		p = res.ptr;
		return v;
		};

	nd_.clear();
	nd_.reserve(ndCount);

	for (int i = 0; i < ndCount; ++i) {
		int v = readInt("namų darbo");
		if (!pazymysTinkamas(v))
			throw std::runtime_error(ribuKlaida(lineNumber, "namų darbo", v));
		nd_.push_back(v);
	}

	egzaminas_ = readInt("egzamino");
	if (!pazymysTinkamas(egzaminas_))
		throw std::runtime_error(ribuKlaida(lineNumber, "egzamino", egzaminas_));
}

// -------------------------------------------------------
// appendListTo — GREITAS formatavimas failo rašymui
//
// Rašoma tiesiai į bendrą std::string buferį naudojant
// std::to_chars — be std::setw manipuliatorių ir be atskirų
// << operacijų į srautą kiekvienam laukui.
// -------------------------------------------------------

void studentas::appendListTo(std::string& out) const
{
	// Vardas — kairinis lygiavimas plotyje 25
	out.append(vardas_);
	if (vardas_.size() < 25) out.append(25 - vardas_.size(), ' ');

	// Pavardė — kairinis lygiavimas plotyje 25
	out.append(pavarde_);
	if (pavarde_.size() < 25) out.append(25 - pavarde_.size(), ' ');

	// Skaičius — dešininis lygiavimas plotyje 10
	char buf[16];
	auto appendInt = [&](int v) {
		auto res = std::to_chars(buf, buf + sizeof(buf), v);
		size_t len = static_cast<size_t>(res.ptr - buf);
		if (len < 10) out.append(10 - len, ' ');
		out.append(buf, len);
		};

	for (int nd : nd_)
		appendInt(nd);
	appendInt(egzaminas_);

	out.push_back('\n');
}


// -------------------------------------------------------
// Įvestis iš srauto
// -------------------------------------------------------

std::istream& studentas::readStudentas(std::istream& is, int ndCount, int lineNumber) {
	if (!(is >> vardas_ >> pavarde_))
		throw std::runtime_error(vietosTekstas(lineNumber)
			+ " trūksta vardo arba pavardės.");

	nd_.clear();

	if (ndCount > 0) {
		nd_.reserve(ndCount);
		for (int i = 0; i < ndCount; ++i) {
			int p;
			if (!(is >> p))
				throw std::runtime_error(vietosTekstas(lineNumber)
					+ " trūksta namų darbo pažymio.");
			if (!pazymysTinkamas(p))
				throw std::runtime_error(ribuKlaida(lineNumber, "namų darbo", p));
			nd_.push_back(p);
		}
		if (!(is >> egzaminas_))
			throw std::runtime_error(vietosTekstas(lineNumber)
				+ " trūksta egzamino pažymio.");
	}
	else {
		// Nežinomas skaičius — skaitome iki galo, paskutinis yra egzaminas
		int p;
		while (is >> p) {
			if (!pazymysTinkamas(p))
				throw std::runtime_error(ribuKlaida(lineNumber, "namų darbo", p));
			nd_.push_back(p);
		}
		if (nd_.empty())
			throw std::runtime_error(vietosTekstas(lineNumber)
				+ " trūksta egzamino pažymio.");

		egzaminas_ = nd_.back();
		nd_.pop_back();
		is.clear();   // eilutės pabaiga nėra klaida šiame režime
	}

	if (!pazymysTinkamas(egzaminas_))
		throw std::runtime_error(ribuKlaida(lineNumber, "egzamino", egzaminas_));

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