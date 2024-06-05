#pragma once

#include "common.hpp"
#include "value.hpp"

#include <string>
#include <memory>

#ifdef DON_USE_UNORDERED_MAP
    #include <unordered_map>

    template<class K, class V>
    using map_type = std::unordered_map<K, V>;
#else
    #include <map>

    template<class K, class V>
    using map_type = std::map<K, V>;
#endif

DON_NAMESPACE_BEGIN

class object : public value
{
public:
    void insert(std::string_view key, value *val) noexcept { m_buffer.emplace(key, val); }

    const value *get(std::string_view key) const noexcept { return has(key) ? m_buffer.at(key.data()).get() : nullptr; }
    
    bool has(std::string_view key) const noexcept { return m_buffer.find(key.data()) != m_buffer.end(); }

    std::size_t size() const noexcept { return m_buffer.size(); }
    
    const auto begin() const noexcept { return m_buffer.begin(); }
    auto begin() noexcept { return m_buffer.begin(); }

    const auto end() const noexcept { return m_buffer.end(); }
    auto end() noexcept { return m_buffer.end(); }

    bool is_object() const noexcept { return true; }

private:
    map_type<std::string,
        std::shared_ptr<value>
    > m_buffer;
};

DON_NAMESPACE_END

