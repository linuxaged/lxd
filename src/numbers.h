//
// Copyright 2017 The Abseil Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// -----------------------------------------------------------------------------
// File: numbers.h
// -----------------------------------------------------------------------------
//
// This package contains functions for converting strings to numbers. For
// converting numbers to strings, use `StrCat()` or `StrAppend()` in str_cat.h,
// which automatically detect and convert most number values appropriately.

#pragma once

#include <string_view>
#include <cassert>
#include <intrin.h>
#include "defines.h"

namespace lxd {
    // SimpleAtoi()
    //
    // Converts the given string (optionally followed or preceded by ASCII
    // whitespace) into an integer value, returning `true` if successful. The string
    // must reflect a base-10 integer whose value falls within the range of the
    // integer type (optionally preceded by a `+` or `-`). If any errors are
    // encountered, this function returns `false`, leaving `out` in an unspecified
    // state.
    template <typename int_type>
    bool SimpleAtoi(std::string_view str, int_type* out);

    // SimpleAtof()
    //
    // Converts the given string (optionally followed or preceded by ASCII
    // whitespace) into a float, which may be rounded on overflow or underflow,
    // returning `true` if successful.
    // See https://en.cppreference.com/w/c/string/byte/strtof for details about the
    // allowed formats for `str`, except SimpleAtof() is locale-independent and will
    // always use the "C" locale. If any errors are encountered, this function
    // returns `false`, leaving `out` in an unspecified state.
    bool SimpleAtof(std::string_view str, float* out);

    // SimpleAtod()
    //
    // Converts the given string (optionally followed or preceded by ASCII
    // whitespace) into a double, which may be rounded on overflow or underflow,
    // returning `true` if successful.
    // See https://en.cppreference.com/w/c/string/byte/strtof for details about the
    // allowed formats for `str`, except SimpleAtod is locale-independent and will
    // always use the "C" locale. If any errors are encountered, this function
    // returns `false`, leaving `out` in an unspecified state.
    bool SimpleAtod(std::string_view str, double* out);

    // SimpleAtob()
    //
    // Converts the given string into a boolean, returning `true` if successful.
    // The following case-insensitive strings are interpreted as boolean `true`:
    // "true", "t", "yes", "y", "1". The following case-insensitive strings
    // are interpreted as boolean `false`: "false", "f", "no", "n", "0". If any
    // errors are encountered, this function returns `false`, leaving `out` in an
    // unspecified state.
    bool SimpleAtob(std::string_view str, bool* out);

}  // namespace lxd

// End of public API.  Implementation details follow.

namespace lxd {
        namespace numbers_internal {

        // Digit conversion.
        DLL_PUBLIC extern const char kHexChar[17];  // 0123456789abcdef
        DLL_PUBLIC extern const char
            kHexTable[513];  // 000102030405060708090a0b0c0d0e0f1011...
        DLL_PUBLIC extern const char
            two_ASCII_digits[100][2];  // 00, 01, 02, 03...

        // Writes a two-character representation of 'i' to 'buf'. 'i' must be in the
        // range 0 <= i < 100, and buf must have space for two characters. Example:
        //   char buf[2];
        //   PutTwoDigits(42, buf);
        //   // buf[0] == '4'
        //   // buf[1] == '2'
        inline void PutTwoDigits(size_t i, char* buf) {
            assert(i < 100);
            memcpy(buf, two_ASCII_digits[i], 2);
        }

        // safe_strto?() functions for implementing SimpleAtoi()
        bool safe_strto32_base(std::string_view text, int32_t* value, int base);
        bool safe_strto64_base(std::string_view text, int64_t* value, int base);
        bool safe_strtou32_base(std::string_view text, uint32_t* value, int base);
        bool safe_strtou64_base(std::string_view text, uint64_t* value, int base);
        //bool safe_strtou128_base(std::string_view text, lxd::uint128* value,
        //                         int base);

        static const int kFastToBufferSize = 32;
        static const int kSixDigitsToBufferSize = 16;

        // Helper function for fast formatting of floating-point values.
        // The result is the same as printf's "%g", a.k.a. "%.6g"; that is, six
        // significant digits are returned, trailing zeros are removed, and numbers
        // outside the range 0.0001-999999 are output using scientific notation
        // (1.23456e+06). This routine is heavily optimized.
        // Required buffer size is `kSixDigitsToBufferSize`.
        size_t SixDigitsToBuffer(double d, char* buffer);

        // These functions are intended for speed. All functions take an output buffer
        // as an argument and return a pointer to the last byte they wrote, which is the
        // terminating '\0'. At most `kFastToBufferSize` bytes are written.
        char* FastIntToBuffer(int32_t, char*);
        char* FastIntToBuffer(uint32_t, char*);
        char* FastIntToBuffer(int64_t, char*);
        char* FastIntToBuffer(uint64_t, char*);

