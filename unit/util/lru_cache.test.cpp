/*******************************************************************
 Module: LRU Cache unit test

 Author: Rafael Sá Menezes

 Date: January 2019

 Test Plan:
   - Constructors
   - Base Methods
   - Integrity/Atomicity
 \*******************************************************************/

#define BOOST_TEST_MODULE "LRU Cache"

#include <util/lru_cache.h>
#include <boost/test/unit_test.hpp>
namespace utf = boost::unit_test;

// ******************** TESTS ********************

// ** Constructors
// Check whether the object is initialized correctly

BOOST_AUTO_TEST_SUITE(constructors)
BOOST_AUTO_TEST_CASE(ok)
{
  lru_cache<int, int> obj(10);
  size_t expected = 10;
  size_t actual = obj.max_size();
  BOOST_TEST(expected == actual);
}

BOOST_AUTO_TEST_CASE(fail)
{
  lru_cache<int, int> obj(10);
  size_t expected = 11;
  size_t actual = obj.max_size();
  BOOST_TEST(expected != actual);
}
BOOST_AUTO_TEST_SUITE_END()