# Studentų Informacinė Sistema OOP_Marius_Augustinas
VU ISI Objektinio programavimo kurso laboratoriniai darbai

---

## Versijų istorija

| Versija | Pagrindiniai pakeitimai |
|---|---|
| **v0.1** | Pradinė realizacija — `std::vector`, rankinis įvedimas, vidurkio arba medianos skaičiavimas |
| **v0.2** | Failo skaitymas ir rašymas, rikiavimas, abu galutiniai balai skaičiuojami vienu metu |
| **v0.3** | Kodas suskaidytas į modulius (`*.h`/`*.cpp`), išimčių valdymas (`try`/`catch`) |
| **v0.4** | Studentų skirstymas į dvi grupes, spartos testavimas (1 ir 2 tyrimas), failų generatorius |
| **v0.5** | Programa pertvarkyta į `template<typename Container>` — vienas kodo kelias `vector`/`list`/`deque`, 3-ias tyrimas (konteinerių palyginimas) |
| **v1.0** | Trijų skirstymo strategijų realizacija ir spartos palyginimas, atminties analizė, `CMakeLists.txt` |
| **v1.1** | `struct studentas` → `class studentas`; enkapsuliacija, konstruktoriai, `friend` operatoriai; I/O optimizacija (`from_chars`/`to_chars`); struct vs class spartos ir optimizavimo flag'ų tyrimas |

---

# v1.1 — Perėjimas nuo `struct` prie `class`

## Techninė aplinka

| Parametras | Reikšmė |
|---|---|
| Procesorius | Intel Core i7-8565U (4 branduoliai / 8 gijų, iki 4.6 GHz) |
| RAM | 8 GB DDR4 |
| Saugykla | 250 GB SSD |
| OS | Windows 11 |
| **`class` versija** | Visual Studio 2022, MSVC (`cl.exe`), x64 Release |
| **`struct` versija** | VS Code + MinGW-w64 (`g++`), x64 |

---

## 1. Klasės realizacija

### Kas pasikeitė

Ankstesnė struktūra:
```cpp
struct studentas {
    std::string vardas;
    std::string pavarde;
    std::vector<int> namuDarbai;
    int egzaminas;
    double galutinisVid;
    double galutinisMed;
};
```

Nauja klasė (`studentas.h`) — duomenys `private`, prieiga tik per interfeisą:
```cpp
class studentas {
private:
    std::string vardas_;
    std::string pavarde_;
    std::vector<int> nd_;
    int egzaminas_;
    double galutinisVid_;
    double galutinisMed_;

    static double vidurkis(const std::vector<int>& nd);
    static double mediana(const std::vector<int>& nd);
    static inline bool pazymysTinkamas(int p);

public:
    static constexpr int minPazymys = 1;
    static constexpr int maxPazymys = 10;
    static constexpr double namuDarbaiSvoris = 0.4;
    static constexpr double egzaminasSvoris = 0.6;
    // ...
};
```

### Konstruktoriai ir destruktorius

| Elementas | Realizacija |
|---|---|
| **Numatytasis konstruktorius** | `studentas()` — inicijuoja visus laukus per member initializer list |
| **Pilnas konstruktorius** | `studentas(vardas, pavarde, nd, egzaminas)` — validuoja duomenis, meta `std::invalid_argument` / `std::runtime_error` |
| **Konstruktorius iš srauto** | `explicit studentas(std::istream&)` — kreipiasi į `readStudentas` |
| **Destruktorius** | Generuojamas kompiliatoriaus (**Rule of Zero**) |
| **Kopijavimo / perkėlimo operacijos** | Generuojamos kompiliatoriaus |

**Kodėl destruktorius nedeklaruotas aiškiai.** Klasė nevaldo jokių resursų rankiniu būdu — visi laukai yra `std::string` ir `std::vector`, kurie patys tvarko savo atmintį. Aiškiai apibrėžtas (net ir tuščias) destruktorius `~studentas() {}` **užblokuotų automatinį move konstruktoriaus generavimą**, todėl `std::sort` ir `std::stable_partition` su dideliais konteineriais **kopijuotų** kiekvieną objektą vietoj perkėlimo — kiekviena kopija reikštų tris atminties alokacijas (`vardas_`, `pavarde_`, `nd_`). Todėl laikomasi **Rule of Zero**: kompiliatorius sugeneruoja visas penkias specialiąsias funkcijas, įskaitant `studentas(studentas&&)`.

