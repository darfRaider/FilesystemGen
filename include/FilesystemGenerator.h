#include <jsoncpp/json/json.h>
#include <jsoncpp/json/value.h>

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <filesystem>
#include "File.hpp"
#include "JsonHandler.hpp"
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "md5.h"

using recursive_directory_iterator =
    std::filesystem::recursive_directory_iterator;
