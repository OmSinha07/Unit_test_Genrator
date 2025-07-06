#include <gtest/gtest.h>
#include "Person.h"
#include "Department.h"
#include <trantor/utils/Date.h>

using namespace drogon_model::org_chart;

// ✅ Test: Default Constructor
TEST(PersonConstructorTest, DefaultConstructor) {
    Person person;
    EXPECT_EQ(person.getId().get(), nullptr);
    EXPECT_EQ(person.getJobId().get(), nullptr);
    EXPECT_EQ(person.getDepartmentId().get(), nullptr);
    EXPECT_EQ(person.getManagerId().get(), nullptr);
    EXPECT_EQ(person.getFirstName().get(), nullptr);
    EXPECT_EQ(person.getLastName().get(), nullptr);
    EXPECT_EQ(person.getHireDate().get(), nullptr);
}

// ✅ Test: JSON Constructor
TEST(PersonConstructorTest, JsonConstructor) {
    Json::Value json;
    json["id"] = 1;
    json["jobId"] = 2;
    json["departmentId"] = 3;
    json["managerId"] = 4;
    json["firstName"] = "John";
    json["lastName"] = "Doe";
    json["hireDate"] = "2024-01-01";

    Person person(json);

    EXPECT_EQ(person.getValueOfId(), 1);
    EXPECT_EQ(person.getValueOfJobId(), 2);
    EXPECT_EQ(person.getValueOfDepartmentId(), 3);
    EXPECT_EQ(person.getValueOfManagerId(), 4);
    EXPECT_EQ(person.getValueOfFirstName(), "John");
    EXPECT_EQ(person.getValueOfLastName(), "Doe");

    ASSERT_NE(person.getHireDate(), nullptr);
    EXPECT_EQ(person.getHireDate()->toDbStringLocal(), "2024-01-01");
}

// ✅ Test: Masqueraded JSON Constructor
TEST(PersonConstructorTest, MasqueradedJsonConstructor) {
    Json::Value json;
    json["person_id"] = 10;
    json["person_jobId"] = 20;
    json["person_departmentId"] = 30;
    json["person_managerId"] = 40;
    json["person_firstName"] = "Alice";
    json["person_lastName"] = "Smith";
    json["person_hireDate"] = "2023-06-15";

    std::vector<std::string> masqueradingVector = {
        "person_id", "person_jobId", "person_departmentId",
        "person_managerId", "person_firstName", "person_lastName", "person_hireDate"
    };

    Person person(json, masqueradingVector);

    EXPECT_EQ(person.getValueOfId(), 10);
    EXPECT_EQ(person.getValueOfJobId(), 20);
    EXPECT_EQ(person.getValueOfDepartmentId(), 30);
    EXPECT_EQ(person.getValueOfManagerId(), 40);
    EXPECT_EQ(person.getValueOfFirstName(), "Alice");
    EXPECT_EQ(person.getValueOfLastName(), "Smith");

    ASSERT_NE(person.getHireDate(), nullptr);
    EXPECT_EQ(person.getHireDate()->toDbStringLocal(), "2023-06-15");
}

// ✅ Test: Setters & Getters
TEST(PersonGettersSettersTest, BasicSettersAndGetters) {
    Person person;
    person.setId(5);
    person.setJobId(6);
    person.setDepartmentId(7);
    person.setManagerId(8);
    person.setFirstName("Foo");
    person.setLastName("Bar");
    person.setHireDate(trantor::Date::fromDbStringLocal("2025-12-31"));

    EXPECT_EQ(person.getValueOfId(), 5);
    EXPECT_EQ(person.getValueOfJobId(), 6);
    EXPECT_EQ(person.getValueOfDepartmentId(), 7);
    EXPECT_EQ(person.getValueOfManagerId(), 8);
    EXPECT_EQ(person.getValueOfFirstName(), "Foo");
    EXPECT_EQ(person.getValueOfLastName(), "Bar");

    ASSERT_NE(person.getHireDate(), nullptr);
    EXPECT_EQ(person.getHireDate()->toDbStringLocal(), "2025-12-31");
}

// ✅ Test: toJson() Serialization
TEST(PersonToJsonTest, BasicJsonSerialization) {
    Person person;
    person.setId(1);
    person.setJobId(2);
    person.setDepartmentId(3);
    person.setManagerId(4);
    person.setFirstName("John");
    person.setLastName("Doe");
    person.setHireDate(trantor::Date::fromDbStringLocal("2022-05-01"));

    Json::Value json = person.toJson();

    EXPECT_EQ(json["id"].asInt(), 1);
    EXPECT_EQ(json["jobId"].asInt(), 2);
    EXPECT_EQ(json["departmentId"].asInt(), 3);
    EXPECT_EQ(json["managerId"].asInt(), 4);
    EXPECT_EQ(json["firstName"].asString(), "John");
    EXPECT_EQ(json["lastName"].asString(), "Doe");
    EXPECT_EQ(json["hireDate"].asString(), "2022-05-01");
}

// ✅ Test: toMasqueradedJson() Serialization
TEST(PersonToMasqueradedJsonTest, BasicMasqueradedJsonSerialization) {
    Person person;
    person.setId(1);
    person.setJobId(2);
    person.setDepartmentId(3);
    person.setManagerId(4);
    person.setFirstName("Masq");
    person.setLastName("User");
    person.setHireDate(trantor::Date::fromDbStringLocal("2022-10-01"));

    std::vector<std::string> masqueradingVector = {
        "id", "jobId", "departmentId", "managerId", "firstName", "lastName", "hireDate"
    };

    Json::Value json = person.toMasqueradedJson(masqueradingVector);

    EXPECT_EQ(json["id"].asInt(), 1);
    EXPECT_EQ(json["jobId"].asInt(), 2);
    EXPECT_EQ(json["departmentId"].asInt(), 3);
    EXPECT_EQ(json["managerId"].asInt(), 4);
    EXPECT_EQ(json["firstName"].asString(), "Masq");
    EXPECT_EQ(json["lastName"].asString(), "User");
    EXPECT_EQ(json["hireDate"].asString(), "2022-10-01");
}

// ✅ Test: updateByJson()
TEST(PersonUpdateByJsonTest, BasicUpdate) {
    Person person;
    Json::Value json;
    json["id"] = 9;
    json["firstName"] = "Updated";

    person.updateByJson(json);

    EXPECT_EQ(person.getValueOfId(), 9);
    EXPECT_EQ(person.getValueOfFirstName(), "Updated");
}

// ✅ Test: validateJsonForCreation()
TEST(PersonValidateJsonForCreationTest, ValidInput) {
    Json::Value json;
    json["firstName"] = "Test";
    json["lastName"] = "User";

    std::string err;
    bool isValid = Person::validateJsonForCreation(json, err);

    EXPECT_TRUE(isValid);
    EXPECT_TRUE(err.empty());
}

// ✅ Test: Get Department Relationship
TEST(PersonGetDepartmentRelationshipTest, BasicRelationship) {
    Person person;
    person.setId(1);  // Required to avoid null checks

    drogon::orm::DbClientPtr clientPtr;
    auto callback = [](Department department) {
        // Mocked callback, nothing to assert here
    };

    person.getDepartment(clientPtr, callback, nullptr);

    EXPECT_NE(person.getId(), nullptr);
}