### Get'eriai ir set'eriai

```cpp
// Get'eriai — inline, grąžina nuorodas kur įmanoma (be kopijų)
inline const std::string& vardas() const { return vardas_; }
inline const std::vector<int>& nd() const { return nd_; }
inline double galutinisVid() const { return galutinisVid_; }

// Set'erio vaidmenį atlieka konstruktorius ir įvesties metodai
std::istream& readStudentas(std::istream& is, int ndCount = 0, int lineNumber = 0);
void parseFromLine(const std::string& line, int ndCount, int lineNumber);
```

Atskirų `setVardas()` / `setEgzaminas()` metodų sąmoningai **nėra** — objektas kuriamas per konstruktorių arba nuskaitomas iš srauto, todėl visada yra pilnai apibrėžtoje ir validuotoje būsenoje.

### `friend` operatoriai

```cpp
friend std::istream& operator>>(std::istream& is, studentas& s);
friend std::ostream& operator<<(std::ostream& os, const studentas& s);
```

Jie turi tiesioginę prieigą prie privačių laukų, todėl `printStudentai` ir `writeStudentaiToFile` supaprastėjo iki `std::cout << s << "\n";`.

### Ne-nariai lyginimo funkcijos

Operuoja su `studentas` objektais, bet nėra klasės nariai — naudoja tik viešą interfeisą:
```cpp
bool comparePagalVarda(const studentas& s1, const studentas& s2);
bool comparePagalPavarde(const studentas& s1, const studentas& s2);
bool comparePagalEgzamina(const studentas& s1, const studentas& s2);
bool comparePagalVidurki(const studentas& s1, const studentas& s2);
bool comparePagalMediana(const studentas& s1, const studentas& s2);
```

### Pakeitimai kitame kode

| Failas | Kas pakeista |
|---|---|
| `main.h` | Pridėtas `#include "studentas.h"`; pašalintos globalios `minPazymys`/`maxPazymys` (dabar klasės konstantos) |
| `calculate.h` | `sortStudentai` lambda kviečia laisvas `compare*` funkcijas; `splitStudentai`/`partitionStudentai` naudoja `s.galutinisVid()` |
| `file.h` | `parseStudentasLine` pašalinta — jos vaidmenį perėmė `studentas::parseFromLine` |
| `generate.h` | Laukų priskyrimą pakeitė pilnas konstruktorius |
| `print.h` | Naudoja `operator<<` |
| `enter.cpp` | `enterStudentas` grąžina objektą per konstruktorių; naudoja `studentas::minPazymys` |
| `calculate.cpp` | **Ištrintas** — `mediana` tapo privačiu klasės metodu |

---

## `struct` vs `class` spartos palyginimas

**Metodika:** fiksuotas konteineris `std::vector`, greičiausia skirstymo strategija (3 strategija — `stable_partition` + blokinis `erase`), failai `studentai100000.txt` ir `studentai1000000.txt`. Kiekvienas matavimas kartotas **2 kartus**, lentelėse — vidurkiai.

### 100 000 studentų

| Žingsnis | struct `-O1` | class `/O1` | struct `-O2` | class `/O2` |
|---|---|---|---|---|
| Failo skaitymas | 0.313628 | **0.275893** | 0.333408 | **0.255890** |
| Galutinio balo skaičiavimas | 0.0232695 | **0.022625** | 0.0250269 | **0.027367** |
| Rikiavimas | 0.0317756 | **0.014289** | 0.0264796 | **0.017697** |
| Skirstymas | **0.024133** | 0.028282 | **0.025452** | 0.038317 |
| Kietiakai rašymas | 0.094059 | **0.028922** | 0.203233 | **0.033234** |
| Vargsiukai rašymas | 0.072607 | **0.022236** | 0.157009 | **0.028214** |
| **Visas testavimas** | 0.563690 | **0.397912** | 0.776818 | **0.407080** |

