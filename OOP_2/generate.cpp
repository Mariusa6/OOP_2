#include "generate.h"

std::mt19937& getRng()
{
    static std::mt19937 rng(std::random_device{}());
    return rng;
}

std::vector<int> generatePazymiai(int m) // m - pažymių skaičius
{
    std::vector<int> tmp;
    std::uniform_int_distribution<int> dist(minPazymys, maxPazymys);
    std::mt19937& rng = getRng();
    for (int i{ 0 }; i < m; i++)
    {
        tmp.push_back(dist(rng));
    }
    return tmp;
}