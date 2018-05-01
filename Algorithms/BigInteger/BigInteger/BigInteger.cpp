//
//  BigInteger.cpp
//  biginteger
//
//  Created by Ivan on 14.12.17.
//  Copyright © 2017 Ivan. All rights reserved.
//

#include "BigInteger.hpp"
BigInteger::BigInteger(const int &n) : sign(false), len(0), N(std::vector<int>(0)) {
    int n1 = n;
    if (n1 < 0) {
        sign = true;
        n1 = -n1;
    }
    while (n1) {
        ++len;
        N.push_back(n1 % BASE);
        n1 /= BASE;
    }
}

const BigInteger BigInteger::operator-() const {
    BigInteger other = *this;
    other.sign ^= 1;
    return other;
}

bool operator ==(const BigInteger &self, const BigInteger &other) {
    bool are_equiv = (self.len == other.len && self.sign == other.sign);
    for (int i = 0; i < self.len && are_equiv; ++i){
        are_equiv = (self.N[i] == other.N[i] && are_equiv);
    }
    return are_equiv;
}

bool operator >(const BigInteger &self, const BigInteger &other) {
    if (self == other) return false;
    if (self.sign && !other.sign) return false;
    if (!self.sign && other.sign) return true;
    if (self.sign && other.sign) {
        if (self.len < other.len) return true;
        if (self.len > other.len) return false;
        for (int i = self.len; i >= 0; --i) {
            if (self.N[i] < other.N[i]) return true;
            if (self.N[i] > other.N[i]) return false;
        }
        return false;
    }
    if (self.len > other.len) return true;
    if (self.len < other.len) return false;
    for (int i = self.len - 1; i >= 0; --i) {
        if (self.N[i] > other.N[i]) return true;
        if (self.N[i] < other.N[i])return false;
    }
    return false;
}

BigInteger& BigInteger::operator +=(const BigInteger &other) {
    if (sign == other.sign) {
        if (len < other.len) {
            len = other.len;
            N.resize(len);
        }
        N.push_back(0);
        ++len;
        int i = 0;
        for (; i < other.len; ++i) {
            if (N[i] + other.N[i] >= BASE){
                ++N[i + 1];
            }
            N[i] = (N[i] + other.N[i]) % BASE;
        }
        while (i < len && N[i] >= BASE) {
            N[i] %= BASE;
            ++N[++i];
        }
    }
    else if (this->abs() >= other.abs()) {
        int i = 0;
        for (; i < other.len; ++i) {
            if (N[i] - other.N[i] < 0){
                --N[i + 1];
            }
            N[i] = (N[i] - other.N[i] + BASE) % BASE;
        }
        while (i < len && N[i] < 0) {
            N[i] += BASE;
            --N[++i];
        }
    }
    else {
        sign = other.sign;
        if (len < other.len) {
            len = other.len;
            N.resize(len);
        }
        for (int i = 0; i < other.len; ++i) {
            if (other.N[i] - N[i] < 0){
                ++N[i + 1];
            }
            N[i] = (other.N[i] - N[i] + BASE) % BASE;
        }
    }
    for (int i = len - 1; i >= 0 && !N[i]; --i){
        --len;
        N.pop_back();
    }
    if (!len)
        sign = false;
    return *this;
}

BigInteger& BigInteger::operator -=(const BigInteger &other) {
    sign ^= 1;
    *this += other;
    sign ^= 1;
    return *this;
}

const BigInteger BigInteger::operator ++(int) {
    BigInteger res = *this;
    ++*this;
    return res;
}

const BigInteger BigInteger::operator --(int) {
    BigInteger res = *this;
    --*this;
    return res;
}

BigInteger& BigInteger::operator *=(const BigInteger &other) {
    std::vector<long long> mul_vect(len + other.len);
    for (int i = 0; i < len + other.len - 1; ++i) {
        for(int j = (i - other.len + 1 <= 0 ? 0 : i - other.len + 1); j <= i && j < len && i - j < other.len; ++j){
            mul_vect[i] += static_cast<long long>(N[j] * static_cast<long long>(other.N[i - j]));
        }
        mul_vect[i + 1] += mul_vect[i] / BASE;
        mul_vect[i] %= BASE;
    }
    N.resize(len + other.len);
    len = len + other.len;
    for (int i = 0; i < len; ++i){
        N[i] = static_cast<int>(mul_vect[i]);
    }
    while (len > 0 && N[len - 1] == 0){
        --len;
    }
    sign ^= other.sign;
    return *this;
}


BigInteger BigInteger::pow(const int &n){
    BigInteger ANS(1);
    for(int i = 0; i < n; ++i){
        int d = BASE;
        ANS *= BigInteger(d);
    }
    return ANS;
}

const std::string BigInteger::toString() const {
    if (!len)
        return "0";
    std::string str;
    if (sign)
        str += '-';
    bool zero_flag = false;
    for (int i = len - 1; i >= 0; --i) {
        std::string curr_decade = std::to_string(N[i]);
        if(zero_flag){
            for (int j = 0; j < log_BASE - static_cast<int>(curr_decade.size()); ++j){
                str += '0';
            }
        }
        str += curr_decade;
        zero_flag = true;
    }
    return str;
}

std::ostream& operator<<(std::ostream& out, const BigInteger &other) {
    out << other.toString(); return out;
}

std::istream& operator >>(std::istream& in, BigInteger &other) {
    std::string s;
    in >> s;
    other.sign = (s[0] == '-' ? 1 : 0);
    other.len = 0;
    other.N.resize(0);
    int i = 0;
    if (s.size() == 1 && s[0] == '0')
        return in;
    for (i = static_cast<int>(s.size()) - BigInteger::log_BASE; i >= 0 && s[i] != '-'; i -= BigInteger::log_BASE) {
        ++other.len;
        other.N.push_back(atoi(s.substr(i, BigInteger::log_BASE).c_str()));
    }
    int j = (s[0] == '-' ? 1 : 0);
    i += BigInteger::log_BASE;
    if (i - j > 0) {
        ++other.len;
        other.N.push_back(atoi(s.substr(j, i - j).c_str()));
    }
    return in;
}

BigInteger const operator /(const BigInteger &first, const BigInteger &divisor) {
    BigInteger quotient;
    if (first.abs() < divisor.abs()) return BigInteger(0);
    
    BigInteger dividend, remainder = 0;
    dividend = first.abs();
    int i = dividend.len - 1;
    int base = BigInteger::BASE;
    while(i >= 0){
        while(remainder < divisor.abs() && i >= 0) {
            remainder *= BigInteger(base);
            remainder += dividend.N[i];
            quotient *= BigInteger(base);
            --i;
        }
        int left = 0, right = base, m;
        while(right - left > 1){
            m = (right + left) / 2;
            BigInteger curr = m * divisor;
            (m * divisor.abs() <= remainder ? left = m : right = m);
        }
        (right * divisor.abs() < remainder ? left = right : 0);
        remainder -= left * divisor.abs();
        quotient += left;
    }
    quotient.sign = first.sign != divisor.sign;
    return quotient;
}
