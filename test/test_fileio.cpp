#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>
#include "fileio.h"
#include "encoding.h"

unsigned int Factorial(unsigned int number) {
    return number <= 1 ? number : Factorial(number - 1) * number;
}

TEST_CASE("Factorials are computed", "[factorial]") {
    REQUIRE(Factorial(1) == 1);
    REQUIRE(Factorial(2) == 2);
    REQUIRE(Factorial(3) == 6);
    REQUIRE(Factorial(10) == 3628800);
}

TEST_CASE("Write string to file", "[WriteIntoString]") {
    char const* text = u8"ÄãºÃ£¡";
    REQUIRE(true == lxd::fwrite(L"test.txt", text, strlen(text)));
}

TEST_CASE("Read file into string", "[ReadIntoString]") {
    auto str = lxd::fread(L"test.txt");
    printf("%s\n", lxd::utf8_decode(str).c_str());
}
