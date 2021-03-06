/*
 * Copyright 2020 Denis Yaroshevskiy
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * ================================================================
 *
 *  GENERATED CODE. Script: ../src/simd/mm_operations_generator.py
 *
 * ================================================================
 */
#ifndef SIMD_MM_H_
#define SIMD_MM_H_

#include <immintrin.h>

#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace mm {

struct error_t {};

template <typename T>
struct type_t {
  using type = T;
};

// Helper to support pointers.
template <typename T, typename Int>
constexpr bool is_equivalent() {
  if (sizeof(T) != sizeof(Int)) return false;
  if (std::is_signed_v<Int>) return std::is_signed_v<T> || std::is_pointer_v<T>;
  return !std::is_signed_v<T>;
}

// register_i ------------------------------

namespace _mm {

template <std::size_t W>
constexpr auto register_i_impl() {
  if constexpr (W == 128)
    return type_t<__m128i>{};
  else if constexpr (W == 256)
    return type_t<__m256i>{};
  else if constexpr (W == 512)
    return type_t<__m512i>{};
  else
    return error_t{};
}
}  // namespace _mm

template <std::size_t W>
using register_i = typename decltype(_mm::register_i_impl<W>())::type;

// sizes -----------------------------------

template <typename Register>
constexpr std::size_t bit_width() {
  if constexpr (std::is_same_v<Register, register_i<128>>) {
    return 128;
  } else if constexpr (std::is_same_v<Register, register_i<256>>) {
    return 256;
  } else if constexpr (std::is_same_v<Register, register_i<512>>) {
    return 512;
  }
  throw error_t{};
}

template <typename Register>
constexpr size_t byte_width() {
  return bit_width<Register>() / 8;
}

template <typename Register>
constexpr size_t alignment() {
  return alignof(Register);
}

// load/store ------------------------------

__attribute__((no_sanitize_address)) inline register_i<128> load(
    const register_i<128>* addr) {
  return _mm_load_si128(addr);
}

__attribute__((no_sanitize_address)) inline register_i<256> load(
    const register_i<256>* addr) {
  return _mm256_load_si256(addr);
}

__attribute__((no_sanitize_address)) inline register_i<512> load(
    const register_i<512>* addr) {
  return _mm512_load_si512(addr);
}

__attribute__((no_sanitize_address)) inline register_i<128> loadu(
    const register_i<128>* addr) {
  return _mm_loadu_si128(addr);
}

__attribute__((no_sanitize_address)) inline register_i<256> loadu(
    const register_i<256>* addr) {
  return _mm256_loadu_si256(addr);
}

__attribute__((no_sanitize_address)) inline register_i<512> loadu(
    const register_i<512>* addr) {
  return _mm512_loadu_si512(addr);
}

inline void store(register_i<128>* addr, register_i<128> a) {
  _mm_store_si128(addr, a);
}

inline void store(register_i<256>* addr, register_i<256> a) {
  _mm256_store_si256(addr, a);
}

inline void store(register_i<512>* addr, register_i<512> a) {
  _mm512_store_si512(addr, a);
}

// set one value everywhere ----------------

// Does not exist for floats.
template <typename Register>
inline auto setzero() {
  static constexpr size_t register_width = bit_width<Register>();
  if constexpr (register_width == 128)
    return _mm_setzero_si128();
  else if constexpr (register_width == 256)
    return _mm256_setzero_si256();
  else if constexpr (register_width == 512)
    return _mm512_setzero_si512();
  else
    return error_t{};
}

template <typename Register, typename T>
inline auto set1(T a) {
  static constexpr std::size_t register_width = bit_width<Register>();
  static constexpr std::size_t t_width = sizeof(T) * 8;

  if constexpr (register_width == 128 && t_width == 8)
    return _mm_set1_epi8((std::int8_t)a);
  else if constexpr (register_width == 128 && t_width == 16)
    return _mm_set1_epi16((std::int16_t)a);
  else if constexpr (register_width == 128 && t_width == 32)
    return _mm_set1_epi32((std::int32_t)a);
  else if constexpr (register_width == 128 && t_width == 64)
    return _mm_set1_epi64x((std::int64_t)a);
  else if constexpr (register_width == 256 && t_width == 8)
    return _mm256_set1_epi8((std::int8_t)a);
  else if constexpr (register_width == 256 && t_width == 16)
    return _mm256_set1_epi16((std::int16_t)a);
  else if constexpr (register_width == 256 && t_width == 32)
    return _mm256_set1_epi32((std::int32_t)a);
  else if constexpr (register_width == 256 && t_width == 64)
    return _mm256_set1_epi64x((std::int64_t)a);
  else if constexpr (register_width == 512 && t_width == 8)
    return _mm512_set1_epi8((std::int8_t)a);
  else if constexpr (register_width == 512 && t_width == 16)
    return _mm512_set1_epi16((std::int16_t)a);
  else if constexpr (register_width == 512 && t_width == 32)
    return _mm512_set1_epi32((std::int32_t)a);
  else if constexpr (register_width == 512 && t_width == 64)
    return _mm512_set1_epi64((std::int64_t)a);
  else
    return error_t{};
}

