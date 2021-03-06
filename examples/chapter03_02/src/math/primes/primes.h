#ifndef PRIMES_2018_02_09_H_
  #define PRIMES_2018_02_09_H_

  #include <cmath>
  #include <cstdint>
  #include <limits>
  #include <vector>

  #include <util/memory/util_ring_allocator.h>
  #include <util/utility/util_dynamic_bitset.h>

  namespace math { namespace primes {

  namespace detail {

  float log_integral_asym(const float x);

  float log_integral_asym(const float x)
  {
    // Compute an asymptotic approximation
    // of the log_integral function, li(x).

    using std::log;

    const float log_x = log(x);

    float sum  = 1.0F;
    float term = 1.0F;

    float min_term = (std::numeric_limits<float>::max)();

    // Perform the asymptotic expansion of li(x).
    for(std::uint_fast8_t k = 1U; k <= 64U; ++k)
    {
      term *= k;
      term /= log_x;

      if((k > 3U) && (term > min_term))
      {
        // This asymptotic expansion is actually divergent.
        // Good results can be obtained by stopping the
        // expansion when the series begins to diverge.

        break;
      }

      if(term < min_term)
      {
        min_term = term;
      }

      sum += term;
    }

    return (sum * x) / log_x;
  }

  } // namespace math::primes::detail

  template<const std::size_t maximum_value,
           typename forward_iterator_type>
  void compute_primes_via_sieve(forward_iterator_type first)
  {
    // Use a sieve algorithm to generate a table of primes.
    // In this sieve, the logic is inverted. A value
    // of 1 in the relevant bit means that the number
    // is *not* prime, whereas a value of 0 indicates
    // that the number is prime.

    // Define a local value type.
    using local_value_type = typename std::iterator_traits<forward_iterator_type>::value_type;

    // Establish the upper limit of the sieving.
    using std::sqrt;

    const local_value_type imax =
      static_cast<local_value_type>(sqrt(static_cast<float>(maximum_value)));

    // Create the sieve of primes.

    // Use a custom bitset to contain the sieve.
    // This can save a lot of storage space.
    util::dynamic_bitset<maximum_value,
                         util::ring_allocator<std::uint8_t>>
    sieve;

    for(local_value_type outer_index_i = 2U; outer_index_i < local_value_type(imax); ++outer_index_i)
    {
      if(sieve.test(outer_index_i) == false)
      {
        const local_value_type i2 = outer_index_i * outer_index_i;

        for(local_value_type inner_index_j = i2; inner_index_j < maximum_value; inner_index_j += outer_index_i)
        {
          sieve.set(inner_index_j);
        }
      }
    }

    // Fill the prime numbers into the data table
    // by extracting them from the sieve of primes.

    local_value_type prime_counter  = 2U;
    local_value_type running_number = 2U;

    for(local_value_type i = 2U; i < sieve.size(); ++i)
    {
      if(sieve.test(i) == false)
      {
        *first = running_number;

        ++first;

        ++prime_counter;
      }

      ++running_number;
    }
  }

  } } // namespace math::primes

#endif // PRIMES_2018_02_09_H_
