#pragma once

class nonecopyable
{
public:
    nonecopyable() = default;
    ~nonecopyable() = default;

    nonecopyable(const nonecopyable&) = delete;
    nonecopyable& operator=(const nonecopyable&) = delete;
};