        // For enums and integer types that are not an exact match for the types above,
        // use templates to call the appropriate one of the four overloads above.
        template <typename int_type>
        char* FastIntToBuffer(int_type i, char* buffer) {
            static_assert(sizeof(i) <= 64 / 8,
                          "FastIntToBuffer works only with 64-bit-or-less integers.");
            // TODO(jorg): This signed-ness check is used because it works correctly
            // with enums, and it also serves to check that int_type is not a pointer.
            // If one day something like std::is_signed<enum E> works, switch to it.
            if(static_cast<int_type>(1) - 2 < 0) {  // Signed
                if(sizeof(i) > 32 / 8) {           // 33-bit to 64-bit
                    return FastIntToBuffer(static_cast<int64_t>(i), buffer);
                } else {  // 32-bit or less
                    return FastIntToBuffer(static_cast<int32_t>(i), buffer);
                }
            } else {                     // Unsigned
                if(sizeof(i) > 32 / 8) {  // 33-bit to 64-bit
                    return FastIntToBuffer(static_cast<uint64_t>(i), buffer);
                } else {  // 32-bit or less
                    return FastIntToBuffer(static_cast<uint32_t>(i), buffer);
                }
            }
        }

        // Implementation of SimpleAtoi, generalized to support arbitrary base (used
        // with base different from 10 elsewhere in Abseil implementation).
        template <typename int_type>
        bool safe_strtoi_base(std::string_view s, int_type* out,
                                                   int base) {
            static_assert(sizeof(*out) == 4 || sizeof(*out) == 8,
                          "SimpleAtoi works only with 32-bit or 64-bit integers.");
            static_assert(!std::is_floating_point<int_type>::value,
                          "Use SimpleAtof or SimpleAtod instead.");
            bool parsed;
            // TODO(jorg): This signed-ness check is used because it works correctly
            // with enums, and it also serves to check that int_type is not a pointer.
            // If one day something like std::is_signed<enum E> works, switch to it.
            if(static_cast<int_type>(1) - 2 < 0) {  // Signed
                if(sizeof(*out) == 64 / 8) {       // 64-bit
                    int64_t val;
                    parsed = numbers_internal::safe_strto64_base(s, &val, base);
                    *out = static_cast<int_type>(val);
                } else {  // 32-bit
                    int32_t val;
                    parsed = numbers_internal::safe_strto32_base(s, &val, base);
                    *out = static_cast<int_type>(val);
                }
            } else {                         // Unsigned
                if(sizeof(*out) == 64 / 8) {  // 64-bit
                    uint64_t val;
                    parsed = numbers_internal::safe_strtou64_base(s, &val, base);
                    *out = static_cast<int_type>(val);
                } else {  // 32-bit
                    uint32_t val;
                    parsed = numbers_internal::safe_strtou32_base(s, &val, base);
                    *out = static_cast<int_type>(val);
                }
            }
            return parsed;
        }

        inline int CountLeadingZeros64Slow(uint64_t n) {
            int zeroes = 60;
            if(n >> 32) {
                zeroes -= 32;
                n >>= 32;
            }
            if(n >> 16) {
                zeroes -= 16;
                n >>= 16;
            }
            if(n >> 8) {
                zeroes -= 8;
                n >>= 8;
            }
            if(n >> 4) {
                zeroes -= 4;
                n >>= 4;
            }
            return "\4\3\2\2\1\1\1\1\0\0\0\0\0\0\0"[n] + zeroes;
        }

        inline int CountLeadingZeros64(uint64_t n) {
#if defined(_MSC_VER) && !defined(__clang__) && defined(_M_X64)
            // MSVC does not have __buitin_clzll. Use _BitScanReverse64.
            unsigned long result = 0;  // NOLINT(runtime/int)
            if(_BitScanReverse64(&result, n)) {
                return 63 - result;
            }
            return 64;
#elif defined(_MSC_VER) && !defined(__clang__)
            // MSVC does not have __buitin_clzll. Compose two calls to _BitScanReverse
            unsigned long result = 0;  // NOLINT(runtime/int)
            if((n >> 32) &&
               _BitScanReverse(&result, static_cast<unsigned long>(n >> 32))) {
                return 31 - result;
            }
            if(_BitScanReverse(&result, static_cast<unsigned long>(n))) {
                return 63 - result;
            }
            return 64;
#elif defined(__GNUC__) || defined(__clang__)
            // Use __builtin_clzll, which uses the following instructions:
            //  x86: bsr
            //  ARM64: clz
            //  PPC: cntlzd
            static_assert(sizeof(unsigned long long) == sizeof(n),  // NOLINT(runtime/int)
                          "__builtin_clzll does not take 64-bit arg");

            // Handle 0 as a special case because __builtin_clzll(0) is undefined.
            if(n == 0) {
                return 64;
            }
            return __builtin_clzll(n);
#else
            return CountLeadingZeros64Slow(n);
#endif
        }

