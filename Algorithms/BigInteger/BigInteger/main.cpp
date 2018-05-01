//
//  main.cpp
//  BigInteger
//
//  Created by Ivan on 09.12.17.
//  Copyright © 2017 Ivan. All rights reserved.
//

#include <iostream>
#include "BigInteger.hpp"

int main(int argc, const char * argv[]) {
    BigInteger a, b, c;
    std::cin >> a >> b;
    c = a / b + b * (-a);
    std::cout << c.toString();
    return 0;
}
