#pragma once

#include "common.hpp"
#include "error.hpp"

#include <string_view>
#include <string>
#include <cstddef>

DON_NAMESPACE_BEGIN

class value
{
public:
    virtual ~value() = default;

    virtual void insert(std::string_view key, value *val) { throw don::error(don::DON_UNIMPLEMENTED); }
    virtual void insert(value *val) { throw don::error(don::DON_UNIMPLEMENTED); }

    virtual const value *get(std::string_view key) const { throw don::error(don::DON_UNIMPLEMENTED); }
    virtual const value *get(std::size_t index) const { throw don::error(don::DON_UNIMPLEMENTED); }

    virtual bool has(std::string_view key) const { throw don::error(don::DON_UNIMPLEMENTED); }
    virtual bool has(std::size_t index) const { throw don::error(don::DON_UNIMPLEMENTED); }

    virtual bool is_array() const noexcept { return false; }
    virtual bool is_object() const noexcept { return false; }
    virtual bool is_raw() const noexcept { return false; }

    virtual double as_float() const { throw don::error(don::DON_UNIMPLEMENTED); }
    virtual long as_int() const { throw don::error(don::DON_UNIMPLEMENTED); }
    virtual const std::string& as_string() const { throw don::error(don::DON_UNIMPLEMENTED); }
    virtual bool as_bool() const { throw don::error(don::DON_UNIMPLEMENTED); }
};

DON_NAMESPACE_END