// min/max ---------------------------------

template <typename T, typename Register>
inline auto min(Register a, Register b) {
  static constexpr size_t register_width = bit_width<Register>();

  if constexpr (register_width == 128 && is_equivalent<T, std::int8_t>())
    return _mm_min_epi8(a, b);
  else if constexpr (register_width == 128 && is_equivalent<T, std::uint8_t>())
    return _mm_min_epu8(a, b);
  else if constexpr (register_width == 128 && is_equivalent<T, std::int16_t>())
    return _mm_min_epi16(a, b);
  else if constexpr (register_width == 128 && is_equivalent<T, std::uint16_t>())
    return _mm_min_epu16(a, b);
  else if constexpr (register_width == 128 && is_equivalent<T, std::int32_t>())
    return _mm_min_epi32(a, b);
  else if constexpr (register_width == 128 && is_equivalent<T, std::uint32_t>())
    return _mm_min_epu32(a, b);
  else if constexpr (register_width == 128 && is_equivalent<T, std::int64_t>())
    return _mm_min_epi64(a, b);
  else if constexpr (register_width == 128 && is_equivalent<T, std::uint64_t>())
    return _mm_min_epu64(a, b);
  else if constexpr (register_width == 256 && is_equivalent<T, std::int8_t>())
    return _mm256_min_epi8(a, b);
  else if constexpr (register_width == 256 && is_equivalent<T, std::uint8_t>())
    return _mm256_min_epu8(a, b);
  else if constexpr (register_width == 256 && is_equivalent<T, std::int16_t>())
    return _mm256_min_epi16(a, b);
  else if constexpr (register_width == 256 && is_equivalent<T, std::uint16_t>())
    return _mm256_min_epu16(a, b);
  else if constexpr (register_width == 256 && is_equivalent<T, std::int32_t>())
    return _mm256_min_epi32(a, b);
  else if constexpr (register_width == 256 && is_equivalent<T, std::uint32_t>())
    return _mm256_min_epu32(a, b);
  else if constexpr (register_width == 256 && is_equivalent<T, std::int64_t>())
    return _mm256_min_epi64(a, b);
  else if constexpr (register_width == 256 && is_equivalent<T, std::uint64_t>())
    return _mm256_min_epu64(a, b);
  else if constexpr (register_width == 512 && is_equivalent<T, std::int8_t>())
    return _mm512_min_epi8(a, b);
  else if constexpr (register_width == 512 && is_equivalent<T, std::uint8_t>())
    return _mm512_min_epu8(a, b);
  else if constexpr (register_width == 512 && is_equivalent<T, std::int16_t>())
    return _mm512_min_epi16(a, b);
  else if constexpr (register_width == 512 && is_equivalent<T, std::uint16_t>())
    return _mm512_min_epu16(a, b);
  else if constexpr (register_width == 512 && is_equivalent<T, std::int32_t>())
    return _mm512_min_epi32(a, b);
  else if constexpr (register_width == 512 && is_equivalent<T, std::uint32_t>())
    return _mm512_min_epu32(a, b);
  else if constexpr (register_width == 512 && is_equivalent<T, std::int64_t>())
    return _mm512_min_epi64(a, b);
  else if constexpr (register_width == 512 && is_equivalent<T, std::uint64_t>())
    return _mm512_min_epu64(a, b);
  else
    return error_t{};
}

