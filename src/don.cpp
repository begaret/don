#include "don.hpp"
#include "error.hpp"

#include <cstdio>

#define isident(c)\
    (isalnum(c) || (c) == '_')

DON_NAMESPACE_BEGIN

static don::object parse_buffer(char *p);
static don::value *parse_value(char *p, char **n);

static void write_value(FILE *file, const don::value *value, int level);

don::object read(std::string_view path)
{
    FILE *fp = fopen(path.data(), "r");
    auto root = read(fp);

    fclose(fp);
    return root;
}

don::object read(FILE *file)
{
    if (file == nullptr) {
        throw don::error(don::DON_NULL_FILE);
    }

    fseek(file, 0, SEEK_END);
    std::size_t size = ftell(file);
    rewind(file);

    char *buffer = new char[size + 1];
    fread(buffer, 1, size, file);
    buffer[size] = '\0';

    auto root = parse_buffer(buffer);

    delete [] buffer;
    return root;
}

don::object parse_buffer(char *p)
{
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

        don::value *v = parse_value(p, &p);
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

// TODO: better parsing
don::value *parse_value(char *p, char **n)
{   
    // raw 
    if (isident(*p)) {
        char *start = p;
        while (*p && isident(*p))
            ++p;

        *n = p;
        return new don::raw(start, p);
    }

    // raw string
    if (*p == '"') {
        ++p;

        char *start = p;
        while (*p && *p != '"')
            ++p;

        *n = p;
        return new don::raw(start, p);
    }

    // array
    if (*p == '[') {
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

            don::value *v = parse_value(p, &p);
            if (v) {
                array->insert(v);
            }
        }

        *n = p;
        return array;
    }

    // object
    if (*p == '{') {
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

                don::value *v = parse_value(p, &p);
                if (v) {
                    object->insert(k, v);
                    k.clear();
                }
            }
        }

        *n = p;
        return object;
    }

    return nullptr;
}

void write(std::string_view path, const don::object& root)
{
    FILE *fp = fopen(path.data(), "w");
    don::write(fp, root);

    fclose(fp);
}

void write(FILE *file, const don::object& root)
{
    if (file == nullptr) {
        throw don::error(don::DON_NULL_FILE);
    }

    for (auto [k, v] : root) {
        fprintf(file, "%s = ", k.c_str());
        write_value(file, v.get(), 1);
        fprintf(file, "\n\n");
    }
}

static void write_value(FILE *file, const don::value *value, int level)
{
    if (value->is_raw()) {
        fprintf(file, "\"%s\"", value->as_string().c_str());
    } else if (value->is_array()) {
        auto array = dynamic_cast<const don::array *>(value);
        fprintf(file, "[");
        write_value(file, array->get(0), level);
        for (std::size_t i = 1; i < array->size(); i++) {
            fprintf(file, ", ");
            write_value(file, array->get(i), level);
        }

        fprintf(file, "]");
    } else if (value->is_object()) {
        auto object = dynamic_cast<const don::object *>(value);
        fprintf(file, "{\n");
        for (auto [k, v] : *object) {
            for (int i = 0; i < level; i++)
                fprintf(file, "    ");
            fprintf(file, "%s = ", k.c_str());
            write_value(file, v.get(), level + 1);
            fprintf(file, ",\n");
        }

        for (int i = 0; i < level - 1; i++)
            fprintf(file, "    ");
        fprintf(file, "}");
    }
}

DON_NAMESPACE_END

