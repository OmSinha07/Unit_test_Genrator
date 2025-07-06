#include <gtest/gtest.h>
#include "PersonInfo.h"
#include <trantor/utils/Date.h>
#include <drogon/orm/Row.h>
#include <drogon/orm/Result.h>

using namespace drogon_model::org_chart;
using namespace drogon::orm;

TEST(PersonInfoTest, GettersWithRowData) {
    Row row;
    row["id"] = 123;
    row["job_id"] = 456;
    row["job_title"] = "Manager";
    row["department_id"] = 789;
    row["department_name"] = "Engineering";
    row["manager_id"] = 101112;
    row["manager_full_name"] = "John Doe";
    row["first_name"] = "Alice";
    row["last_name"] = "Smith";
    row["hire_date"] = "2022-05-01";

    PersonInfo personInfo(row, -1);

    EXPECT_EQ(personInfo.getValueOfId(), 123);
    EXPECT_EQ(personInfo.getValueOfJobId(), 456);
    EXPECT_EQ(personInfo.getValueOfJobTitle(), "Manager");
    EXPECT_EQ(personInfo.getValueOfDepartmentId(), 789);
    EXPECT_EQ(personInfo.getValueOfDepartmentName(), "Engineering");
    EXPECT_EQ(personInfo.getValueOfManagerId(), 101112);
    EXPECT_EQ(personInfo.getValueOfManagerFullName(), "John Doe");
    EXPECT_EQ(personInfo.getValueOfFirstName(), "Alice");
    EXPECT_EQ(personInfo.getValueOfLastName(), "Smith");
    EXPECT_EQ(personInfo.getHireDate()->toDbStringLocal(), "2022-05-01");
}

TEST(PersonInfoTest, ToJsonSerialization) {
    Row row;
    row["id"] = 1;
    row["job_id"] = 2;
    row["job_title"] = "Developer";
    row["department_id"] = 3;
    row["department_name"] = "Tech";
    row["manager_id"] = 4;
    row["manager_full_name"] = "Jane Doe";
    row["first_name"] = "Bob";
    row["last_name"] = "Johnson";
    row["hire_date"] = "2020-01-15";

    PersonInfo personInfo(row, -1);

    Json::Value json = personInfo.toJson();

    EXPECT_EQ(json["id"].asInt(), 1);
    EXPECT_EQ(json["job_id"].asInt(), 2);
    EXPECT_EQ(json["job_title"].asString(), "Developer");
    EXPECT_EQ(json["department_id"].asInt(), 3);
    EXPECT_EQ(json["department_name"].asString(), "Tech");
    EXPECT_EQ(json["manager_id"].asInt(), 4);
    EXPECT_EQ(json["manager_full_name"].asString(), "Jane Doe");
    EXPECT_EQ(json["first_name"].asString(), "Bob");
    EXPECT_EQ(json["last_name"].asString(), "Johnson");
    EXPECT_EQ(json["hire_date"].asString(), "2020-01-15");
}
