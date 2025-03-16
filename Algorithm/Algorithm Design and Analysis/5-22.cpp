#include <algorithm>
#include <iostream>
#include <string>

class Solution {
public:
    std::string addBinary(const std::string &a, const std::string &b) {
        std::string result;
        int carry = 0;
        int i = a.length() - 1;
        int j = b.length() - 1;

        while (i >= 0 || j >= 0 || carry) {
            int sum = carry;
            if (i >= 0)
                sum += a[i--] - '0';
            if (j >= 0)
                sum += b[j--] - '0';
            result = char(sum % 2 + '0') + result;
            carry = sum / 2;
        }

        return result;
    }

    std::string subtractBinary(std::string a, std::string b) {
        std::string result;
        int borrow = 0;

        while (a.length() < b.length())
            a = "0" + a;
        while (b.length() < a.length())
            b = "0" + b;

        for (int i = a.length() - 1; i >= 0; i--) {
            int diff = (a[i] - '0') - (b[i] - '0') - borrow;
            if (diff < 0) {
                diff += 2;
                borrow = 1;
            } else {
                borrow = 0;
            }
            result = char(diff + '0') + result;
        }

        size_t pos = result.find_first_not_of('0');
        if (pos != std::string::npos) {
            result = result.substr(pos);
        } else {
            result = "0";
        }

        return result;
    }

    // 左移操作（相当于乘以2^k）
    std::string shiftLeft(const std::string &s, int k) {
        if (s == "0")
            return "0";
        return s + std::string(k, '0');
    }

    // 分治法计算二进制乘法
    std::string multiplyBinary(std::string a, std::string b) {
        a.erase(0, a.find_first_not_of('0'));
        b.erase(0, b.find_first_not_of('0'));

        if (a.empty())
            a = "0";
        if (b.empty())
            b = "0";

        if (a == "0" || b == "0")
            return "0";
        if (a == "1")
            return b;
        if (b == "1")
            return a;

        int n = std::max(a.length(), b.length());
        int power = 1;
        while (power < n)
            power *= 2;
        n = power;

        a = std::string(n - a.length(), '0') + a;
        b = std::string(n - b.length(), '0') + b;

        if (n == 1) {
            return std::to_string((a[0] - '0') * (b[0] - '0'));
        }

        int half = n / 2;
        std::string a_left = a.substr(0, half);
        std::string a_right = a.substr(half);
        std::string b_left = b.substr(0, half);
        std::string b_right = b.substr(half);

        // 计算c2, c0
        std::string c2 = multiplyBinary(a_left, b_left);
        std::string c0 = multiplyBinary(a_right, b_right);

        // 计算(a_left + a_right)和(b_left + b_right)
        std::string a_sum = addBinary(a_left, a_right);
        std::string b_sum = addBinary(b_left, b_right);

        // 计算c1 = (a_left + a_right) * (b_left + b_right) - c2 - c0
        std::string prod_sum = multiplyBinary(a_sum, b_sum);
        std::string c1 = subtractBinary(prod_sum, addBinary(c2, c0));

        // 合并结果: c2 * 2^n + c1 * 2^(n/2) + c0
        return addBinary(addBinary(shiftLeft(c2, n), shiftLeft(c1, half)), c0);
    }
};

int main() {
    std::string a, b;
    std::cout << "输入第一个二进制数: ";
    std::cin >> a;
    std::cout << "输入第二个二进制数: ";
    std::cin >> b;

    Solution solution;
    std::string result = solution.multiplyBinary(a, b);
    std::cout << "乘积: " << result << std::endl;

    return 0;
}
