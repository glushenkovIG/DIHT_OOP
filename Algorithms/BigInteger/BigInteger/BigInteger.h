//
//  BigInteger.h
//  BigInteger
//
//  Created by Ivan on 09.12.17.
//  Copyright © 2017 Ivan. All rights reserved.
//

#ifndef BigInteger_h
#define BigInteger_h

#include <iostream>
#include <vector>
#include <string>

class BigInteger {
public:
    BigInteger(const int &n);
    BigInteger(const BigInteger &other) : sign(other.sign), len(other.len), N(other.N) {}
    BigInteger() : sign(false), len(0), N({}) {};
    
    const BigInteger operator-() const;
    
    friend bool operator ==(const BigInteger &self, const BigInteger &other);
    friend bool operator >(const BigInteger &self, const BigInteger &other);
    friend bool operator <(const BigInteger &self, const BigInteger &other) { return other > self;}
    friend bool operator >=(const BigInteger &self, const BigInteger &other) { return (self > other || self == other);}
    bool operator <=(const BigInteger &other) const { return (*this < other || *this == other);}
    bool operator !=(const BigInteger &other) const { return !(*this == other);}
    
    explicit operator bool() const { return (len > 0);}
    
    BigInteger& operator++() { return *this += 1;}
    BigInteger& operator--() { return *this -= 1;}
    const BigInteger operator ++(int);
    const BigInteger operator --(int);
    
    BigInteger& operator +=(const BigInteger &other);
    BigInteger& operator -=(const BigInteger &other);
    BigInteger& operator *=(const BigInteger &other);
    BigInteger& operator /=(const BigInteger &other) { return *this = *this / other;}
    BigInteger& operator %=(const BigInteger &other) {return *this -= (*this / other) * other;}
    
    static BigInteger pow(const int &n);//BASE^n
    const std::string toString() const;
    const BigInteger abs() const { return (this->sign ? -(*this) : *this);}
    
    friend const BigInteger operator +(const BigInteger &self, const BigInteger &other) { return BigInteger(self) += other;}
    friend const BigInteger operator -(const BigInteger &self, const BigInteger &other) { return BigInteger(self) -= other;}
    friend const BigInteger operator %(const BigInteger &self, const BigInteger &other) { return BigInteger(self) %= other;}
    friend const BigInteger operator *(const BigInteger &self, const BigInteger &other) { return BigInteger(self) *= other;}
    friend const BigInteger operator /(const BigInteger &first, const BigInteger &second);
    friend std::istream& operator>>(std::istream &in, BigInteger &other);
private:
    const static int BASE = 1e7;
    const static int log_BASE = 7;
    bool sign;
    int len;//length
    std::vector<int> N;
};
#endif
