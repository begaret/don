#include <don.hpp>

#include <cstdio>

void print_value(std::string k, don::value *v, int level)
{
    for (int i = 0; i < level; i++)
        printf("    ");

    if (v->is_object()) {
        don::object *o = (don::object*)v;

        printf("{%s}\n", k.c_str());
        for (auto [K, V] : *o) {
            print_value(K, V.get(), level + 1);
        }
    } else if (v->is_array()) {
        don::array *a = (don::array*)v;

        printf("[%s]\n", k.c_str());
        int i = 0;
        for (auto v : *a) {
           print_value(std::to_string(i++), v.get(), level + 1); 
        }
    } else {
        don::raw *r = (don::raw*)v;
        printf("%s = '%s'\n", k.c_str(), r->as_string().c_str());
    }
}

int main(int argc, char *argv[])
{
    don::object root;
    try {
        root = don::read("test.txt");
    } catch (don::error& e) {
        fprintf(stderr, "\e[31m%s\n", e.what());
    }

    for (auto [k, v] : root) {
        print_value(k, v.get(), 0);
    }

    don::write("out.txt", root);

    return 0;
}

