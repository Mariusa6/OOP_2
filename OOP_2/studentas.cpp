#include "studentas.h"

// -------------------------------------------------------
// Statinis narys — gyvuojančių objektų skaitiklis
// -------------------------------------------------------
int studentas::gyvuObjektu = 0;

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

// =======================================================
// 1. KONSTRUKTORIAI
// =======================================================

// Numatytasis konstruktorius
studentas::studentas()
    : vardas_(), pavarde_(), nd_(),
    egzaminas_(0), galutinisVid_(0.0), galutinisMed_(0.0)
{
    ++gyvuObjektu;
}

// Pilnas konstruktorius — su validacija
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

    ++gyvuObjektu;
}

// Konstruktorius iš srauto
studentas::studentas(std::istream& is)
    : vardas_(), pavarde_(), nd_(),
    egzaminas_(0), galutinisVid_(0.0), galutinisMed_(0.0)
{
    ++gyvuObjektu;
    readStudentas(is);
}

// =======================================================
// 2. RULE OF FIVE — pilnos realizacijos
// =======================================================

// -------------------------------------------------------
// 2.1 DESTRUKTORIUS
// -------------------------------------------------------
studentas::~studentas()
{
    vardas_.clear();
    pavarde_.clear();
    nd_.clear();
    nd_.shrink_to_fit();      // faktiškai atlaisvina vektoriaus buferį
    egzaminas_ = 0;
    galutinisVid_ = 0.0;
    galutinisMed_ = 0.0;

    --gyvuObjektu;
}

// -------------------------------------------------------
// 2.2 KOPIJAVIMO KONSTRUKTORIUS
// -------------------------------------------------------
studentas::studentas(const studentas& other)
    : vardas_(other.vardas_),
    pavarde_(other.pavarde_),
    nd_(other.nd_),
    egzaminas_(other.egzaminas_),
    galutinisVid_(other.galutinisVid_),
    galutinisMed_(other.galutinisMed_)
{
    ++gyvuObjektu;
}

// -------------------------------------------------------
// 2.3 KOPIJAVIMO PRISKYRIMO OPERATORIUS
// -------------------------------------------------------
studentas& studentas::operator=(const studentas& other)
{
    if (this == &other)       // apsauga nuo priskyrimo sau
        return *this;

    vardas_ = other.vardas_;
    pavarde_ = other.pavarde_;
    nd_ = other.nd_;
    egzaminas_ = other.egzaminas_;
    galutinisVid_ = other.galutinisVid_;
    galutinisMed_ = other.galutinisMed_;

    return *this;
}

// -------------------------------------------------------
// 2.4 PERKĖLIMO (MOVE) KONSTRUKTORIUS
// -------------------------------------------------------
studentas::studentas(studentas&& other) noexcept
    : vardas_(std::move(other.vardas_)),
    pavarde_(std::move(other.pavarde_)),
    nd_(std::move(other.nd_)),
    egzaminas_(other.egzaminas_),
    galutinisVid_(other.galutinisVid_),
    galutinisMed_(other.galutinisMed_)
{
    // Šaltinis paliekamas apibrėžtoje tuščioje būsenoje
    other.egzaminas_ = 0;
    other.galutinisVid_ = 0.0;
    other.galutinisMed_ = 0.0;

    ++gyvuObjektu;
}

// -------------------------------------------------------
// 2.5 PERKĖLIMO (MOVE) PRISKYRIMO OPERATORIUS
// -------------------------------------------------------
studentas& studentas::operator=(studentas&& other) noexcept
{
    if (this == &other)       // apsauga nuo priskyrimo sau
        return *this;

    vardas_ = std::move(other.vardas_);
    pavarde_ = std::move(other.pavarde_);
    nd_ = std::move(other.nd_);
    egzaminas_ = other.egzaminas_;
    galutinisVid_ = other.galutinisVid_;
    galutinisMed_ = other.galutinisMed_;

    // Šaltinis paliekamas apibrėžtoje tuščioje būsenoje
    other.egzaminas_ = 0;
    other.galutinisVid_ = 0.0;
    other.galutinisMed_ = 0.0;

    return *this;
}

// =======================================================
// 3. SET'ERIAI
// =======================================================

void studentas::setVardas(const std::string& v) {
    if (v.empty())
        throw std::invalid_argument("Vardas negali būti tuščias");
    vardas_ = v;
}

