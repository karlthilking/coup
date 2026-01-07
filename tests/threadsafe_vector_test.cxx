#include <gtest/gtest.h>
#include "../include/threadsafe_vector.hxx"
#include <thread>
#include <string>
#include <vector>

using namespace coup;

class test_threadsafe_vector : public testing::Test
{
protected:
    test_threadsafe_vector() = default;
    ~test_threadsafe_vector() = default;

    void SetUp() override {}
};

TEST_F(test_threadsafe_vector, push_back_test)
{
    threadsafe_vector<int> v;
    std::vector<std::thread> threads;

    for (int i{}; i < 10; ++i)
    {
        threads.emplace_back([&]
        {
            for (int j{}; j < 10; ++j)
            {
                v.push_back(99);
            }
        });
    }
    for (std::thread& th : threads)
    {
        th.join();
    }
    EXPECT_TRUE(!v.empty());
    EXPECT_EQ(v.size(), 100);

    for (size_t i{}; i < 100; ++i)
    {
        EXPECT_EQ(v[i], 99);
    }
}

TEST_F(test_threadsafe_vector, reserve_test)
{
    threadsafe_vector<int> v;
    std::vector<std::thread> threads;
    threads.reserve(3);

    for (int i{}; i < 2; ++i)
    {
        threads.emplace_back([&]
        {
            for (int j{}; j < 100; ++j)
            {
                v.push_back(0);
            }
        });
    }
    threads.emplace_back([&]
    {
        v.reserve(500);
    });
    for (std::thread& th : threads) { th.join(); }

    EXPECT_EQ(v.capacity(), 500);
    EXPECT_EQ(v.size(), 200);
}

TEST_F(test_threadsafe_vector, emplace_back_test)
{
    threadsafe_vector<std::string> v;
    std::vector<std::thread> threads;
    threads.reserve(8);

    for (int i{}; i < 8; ++i)
    {
        threads.emplace_back([&]
        {
            for (int j{}; j < 10; ++j)
            {
                v.emplace_back("hello");
            }
        });
    }
    for (std::thread& th : threads) { th.join(); }

    EXPECT_TRUE(v.size() == 80);
    for (size_t i{}; i < 80; ++i)
    {
        EXPECT_EQ(v[i], "hello");
    }
    EXPECT_EQ(v.front(), v.back());
}
