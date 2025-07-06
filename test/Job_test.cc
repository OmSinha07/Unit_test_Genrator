#include "gtest/gtest.h"
#include "Job.h"
#include <json/json.h>

using namespace drogon_model::org_chart;

// ✅ Test: Constructor with Json object
TEST(JobTest, ConstructorWithJson) {
    Json::Value json;
    json["id"] = 2;
    json["title"] = "Data Scientist";

    Job job(json);

    EXPECT_EQ(job.getValueOfId(), 2);
    EXPECT_EQ(job.getValueOfTitle(), "Data Scientist");
}

// ✅ Test: Constructor with Json object and masquerading vector
TEST(JobTest, ConstructorWithMasqueradedJson) {
    Json::Value json;
    json["Id"] = 3;
    json["JobTitle"] = "Machine Learning Engineer";

    std::vector<std::string> masqueradingVector = {"Id", "JobTitle"};

    Job job(json, masqueradingVector);

    EXPECT_EQ(job.getValueOfId(), 3);
    EXPECT_EQ(job.getValueOfTitle(), "Machine Learning Engineer");
}

// ✅ Test: Default Constructor
TEST(JobTest, ConstructorWithDefaultValues) {
    Job job;

    EXPECT_EQ(job.getValueOfId(), 0);
    EXPECT_EQ(job.getValueOfTitle(), "");

    // Check shared_ptr validity (NOT expired — shared_ptr doesn't expire, weak_ptr does)
    EXPECT_FALSE(job.getId());     // Should be nullptr
    EXPECT_FALSE(job.getTitle());  // Should be nullptr
}

// ✅ Test: Setters and Getters
TEST(JobTest, SettersAndGetters) {
    Job job;
    job.setId(5);
    job.setTitle("AI Engineer");

    EXPECT_EQ(job.getValueOfId(), 5);
    EXPECT_EQ(job.getValueOfTitle(), "AI Engineer");
}

// ✅ Test: toJson
TEST(JobTest, ToJsonSerialization) {
    Job job;
    job.setId(10);
    job.setTitle("DevOps Engineer");

    Json::Value json = job.toJson();

    EXPECT_FALSE(json["id"].isNull());
    EXPECT_FALSE(json["title"].isNull());
    EXPECT_EQ(json["id"].asInt(), 10);
    EXPECT_EQ(json["title"].asString(), "DevOps Engineer");
}
