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

#include "bench_generic/copy.h"

#include <algorithm>
#include <iterator>

#include "algo/copy.h"

namespace bench {

struct algo_copy {
  template <typename I, typename O>
  O operator()(I f, I l, O o) {
    return algo::copy(f, l, o);
  }
};

struct std_copy {
  template <typename I, typename O>
  O operator()(I f, I l, O o) {
    return std::copy(f, l, o);
  }
};

#if defined(STD_COPY)
using alg = std_copy;
#elif defined(ALGO_COPY)
using alg = algo_copy;
#endif

BENCHMARK_TEMPLATE(copy_reverse_iterators_int_vec, 1000, alg);

}  // namespace bench
