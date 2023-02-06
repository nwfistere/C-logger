#pragma once

#include <iostream>
#include <sstream>

struct redirect_cout {
    std::stringstream cout_stream;
    redirect_cout() {
        oldbuf = std::cout.rdbuf( cout_stream.rdbuf() );
    }
    ~redirect_cout() {
        std::cout.rdbuf(oldbuf);
    }
private:
    std::streambuf * oldbuf;
};
