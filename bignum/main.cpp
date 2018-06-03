#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


class BigNumber
{
public:
    BigNumber();
    BigNumber(unsigned short x);
    BigNumber(const BigNumber& other);
    BigNumber(BigNumber&& other);
    BigNumber& operator=(const BigNumber& rhs);
    BigNumber& operator=(BigNumber&& rhs);
    ~BigNumber() = default;

    std::size_t digits() const;
    std::string toString() const;
private:
    std::vector<uint8_t> digits_ = {0};
    bool negative_ = false;
};

BigNumber::BigNumber()
{
}

BigNumber::BigNumber(unsigned short x)
{
    digits_.clear();

    const std::size_t numDigits = std::ceil(std::log10(x));
    for (std::size_t i = 0; i < numDigits; ++i)
    {
        const uint8_t digit = x % 10;
        digits_.emplace_back(digit);
        x /= 10;
    }

    std::reverse(std::begin(digits_), std::end(digits_));
}

std::size_t BigNumber::digits() const
{
    return digits_.size();
}



std::string BigNumber::toString() const
{
    std::stringstream ss;
    if (negative_)
        ss << '-';
    for (const auto& digit : digits_)
        ss.put(digit + '0');

    return ss.str();
}

int main()
{
    BigNumber bn = 123;
    std::cout << bn.toString();
}