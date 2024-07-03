#include "parse.hpp"
#include "value.hpp"
#include "array.hpp"
#include "object.hpp"
#include "raw.hpp"

#include <string>

#define isident(c)\
    (isalnum(c) || (c) == '_')

DON_NAMESPACE_BEGIN

static don::object *root_ptr = nullptr;
static char *p = nullptr;

static don::value *parse_value();
static don::value *parse_array();
static don::value *parse_object();
static don::value *parse_raw();
static don::value *parse_macro();

don::object parse(char *buf)
{
    p = buf;

    don::object root;
    root_ptr = &root;

    std::string k;
    bool expect_value = false;

    for (;;) {
        // skip white space
        while (isspace(*p))
            ++p;

        // skip comments
        if (*p == '!') {
            while (*p && *p != '\n')
                ++p;
        }

        // key
        if (!expect_value && isident(*p)) {
            char *start = p;
            while (*p && isident(*p))
                ++p;

            k = {start, p};
        }

        // value
        if (*p == '=') {
            expect_value = true;
        }

        don::value *v = parse_value();
        if (v) {
            root.insert(k, v);
            expect_value = false;
        }
        
        // end of file
        if (*p == 0)
            break;
        ++p;       
    }

    root_ptr = nullptr;
    p = nullptr;
    return root;
}

static don::value *parse_value()
{
    switch (*p) {
        case '[':   return parse_array();
        case '{':   return parse_object();
        case '$':   return parse_macro();
    }

    return parse_raw();
}

static don::value *parse_raw()
{
    if (isident(*p)) {
        char *start = p;
        while (*p && (isident(*p) || *p == '.'))
            ++p;

        return new don::raw(start, p);
    }

    // raw string
    if (*p == '"') {
        char *start = ++p;
        while (*p && *p != '"')
            ++p;

        return new don::raw(start, p);
    }

    if (*p == '\'') {
        char *start = ++p;
        while (*p && *p != '\'')
            ++p;

        return new don::raw(start, p);
    }

    return nullptr;
}

static don::value *parse_macro()
{
    // parse reference
    char *start = ++p;
    while (*p && isident(*p))
        ++p;

    std::string k = {start, p};
    auto v = root_ptr->get(k);

    // parse keys ($object:key = object[key])
    while (*p == ':') {
        ++p;
        auto k = parse_raw();
        if (k) {
            if (v->is_object()) {
                v = v->get(k->as_string());
            } else if (v->is_array()) {
                v = v->get(k->as_int());
            }
        }
    }

    if (v->is_object()) {
        auto o = dynamic_cast<const don::object*>(v);
        return new don::object(*o);
    }

    if (v->is_array()) {
        auto a = dynamic_cast<const don::array*>(v);
        return new don::array(*a);
    }

    return new don::raw(v->as_string().c_str());
}

static don::value *parse_array()
{
    don::array *array = new don::array;

    while (*p && *p++ != ']') {
        while (isspace(*p))
            ++p;
    
        // skip comments
        if (*p == '!') {
            while (*p && *p != '\n')
                ++p;
        }

        // skip commas
        if (*p == ',')
            ++p;

        don::value *v = parse_value();
        if (v) {
            array->insert(v);
        }
    }

    return array;
}

static don::value *parse_object()
{
    don::object *object = new don::object;

    std::string k;
    bool expect_value = false;

    while (*p && *p++ != '}') {
        while (isspace(*p))
            ++p;
        
        // skip comments
        if (*p == '!') {
            while (*p && *p != '\n')
                ++p;
        }

        // skip commas
        if (*p == ',')
            ++p;

        if (!expect_value && isident(*p)) {
            char *start = p;
            while (*p && isident(*p))
                ++p;

            k = {start, p};
        }

        if (*p == '=') {
            expect_value = true;
        }

        don::value *v = parse_value();
        if (v) {
            object->insert(k, v);
            expect_value = false;
        }
    }

    return object;
}

DON_NAMESPACE_END