### 1 000 000 studentų

| Žingsnis | struct `-O1` | class `/O1` | struct `-O2` | class `/O2` |
|---|---|---|---|---|
| Failo skaitymas | 3.240460 | **2.811455** | 3.573010 | **2.255440** |
| Galutinio balo skaičiavimas | 0.314891 | **0.278809** | 0.241921 | **0.225568** |
| Rikiavimas | 0.518813 | **0.175206** | 0.371181 | **0.154773** |
| Skirstymas | **0.340469** | 0.367268 | **0.375373** | 0.338150 |
| Kietiakai rašymas | 1.227502 | **0.331729** | 3.705215 | **1.061501** |
| Vargsiukai rašymas | 0.878340 | **0.799635** | 2.139240 | **0.674213** |
| **Visas testavimas** | 6.554700 | **4.771115** | 10.627750 | **4.715710** |

¹ `struct` versijoje skaičiavimas ir rikiavimas nebuvo matuojami atskirai (jie įėjo į bendrą laiką), todėl atskirų reikšmių nėra. Bendro laiko palyginimas nuo to nenukenčia — abi versijos atlieka tą patį darbą.

### Bendro laiko santrauka

| Duomenų dydis | Optimizavimas | struct | class | **Pagreitėjimas** |
|---|---|---|---|---|
| 100 000 | O1 | 0.5637 s | 0.3979 s | **1.42×** |
| 100 000 | O2 | 0.7768 s | 0.4071 s | **1.91×** |
| 1 000 000 | O1 | 6.5547 s | 4.7711 s | **1.37×** |
| 1 000 000 | O2 | 10.6278 s | 4.7157 s | **2.25×** |

---

## 4. Optimizavimo flag'ų analizė

### Vykdomojo failo dydis

| Versija | Optimizavimas | `.exe` dydis |
|---|---|---|
| class (MSVC) | `/O1` (Minimize Size) | **120 KB** |
| class (MSVC) | `/O2` (Maximize Speed) | **160 KB** |
| struct (MinGW g++) | `-O1` | **371 KB** |
| struct (MinGW g++) | `-O2` | **336 KB** |

### Greitis vs dydis

| Versija | Flag | `.exe` | 100k (s) | 1M (s) |
|---|---|---|---|---|
| class | `/O1` | 120 KB | 0.3979 | 4.7711 |
| class | `/O2` | 160 KB | 0.4071 | **4.7157** |
| struct | `-O1` | 371 KB | **0.5637** | **6.5547** |
| struct | `-O2` | 336 KB | 0.7768 | 10.6278 |

---

## 5. Rezultatų komentarai

### Kodėl `class` versija greitesnė

Pagrindinis pagreitėjimas ateina **ne iš `class` vietoj `struct`** — pats perėjimas prie klasės spartos nekeičia, nes get'eriai yra `inline` ir kompiliatorius juos visiškai pašalina. Skirtumą lemia trys dalykai, kuriuos klasės realizacija padarė įmanomus arba natūraliais:

1. **Rašymo optimizacija** (didžiausias efektas). `class` versija su 1M studentų kietiakų failą rašo per 0.33 s (`/O1`), o `struct` — per 1.23 s (`-O1`) ir net 3.71 s (`-O2`). Priežastis — `appendListTo` su `std::to_chars` ir 1 MB buferiu vietoj `std::setw` manipuliatorių ir atskirų `<<` operacijų kiekvienam iš 16 laukų × 1M eilučių.

2. **Skaitymo optimizacija.** `parseFromLine` su `std::from_chars` vietoj `std::istringstream` + `operator>>`. Su 1M studentų × 16 skaičių tai ~16M parsinimo operacijų be locale ir stream būsenos apdorojimo.

3. **Move semantika.** Rule of Zero užtikrina, kad `std::sort` ir `stable_partition` perkelia objektus, o ne kopijuoja.

---

## Failų struktūra

