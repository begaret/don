#pragma once

#include "common.hpp"
#include "value.hpp"

#include <vector>
#include <memory>

DON_NAMESPACE_BEGIN

class array : public value
{
public:
    void insert(value *val) noexcept { m_buffer.push_back(std::shared_ptr<value>(val)); }

    const value *get(std::size_t index) const noexcept { return has(index) ? m_buffer.at(index).get() : nullptr; }
    
    bool has(std::size_t index) const noexcept { return index < size(); }

    std::size_t size() const noexcept { return m_buffer.size(); }
    
    const auto begin() const noexcept { return m_buffer.begin(); }
    auto begin() noexcept { return m_buffer.begin(); }

    const auto end() const noexcept { return m_buffer.end(); }
    auto end() noexcept { return m_buffer.end(); }

    bool is_array() const noexcept { return true; }

private:
    std::vector<
        std::shared_ptr<value>
    > m_buffer;
};

DON_NAMESPACE_END
