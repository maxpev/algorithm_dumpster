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

#include "simd/bits.h"
#include "simd/pack_detail/masks.h"
#include "simd/pack_detail/minmax_pairwise.h"
#include "simd/pack_detail/pack_declaration.h"

#include <iostream>

namespace simd {

template <typename T, size_t W>
bool equal_full(const pack<T, W>& x, const pack<T, W>& y) {
  // For equality (for integers) we are OK to use bitwise equality.
  // We could also use memcmp, on clang that produced ~the same code.
  // However on gcc it didn't - I decided against it.

  using reg_t = register_t<pack<T, W>>;

  const reg_t byte_equal = mm::cmpeq<std::uint8_t>(x.reg, y.reg);
  const std::int32_t mmask = mm::movemask<std::uint8_t>(byte_equal);

  static constexpr std::int32_t ones = lower_n_bits_1(sizeof(pack<T, W>));
  return mmask == ones;
}

template <typename T, std::size_t W>
vbool_t<pack<T, W>> equal_pairwise(const pack<T, W>& x, const pack<T, W>& y) {
  return vbool_t<pack<T, W>>{mm::cmpeq<T>(x.reg, y.reg)};
}

/*
template <typename T, std::size_t W>
vbool_t<pack<T, W>> greater_pairwise(const pack<T, W>& x, const pack<T, W>& y) {
  if constexpr (std::is_signed_v<T>) {
    return vbool_t<pack<T, W>> { mm::cmpgt<T>(x.reg, y.reg); };
  } else if constexpr (sizeof(T) < 8) {
    pack<T, W> pick_max = max_pairwise(x, y);
    return not(equal_pairwise(y, pick_max));
  }
}
*/

template <typename T, std::size_t W>
bool less_full(const pack<T, W>& x, const pack<T, W>& y) {
  // The trick is: compare both with mininum.
  // If x compares equal to the left of y we return true.

  const pack<T, W> mins = min_pairwise(x, y);
  const auto x_cmp = equal_pairwise(x, mins);
  const auto y_cmp = equal_pairwise(y, mins);

  // I'm good doing this with bytes, since for not bytes
  // we get more bytes.
  // FFFFF0000 and FF00 would both compare the same if comparing bytes.

  const std::int32_t x_mmask = mm::movemask<std::uint8_t>(x_cmp.reg);
  const std::int32_t y_mmask = mm::movemask<std::uint8_t>(y_cmp.reg);

  // The true bits are where it's equal to minimun.
  // The one that's bigger is going to compare to min the most significatn place.
  return lsb_less(
    static_cast<std::uint32_t>(y_mmask),
    static_cast<std::uint32_t>(x_mmask));
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_COMPARISONS_H
