add_executable(lrucachetest lru_cache.test.cpp)
target_link_libraries(lrucachetest ${Boost_LIBRARIES})
add_test(NAME LruCache COMMAND lrucachetest)