void studentas::setPavarde(const std::string& p) {
    if (p.empty())
        throw std::invalid_argument("Pavardė negali būti tuščia");
    pavarde_ = p;
}

void studentas::setNd(const std::vector<int>& nd) {
    for (int p : nd)
        if (!pazymysTinkamas(p))
            throw std::runtime_error("Namų darbo pažymys už ribų: " + std::to_string(p));
    nd_ = nd;
}

void studentas::setEgzaminas(int e) {
    if (!pazymysTinkamas(e))
        throw std::runtime_error("Egzamino pažymys už ribų: " + std::to_string(e));
    egzaminas_ = e;
}

void studentas::addPazymys(int p) {
    if (!pazymysTinkamas(p))
        throw std::runtime_error("Namų darbo pažymys už ribų: " + std::to_string(p));
    nd_.push_back(p);
}

void studentas::isvalyk() {
    vardas_.clear();
    pavarde_.clear();
    nd_.clear();
    nd_.shrink_to_fit();
    egzaminas_ = 0;
    galutinisVid_ = 0.0;
    galutinisMed_ = 0.0;
}

// =======================================================
// 4. SKAIČIAVIMAS
// =======================================================

void studentas::calculateGalutinis() {
    galutinisVid_ = namuDarbaiSvoris * vidurkis(nd_) + egzaminasSvoris * egzaminas_;
    galutinisMed_ = namuDarbaiSvoris * mediana(nd_) + egzaminasSvoris * egzaminas_;
}

// =======================================================
// 5. ĮVESTIS
// =======================================================

// -------------------------------------------------------
// parseFromLine — GREITAS parsinimas failo skaitymui
// Nenaudoja std::istringstream, naudoja std::from_chars
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
// readStudentas — įvestis per std::istream
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

// =======================================================
// 6. IŠVESTIS
// =======================================================
void studentas::appendListTo(std::string& out) const
{
    out.append(vardas_);
    if (vardas_.size() < 25) out.append(25 - vardas_.size(), ' ');

    out.append(pavarde_);
    if (pavarde_.size() < 25) out.append(25 - pavarde_.size(), ' ');

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

// =======================================================
// 7. PERDENGTI OPERATORIAI
// =======================================================

// -------------------------------------------------------
// operator>> — įvesties operatorius
// Nuskaito: vardas pavardė nd1 nd2 ... egzaminas
// (paskutinis skaičius eilutėje laikomas egzaminu)
// -------------------------------------------------------
std::istream& operator>>(std::istream& is, studentas& s) {
    return s.readStudentas(is);
}

// -------------------------------------------------------
// operator<< — išvesties operatorius
// Išveda suformatuotą eilutę su galutiniais balais
// -------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const studentas& s) {
    os << std::left << std::setw(20) << s.vardas_
        << std::left << std::setw(20) << s.pavarde_
        << std::fixed << std::setprecision(2)
        << std::left << std::setw(20) << s.galutinisVid_
        << std::left << std::setw(20) << s.galutinisMed_;
    return os;
}

// -------------------------------------------------------
// Lyginimo operatoriai
// -------------------------------------------------------

bool studentas::operator==(const studentas& other) const {
    return vardas_ == other.vardas_
        && pavarde_ == other.pavarde_
        && nd_ == other.nd_
        && egzaminas_ == other.egzaminas_;
}

bool studentas::operator!=(const studentas& other) const {
    return !(*this == other);
}

// Rikiavimas pagal pavardę, esant vienodoms — pagal vardą
bool studentas::operator<(const studentas& other) const {
    if (pavarde_ != other.pavarde_)
        return pavarde_ < other.pavarde_;
    return vardas_ < other.vardas_;
}

bool studentas::operator>(const studentas& other) const {
    return other < *this;
}

// -------------------------------------------------------
// operator[] — prieiga prie namų darbo pažymio pagal indeksą
// -------------------------------------------------------
int studentas::operator[](size_t i) const {
    if (i >= nd_.size())
        throw std::out_of_range("Namų darbo indeksas už ribų: " + std::to_string(i));
    return nd_[i];
}

// -------------------------------------------------------
// operator bool — ar objektas turi duomenų
// explicit, kad neįvyktų netyčinis konvertavimas į int
// -------------------------------------------------------
studentas::operator bool() const {
    return !vardas_.empty() && !pavarde_.empty();
}

// =======================================================
// 8. NE-NARIAI LYGINIMO FUNKCIJOS
// =======================================================

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