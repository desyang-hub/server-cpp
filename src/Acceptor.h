#pragma once

#include "Epoll.h"

class Acceptor
{
private:
    Epoll* epoll_;
public:
    Acceptor(/* args */);
    ~Acceptor();
};
