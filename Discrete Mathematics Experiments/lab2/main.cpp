#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm>

/**
 * @brief Checks if the size of the vector is even.
 *
 * This function takes a vector of integers and returns true if
 * the number of elements in the vector is even. Otherwise, it
 * returns false.
 *
 * @param relation A vector of integers.
 * @return true if the size of the vector is even, false otherwise.
 */
bool isEven(const std::vector<int>& relation) {
    return relation.size() % 2 == 0;
}

/**
 * @brief Preprocessing the relation.
 *
 * Generates an adjacency matrix for the given relation and set.
 *
 * @param set The set of elements.
 * @param relation The relation represented as pairs of elements.
 * @return The adjacency matrix representing the relation.
 */
std::vector<std::vector<int>> preRelation(const std::vector<int>& set, const std::vector<int>& relation) {
    std::vector<std::vector<int>> preMatrix(set.size(), std::vector<int>(set.size(), 0));
    for (size_t i = 0; i < relation.size(); i += 2) {
        if (i + 1 < relation.size()) {
            int a = relation[i];
            for(size_t j = 0; j < set.size(); j++) {
                if (set[j] == a) {
                    a = j;
                    break;
                }
            }
            int b = relation[i + 1];
            for(size_t j = 0; j < set.size(); j++) {
                if (set[j] == b) {
                    b = j;
                    break;
                }
            }
            preMatrix[a][b] = 1;
        }
    }
    return preMatrix;
}

/**
 * @brief Multiplies two matrices.
 *
 * @param A The first matrix.
 * @param B The second matrix.
 * @return The result of matrix multiplication.
 */
std::vector<std::vector<int>> multiplyMatrices(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B) {
    if (A.empty() || B.empty() || A[0].size() != B.size()) {
        throw std::invalid_argument("Matrix dimensions are incompatible for multiplication.");
    }

    std::vector<std::vector<int>> C(A.size(), std::vector<int>(B[0].size(), 0));
    for (size_t i = 0; i < A.size(); ++i) {
        for (size_t j = 0; j < B[0].size(); ++j) {
            for (size_t k = 0; k < A[0].size(); ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
            C[i][j] = C[i][j] > 0 ? 1 : 0; // For boolean matrix multiplication
        }
    }
    return C;
}

/**
 * @brief Checks for reflexivity.
 *
 * @param set The set of elements.
 * @param relation The relation represented as pairs of elements.
 * @return True if the relation is reflexive, false otherwise.
 */
bool isReflexivity(const std::vector<int>& set, const std::vector<int>& relation) {
    auto matrix = preRelation(set, relation);
    for (size_t i = 0; i < set.size(); i++) {
        if (matrix[i][i] != 1) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Checks for symmetry.
 *
 * @param set The set of elements.
 * @param relation The relation represented as pairs of elements.
 * @return True if the relation is symmetric, false otherwise.
 */
bool isSymmetry(const std::vector<int>& set, const std::vector<int>& relation) {
    auto matrix = preRelation(set, relation);
    for (size_t i = 0; i < set.size(); i++) {
        for (size_t j = 0; j < set.size(); j++) {
            if (matrix[i][j] != matrix[j][i]) {
                return false;
            }
        }
    }
    return true;
}

/**
 * @brief Checks for transitivity.
 *
 * @param set The set of elements.
 * @param relation The relation represented as pairs of elements.
 * @return True if the relation is transitive, false otherwise.
 */
bool isTransitivity(const std::vector<int>& set, const std::vector<int>& relation) {
    auto matrix = preRelation(set, relation);
    auto product = multiplyMatrices(matrix, matrix);
    for (size_t i = 0; i < set.size(); i++) {
        for (size_t j = 0; j < set.size(); j++) {
            if (product[i][j] == 1 && matrix[i][j] != 1) {
                return false;
            }
        }
    }
    return true;
}

/**
 * @brief Checks for irreflexivity.
 *
 * @param set The set of elements.
 * @param relation The relation represented as pairs of elements.
 * @return True if the relation is irreflexive, false otherwise.
 */
bool isIrreflexivity(const std::vector<int>& set, const std::vector<int>& relation) {
    auto matrix = preRelation(set, relation);
    for (size_t i = 0; i < set.size(); i++) {
        if (matrix[i][i] != 0) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Checks for antisymmetry.
 *
 * @param set The set of elements.
 * @param relation The relation represented as pairs of elements.
 * @return True if the relation is antisymmetric, false otherwise.
 */
bool isAntisymmetry(const std::vector<int>& set, const std::vector<int>& relation) {
    auto matrix = preRelation(set, relation);
    for (size_t i = 0; i < set.size(); i++) {
        for (size_t j = 0; j < set.size(); j++) {
            if (i != j && matrix[i][j] == 1 && matrix[j][i] == 1) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    std::vector<int> set;
    std::vector<int> relation;

    int n;
    std::cout << "Enter the number of elements in the set: ";
    std::cin >> n;
    set.resize(n);

    std::cout << "Please input the set elements: ";
    for (int i = 0; i < n; ++i) {
        std::cin >> set[i];
    }

    std::cout << "Enter the number of elements in the relation: ";
    std::cin >> n;
    relation.resize(n);

    std::cout << "Please input the relation elements (as pairs): ";
    for (int i = 0; i < n; ++i) {
        std::cin >> relation[i];
    }

    if (isEven(relation)) {
        bool hasProperty = false;
        if (isReflexivity(set, relation)) {
            std::cout << "Reflexivity" << std::endl;
            hasProperty = true;
        }
        if (isSymmetry(set, relation)) {
            std::cout << "Symmetry" << std::endl;
            hasProperty = true;
        }
        if (isTransitivity(set, relation)) {
            std::cout << "Transitivity" << std::endl;
            hasProperty = true;
        }
        if (isAntisymmetry(set, relation)) {
            std::cout << "Antisymmetry" << std::endl;
            hasProperty = true;
        }
        if (isIrreflexivity(set, relation)) {
            std::cout << "Irreflexivity" << std::endl;
            hasProperty = true;
        }
        if (!hasProperty) {
            std::cout << "None" << std::endl;
        }
    } else {
        std::cout << "Relation size must be even." << std::endl;
    }

    return 0;
}
