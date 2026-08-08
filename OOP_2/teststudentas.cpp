#include "testStudentas.h"

// =======================================================
// Testavimo infrastruktūra
// =======================================================

namespace {

    int testuPraejo = 0;
    int testuKrito = 0;

    // Vieno patikrinimo rezultatas
    void tikrink(bool salyga, const std::string& aprasymas)
    {
        if (salyga) {
            ++testuPraejo;
            std::cout << "  [ OK ]  " << aprasymas << "\n";
        }
        else {
            ++testuKrito;
            std::cout << "  [KLAIDA] " << aprasymas << "\n";
        }
    }

    void skyrius(const std::string& pavadinimas)
    {
        std::cout << "\n--- " << pavadinimas << " ---\n";
    }

    // Testinis studentas su žinomomis reikšmėmis
    studentas kurkTestini()
    {
        // ND: 5,5,5,5,5 -> vidurkis 5.0, mediana 5.0
        // Egzaminas 10 -> galutinis = 0.4*5 + 0.6*10 = 8.0
        return studentas("Jonas", "Jonaitis", { 5, 5, 5, 5, 5 }, 10);
    }

} // anoniminis namespace

// =======================================================
// Pagrindinė testavimo funkcija
// =======================================================

bool testStudentasKlase()
{
    testuPraejo = 0;
    testuKrito = 0;

    std::cout << "\n===============================================\n";
    std::cout << " KLASES studentas METODU TESTAVIMAS\n";
    std::cout << "===============================================\n";

    const int pradinisSkaicius = studentas::gyvuObjektu;

    // ===================================================
    // 1. KONSTRUKTORIAI
    // ===================================================
    skyrius("1. Numatytasis konstruktorius");
    {
        studentas s;
        tikrink(s.vardas().empty(), "vardas tuscias");
        tikrink(s.pavarde().empty(), "pavarde tuscia");
        tikrink(s.nd().empty(), "nd vektorius tuscias");
        tikrink(s.egzaminas() == 0, "egzaminas = 0");
        tikrink(s.galutinisVid() == 0.0, "galutinisVid = 0.0");
        tikrink(s.galutinisMed() == 0.0, "galutinisMed = 0.0");
        tikrink(!static_cast<bool>(s), "operator bool grazina false tusciam objektui");
    }

    skyrius("2. Pilnas konstruktorius");
    {
        studentas s("Jonas", "Jonaitis", { 5, 5, 5, 5, 5 }, 10);
        tikrink(s.vardas() == "Jonas", "vardas issaugotas");
        tikrink(s.pavarde() == "Jonaitis", "pavarde issaugota");
        tikrink(s.nd().size() == 5, "nd dydis = 5");
        tikrink(s.egzaminas() == 10, "egzaminas = 10");
        tikrink(static_cast<bool>(s), "operator bool grazina true");
    }

    skyrius("3. Pilnas konstruktorius — validacija");
    {
        bool metaTusciaVarda = false;
        try { studentas s("", "Jonaitis", { 5 }, 10); }
        catch (const std::invalid_argument&) { metaTusciaVarda = true; }
        tikrink(metaTusciaVarda, "meta invalid_argument, kai vardas tuscias");

        bool metaTusciaPavarde = false;
        try { studentas s("Jonas", "", { 5 }, 10); }
        catch (const std::invalid_argument&) { metaTusciaPavarde = true; }
        tikrink(metaTusciaPavarde, "meta invalid_argument, kai pavarde tuscia");

        bool metaBlogaND = false;
        try { studentas s("Jonas", "Jonaitis", { 5, 99 }, 10); }
        catch (const std::runtime_error&) { metaBlogaND = true; }
        tikrink(metaBlogaND, "meta runtime_error, kai ND pazymys = 99");

        bool metaBlogaEgz = false;
        try { studentas s("Jonas", "Jonaitis", { 5 }, 0); }
        catch (const std::runtime_error&) { metaBlogaEgz = true; }
        tikrink(metaBlogaEgz, "meta runtime_error, kai egzaminas = 0");
    }

    skyrius("4. Konstruktorius is srauto");
    {
        std::istringstream iss("Petras Petraitis 8 9 10 7");
        studentas s(iss);
        tikrink(s.vardas() == "Petras", "vardas nuskaitytas");
        tikrink(s.pavarde() == "Petraitis", "pavarde nuskaityta");
        tikrink(s.nd().size() == 3, "nd dydis = 3 (paskutinis skaicius = egzaminas)");
        tikrink(s.egzaminas() == 7, "egzaminas = 7");
    }

    // ===================================================
    // 2. RULE OF FIVE
    // ===================================================
    skyrius("5. Kopijavimo konstruktorius");
    {
        studentas originalas = kurkTestini();
        originalas.calculateGalutinis();

        studentas kopija(originalas);

        tikrink(kopija.vardas() == originalas.vardas(), "vardas nukopijuotas");
        tikrink(kopija.pavarde() == originalas.pavarde(), "pavarde nukopijuota");
        tikrink(kopija.nd() == originalas.nd(), "nd vektorius nukopijuotas");
        tikrink(kopija.egzaminas() == originalas.egzaminas(), "egzaminas nukopijuotas");
        tikrink(kopija.galutinisVid() == originalas.galutinisVid(), "galutinisVid nukopijuotas");

        // Svarbiausia: ar objektai nepriklausomi (deep copy)?
        kopija.setVardas("Pakeistas");
        tikrink(originalas.vardas() == "Jonas",
            "originalas NEPAKISTA, kai keiciama kopija (deep copy)");
    }

    skyrius("6. Kopijavimo priskyrimo operatorius");
    {
        studentas a = kurkTestini();
        a.calculateGalutinis();
        studentas b;

        b = a;

        tikrink(b.vardas() == a.vardas(), "vardas priskirtas");
        tikrink(b.nd() == a.nd(), "nd vektorius priskirtas");
        tikrink(b.galutinisVid() == a.galutinisVid(), "galutinisVid priskirtas");

        // Priskyrimas sau
        a = a;
        tikrink(a.vardas() == "Jonas" && a.nd().size() == 5,
            "priskyrimas sau (a = a) nesugadina objekto");

        // Grandinis priskyrimas
        studentas c, d;
        c = d = a;
        tikrink(c.vardas() == "Jonas" && d.vardas() == "Jonas",
            "grandinis priskyrimas (c = d = a) veikia");
    }

    skyrius("7. Perkelimo (move) konstruktorius");
    {
        studentas saltinis = kurkTestini();
        saltinis.calculateGalutinis();
        const double tiketinasVid = saltinis.galutinisVid();

        studentas tikslas(std::move(saltinis));

        tikrink(tikslas.vardas() == "Jonas", "vardas perkeltas i tiksla");
        tikrink(tikslas.nd().size() == 5, "nd vektorius perkeltas i tiksla");
        tikrink(tikslas.galutinisVid() == tiketinasVid, "galutinisVid perkeltas");

        // Saltinis turi likti galiojancioje, bet tuscioje busenoje
        tikrink(saltinis.vardas().empty(), "saltinio vardas tuscias po perkelimo");
        tikrink(saltinis.nd().empty(), "saltinio nd tuscias po perkelimo");
        tikrink(saltinis.egzaminas() == 0, "saltinio egzaminas = 0 po perkelimo");
        tikrink(!static_cast<bool>(saltinis), "saltinis konvertuojasi i false");
    }

    skyrius("8. Perkelimo (move) priskyrimo operatorius");
    {
        studentas saltinis = kurkTestini();
        saltinis.calculateGalutinis();
        studentas tikslas("Senas", "Senaitis", { 1, 1 }, 1);

        tikslas = std::move(saltinis);

        tikrink(tikslas.vardas() == "Jonas", "vardas perkeltas priskyrimu");
        tikrink(tikslas.nd().size() == 5, "nd perkeltas priskyrimu");
        tikrink(saltinis.vardas().empty(), "saltinis istustintas");

        // Priskyrimas sau
        studentas x = kurkTestini();
        x = std::move(x);
        tikrink(x.vardas() == "Jonas" || x.vardas().empty(),
            "move priskyrimas sau nesukelia luzimo");
    }

    skyrius("9. noexcept specifikatorius (svarbu std::vector spartai)");
    {
        tikrink(std::is_nothrow_move_constructible<studentas>::value,
            "move konstruktorius yra noexcept");
        tikrink(std::is_nothrow_move_assignable<studentas>::value,
            "move priskyrimas yra noexcept");
    }

    skyrius("10. Destruktorius");
    {
        const int priesBloka = studentas::gyvuObjektu;
        {
            studentas a = kurkTestini();
            studentas b = kurkTestini();
            studentas c(a);
            tikrink(studentas::gyvuObjektu == priesBloka + 3,
                "sukurti 3 objektai — skaitiklis padidejo 3");
        }
        tikrink(studentas::gyvuObjektu == priesBloka,
            "isejus is bloko destruktorius sumazino skaitikli iki pradinio");
    }

    // ===================================================
    // 3. ĮVESTIES / IŠVESTIES OPERATORIAI
    // ===================================================
    skyrius("11. Ivesties operatorius >>");
    {
        std::istringstream iss("Ona Onaite 10 9 8 6");
        studentas s;
        iss >> s;

        tikrink(s.vardas() == "Ona", "vardas nuskaitytas per >>");
        tikrink(s.pavarde() == "Onaite", "pavarde nuskaityta per >>");
        tikrink(s.nd().size() == 3, "nuskaityti 3 ND pazymiai");
        tikrink(s.egzaminas() == 6, "egzaminas = 6 (paskutinis skaicius)");
    }

    skyrius("12. Isvesties operatorius <<");
    {
        studentas s = kurkTestini();
        s.calculateGalutinis();

        std::ostringstream oss;
        oss << s;
        const std::string rez = oss.str();

        tikrink(rez.find("Jonas") != std::string::npos, "isvestyje yra vardas");
        tikrink(rez.find("Jonaitis") != std::string::npos, "isvestyje yra pavarde");
        tikrink(rez.find("8.00") != std::string::npos,
            "isvestyje yra galutinis balas 8.00 (0.4*5 + 0.6*10)");
        tikrink(rez.size() >= 60, "isvestis suformatuota stulpeliais");
    }

    skyrius("13. Ivestis is failo (parseFromLine)");
    {
        studentas s;
        s.parseFromLine("Antanas Antanaitis 7 8 9 10 5", 4, 1);

        tikrink(s.vardas() == "Antanas", "vardas parsintas");
        tikrink(s.pavarde() == "Antanaitis", "pavarde parsinta");
        tikrink(s.nd().size() == 4, "parsinti 4 ND pazymiai");
        tikrink(s.egzaminas() == 5, "egzaminas = 5");

        bool metaBloga = false;
        try { studentas bad; bad.parseFromLine("Vardas Pavarde 99 5", 2, 7); }
        catch (const std::runtime_error&) { metaBloga = true; }
        tikrink(metaBloga, "meta runtime_error, kai pazymys uz ribu");

        bool metaTrukstama = false;
        try { studentas bad; bad.parseFromLine("TikVardas", 2, 3); }
        catch (const std::runtime_error&) { metaTrukstama = true; }
        tikrink(metaTrukstama, "meta runtime_error, kai truksta duomenu");
    }

    skyrius("14. Isvestis i faila (appendListTo)");
    {
        studentas s("Rasa", "Rasaite", { 6, 7, 8 }, 9);

        std::string buferis;
        s.appendListTo(buferis);

        tikrink(buferis.find("Rasa") != std::string::npos, "buferyje yra vardas");
        tikrink(buferis.find("Rasaite") != std::string::npos, "buferyje yra pavarde");
        tikrink(buferis.back() == '\n', "eilute baigiasi nauja eilute");
        tikrink(buferis.size() == 25 + 25 + 4 * 10 + 1,
            "eilutes ilgis atitinka formatavima (25+25+4*10+1)");

        // Ar isvesta eilute gali buti nuskaityta atgal?
        studentas s2;
        s2.parseFromLine(buferis, 3, 1);
        tikrink(s2.vardas() == s.vardas() && s2.nd() == s.nd() &&
            s2.egzaminas() == s.egzaminas(),
            "isvesta eilute korektiskai nuskaitoma atgal (round-trip)");
    }

    // ===================================================
    // 4. PAPILDOMI OPERATORIAI
    // ===================================================
    skyrius("15. Lyginimo operatoriai == ir !=");
    {
        studentas a("Jonas", "Jonaitis", { 5, 6 }, 7);
        studentas b("Jonas", "Jonaitis", { 5, 6 }, 7);
        studentas c("Petras", "Petraitis", { 5, 6 }, 7);

        tikrink(a == b, "vienodi objektai lygus (==)");
        tikrink(a != c, "skirtingi objektai nelygus (!=)");
        tikrink(!(a == c), "== grazina false skirtingiems");
    }

    skyrius("16. Rikiavimo operatoriai < ir >");
    {
        studentas a("Jonas", "Adamkus", { 5 }, 7);
        studentas b("Jonas", "Zukauskas", { 5 }, 7);

        tikrink(a < b, "Adamkus < Zukauskas (pagal pavarde)");
        tikrink(b > a, "Zukauskas > Adamkus");

        // Vienodos pavardes — lyginama pagal varda
        studentas c("Antanas", "Petraitis", { 5 }, 7);
        studentas d("Zigmas", "Petraitis", { 5 }, 7);
        tikrink(c < d, "esant vienodoms pavardems lyginama pagal varda");

        // Ar veikia su std::sort?
        std::vector<studentas> v = { b, a };
        std::sort(v.begin(), v.end());
        tikrink(v[0].pavarde() == "Adamkus", "std::sort su operator< veikia");
    }

    skyrius("17. Indeksavimo operatorius []");
    {
        studentas s("Jonas", "Jonaitis", { 3, 6, 9 }, 10);

        tikrink(s[0] == 3, "s[0] = 3");
        tikrink(s[1] == 6, "s[1] = 6");
        tikrink(s[2] == 9, "s[2] = 9");

        bool metaUzRibu = false;
        try { int x = s[99]; (void)x; }
        catch (const std::out_of_range&) { metaUzRibu = true; }
        tikrink(metaUzRibu, "meta out_of_range, kai indeksas uz ribu");
    }

    skyrius("18. Skaiciavimo metodas calculateGalutinis");
    {
        // ND: 2,4,6,8,10 -> vidurkis 6.0, mediana 6.0
        studentas s("Testas", "Testaitis", { 2, 4, 6, 8, 10 }, 5);
        s.calculateGalutinis();

        // 0.4*6.0 + 0.6*5 = 2.4 + 3.0 = 5.4
        tikrink(std::abs(s.galutinisVid() - 5.4) < 1e-9,
            "galutinisVid = 5.4 (0.4*6.0 + 0.6*5)");
        tikrink(std::abs(s.galutinisMed() - 5.4) < 1e-9,
            "galutinisMed = 5.4 (mediana 6.0)");

        // Nelyginis vs lyginis medianos atvejis
        // ND: 1,2,3,4 -> vidurkis 2.5, mediana (2+3)/2 = 2.5
        studentas s2("Testas", "Testaitis", { 1, 2, 3, 4 }, 10);
        s2.calculateGalutinis();
        tikrink(std::abs(s2.galutinisMed() - (0.4 * 2.5 + 0.6 * 10)) < 1e-9,
            "mediana lyginiam kiekiui = (2+3)/2 = 2.5");

        // Tuscias ND sarasas
        studentas s3;
        s3.setVardas("Tuscias");
        s3.setPavarde("Tusciaitis");
        s3.setEgzaminas(8);
        s3.calculateGalutinis();
        tikrink(std::abs(s3.galutinisVid() - 4.8) < 1e-9,
            "tuscias ND sarasas: galutinis = 0.6*8 = 4.8");
    }

    skyrius("19. Set'eriai ir isvalyk");
    {
        studentas s;
        s.setVardas("Naujas");
        s.setPavarde("Naujaitis");
        s.setNd({ 5, 6, 7 });
        s.setEgzaminas(9);

        tikrink(s.vardas() == "Naujas", "setVardas veikia");
        tikrink(s.nd().size() == 3, "setNd veikia");
        tikrink(s.egzaminas() == 9, "setEgzaminas veikia");

        s.addPazymys(10);
        tikrink(s.nd().size() == 4 && s.nd().back() == 10, "addPazymys veikia");

        bool metaBloga = false;
        try { s.setEgzaminas(50); }
        catch (const std::runtime_error&) { metaBloga = true; }
        tikrink(metaBloga, "setEgzaminas meta klaida, kai reiksme uz ribu");

        s.isvalyk();
        tikrink(s.vardas().empty() && s.nd().empty() && s.egzaminas() == 0,
            "isvalyk() istustina objekta");
    }

    skyrius("20. Veikimas konteineriuose (move semantika)");
    {
        std::vector<studentas> v;
        v.reserve(4);
        for (int i = 0; i < 4; ++i)
            v.push_back(studentas("Vardas" + std::to_string(i),
                "Pavarde" + std::to_string(i),
                { 5, 6, 7 }, 8));

        tikrink(v.size() == 4, "4 objektai idėti i vektoriu");

        // Perskirstymas — turi naudoti move, ne copy
        v.reserve(100);
        tikrink(v[0].vardas() == "Vardas0" && v[3].vardas() == "Vardas3",
            "duomenys islieka po vektoriaus perskirstymo");

        // Perkelimas tarp konteineriu
        std::vector<studentas> v2 = std::move(v);
        tikrink(v2.size() == 4, "vektorius perkeltas su std::move");
        tikrink(v2[2].vardas() == "Vardas2", "duomenys teisingi po perkelimo");
    }

    // ===================================================
    // Rezultatai
    // ===================================================
    const int galutinisSkaicius = studentas::gyvuObjektu;

    std::cout << "\n===============================================\n";
    std::cout << " REZULTATAI\n";
    std::cout << "-----------------------------------------------\n";
    std::cout << "  Praejo:  " << testuPraejo << "\n";
    std::cout << "  Krito:   " << testuKrito << "\n";
    std::cout << "  Is viso: " << (testuPraejo + testuKrito) << "\n";
    std::cout << "-----------------------------------------------\n";
    std::cout << "  Objektu nutekejimo patikra: "
        << (galutinisSkaicius == pradinisSkaicius
            ? "OK (visi objektai sunaikinti)"
            : "KLAIDA (liko nesunaikintu objektu)") << "\n";
    std::cout << "===============================================\n\n";

    return testuKrito == 0 && galutinisSkaicius == pradinisSkaicius;
}