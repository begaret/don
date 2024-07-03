#include "raw.hpp"

DON_NAMESPACE_BEGIN

don::raw::raw(std::string_view string) noexcept
{
    m_string = string;
}

don::raw::raw(const char *p, const char *q) noexcept
{
    m_string = {p, q};
}

double don::raw::as_float() const noexcept
{
    return strtod(m_string.c_str(), nullptr);
}

long don::raw::as_int() const noexcept
{
    // binary (0b0000)
    if (m_string[0] == '0' && m_string[1] == 'b') {
        strtol(m_string.c_str() + 2, nullptr, 2);
    }

    return strtol(m_string.c_str(), nullptr, 0);
}

const std::string& don::raw::as_string() const noexcept
{
    return m_string;
}

bool don::raw::as_bool() const noexcept
{
    if (m_string == "false" || m_string == "no")
        return false;

    if (isalpha(m_string[0]))
        return true;

    return as_int();
}

DON_NAMESPACE_END
