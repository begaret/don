#include "object.hpp"

DON_NAMESPACE_BEGIN

const value *don::object::get(std::string_view key) const noexcept
{
    if (has(key)) {
        return m_buffer.at(key.data()).get();
    }

    const don::value *v = this;

    int start = 0;
    int end = 0;

    while (end != key.length()) {
        end = key.find('.', start);
        if (end == key.npos) {
            end = key.length();
        }

        std::string real_key = {&key[start], &key[end]};
        if (!v->is_object() || !v->has(real_key)) {
            return nullptr;
        }

        v = v->get(real_key);
        start = end + 1;
    }

    return v;
}

DON_NAMESPACE_END
