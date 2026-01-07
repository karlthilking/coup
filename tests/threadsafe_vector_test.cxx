#include <gtest/gtest.h>
#include "../include/threadsafe_vector.hxx"
#include <thread>
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
                v.push_back(0);
            }
        });
    }
    for (std::thread& th : threads)
    {
        th.join();
    }
    EXPECT_TRUE(!v.empty());
    EXPECT_EQ(v.size(), 100);
}

TEST_F(test_threadsafe_vector, reserve_test)
{
    threadsafe_vector<int> v;
    std::vector<std::thread> threads;

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
    threads[2].join();
    EXPECT_EQ(v.capacity(), 500);

    threads[0].join();
    theards[1].join();
}
