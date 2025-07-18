#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

std::vector<int> generateNumbers(int count, int min, int max) {
    std::vector<int> pool;
    for (int i = min; i <= max; ++i)
        pool.push_back(i);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(pool.begin(), pool.end(), gen);

    return std::vector<int>(pool.begin(), pool.begin() + count);
}

int main() {
    std::cout << "EuroJackpot Zahlen:\n";
    for(int i=0; i<10; i++) {
        // 5 Hauptzahlen aus 1–50
        std::vector<int> mainNumbers = generateNumbers(5, 1, 50);
        std::sort(mainNumbers.begin(), mainNumbers.end());

        // 2 Eurozahlen aus 1–12
        std::vector<int> euroNumbers = generateNumbers(2, 1, 12);
        std::sort(euroNumbers.begin(), euroNumbers.end());

        std::cout << "Hauptzahlen: ";
        for (int n : mainNumbers)
            std::cout << n << " ";

        std::cout << "    Eurozahlen: ";
        for (int e : euroNumbers)
            std::cout << e << " ";

        std::cout << std::endl;
    }
    return 0;
}