#!/bin/bash
echo "Downloading dependencies..."

curl -L -o httplib.h https://raw.githubusercontent.com/yhirose/cpp-httplib/master/httplib.h
if [ $? -ne 0 ]; then
    echo "Failed to download httplib.h"
    exit 1
fi

curl -L -o json.hpp https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp
if [ $? -ne 0 ]; then
    echo "Failed to download json.hpp"
    exit 1
fi

echo "Dependencies downloaded successfully!"
