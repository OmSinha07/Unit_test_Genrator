#include <gtest/gtest.h>
#include "User.h"
#include <json/json.h>

using namespace drogon_model::org_chart;

// ✅ Test: Constructor with Json
TEST(UserTest, ConstructorWithJson) {
    Json::Value json;
    json["id"] = 123;
    json["username"] = "john_doe";
    json["password"] = "secure_password";

    User user(json);

    EXPECT_EQ(user.getValueOfId(), 123);
    EXPECT_EQ(user.getValueOfUsername(), "john_doe");
    EXPECT_EQ(user.getValueOfPassword(), "secure_password");
}

// ✅ Test: Constructor with Masqueraded Json
TEST(UserTest, ConstructorWithMasqueradedJson) {
    Json::Value json;
    json["user_id"] = 1;
    json["user_name"] = "alice";
    json["user_pass"] = "pass123";

    std::vector<std::string> masqueradingVector = {"user_id", "user_name", "user_pass"};

    User user(json, masqueradingVector);

    EXPECT_EQ(user.getValueOfId(), 1);
    EXPECT_EQ(user.getValueOfUsername(), "alice");
    EXPECT_EQ(user.getValueOfPassword(), "pass123");
}

// ✅ Test: Setters and Getters
TEST(UserTest, SettersAndGetters) {
    User user;
    user.setId(10);
    user.setUsername("bob");
    user.setPassword("secret");

    EXPECT_EQ(user.getValueOfId(), 10);
    EXPECT_EQ(user.getValueOfUsername(), "bob");
    EXPECT_EQ(user.getValueOfPassword(), "secret");

    EXPECT_NE(user.getId(), nullptr);
    EXPECT_NE(user.getUsername(), nullptr);
    EXPECT_NE(user.getPassword(), nullptr);
}

// ✅ Test: updateByJson
TEST(UserTest, UpdateByJson) {
    User user;
    Json::Value json;
    json["id"] = 99;
    json["username"] = "new_user";
    json["password"] = "new_pass";

    user.updateByJson(json);

    EXPECT_EQ(user.getValueOfId(), 99);
    EXPECT_EQ(user.getValueOfUsername(), "new_user");
    EXPECT_EQ(user.getValueOfPassword(), "new_pass");
}

// ✅ Test: updateByMasqueradedJson
TEST(UserTest, UpdateByMasqueradedJson) {
    User user;
    Json::Value json;
    json["uid"] = 88;
    json["uname"] = "mask_user";
    json["upass"] = "mask_pass";

    std::vector<std::string> masqueradingVector = {"uid", "uname", "upass"};

    user.updateByMasqueradedJson(json, masqueradingVector);

    EXPECT_EQ(user.getValueOfId(), 88);
    EXPECT_EQ(user.getValueOfUsername(), "mask_user");
    EXPECT_EQ(user.getValueOfPassword(), "mask_pass");
}

// ✅ Test: toJson serialization
TEST(UserTest, ToJsonSerialization) {
    User user;
    user.setId(11);
    user.setUsername("serialize_me");
    user.setPassword("12345");

    Json::Value json = user.toJson();

    EXPECT_EQ(json["id"].asInt(), 11);
    EXPECT_EQ(json["username"].asString(), "serialize_me");
    EXPECT_EQ(json["password"].asString(), "12345");
}

// ✅ Test: toMasqueradedJson
TEST(UserTest, ToMasqueradedJson) {
    User user;
    user.setId(22);
    user.setUsername("mask");
    user.setPassword("pass");

    std::vector<std::string> masquerade = {"userId", "userName", "userPass"};
    Json::Value json = user.toMasqueradedJson(masquerade);

    EXPECT_EQ(json[masquerade[0]].asInt(), 22);
    EXPECT_EQ(json[masquerade[1]].asString(), "mask");
    EXPECT_EQ(json[masquerade[2]].asString(), "pass");
}

// ✅ Test: Column Number and Name
TEST(UserTest, ColumnNumberAndName) {
    EXPECT_EQ(User::getColumnNumber(), 3);
    EXPECT_EQ(User::getColumnName(0), "id");
    EXPECT_EQ(User::getColumnName(1), "username");
    EXPECT_EQ(User::getColumnName(2), "password");
    EXPECT_THROW(User::getColumnName(5), std::out_of_range);
}

// ✅ Test: SQL Generation (Only valid calls)
TEST(UserTest, SqlGeneration) {
    User user;

    EXPECT_EQ(user.sqlForFindingByPrimaryKey(), "select * from org_chart_user where id = $1");
    EXPECT_EQ(user.sqlForDeletingByPrimaryKey(), "delete from org_chart_user where id = $1");

    bool needSelection = false;
    EXPECT_EQ(user.sqlForInserting(needSelection), "insert into org_chart_user (id, username, password) values ($1,default,default)");
}
