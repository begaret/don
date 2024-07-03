#pragma once

#include "common.hpp"
#include "error.hpp"
#include "array.hpp"
#include "object.hpp"
#include "raw.hpp"

#include <string_view>

DON_NAMESPACE_BEGIN

don::object read(std::string_view path, bool preprocess = false);
don::object read(FILE *file, bool preprocess = false);

void write(std::string_view path, const don::object& root);
void write(FILE *file, const don::object& root);

DON_NAMESPACE_END
