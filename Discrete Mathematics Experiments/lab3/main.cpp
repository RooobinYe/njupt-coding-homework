#include <vector>
#include <algorithm>
#include <iostream>
#include <print>
#include <numeric>
#include <cmath>

class Poset {
private:
    int n;

public:
    explicit Poset(int n) : n(n) {
    }

    // Get sorted factors of n
    [[nodiscard]] std::vector<int> getFactors() const {
        std::vector<int> factors;
        for (int i = 1; i <= std::sqrt(n); ++i) {
            if (n % i == 0) {
                factors.push_back(i);
                if (i != n / i) {
                    factors.push_back(n / i);
                }
            }
        }
        std::ranges::sort(factors);
        return factors;
    }

    // Get divisibility poset
    [[nodiscard]] std::vector<std::pair<int, int> > getDivisibility() const {
        std::vector<int> factors = getFactors();
        std::vector<std::pair<int, int> > poset;
        for (int factor1: factors) {
            for (int factor2: factors) {
                if (factor2 % factor1 == 0) {
                    poset.emplace_back(factor1, factor2);
                }
            }
        }
        return poset;
    }

    // Get cover relation
    [[nodiscard]] std::vector<std::pair<int, int> > getCover() const {
        std::vector<std::pair<int, int> > divisibility = getDivisibility();
        std::vector<int> factors = getFactors();
        std::vector<std::pair<int, int> > cover;

        for (const auto &pair: divisibility) {
            if (pair.first != pair.second) {
                bool isCover = true;
                for (int factor: factors) {
                    if (pair.first < factor && factor < pair.second && pair.second % factor == 0 && factor % pair.first
                        == 0) {
                        isCover = false;
                        break;
                    }
                }
                if (isCover) {
                    cover.push_back(pair);
                }
            }
        }
        return cover;
    }

    // Check if the poset is a complemented lattice
    [[nodiscard]] bool isComplementedLattice() const {
        std::vector<int> factors = getFactors();
        int zero = factors.front(); // Assuming the smallest element as zero (1 in this context)
        int one = factors.back(); // Assuming the largest element as one (n in this context)

        for (int factor1: factors) {
            bool foundComplement = false;
            for (int factor2: factors) {
                // Check if factor1 and factor2 are complements
                if (std::gcd(factor1, factor2) == zero && std::lcm(factor1, factor2) == one) {
                    foundComplement = true;
                    break;
                }
            }
            if (!foundComplement) return false;
        }
        return true;
    }


    // Display functions
    void displayFactors() const {
        const std::vector<int> factors = getFactors();
        std::print("Factors: ");
        for (const auto &factor: factors) {
            std::print("{} ", factor);
        }
        std::cout << "\n";
    }

    void displayDivisibility() const {
        const std::vector<std::pair<int, int> > divisibility = getDivisibility();
        std::print("Divisibility: ");
        for (const auto &pair: divisibility) {
            std::print("({}, {}) ", pair.first, pair.second);
        }
        std::cout << "\n";
    }

    void displayCover() const {
        const std::vector<std::pair<int, int> > cover = getCover();
        std::print("Cover: ");
        for (const auto &pair: cover) {
            std::print("({}, {}) ", pair.first, pair.second);
        }
        std::cout << "\n";
    }
};

int main() {
    int n;
    std::cin >> n;
    Poset poset(n);
    poset.displayFactors();
    poset.displayDivisibility();
    poset.displayCover();

    if (poset.isComplementedLattice()) {
        std::print("The poset is a complemented lattice.\n");
    } else {
        std::print("The poset is not a complemented lattice.\n");
    }

    return 0;
}
