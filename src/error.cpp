#include "error.hpp"

DON_NAMESPACE_BEGIN

const char *don::error::what() const noexcept
{
    switch (m_type) {
        case DON_UNIMPLEMENTED: return "unexpected type conversion";
        case DON_NULL_FILE:     return "file pointer is null";
        default:                return "an unknown error occurred";
    }
}

DON_NAMESPACE_END
