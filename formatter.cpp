#ifndef FORMATTER_CPP
#define FORMATTER_CPP

#include <string>
#include <sstream>
#include <iostream>

std::string format(const std::string &fmt_str) {
    return fmt_str;
}

template <typename T, typename ... Rest>
std::string format(const std::string &fmt_str, T head, Rest ... tail) {
    std::string res;
    int i = fmt_str.find('%');
    if (i == fmt_str.npos) return fmt_str;
    res += fmt_str.substr(0, i);
    std::stringstream ss; 
    ss << head; // at c time, if the << method does not exist, it will throw an error (cannot compile the template fn) at c time.
    res += ss.str();
    res += format(fmt_str.substr(i+1), tail...);
    return res;
}

#endif

#ifdef DEBUG
class A {
public:
    int x;
    A(int x): x(x){}
};

class B: public A {
public:
    B(int x): A(x){}
};

std::ostream &operator<<(std::ostream &ost, const A &a) {
    ost << a.x; return ost;
}

int main() {
    int a = 3;
    std::string b = "abc";
    A *c = nullptr;
    int k; std::cin >> k;
    if (k > 0) c = new A{3}; else c = new B{4};
    std::cout << format("let's see if % prints % times", b, *c) << std::endl;
}
#endif