template <typename T, typename Register>
inline auto max(Register a, Register b) {
  static constexpr size_t register_width = bit_width<Register>();

  if constexpr (register_width == 128 && is_equivalent<T, std::int8_t>())
    return _mm_max_epi8(a, b);
  else if constexpr (register_width == 128 && is_equivalent<T, std::uint8_t>())
    return _mm_max_epu8(a, b);
  else if constexpr (register_width == 128 && is_equivalent<T, std::int16_t>())
    return _mm_max_epi16(a, b);
  else if constexpr (register_width == 128 && is_equivalent<T, std::uint16_t>())
    return _mm_max_epu16(a, b);
  else if constexpr (register_width == 128 && is_equivalent<T, std::int32_t>())
    return _mm_max_epi32(a, b);
  else if constexpr (register_width == 128 && is_equivalent<T, std::uint32_t>())
    return _mm_max_epu32(a, b);
  else if constexpr (register_width == 128 && is_equivalent<T, std::int64_t>())
    return _mm_max_epi64(a, b);
  else if constexpr (register_width == 128 && is_equivalent<T, std::uint64_t>())
    return _mm_max_epu64(a, b);
  else if constexpr (register_width == 256 && is_equivalent<T, std::int8_t>())
    return _mm256_max_epi8(a, b);
  else if constexpr (register_width == 256 && is_equivalent<T, std::uint8_t>())
    return _mm256_max_epu8(a, b);
  else if constexpr (register_width == 256 && is_equivalent<T, std::int16_t>())
    return _mm256_max_epi16(a, b);
  else if constexpr (register_width == 256 && is_equivalent<T, std::uint16_t>())
    return _mm256_max_epu16(a, b);
  else if constexpr (register_width == 256 && is_equivalent<T, std::int32_t>())
    return _mm256_max_epi32(a, b);
  else if constexpr (register_width == 256 && is_equivalent<T, std::uint32_t>())
    return _mm256_max_epu32(a, b);
  else if constexpr (register_width == 256 && is_equivalent<T, std::int64_t>())
    return _mm256_max_epi64(a, b);
  else if constexpr (register_width == 256 && is_equivalent<T, std::uint64_t>())
    return _mm256_max_epu64(a, b);
  else if constexpr (register_width == 512 && is_equivalent<T, std::int8_t>())
    return _mm512_max_epi8(a, b);
  else if constexpr (register_width == 512 && is_equivalent<T, std::uint8_t>())
    return _mm512_max_epu8(a, b);
  else if constexpr (register_width == 512 && is_equivalent<T, std::int16_t>())
    return _mm512_max_epi16(a, b);
  else if constexpr (register_width == 512 && is_equivalent<T, std::uint16_t>())
    return _mm512_max_epu16(a, b);
  else if constexpr (register_width == 512 && is_equivalent<T, std::int32_t>())
    return _mm512_max_epi32(a, b);
  else if constexpr (register_width == 512 && is_equivalent<T, std::uint32_t>())
    return _mm512_max_epu32(a, b);
  else if constexpr (register_width == 512 && is_equivalent<T, std::int64_t>())
    return _mm512_max_epi64(a, b);
  else if constexpr (register_width == 512 && is_equivalent<T, std::uint64_t>())
    return _mm512_max_epu64(a, b);
  else
    return error_t{};
}

// comparisons -----------------------------

template <typename T, typename Register>
inline auto cmpeq(Register a, Register b) {
  static constexpr size_t register_width = bit_width<Register>();
  static constexpr size_t t_width = sizeof(T) * 8;
  if constexpr (register_width == 128 && t_width == 8)
    return _mm_cmpeq_epi8(a, b);
  else if constexpr (register_width == 128 && t_width == 16)
    return _mm_cmpeq_epi16(a, b);
  else if constexpr (register_width == 128 && t_width == 32)
    return _mm_cmpeq_epi32(a, b);
  else if constexpr (register_width == 128 && t_width == 64)
    return _mm_cmpeq_epi64(a, b);
  else if constexpr (register_width == 256 && t_width == 8)
    return _mm256_cmpeq_epi8(a, b);
  else if constexpr (register_width == 256 && t_width == 16)
    return _mm256_cmpeq_epi16(a, b);
  else if constexpr (register_width == 256 && t_width == 32)
    return _mm256_cmpeq_epi32(a, b);
  else if constexpr (register_width == 256 && t_width == 64)
    return _mm256_cmpeq_epi64(a, b);
  else if constexpr (register_width == 512 && t_width == 8)
    return _mm512_cmpeq_epi8(a, b);
  else if constexpr (register_width == 512 && t_width == 16)
    return _mm512_cmpeq_epi16(a, b);
  else if constexpr (register_width == 512 && t_width == 32)
    return _mm512_cmpeq_epi32(a, b);
  else if constexpr (register_width == 512 && t_width == 64)
    return _mm512_cmpeq_epi64(a, b);
  else
    return error_t{};
}

// Instruction is not avaliable for unsigned ints.
template <typename T, typename Register>
inline auto cmpgt(Register a, Register b) {
  static constexpr size_t register_width = bit_width<Register>();
  if constexpr (register_width == 128 && is_equivalent<T, std::int8_t>())
    return _mm_cmpgt_epi8(a, b);
  else if constexpr (register_width == 128 && is_equivalent<T, std::int16_t>())
    return _mm_cmpgt_epi16(a, b);
  else if constexpr (register_width == 128 && is_equivalent<T, std::int32_t>())
    return _mm_cmpgt_epi32(a, b);
  else if constexpr (register_width == 128 && is_equivalent<T, std::int64_t>())
    return _mm_cmpgt_epi64(a, b);
  else if constexpr (register_width == 256 && is_equivalent<T, std::int8_t>())
    return _mm256_cmpgt_epi8(a, b);
  else if constexpr (register_width == 256 && is_equivalent<T, std::int16_t>())
    return _mm256_cmpgt_epi16(a, b);
  else if constexpr (register_width == 256 && is_equivalent<T, std::int32_t>())
    return _mm256_cmpgt_epi32(a, b);
  else if constexpr (register_width == 256 && is_equivalent<T, std::int64_t>())
    return _mm256_cmpgt_epi64(a, b);
  else if constexpr (register_width == 512 && is_equivalent<T, std::int8_t>())
    return _mm512_cmpgt_epi8(a, b);
  else if constexpr (register_width == 512 && is_equivalent<T, std::int16_t>())
    return _mm512_cmpgt_epi16(a, b);
  else if constexpr (register_width == 512 && is_equivalent<T, std::int32_t>())
    return _mm512_cmpgt_epi32(a, b);
  else if constexpr (register_width == 512 && is_equivalent<T, std::int64_t>())
    return _mm512_cmpgt_epi64(a, b);
  else
    return error_t{};
}