        // Use compiler byte-swapping intrinsics if they are available.  32-bit
        // and 64-bit versions are available in Clang and GCC as of GCC 4.3.0.
        // The 16-bit version is available in Clang and GCC only as of GCC 4.8.0.
        // For simplicity, we enable them all only for GCC 4.8.0 or later.
#if defined(__clang__) || \
    (defined(__GNUC__) && \
     ((__GNUC__ == 4 && __GNUC_MINOR__ >= 8) || __GNUC__ >= 5))
        inline uint64_t gbswap_64(uint64_t host_int) {
            return __builtin_bswap64(host_int);
        }
        inline uint32_t gbswap_32(uint32_t host_int) {
            return __builtin_bswap32(host_int);
        }
        inline uint16_t gbswap_16(uint16_t host_int) {
            return __builtin_bswap16(host_int);
        }

#elif defined(_MSC_VER)
        inline uint64_t gbswap_64(uint64_t host_int) {
            return _byteswap_uint64(host_int);
        }
        inline uint32_t gbswap_32(uint32_t host_int) {
            return _byteswap_ulong(host_int);
        }
        inline uint16_t gbswap_16(uint16_t host_int) {
            return _byteswap_ushort(host_int);
        }

#else
        inline uint64_t gbswap_64(uint64_t host_int) {
#if defined(__GNUC__) && defined(__x86_64__) && !defined(__APPLE__)
            // Adapted from /usr/include/byteswap.h.  Not available on Mac.
            if(__builtin_constant_p(host_int)) {
                return __bswap_constant_64(host_int);
            } else {
                uint64_t result;
                __asm__("bswap %0" : "=r"(result) : "0"(host_int));
                return result;
            }
#elif defined(__GLIBC__)
            return bswap_64(host_int);
#else
            return (((host_int & uint64_t{0xFF}) << 56) |
                    ((host_int & uint64_t{0xFF00}) << 40) |
                    ((host_int & uint64_t{0xFF0000}) << 24) |
                    ((host_int & uint64_t{0xFF000000}) << 8) |
                    ((host_int & uint64_t{0xFF00000000}) >> 8) |
                    ((host_int & uint64_t{0xFF0000000000}) >> 24) |
                    ((host_int & uint64_t{0xFF000000000000}) >> 40) |
                    ((host_int & uint64_t{0xFF00000000000000}) >> 56));
#endif  // bswap_64
        }

        inline uint32_t gbswap_32(uint32_t host_int) {
#if defined(__GLIBC__)
            return bswap_32(host_int);
#else
            return (((host_int & uint32_t{0xFF}) << 24) |
                    ((host_int & uint32_t{0xFF00}) << 8) |
                    ((host_int & uint32_t{0xFF0000}) >> 8) |
                    ((host_int & uint32_t{0xFF000000}) >> 24));
#endif
        }

        inline uint16_t gbswap_16(uint16_t host_int) {
#if defined(__GLIBC__)
            return bswap_16(host_int);
#else
            return (((host_int & uint16_t{0xFF}) << 8) |
                    ((host_int & uint16_t{0xFF00}) >> 8));
#endif
        }

#endif  // intrinsics available

        // FastHexToBufferZeroPad16()
        //
        // Outputs `val` into `out` as if by `snprintf(out, 17, "%016x", val)` but
        // without the terminating null character. Thus `out` must be of length >= 16.
        // Returns the number of non-pad digits of the output (it can never be zero
        // since 0 has one digit).
        inline size_t FastHexToBufferZeroPad16(uint64_t val, char* out) {
//#ifdef __SSE4_2__
            uint64_t be = gbswap_64(val);
            const auto kNibbleMask = _mm_set1_epi8(0xf);
            const auto kHexDigits = _mm_setr_epi8('0', '1', '2', '3', '4', '5', '6', '7',
                                                  '8', '9', 'a', 'b', 'c', 'd', 'e', 'f');
            auto v = _mm_loadl_epi64(reinterpret_cast<__m128i*>(&be));  // load lo dword
            auto v4 = _mm_srli_epi64(v, 4);                            // shift 4 right
            auto il = _mm_unpacklo_epi8(v4, v);                        // interleave bytes
            auto m = _mm_and_si128(il, kNibbleMask);                   // mask out nibbles
            auto hexchars = _mm_shuffle_epi8(kHexDigits, m);           // hex chars
            _mm_storeu_si128(reinterpret_cast<__m128i*>(out), hexchars);
//#else
//            for(int i = 0; i < 8; ++i) {
//                auto byte = (val >> (56 - 8 * i)) & 0xFF;
//                auto* hex = &lxd::numbers_internal::kHexTable[byte * 2];
//                std::memcpy(out + 2 * i, hex, 2);
//            }
//#endif
            // | 0x1 so that even 0 has 1 digit.
            return 16 - CountLeadingZeros64(val | 0x1) / 4;
        }

    }  // namespace numbers_internal

    // SimpleAtoi()
    //
    // Converts a string to an integer, using `safe_strto?()` functions for actual
    // parsing, returning `true` if successful. The `safe_strto?()` functions apply
    // strict checking; the string must be a base-10 integer, optionally followed or
    // preceded by ASCII whitespace, with a value in the range of the corresponding
    // integer type.
    template <typename int_type>
    bool SimpleAtoi(std::string_view str, int_type* out) {
        return numbers_internal::safe_strtoi_base(str, out, 10);
    }

    //inline bool SimpleAtoi(std::string_view str,
    //                                            std::uint128* out) {
    //    return numbers_internal::safe_strtou128_base(str, out, 10);
    //}
}