#include <vector>
#include <iostream>
#include <algorithm>

void test_sort() {
    std::vector<int> data{4,2,6,1,-1,5};
    std::cout << "data is ";
    for (auto v : data) {
        std::cout << v << " ";
    }
    std::cout << std::endl;

    std::sort(data.begin(), data.end());
    std::cout << "after sort, data is ";
    for (auto v : data) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
}
class DistanceTo{
public:
    explicit DistanceTo(int center)
    : center_{center}
    {}
    bool operator()(int a, int b) const{
        return abs(a-center_) < std::abs(b-center_);
    }
private:
    int center_;
};
void test_sort_func_obj() {
    std::vector<int> data{4,2,6,1,-1,5};
    std::cout << "data is ";
    for (auto v : data) {
        std::cout << v << " ";
    }
    std::cout << std::endl;

    DistanceTo distanceTo(4);
    std::sort(data.begin(), data.end(), distanceTo);
    std::cout << "after sort, data is ";
    for (auto v : data) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
}
void test_sort_lambda() {
    std::vector<int> data{4,2,6,1,-1,5};
    std::cout << "data is ";
    for (auto v : data) {
        std::cout << v << " ";
    }
    std::cout << std::endl;

    std::sort(data.begin(), data.end(), [](int a, int b) {return abs(a-4) < abs(b-4);});
    std::cout << "after sort, data is ";
    for (auto v : data) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
}
void test_sort_lambda_2() {
    std::vector<int> data{4,2,6,1,-1,5};
    std::cout << "data is ";
    for (auto v : data) {
        std::cout << v << " ";
    }
    std::cout << std::endl;

    int center = 4;
    std::sort(data.begin(), data.end(), [center](int a, int b) {return abs(a-center) < abs(b-center);});
    std::cout << "after sort, data is ";
    for (auto v : data) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
}
void test_sort_lambda_3() {
    std::vector<int> data{4,2,6,1,-1,5};
    std::cout << "data is ";
    for (auto v : data) {
        std::cout << v << " ";
    }
    std::cout << std::endl;

    std::sort(data.begin(), data.end(), [](int a, int b) {return a > b; });
    std::cout << "after sort, data is ";
    for (auto v : data) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
}
int main() {
    test_sort();
    test_sort_func_obj();
    test_sort_lambda();
    test_sort_lambda_2();
    test_sort_lambda_3();
    return 0;
}