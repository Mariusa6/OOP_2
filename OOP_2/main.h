#ifdef _WIN32
#include <windows.h>
#endif

#ifndef MAIN_H_DEFINED
#define MAIN_H_DEFINED

#include <iostream>     // cout, cin
#include <string>       // string
#include <vector>       // vector
#include <list>         // list
#include <deque>        // deque
#include <exception>    // exception

#define NUMBER_OF_PAZYMYS 15

template<typename Container>
struct splitResult
{
    Container kietiakai;
    Container vargsiukai;
};

const int minPazymys{ 1 };
const int maxPazymys{ 10 };

// Function declarations

// Menu functions
char askMenuChoice();

// Generate functions
std::vector<int> generatePazymiai(int m);

// Print functions
void printWelcome();
void printNameAsk();

// User input functions
int enterNumberOfStudents();
bool askIfMoreStudents();
studentas enterStudentas(int n);
std::string enterName(int n);
std::string enterSurname(int n);
int enterNumberOfPazymys(int n);
int enterPazymys(int n);
std::vector<int> enterPazymiai(int n, int m);
int enterEgzaminas(int n);
char askAverageOrMedian();
char askSortBy();
char askOutputChoice();
char askContainerChoice();

// Calculation functions
double mediana(const std::vector<int>& namuDarbai);

// File handling functions
std::string enterFileName();
std::string enterOutputFileName(std::string desc = "išvesties");

// Test functions
void testContainers(int n);

// Utility functions
bool isAllLetters(const std::string& input);

#endif