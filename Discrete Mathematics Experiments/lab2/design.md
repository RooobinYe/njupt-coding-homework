# 集合上二元关系性质的判定

## 二元关系的性质

二元关系指的是在集合 X 上的二元关系 R 的一些特殊性质。

此时的对角线指的是从左上到右下的一条线。

- 自反性：只要有一个对角线元素为 0：即不满足
- 对称性：只要有一对沿着对角线上的元素不相等：即不满足对称性
- 传递性：// TODO
- 反自反性：只要有一个对角线元素为 1：即不满足
- 反对称性：只要有一对沿着对角线上的元素相等：即不满足反对称性

## `main.cpp`

### `int main()`

Variables:

`std::vector<int> set`: 集合 X
`std::vector<int> relation`: 二元关系 R。它的本质应该是一个矩阵。

### `bool isOven(std::string relation)`

### 5 isFunctions (check relationships)

### `std::vector<std::vector<int> > preRelation(std::vector<int> set, std::vector<int> relation)`