#include <algorithm>
#include <utility>
#include <vector>
class Solution {
public:
    std::vector<std::pair<int, int>> maxAcountOfP(std::vector<std::pair<int, int>> P) {
        if (P.empty())
            return {};

        // Sort points by x-coordinate (ascending), and for same x, by y-coordinate (descending)
        std::sort(P.begin(), P.end(), [](const std::pair<int, int> &a, const std::pair<int, int> &b) {
            if (a.first != b.first) {
                return a.first < b.first;
            } else {
                return a.second > b.second;
            }
        });

        return findMaxPoints(P, 0, P.size() - 1);
    }

private:
    std::vector<std::pair<int, int>> findMaxPoints(const std::vector<std::pair<int, int>> &P, int left, int right) {
        // Base case: single point is always a maximum point
        if (left == right) {
            return {P[left]};
        }

        // Divide
        int mid = left + (right - left) / 2;
        std::vector<std::pair<int, int>> leftMax = findMaxPoints(P, left, mid);
        std::vector<std::pair<int, int>> rightMax = findMaxPoints(P, mid + 1, right);

        // Merge results
        return merge(leftMax, rightMax);
    }

    std::vector<std::pair<int, int>> merge(const std::vector<std::pair<int, int>> &leftMax,
                                           const std::vector<std::pair<int, int>> &rightMax) {
        std::vector<std::pair<int, int>> result;

        // All points in the left half need to be checked
        for (const auto &leftPoint: leftMax) {
            bool isDominated = false;

            // Check if the left point is dominated by any point in the right half
            for (const auto &rightPoint: rightMax) {
                if (rightPoint.first >= leftPoint.first && rightPoint.second >= leftPoint.second) {
                    isDominated = true;
                    break;
                }
            }

            if (!isDominated) {
                result.push_back(leftPoint);
            }
        }

        // All points in the right half are added (they cannot be dominated by left points due to sorting)
        for (const auto &rightPoint: rightMax) {
            result.push_back(rightPoint);
        }

        return result;
    }
};
