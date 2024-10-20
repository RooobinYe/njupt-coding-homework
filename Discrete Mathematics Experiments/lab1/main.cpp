// Reference: https://github.com/doublingimba/Truth-table-of-logical-expression
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <stack>
#include <unordered_set>
#include <map>

// 运算符优先级和结合性
struct OperatorInfo {
    int precedence;
    bool rightAssociative;
};

std::map<char, OperatorInfo> operators = {
    {'!', {4, true}},  // NOT
    {'*', {3, false}}, // AND
    {'|', {2, false}}, // OR
    {'>', {1, true}},  // Implication
    {'-', {0, false}}  // Biconditional
};

// 函数：检查字符是否为运算符
bool isOperator(char c) {
    return operators.find(c) != operators.end();
}

// 函数：将中缀表达式转换为后缀表达式（逆波兰表示法）
std::vector<std::string> infixToPostfix(const std::string& expr) {
    std::vector<std::string> output;
    std::stack<char> opStack;

    size_t i = 0;
    while (i < expr.length()) {
        char token = expr[i];

        if (isspace(token)) {
            i++;
            continue;
        }

        if (isdigit(token)) {
            // 直接数字（0 或 1）
            output.emplace_back(std::string(1, token));
            i++;
        }
        else if (isalpha(token)) {
            // 变量（假设为单个字符）
            output.emplace_back(std::string(1, tolower(token)));
            i++;
        }
        else if (token == '(') {
            opStack.push(token);
            i++;
        }
        else if (token == ')') {
            while (!opStack.empty() && opStack.top() != '(') {
                output.emplace_back(std::string(1, opStack.top()));
                opStack.pop();
            }
            if (!opStack.empty() && opStack.top() == '(') {
                opStack.pop();
            }
            else {
                std::cerr << "Error: mismatched brackets." << std::endl;
                exit(1);
            }
            i++;
        }
        else if (isOperator(token)) {
            while (!opStack.empty() && isOperator(opStack.top())) {
                OperatorInfo currentOp = operators[token];
                OperatorInfo stackOp = operators[opStack.top()];
                if ((currentOp.rightAssociative && currentOp.precedence < stackOp.precedence) ||
                    (!currentOp.rightAssociative && currentOp.precedence <= stackOp.precedence)) {
                    output.emplace_back(std::string(1, opStack.top()));
                    opStack.pop();
                }
                else {
                    break;
                }
            }
            opStack.push(token);
            i++;
        }
        else {
            std::cerr << "Error: Unknown character '" << token << "'." << std::endl;
            exit(1);
        }
    }

    // 弹出所有剩余的运算符
    while (!opStack.empty()) {
        if (opStack.top() == '(' || opStack.top() == ')') {
            std::cerr << "Error: Inconsistent symbols." << std::endl;
            exit(1);
        }
        output.emplace_back(std::string(1, opStack.top()));
        opStack.pop();
    }

    return output;
}

// 函数：评估后缀表达式
char evaluatePostfix(const std::vector<std::string>& postfix, const std::vector<int>& chval) {
    std::stack<char> evalStack;

    for (const auto& token : postfix) {
        if (token == "0" || token == "1") {
            evalStack.push(token[0]);
        }
        else if (isalpha(token[0])) {
            int idx = token[0] - 'a';
            evalStack.push(chval[idx] ? '1' : '0');
        }
        else if (isOperator(token[0])) {
            char result;
            if (token[0] == '!') {
                if (evalStack.empty()) {
                    std::cerr << "Error: operator '!' Missing operand." << std::endl;
                    exit(1);
                }
                char a = evalStack.top(); evalStack.pop();
                result = (a == '0') ? '1' : '0';
                evalStack.push(result);
            }
            else {
                // 二目运算符
                if (evalStack.size() < 2) {
                    std::cerr << "Error: operator '" << token << "' Missing operand." << std::endl;
                    exit(1);
                }
                char b = evalStack.top(); evalStack.pop();
                char a = evalStack.top(); evalStack.pop();
                switch (token[0]) {
                    case '*': // AND
                        result = (a == '1' && b == '1') ? '1' : '0';
                        break;
                    case '|': // OR
                        result = (a == '1' || b == '1') ? '1' : '0';
                        break;
                    case '>': // Implication
                        result = (a == '1' && b == '0') ? '0' : '1';
                        break;
                    case '-': // Biconditional
                        result = (a == b) ? '1' : '0';
                        break;
                    default:
                        std::cerr << "Error: Unknown operator '" << token << "'." << std::endl;
                        exit(1);
                }
                evalStack.push(result);
            }
        }
        else {
            std::cerr << "Error: Unknown token '" << token << "'." << std::endl;
            exit(1);
        }
    }

    if (evalStack.size() != 1) {
        std::cerr << "Error: Multiple values in the stack after expression evaluation." << std::endl;
        exit(1);
    }

    return evalStack.top();
}