// add/sub ---------------------------------

template <typename T, typename Register>
inline auto add(Register a, Register b) {
  static constexpr size_t register_width = bit_width<Register>();
  static constexpr size_t t_width = sizeof(T) * 8;
  if constexpr (register_width == 128 && t_width == 8)
    return _mm_add_epi8(a, b);
  else if constexpr (register_width == 128 && t_width == 16)
    return _mm_add_epi16(a, b);
  else if constexpr (register_width == 128 && t_width == 32)
    return _mm_add_epi32(a, b);
  else if constexpr (register_width == 128 && t_width == 64)
    return _mm_add_epi64(a, b);
  else if constexpr (register_width == 256 && t_width == 8)
    return _mm256_add_epi8(a, b);
  else if constexpr (register_width == 256 && t_width == 16)
    return _mm256_add_epi16(a, b);
  else if constexpr (register_width == 256 && t_width == 32)
    return _mm256_add_epi32(a, b);
  else if constexpr (register_width == 256 && t_width == 64)
    return _mm256_add_epi64(a, b);
  else if constexpr (register_width == 512 && t_width == 8)
    return _mm512_add_epi8(a, b);
  else if constexpr (register_width == 512 && t_width == 16)
    return _mm512_add_epi16(a, b);
  else if constexpr (register_width == 512 && t_width == 32)
    return _mm512_add_epi32(a, b);
  else if constexpr (register_width == 512 && t_width == 64)
    return _mm512_add_epi64(a, b);
  else
    return error_t{};
}

template <typename T, typename Register>
inline auto sub(Register a, Register b) {
  static constexpr size_t register_width = bit_width<Register>();
  static constexpr size_t t_width = sizeof(T) * 8;
  if constexpr (register_width == 128 && t_width == 8)
    return _mm_sub_epi8(a, b);
  else if constexpr (register_width == 128 && t_width == 16)
    return _mm_sub_epi16(a, b);
  else if constexpr (register_width == 128 && t_width == 32)
    return _mm_sub_epi32(a, b);
  else if constexpr (register_width == 128 && t_width == 64)
    return _mm_sub_epi64(a, b);
  else if constexpr (register_width == 256 && t_width == 8)
    return _mm256_sub_epi8(a, b);
  else if constexpr (register_width == 256 && t_width == 16)
    return _mm256_sub_epi16(a, b);
  else if constexpr (register_width == 256 && t_width == 32)
    return _mm256_sub_epi32(a, b);
  else if constexpr (register_width == 256 && t_width == 64)
    return _mm256_sub_epi64(a, b);
  else if constexpr (register_width == 512 && t_width == 8)
    return _mm512_sub_epi8(a, b);
  else if constexpr (register_width == 512 && t_width == 16)
    return _mm512_sub_epi16(a, b);
  else if constexpr (register_width == 512 && t_width == 32)
    return _mm512_sub_epi32(a, b);
  else if constexpr (register_width == 512 && t_width == 64)
    return _mm512_sub_epi64(a, b);
  else
    return error_t{};
}

// movemask --------------------------------

template <typename T, typename Register>
inline auto movemask(Register a) {
  static constexpr size_t register_width = bit_width<Register>();
  static constexpr size_t t_width = sizeof(T) * 8;

  if constexpr (register_width == 128 && t_width == 8)
    return _mm_movemask_epi8(a);
  else if constexpr (register_width == 256 && t_width == 8)
    return _mm256_movemask_epi8(a);
  else
    return error_t{};
}

template <typename T, typename Register>
inline auto blendv(Register a, Register b, Register mask) {
  static constexpr size_t register_width = bit_width<Register>();
  static constexpr size_t t_width = sizeof(T) * 8;

  if constexpr (register_width == 128 && t_width == 8)
    return _mm_blendv_epi8(a, b, mask);
  else if constexpr (register_width == 256 && t_width == 8)
    return _mm256_blendv_epi8(a, b, mask);
  else
    return error_t{};
}

// bitwise ---------------------------------

template <typename Register>
inline auto and_(Register a, Register b) {
  static constexpr size_t register_width = bit_width<Register>();
  if constexpr (register_width == 128)
    return _mm_and_si128(a, b);
  else if constexpr (register_width == 256)
    return _mm256_and_si256(a, b);
  else if constexpr (register_width == 512)
    return _mm512_and_si512(a, b);
  else
    return error_t{};
}

template <typename Register>
inline auto or_(Register a, Register b) {
  static constexpr size_t register_width = bit_width<Register>();
  if constexpr (register_width == 128)
    return _mm_or_si128(a, b);
  else if constexpr (register_width == 256)
    return _mm256_or_si256(a, b);
  else if constexpr (register_width == 512)
    return _mm512_or_si512(a, b);
  else
    return error_t{};
}

