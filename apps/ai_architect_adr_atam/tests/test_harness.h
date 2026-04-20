#pragma once

#include <cstdio>
#include <cstdlib>
#include <functional>
#include <sstream>
#include <string>
#include <vector>

namespace adra_test {

struct TestCase {
    std::string name;
    std::function<void()> fn;
};

inline std::vector<TestCase>& registry() {
    static std::vector<TestCase> r;
    return r;
}

struct Registrar {
    Registrar(const std::string& name, std::function<void()> fn) {
        registry().push_back({name, std::move(fn)});
    }
};

struct AssertionError : std::exception {
    std::string msg;
    explicit AssertionError(std::string m) : msg(std::move(m)) {}
    const char* what() const noexcept override { return msg.c_str(); }
};

inline void fail(const std::string& msg, const char* file, int line) {
    std::ostringstream os;
    os << file << ":" << line << ": " << msg;
    throw AssertionError(os.str());
}

inline int run_all() {
    int passed = 0, failed = 0;
    for (const auto& tc : registry()) {
        try {
            tc.fn();
            std::printf("[ OK ] %s\n", tc.name.c_str());
            ++passed;
        } catch (const AssertionError& e) {
            std::printf("[FAIL] %s — %s\n", tc.name.c_str(), e.what());
            ++failed;
        } catch (const std::exception& e) {
            std::printf("[FAIL] %s — exception: %s\n", tc.name.c_str(), e.what());
            ++failed;
        } catch (...) {
            std::printf("[FAIL] %s — unknown exception\n", tc.name.c_str());
            ++failed;
        }
    }
    std::printf("\n%d passed, %d failed\n", passed, failed);
    return failed == 0 ? 0 : 1;
}

}  // namespace adra_test

#define ADRA_TEST_CONCAT2(a, b) a##b
#define ADRA_TEST_CONCAT(a, b) ADRA_TEST_CONCAT2(a, b)
#define TEST(name)                                                                    \
    static void ADRA_TEST_CONCAT(adra_test_fn_, __LINE__)();                          \
    static ::adra_test::Registrar ADRA_TEST_CONCAT(adra_test_reg_, __LINE__)(         \
        #name, &ADRA_TEST_CONCAT(adra_test_fn_, __LINE__));                           \
    static void ADRA_TEST_CONCAT(adra_test_fn_, __LINE__)()

#define CHECK(cond)                                                                   \
    do {                                                                              \
        if (!(cond)) ::adra_test::fail(std::string("CHECK failed: ") + #cond,         \
                                       __FILE__, __LINE__);                           \
    } while (0)

#define CHECK_EQ(a, b)                                                                \
    do {                                                                              \
        auto _va = (a);                                                               \
        auto _vb = (b);                                                               \
        if (!(_va == _vb)) {                                                          \
            std::ostringstream _os;                                                   \
            _os << "CHECK_EQ failed: " << #a << " != " << #b;                         \
            ::adra_test::fail(_os.str(), __FILE__, __LINE__);                         \
        }                                                                             \
    } while (0)
