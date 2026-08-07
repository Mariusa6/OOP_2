#ifndef CALCULATE_H_DEFINED
#define CALCULATE_H_DEFINED

#include "main.h"
#include <vector>   // vector
#include <algorithm> // sort
#include <numeric>  // accumulate
#include <chrono>   // high_resolution_clock
#include <stdexcept> // runtime_error

// -------------------------------------------------------
// Template: sortStudentai
// std::list neturi random-access iteratorių, todėl
// naudojamas nario metodas .sort(); vektoriui ir deque
// naudojamas std::sort (if constexpr, C++17).
// -------------------------------------------------------
template<typename Container>
void sortStudentai(Container& studentai, char sortBy)
{
    auto cmp = [sortBy](const studentas& a, const studentas& b) -> bool
        {
            switch (sortBy)
            {
            case '1': return a.comparePagalVarda();
            case '2': return a.comparePagalPavarde();
            case '3': return a.comparePagalGalutini();
            default:  return false;
            }
        };

    if constexpr (std::is_same_v<Container, std::list<studentas>>)
        studentai.sort(cmp);
    else
        std::sort(studentai.begin(), studentai.end(), cmp);
}

// -------------------------------------------------------
// Template: splitResult
// Veikia su bet kokiu konteineriu (vector, list, deque).
// -------------------------------------------------------
template<typename Container>
splitResult<Container> splitStudentai(const Container& studentai)
{
    splitResult<Container> result;

    std::partition_copy(studentai.begin(), studentai.end(),
        std::back_inserter(result.kietiakai),
        std::back_inserter(result.vargsiukai),
        [](const studentas& s) { return s.galutinisVid >= 5.0; });

    return result;
}

// -------------------------------------------------------
// Template: partitionStudentai
// Veikia su bet kokiu konteineriu (vector, list, deque).
// -------------------------------------------------------

template<typename Container>
Container partitionStudentai(Container& studentai)
{
    auto it = std::stable_partition(studentai.begin(), studentai.end(),
        [](const studentas& s) { return s.galutinisVid >= 5.0; });

    Container vargsiukai(it, studentai.end());
    studentai.erase(it, studentai.end());

    return vargsiukai;
}

#endif // CALCULATE_H_DEFINED