template <typename Register>
inline auto xor_(Register a, Register b) {
  static constexpr size_t register_width = bit_width<Register>();
  if constexpr (register_width == 128)
    return _mm_xor_si128(a, b);
  else if constexpr (register_width == 256)
    return _mm256_xor_si256(a, b);
  else if constexpr (register_width == 512)
    return _mm512_xor_si512(a, b);
  else
    return error_t{};
}

template <typename Register>
inline auto andnot(Register a, Register b) {
  static constexpr size_t register_width = bit_width<Register>();
  if constexpr (register_width == 128)
    return _mm_andnot_si128(a, b);
  else if constexpr (register_width == 256)
    return _mm256_andnot_si256(a, b);
  else if constexpr (register_width == 512)
    return _mm512_andnot_si512(a, b);
  else
    return error_t{};
}

}  // namespace mm

#endif  // SIMD_MM_H_
/*
 * Copyright 2020 Denis Yaroshevskiy
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SIMD_PACK_DETAIL_BITS_H_
#define SIMD_PACK_DETAIL_BITS_H_

#include <cstdint>
#include <type_traits>

namespace simd {

inline std::int32_t count_trailing_zeroes(std::uint32_t x) {
  return __builtin_ctz(x);
}

// https://stackoverflow.com/questions/18806481/how-can-i-get-the-position-of-the-least-significant-bit-in-a-number
inline std::uint32_t lsb(std::uint32_t x) {
  return x & -x;
}

// Like a regular < but a less significant bit is treated as
// more significant.
inline bool lsb_less(std::uint32_t x, std::uint32_t y) {
  // zero out common bits.
  const std::uint32_t unequal_bits = x ^ y;
  x &= unequal_bits;
  y &= unequal_bits;

  if (y == 0) return false;
  if (x == 0) return true;

  return lsb(x) > lsb(y);
}

constexpr std::uint32_t set_lower_n_bits(std::uint32_t n) {
  std::uint64_t res{1};
  res <<= n;
  res -= 1;
  return static_cast<std::uint32_t>(res);
}

template <typename N>
constexpr N set_highest_4_bits() {
  N res = 0x80;
  res <<= (sizeof(N) - 1) * 8;
  return res;
}

template <typename N>
constexpr N all_ones() {
  return ~N{0};
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_BITS_H_
/*
 * Copyright 2020 Denis Yaroshevskiy
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SIMD_PACK_DETAIL_ADDRESS_MANIPULATION_H_
#define SIMD_PACK_DETAIL_ADDRESS_MANIPULATION_H_

#include <cstddef>

namespace simd {

constexpr std::ptrdiff_t page_size() { return 1 << 12; }

template <typename T>
T* end_of_page(T* addr) {
  std::uintptr_t upage_size = page_size();
  std::uintptr_t mask = ~(upage_size - 1);
  return reinterpret_cast<T*>((reinterpret_cast<std::uintptr_t>(addr) & mask) +
                              upage_size);
}

template <typename Pack, typename T>
T* previous_aligned_address(T* addr) {
  constexpr std::uintptr_t mask = ~(alignof(Pack) - 1);
  return reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(addr) & mask);
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_ADDRESS_MANIPULATION_H_
/*
 * Copyright 2020 Denis Yaroshevskiy
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SIMD_PACK_DETAIL_MASKS_H_
#define SIMD_PACK_DETAIL_MASKS_H_

#include <array>
#include <cstdint>
#include <type_traits>

namespace simd {

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_MASKS_H_
/*
 * Copyright 2020 Denis Yaroshevskiy
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SIMD_PACK_DETAIL_PACK_DECLARATION_H_
#define SIMD_PACK_DETAIL_PACK_DECLARATION_H_

#include <cstddef>
#include <type_traits>


namespace simd {
namespace _pack_declaration {

template <typename T>
auto select_unsigned_equivalent_type() {
  if constexpr (std::is_pointer_v<T>) {
    static_assert(sizeof(T) == sizeof(std::uint64_t));
    return std::uint64_t{};
  } else {
    return std::make_unsigned_t<T>{};
  }
}

}  // namespace _pack_declaration

template <typename T, std::size_t W>
struct pack {
  // Type properties ==============
  using register_type = mm::register_i<W * sizeof(T) * 8>;

  using vbool_type =
      pack<decltype(_pack_declaration::select_unsigned_equivalent_type<T>()),
           W>;

  using value_type = T;

  static constexpr std::size_t size() { return W; }

  // Data  =========================

  register_type reg;
};

template <typename Pack>
using register_t = typename Pack::register_type;

template <typename Pack>
using vbool_t = typename Pack::vbool_type;

template <typename Pack>
using scalar_t = typename Pack::value_type;

template <typename Pack>
constexpr std::size_t size_v = Pack::size();

template <typename T>
constexpr bool asif_signed_v = std::is_signed_v<T> || std::is_pointer_v<T>;

template <typename T>
using signed_equivalent = std::make_signed_t<T>;

template <typename T>
using unsigned_equivalent =
    decltype(_pack_declaration::select_unsigned_equivalent_type<T>());

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_PACK_DECLARATION_H_
/*
 * Copyright 2020 Denis Yaroshevskiy
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SIMD_PACK_DETAIL_VBOOL_TESTS_H_
#define SIMD_PACK_DETAIL_VBOOL_TESTS_H_

#include <optional>


namespace simd {
namespace _vbool_tests {

template <typename T, std::size_t W>
std::uint32_t movemask(const pack<T, W>& x) {
  return static_cast<std::uint32_t>(mm::movemask<std::uint8_t>(x.reg));
}

}  // _vbool_tests

template <typename T, std::size_t W>
bool all_true(const pack<T, W>& x) {
  return _vbool_tests::movemask(x) == set_lower_n_bits(sizeof(pack<T, W>));
}

template <typename T, std::size_t W>
bool any_true(const pack<T, W>& x) {
  return _vbool_tests::movemask(x);
}

template <typename T, std::size_t W>
bool any_true_ignore_first_n(const pack<T, W>& x, std::uint32_t n) {
  return _vbool_tests::movemask(x) & ~set_lower_n_bits(n * sizeof(T));
}


template <typename T, std::size_t W>
std::optional<std::uint32_t> first_true(const pack<T, W>& x) {
  auto mask = _vbool_tests::movemask(x);
  if (!mask) return std::nullopt;
  return count_trailing_zeroes(mask) / sizeof(T);
}

template <typename T, std::size_t W>
std::optional<std::uint32_t> first_true_ignore_first_n(const pack<T, W>& x) {
  return first_true(x);
}

template <typename T, std::size_t W>
std::optional<std::uint32_t> first_true_ignore_first_n(const pack<T, W>& x, std::uint32_t n) {
  auto mask = _vbool_tests::movemask(x);
  mask &= ~set_lower_n_bits(n * sizeof(T));
  if (!mask) return std::nullopt;
  return count_trailing_zeroes(mask) / sizeof(T);
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_VBOOL_TESTS_H_
/*
 * Copyright 2020 Denis Yaroshevskiy
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SIMD_PACK_DETAIL_STORE_H_
#define SIMD_PACK_DETAIL_STORE_H_


namespace simd {

template <typename T, std::size_t W>
void store(T* addr, const pack<T, W>& a) {
  using reg_t = register_t<pack<T, W>>;
  mm::store(reinterpret_cast<reg_t*>(addr), a.reg);
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_STORE_H_
/*
 * Copyright 2020 Denis Yaroshevskiy
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef SIMD_PACK_DETAIL_PACK_CAST_H_
#define SIMD_PACK_DETAIL_PACK_CAST_H_

#include <type_traits>


namespace simd {

template <typename Pack, typename T, std::size_t W>
Pack cast(const pack<T, W>& x) {
  static_assert(std::is_same_v<register_t<Pack>, register_t<pack<T, W>>>);
  return Pack{x.reg};
}

template <typename U, typename T, std::size_t W>
auto cast_elements(const pack<T, W>& x) {
  return cast<pack<U, W>>(x);
}

template <typename T, std::size_t W>
auto cast_to_bytes(const pack<T, W>& x) {
  return cast<pack<std::uint8_t, W * sizeof(T)>>(x);
}

template <typename T, std::size_t W>
auto cast_to_signed(const pack<T, W>& x) {
  return cast_elements<signed_equivalent<T>>(x);
}

template <typename T, std::size_t W>
auto cast_to_unsigned(const pack<T, W>& x) {
  return cast_elements<unsigned_equivalent<T>>(x);
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_PACK_CAST_H_
/*
 * Copyright 2020 Denis Yaroshevskiy
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SIMD_PACK_DETAIL_SET_H_
#define SIMD_PACK_DETAIL_SET_H_


namespace simd {

template <typename Pack>
Pack set_all(scalar_t<Pack> x) {
  return Pack{mm::set1<register_t<Pack>>(x)};
}

template <typename Pack>
Pack set_zero() {
  return Pack{mm::setzero<register_t<Pack>>()};
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_SET_H_
/*
 * Copyright 2020 Denis Yaroshevskiy
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SIMD_PACK_DETAIL_ARITHMETIC_PAIRWISE_H_
#define SIMD_PACK_DETAIL_ARITHMETIC_PAIRWISE_H_


namespace simd {

template <typename T, std::size_t W>
pack<T, W> add_pairwise(const pack<T, W>& x, const pack<T, W>& y) {
  return pack<T, W>{mm::add<T>(x.reg, y.reg)};
}

template <typename T, std::size_t W>
pack<T, W> sub_pairwise(const pack<T, W>& x, const pack<T, W>& y) {
  return pack<T, W>{mm::sub<T>(x.reg, y.reg)};
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_ARITHMETIC_PAIRWISE_H_
/*
 * Copyright 2020 Denis Yaroshevskiy
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SIMD_PACK_DETAIL_LOAD_H_
#define SIMD_PACK_DETAIL_LOAD_H_

#include <cstdint>
#include <utility>


namespace simd {

template <typename Pack, typename T>
Pack load(const T* addr) {
  using reg_t = register_t<Pack>;
  return Pack{mm::load(reinterpret_cast<const reg_t*>(addr))};
}

template <typename Pack, typename T>
Pack load_unaligned(const T* addr) {
  using reg_t = register_t<Pack>;
  return Pack{mm::loadu(reinterpret_cast<const reg_t*>(addr))};
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_LOAD_H_
/*
 * Copyright 2020 Denis Yaroshevskiy
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SIMD_PACK_DETAIL_COMPARISONS_PAIRWISE_H_
#define SIMD_PACK_DETAIL_COMPARISONS_PAIRWISE_H_

#include <type_traits>


namespace simd {

template <typename T, std::size_t W>
vbool_t<pack<T, W>> equal_pairwise(const pack<T, W>& x, const pack<T, W>& y) {
  return vbool_t<pack<T, W>>{mm::cmpeq<T>(x.reg, y.reg)};
}

template <typename T, std::size_t W>
vbool_t<pack<T, W>> greater_pairwise(const pack<T, W>& x, const pack<T, W>& y) {
  if constexpr (asif_signed_v<T>) {
    return vbool_t<pack<T, W>>{mm::cmpgt<T>(x.reg, y.reg)};
  } else {
    // https://stackoverflow.com/a/33173643/5021064

    const auto convertion_mask = set_all<pack<T, W>>(set_highest_4_bits<T>());

    const auto _x = add_pairwise(x, convertion_mask);
    const auto _y = add_pairwise(y, convertion_mask);

    return greater_pairwise(cast_to_signed(_x), cast_to_signed(_y));
  }
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_COMPARISONS_PAIRWISE_H_
/*
 * Copyright 2020 Denis Yaroshevskiy
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SIMD_PACK_DETAIL_BIT_OPERATIONS_H_
#define SIMD_PACK_DETAIL_BIT_OPERATIONS_H_


namespace simd {

template <typename T, std::size_t W>
pack<T, W> and_(const pack<T, W>& x, const pack<T, W>& y) {
  return pack<T, W>{mm::and_(x.reg, y.reg)};
}

template <typename T, std::size_t W>
pack<T, W> or_(const pack<T, W>& x, const pack<T, W>& y) {
  return pack<T, W>{mm::or_(x.reg, y.reg)};
}

template <typename T, std::size_t W>
pack<T, W> xor_(const pack<T, W>& x, const pack<T, W>& y) {
  return pack<T, W>{mm::xor_(x.reg, y.reg)};
}

template <typename T, std::size_t W>
pack<T, W> not_x_and_y(const pack<T, W>& x, const pack<T, W>& y) {
  return pack<T, W>{mm::andnot(x.reg, y.reg)};
}

template <typename T, std::size_t W>
pack<T, W> not_(const pack<T, W>& x) {
  using uscalar = unsigned_equivalent<T>;
  const T FF = (T)all_ones<uscalar>();
  return not_x_and_y(x, set_all<pack<T, W>>(FF));
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_BIT_OPERATIONS_H_
/*
 * Copyright 2020 Denis Yaroshevskiy
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SIMD_PACK_DETAIL_BLEND_H_
#define SIMD_PACK_DETAIL_BLEND_H_


namespace simd {

template <typename T, std::size_t W>
pack<T, W> blend(const pack<T, W>& x, const pack<T, W>& y,
                 const vbool_t<pack<T, W>>& mask) {
  return pack<T, W>{mm::blendv<std::uint8_t>(x.reg, y.reg, mask.reg)};
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_BLEND_H_
/*
 * Copyright 2020 Denis Yaroshevskiy
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SIMD_PACK_DETAIL_MINMAX_PAIRWISE_H
#define SIMD_PACK_DETAIL_MINMAX_PAIRWISE_H


namespace simd {

template <typename T, std::size_t W>
pack<T, W> min_pairwise(const pack<T, W>& x, const pack<T, W>& y) {
 if constexpr (sizeof(T) < 8) {
    return pack<T, W>{mm::min<T>(x.reg, y.reg)};
  } else {
    // blend: if true take second.
    return blend(x, y, greater_pairwise(x, y));
  }
}

template <typename T, std::size_t W>
pack<T, W> max_pairwise(const pack<T, W>& x, const pack<T, W>& y) {
  if constexpr (sizeof(T) < 8) {
    return pack<T, W>{mm::max<T>(x.reg, y.reg)};
  } else {
    // blend: if true take second.
    return blend(y, x, greater_pairwise(x, y));
  }
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_MINMAX_PAIRWISE_H
/*
 * Copyright 2020 Denis Yaroshevskiy
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SIMD_PACK_DETAIL_COMPARISONS_H
#define SIMD_PACK_DETAIL_COMPARISONS_H


namespace simd {
namespace _comparisons {

template <std::size_t W>
std::uint32_t movemask(const pack<std::uint8_t, W>& x) {
  return static_cast<std::uint32_t>(mm::movemask<std::uint8_t>(x.reg));
}

}  // namespace _comparisons

template <typename T, size_t W>
bool equal_full(const pack<T, W>& x, const pack<T, W>& y) {
  // For equality (for integers) we are OK to use bitwise equality.
  // We could also use memcmp, on clang that produced ~the same code.
  // However on gcc it didn't - I decided against it.

  const auto x_bytes = cast_to_bytes(x);
  const auto y_bytes = cast_to_bytes(y);

  const auto eq_bytes = equal_pairwise(x_bytes, y_bytes);
  return all_true(eq_bytes);
}

template <typename T, std::size_t W>
bool less_lexicographical(const pack<T, W>& x, const pack<T, W>& y) {
  // TODO: try an alternative implmentation - compare for equality,
  // store/compare unequal elements.

  // The trick is: compare both with mininum.
  // If x compares equal to the left of y we return true.

  const pack<T, W> mins = min_pairwise(x, y);
  const auto x_cmp = equal_pairwise(x, mins);
  const auto y_cmp = equal_pairwise(y, mins);

  // I'm good doing this with bytes, since for not bytes
  // we get more bytes.
  // FFFFF0000 and FF00 would both compare the same if comparing bytes.

  const std::uint32_t x_mmask = _comparisons::movemask(cast_to_bytes(x_cmp));
  const std::uint32_t y_mmask = _comparisons::movemask(cast_to_bytes(y_cmp));

  // Since the bits are written lsb for the most left one,
  // we need to compare appropriately.
  return lsb_less(y_mmask, x_mmask);
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_COMPARISONS_H
/*
 * Copyright 2020 Denis Yaroshevskiy
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SIMD_PACK_DETAIL_OPERATORS_H_
#define SIMD_PACK_DETAIL_OPERATORS_H_

#include <array>
#include <ostream>


namespace simd {

template <typename T, std::size_t W>
bool operator==(const pack<T, W>& x, const pack<T, W>& y) {
  return equal_full(x, y);
}

template <typename T, std::size_t W>
bool operator!=(const pack<T, W>& x, const pack<T, W>& y) {
  return !(x == y);
}

template <typename T, std::size_t W>
bool operator<(const pack<T, W>& x, const pack<T, W>& y) {
  return less_lexicographical(x, y);
}

template <typename T, std::size_t W>
bool operator>(const pack<T, W>& x, const pack<T, W>& y) {
  return y < x;
}

template <typename T, std::size_t W>
bool operator<=(const pack<T, W>& x, const pack<T, W>& y) {
  return !(y < x);
}

template <typename T, std::size_t W>
bool operator>=(const pack<T, W>& x, const pack<T, W>& y) {
  return !(x < y);
}

template <typename T, std::size_t W>
pack<T, W>& operator+=(pack<T, W>& x, const pack<T, W>& y) {
  x = add_pairwise(x, y);
  return x;
}

template <typename T, std::size_t W>
pack<T, W> operator+(const pack<T, W>& x, const pack<T, W>& y) {
  return add_pairwise(x, y);
}

template <typename T, std::size_t W>
pack<T, W>& operator-=(pack<T, W>& x, const pack<T, W>& y) {
  x = sub_pairwise(x, y);
  return x;
}

template <typename T, std::size_t W>
pack<T, W> operator-(const pack<T, W>& x, const pack<T, W>& y) {
  return sub_pairwise(x, y);
}

template <typename T, std::size_t W>
pack<T, W>& operator&=(pack<T, W>& x, const pack<T, W>& y) {
  x = and_(x, y);
  return x;
}

template <typename T, std::size_t W>
pack<T, W> operator&(const pack<T, W>& x, const pack<T, W>& y) {
  return and_(x, y);
}

template <typename T, std::size_t W>
pack<T, W>& operator|=(pack<T, W>& x, const pack<T, W>& y) {
  x = or_(x, y);
  return x;
}

template <typename T, std::size_t W>
pack<T, W> operator|(const pack<T, W>& x, const pack<T, W>& y) {
  return or_(x, y);
}

template <typename T, std::size_t W>
pack<T, W>& operator^=(pack<T, W>& x, const pack<T, W>& y) {
  x = xor_(x, y);
  return x;
}

template <typename T, std::size_t W>
pack<T, W> operator^(const pack<T, W>& x, const pack<T, W>& y) {
  return xor_(x, y);
}

template <typename T, std::size_t W>
pack<T, W> operator~(const pack<T, W>& x) {
  return not_(x);
}

template <typename T, std::size_t W>
std::ostream& operator<<(std::ostream& out, const pack<T, W>& x) {
  using scalar = scalar_t<pack<T, W>>;

  alignas(pack<T, W>) std::array<scalar, W> stored;
  store(stored.data(), x);

  out << '[' << stored[0];
  for (std::size_t i = 1; i != W; ++i) {
    out << ", " << stored[i];
  }
  out << ']';

  return out;
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_OPERATORS_H_
/*
 * Copyright 2020 Denis Yaroshevskiy
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SIMD_PACK_H_
#define SIMD_PACK_H_








#endif  // SIMD_PACK_H_
