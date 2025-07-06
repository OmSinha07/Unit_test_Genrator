#include <gtest/gtest.h>
#include "models/Department.h"
#include <json/json.h>

using namespace drogon_model::org_chart;

TEST(DepartmentTest, BasicSetGet) {
    Department dept;
    dept.setId(101);
    dept.setName("Computer Science");

    EXPECT_EQ(dept.getValueOfId(), 101);
    EXPECT_EQ(dept.getValueOfName(), "Computer Science");
}

TEST(DepartmentTest, ToJsonConversion) {
    Department dept;
    dept.setId(202);
    dept.setName("Mechanical");

    Json::Value json = dept.toJson();

    EXPECT_TRUE(json.isMember("id"));
    EXPECT_TRUE(json.isMember("name"));
    EXPECT_EQ(json["id"].asInt(), 202);
    EXPECT_EQ(json["name"].asString(), "Mechanical");
}

TEST(DepartmentTest, FromJsonConstructor) {
    Json::Value json;
    json["id"] = 303;
    json["name"] = "Electrical";

    Department dept(json);

    EXPECT_EQ(dept.getValueOfId(), 303);
    EXPECT_EQ(dept.getValueOfName(), "Electrical");
}

TEST(DepartmentTest, ValidateJsonForCreation) {
    Json::Value json;
    json["name"] = "Civil";  // id intentionally not set
    std::string error;

    bool valid = Department::validateJsonForCreation(json, error);

    EXPECT_TRUE(valid);
    EXPECT_TRUE(error.empty());
}
