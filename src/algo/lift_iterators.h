/*
 * Copyright 2019 Denis Yaroshevskiy
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

#ifndef ALGO_LIFT_ITERATORS_H
#define ALGO_LIFT_ITERATORS_H

#include <algorithm>
#include <iterator>
#include <numeric>
#include <utility>
#include <vector>

#include "algo/type_functions.h"

namespace algo {
namespace detail {

template <typename I>
class iterator_with_number {
  I i;
  DifferenceType<I> n;

 public:
  using value_type = ValueType<I>;
  using reference = Reference<I>;
  using pointer = Pointer<I>;
  using difference_type = DifferenceType<I>;
  using iterator_category = IteratorCategory<I>;

  iterator_with_number() = default;
  iterator_with_number(I i, DifferenceType<I> n) : i(i), n(n) {}

  reference operator*() const { return *i; }
  pointer operator->() const { return &**this; }

  iterator_with_number& operator++() {
    ++i;
    ++n;
    return *this;
  }

  iterator_with_number operator++(int) {
    auto tmp = *this;
    ++*this;
    return tmp;
  }

  iterator_with_number& operator--() {
    --i;
    --n;
    return *this;
  }

  iterator_with_number operator--(int) {
    auto tmp = *this;
    --*this;
    return tmp;
  }

  friend bool operator==(const iterator_with_number& x,
                         const iterator_with_number& y) {
    return x.i == y.i;
  }

  friend bool operator!=(const iterator_with_number& x,
                         const iterator_with_number& y) {
    return !(x == y);
  }

  friend bool operator<(const iterator_with_number& x,
                        const iterator_with_number& y) {
    return x.n < y.n;
  }

  friend bool operator<=(const iterator_with_number& x,
                         const iterator_with_number& y) {
    return !(y < x);
  }

  friend bool operator>(const iterator_with_number& x,
                        const iterator_with_number& y) {
    return y < x;
  }

  friend bool operator>=(const iterator_with_number& x,
                         const iterator_with_number& y) {
    return !(x < y);
  }

  friend difference_type operator-(const iterator_with_number& x,
                                   const iterator_with_number& y) {
    return x.n - y.n;
  }
};

}  // namespace detail

template <typename I>
using lifted_iterator = std::conditional_t<RandomAccessIterator<I>, I,
                                           detail::iterator_with_number<I>>;

template <typename I>
std::pair<std::vector<lifted_iterator<I>>, lifted_iterator<I>> lift_as_vector(
    I f, I l) {
  std::vector<lifted_iterator<I>> res;
  if constexpr (RandomAccessIterator<I>) {
    res.resize(std::distance(f, l));
    std::iota(res.begin(), res.end(), f);
    return {std::move(res), l};
  } else {
    DifferenceType<I> n(0);
    while (f != l) {
      res.emplace_back(f, n);
      ++f;
      ++n;
    }
    return {std::move(res), lifted_iterator<I>(f, n)};
  }
}

}  // namespace algo

#endif  // ALGO_LIFT_ITERATORS_H
