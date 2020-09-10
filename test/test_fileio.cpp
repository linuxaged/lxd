#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>
#include <fmt/format.h>
#include "fileio.h"
#include "encoding.h"
#include "timer.h"
#include "debug.h"
#include "numbers.h"

TEST_CASE("String 2 float", "[lxd::SimpleAtof]") {
    float pi;
    REQUIRE(lxd::SimpleAtof("0.0", &pi));
    REQUIRE(pi == 0.0f);
    REQUIRE(lxd::SimpleAtof("0", &pi));
    REQUIRE(pi == 0.0f);
    REQUIRE(lxd::SimpleAtof("01", &pi));
    REQUIRE(pi == 1.0f);
    REQUIRE(lxd::SimpleAtof("314", &pi));
    REQUIRE(pi == 314.0f);
    REQUIRE(lxd::SimpleAtof("-314", &pi));
    REQUIRE(pi == -314.0f);
    REQUIRE(lxd::SimpleAtof("3.14", &pi));
    REQUIRE(pi == 3.14f);
    REQUIRE(lxd::SimpleAtof("3..14", &pi) == false);
}

TEST_CASE("String 2 int", "[lxd::SimpleAtoi]") {
    int pi;
    REQUIRE(lxd::SimpleAtoi("0.0", &pi) == false);
    REQUIRE(lxd::SimpleAtoi("0", &pi));
    REQUIRE(pi == 0);
    REQUIRE(lxd::SimpleAtoi("01", &pi));
    REQUIRE(pi == 1);
    REQUIRE(lxd::SimpleAtoi("314", &pi));
    REQUIRE(pi == 314);
    REQUIRE(lxd::SimpleAtoi("-314", &pi));
    REQUIRE(pi == -314);
    REQUIRE(lxd::SimpleAtoi("0x314", &pi) == false);
}
 
//TEST_CASE("Create Dir", "[lxd::CreateDir]") {
//    REQUIRE(lxd::CreateDir(L"D:\\Download\\test_lxd"));
//}
//
//TEST_CASE("Exists Dir", "[lxd::DirExists]") {
//    REQUIRE(lxd::DirExists(L"D:\\Download\\test_lxd"));
//}
//
//TEST_CASE("Write string to file", "[WriteIntoString]") {
//    char const* text = u8"你好！";
//    REQUIRE(lxd::WriteFile(L"D:\\Download\\test_lxd\\test.txt", text, strlen(text)));
//}
//
//TEST_CASE("Read file into string", "[ReadIntoString]") {
//    auto str = lxd::ReadFile(L"D:\\Download\\test_lxd\\test.txt");
//    wprintf(L"%s\n", lxd::utf8_decode(str).c_str());
//}
//
//TEST_CASE("List directory", "[ListDir]") {
//    for(int i = 0; i < 10; ++i) {
//        lxd::CreateDir(fmt::format(L"D:\\Download\\test_lxd\\dir{}", i));
//        char const* text = u8"测试";
//        lxd::WriteFile(fmt::format(L"D:\\Download\\test_lxd\\dir{}\\{}.txt", i, i), text, strlen(text));
//    }
//    std::vector<std::wstring> files;
//    lxd::ListDir(L"D:\\Download\\test_lxd", files, true);
//    for(auto const& f : files) {
//        wprintf(L"%s\n", f.c_str());
//    }
//}
//
//TEST_CASE("Delete Dir", "[lxd::DeleteDir]") {
//    REQUIRE(0 == lxd::DeleteDir(L"D:\\Download\\test_lxd"));
//}
//
//TEST_CASE("Timer ", "[Timer]") {
//    auto start = lxd::millisecond();
//    lxd::sleep(1000);
//    auto end = lxd::millisecond();
//    printf("timer: %f\n", end - start);
//}
