#include "don.hpp"
#include "error.hpp"
#include "parse.hpp"

#include <cstdio>

DON_NAMESPACE_BEGIN

static void write_value(FILE *file, const don::value *value, int level);

don::object read(std::string_view path, bool preprocess)
{
    FILE *fp = fopen(path.data(), "r");
    auto root = read(fp, preprocess);
    
    fclose(fp);
    return root;
}

don::object read(FILE *file, bool preprocess)
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

    // TODO: preprocessing

    auto root = parse(buffer);

    delete [] buffer;
    return root;
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

    for (auto& [k, v] : root) {
        fprintf(file, "%s = ", k.c_str());
        write_value(file, v.get(), 0);
        fprintf(file, "\n");
    }
}

static void write_value(FILE *file, const don::value *value, int level)
{
    if (value->is_raw()) {
        fprintf(file, "\"%s\"", value->as_string().c_str());
    } else if (value->is_array()) {
        auto array = dynamic_cast<const don::array *>(value);
        fprintf(file, "[");
        if (array->size() > 0) {
            write_value(file, array->get(0), level);
            for (std::size_t i = 1; i < array->size(); i++) {
                fprintf(file, ", ");
                write_value(file, array->get(i), level);
            }
        }

        fprintf(file, "]");
    } else if (value->is_object()) {
        auto object = dynamic_cast<const don::object *>(value);
        fprintf(file, "{\n");
        for (auto [k, v] : *object) {
            for (int i = 0; i < level + 1; i++)
                fprintf(file, "    ");
            fprintf(file, "%s = ", k.c_str());
            write_value(file, v.get(), level + 1);
            fprintf(file, ",\n");
        }

        for (int i = 0; i < level; i++)
            fprintf(file, "    ");
        fprintf(file, "}");
    }
}

DON_NAMESPACE_END
