/**
 * @FilePath     : /server-cpp/src/include/common/nonecopyable.h
 * @Description  : 
 * @Author       : desyang desyang@qq.com
 * @Date         : 2026-06-05 15:23:11
 * @LastEditors  : desyang desyang@qq.com
 * @LastEditTime : 2026-06-05 16:40:01
 * @
 * @Copyright (c) 2026 by desyang, All Rights Reserved. 
**/
#pragma once

class nonecopyable
{
public:
    nonecopyable() = default;
    ~nonecopyable() = default;

    nonecopyable(const nonecopyable&) = delete;
    nonecopyable& operator=(const nonecopyable&) = delete;

    nonecopyable(nonecopyable&&) noexcept = default;
    nonecopyable& operator=(nonecopyable&&) noexcept = default;
};