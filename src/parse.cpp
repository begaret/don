#include "parse.hpp"
#include "value.hpp"
#include "array.hpp"
#include "object.hpp"
#include "raw.hpp"

#include <string>

#define isident(c)\
    (isalnum(c) || (c) == '_')

DON_NAMESPACE_BEGIN

char *p = nullptr;

static don::value *parse_value();
static don::value *parse_array();
static don::value *parse_object();
static don::value *parse_raw();

don::object parse(char *buf)
{
    p = buf;

    don::object root;

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

    return root;
}

static don::value *parse_value()
{
    switch (*p) {
        case '[':   return parse_array();
        case '{':   return parse_object();
        case '"':   return parse_raw();
    }

    if (isident(*p)) {
        return parse_raw();
    }

    return nullptr;
}

static don::value *parse_raw()
{
    if (isident(*p)) {
        char *start = p;
        while (*p && isident(*p))
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

    return nullptr;
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

        if (k.empty() && isident(*p)) {
            char *start = p;
            while (*p && isident(*p))
                ++p;

            k = {start, p};
        }

        if (*p == '=') {
            ++p;

            while (isspace(*p))
                ++p;

            don::value *v = parse_value();
            if (v) {
                object->insert(k, v);
                k.clear();
            }
        }
    }

    return object;
}

DON_NAMESPACE_END

