#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include "../include/coup_err.hxx"
#include <iostream>

class test_json : public testing::Test
{
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(test_json, required_fields) {
    std::cout << REQUIRED_CONFIG_FIELDS;}

TEST_F(test_json, no_config) {
    std::cout << NO_CONFIG_ERR;
}

TEST_F(test_json, missing_fields) {
    std::cout << MISSING_FIELDS_ERR;
}

TEST_F(test_json, invalid_field) {
    std::cout <<
        INVALID_FIELD_ERR("cpp");
}

TEST_F(test_json, invalid_entry) {
    std::cout <<
        INVALID_ENTRY_ERR("cpp", "c++18");
}