```
.
├── CMakeLists.txt
├── LICENSE
├── README.md
├── data/                       # Testiniai failai (archyvas)
│   ├── studentai1000.txt
│   ├── studentai10000.txt
│   ├── studentai100000.txt
│   ├── studentai1000000.txt
│   └── studentai10000000.txt
├── studentas.h / studentas.cpp # class studentas — duomenys, skaičiavimas, I/O
├── main.h                      # splitResult<T>, konstantos, funkcijų deklaracijos
├── OOP_2.cpp                   # main() + runProgram<Container>() šablonas
├── menu.h / menu.cpp           # Meniu ir pasirinkimų funkcijos
├── enter.h / enter.cpp         # Rankinio įvedimo funkcijos
├── generate.h / generate.cpp   # Generavimo funkcijos
├── file.h / file.cpp           # Failo skaitymo ir rašymo šablonai
├── calculate.h                 # calculateGalutinis<T>, sortStudentai<T>, splitStudentai<T>
├── output.h                    # outputStudentai<T>
├── print.h / print.cpp         # Spausdinimo funkcijos
└── test.h / test.cpp           # Spartos testavimo funkcijos
```

---

## Naudojimosi instrukcija

### Paleidimas

```bash
.\programa.exe          # Windows
./programa              # Linux/Mac
```

### Pirmas ekranas — konteinerio pasirinkimas

```
Pasirinkite konteinerį:
1. vector
2. list
3. deque
```

Pasirinkimas galioja visai sesijai.

### Meniu pasirinkimai

| Nr. | Veiksmas |
|---|---|
| 1 | Įvesti studentus ranka |
| 2 | Generuoti tik pažymius (vardai įvedami rankiniu būdu) |
| 3 | Generuoti studentus automatiškai |
| 4 | Nuskaityti iš failo |
| 5 | Generuoti testinį studentų failą |
| 6 | Spartos testas — failų kūrimas (1 tyrimas) |
| 7 | Spartos testas — duomenų apdorojimas (2 tyrimas) |
| 8 | Spartos testas — konteinerių palyginimas (3 tyrimas) |
| 0 | Baigti |

### Failo formatas

```
Vardas                   Pavarde                         ND1       ND2  ...  Egzaminas
VardasNR1                PavardeNR1                        7         3  ...          5
```

Antraštė naudojama automatiškai nustatyti namų darbų stulpelių skaičių pagal `ND` žymėjimą.

---

## Įdiegimo instrukcija

### Reikalavimai
- C++17 palaikantis kompiliatorius (MSVC 2019+, g++ 8+, clang++ 7+)
- CMake 3.16+

### CMake (visos OS)

```bash
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

### g++ tiesiogiai

```bash
g++ -std=c++17 -O2 -Wall OOP_2.cpp studentas.cpp menu.cpp enter.cpp generate.cpp file.cpp print.cpp test.cpp -o programa.exe
```

---

## Pakeitimai v1.0 → v1.1

- `struct studentas` pakeista į `class studentas` su privačiais laukais (`vardas_`, `pavarde_`, `nd_`, ...) ir viešu interfeisu
- Realizuoti trys konstruktoriai: numatytasis, pilnas (su validacija) ir iš `std::istream`
- Destruktorius, kopijavimo ir perkėlimo operacijos paliktos kompiliatoriui (**Rule of Zero**) — taip išsaugoma move semantika, kritinė `std::sort` spartai
- Pridėti `inline` get'eriai, grąžinantys `const` nuorodas (be kopijų)
- `friend operator>>` ir `friend operator<<` — I/O per srautus
- Penkios ne-nariai `compare*` funkcijos rikiavimui
- Konstantos (`minPazymys`, `maxPazymys`, svoriai) perkeltos į klasę kaip `static constexpr`
- `calculate.cpp` ištrintas — `mediana` tapo privačiu klasės metodu
- I/O optimizacija: `std::from_chars` skaitymui, `std::to_chars` + buferis rašymui
- Klaidų pranešimai formuojami tik metant išimtį (anksčiau — kiekvienam pažymiui)
- Atliktas `struct` vs `class` ir `O1` vs `O2` spartos bei `.exe` dydžio tyrimas