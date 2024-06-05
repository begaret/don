#pragma once

#include "common.hpp"
#include "value.hpp"

DON_NAMESPACE_BEGIN

class raw : public value
{
public:
    raw(const char *p, const char *q) noexcept;
    raw() noexcept = default;

    bool is_raw() const noexcept { return true; }

    double as_float() const noexcept;
    long as_int() const noexcept;
    const std::string& as_string() const noexcept;
    bool as_bool() const noexcept;

private:
    std::string m_string;
};

DON_NAMESPACE_END

