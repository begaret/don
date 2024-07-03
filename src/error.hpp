#pragma once

#include "common.hpp"

#include <exception>

DON_NAMESPACE_BEGIN

enum error_type
{
    DON_UNIMPLEMENTED,
    DON_NULL_FILE,
    DON_UNKNOWN
};

class error : public std::exception
{
public:
    error(don::error_type type) noexcept : m_type(type) {}
    
    const char *what() const noexcept;

private:
    don::error_type m_type;
};

DON_NAMESPACE_END