// 函数：提取表达式中的唯一变量
std::vector<char> extractVariables(const std::string& expr) {
    std::unordered_set<char> varSet;
    for (char c : expr) {
        if (isalpha(c)) {
            varSet.insert(tolower(c));
        }
    }
    std::vector<char> vars(varSet.begin(), varSet.end());
    std::sort(vars.begin(), vars.end());
    return vars;
}

// 函数：生成一个 minterm 或 maxterm 的字符串表示
std::string generateTerm(const std::vector<char>& variables, const std::vector<int>& chval, bool isMinterm) {
    std::string term = isMinterm ? "(" : "(";
    for (size_t i = 0; i < variables.size(); ++i) {
        if (isMinterm) {
            // 对于 minterm，如果变量为 1，则直接使用变量；如果为 0，则使用取反
            term += (chval[variables[i] - 'a'] ? std::string(1, variables[i]) : "!" + std::string(1, variables[i]));
            if (i < variables.size() - 1) {
                term += "*";
            }
        }
        else {
            // 对于 maxterm，如果变量为 0，则直接使用变量；如果为 1，则使用取反
            term += (chval[variables[i] - 'a'] ? "!" + std::string(1, variables[i]) : std::string(1, variables[i]));
            if (i < variables.size() - 1) {
                term += "|";
            }
        }
    }
    term += ")";
    return term;
}

int main() {
    std::string inputExpr;
    std::cout << "请输入逻辑表达式 (使用 !, *, |, >, - 和括号): ";
    std::cin >> inputExpr;

    // 提取变量
    std::vector<char> variables = extractVariables(inputExpr);
    size_t numVars = variables.size();
    // if (numVars > 20) {
    //     std::cerr << "错误：变量数量过多 (最多支持20个变量)。" << std::endl;
    //     return 1;
    // }

    // 转换为后缀表达式
    std::vector<std::string> postfix = infixToPostfix(inputExpr);

    // 输出表头
    for (char var : variables) {
        std::cout << var << "\t";
    }
    std::cout << inputExpr << std::endl;

    // 总组合数
    int totalCombinations = 1 << numVars; // 2^n

    // 用于存储 minterms 和 maxterms
    std::vector<std::string> minterms;
    std::vector<std::string> maxterms;

    // 迭代所有可能的变量赋值
    for (int i = 0; i < totalCombinations; ++i) {
        std::vector<int> chval(26, 0); // 假设变量为 a-z
        // 分配真值
        for (size_t j = 0; j < numVars; ++j) {
            // 从高位到低位分配
            chval[variables[j] - 'a'] = (i & (1 << (numVars - 1 - j))) ? 1 : 0;
            std::cout << chval[variables[j] - 'a'] << "\t";
        }

        // 评估表达式
        char result = evaluatePostfix(postfix, chval);
        std::cout << result << std::endl;

        // 根据结果添加到 minterms 或 maxterms
        if (result == '1') {
            minterms.push_back(generateTerm(variables, chval, true));
        }
        else {
            maxterms.push_back(generateTerm(variables, chval, false));
        }
    }

    // 生成 PDNF
    std::string PDNF = "";
    if (!minterms.empty()) {
        PDNF = minterms[0];
        for (size_t i = 1; i < minterms.size(); ++i) {
            PDNF += "|" + minterms[i];
        }
    }
    else {
        PDNF = "0"; // 如果没有任何 minterm，则 PDNF 为 0
    }

    // 生成 PCNF
    std::string PCNF = "";
    if (!maxterms.empty()) {
        PCNF = maxterms[0];
        for (size_t i = 1; i < maxterms.size(); ++i) {
            PCNF += "*" + maxterms[i];
        }
    }
    else {
        PCNF = "1"; // 如果没有任何 maxterm，则 PCNF 为 1
    }

    // 输出 PDNF 和 PCNF
    std::cout << "主析取范式 (PDNF): " << PDNF << std::endl;
    std::cout << "主合取范式 (PCNF): " << PCNF << std::endl;

    return 0;
}
