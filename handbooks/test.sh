#!/bin/sh
set -eu
cd "$(dirname "$0")/.."
build_dir=$(mktemp -d "${TMPDIR:-/tmp}/finals-handbooks.XXXXXX")
trap 'rm -rf "$build_dir"' EXIT HUP INT TERM
if [ "${SANITIZE:-0}" = 1 ]; then
    set -- -O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer
else
    set -- -O2
fi
for source in handbooks/tests/*_test.cpp; do
    name=$(basename "$source" .cpp)
    "${CXX:-c++}" -std=c++17 "$@" -Wall -Wextra -Wshadow \
        "$source" -o "$build_dir/$name"
    "$build_dir/$name"
    if [ "$name" = case_studies_test ] || [ "$name" = math_extra_test ] || [ "$name" = dp_extra_test ] || [ "$name" = practical_test ]; then
        "${CXX:-c++}" -std=c++17 "$@" -Wall -Wextra -Wshadow -DCASE_NATIVE \
            "$source" -o "$build_dir/case_native"
        "$build_dir/case_native"
    fi
done
