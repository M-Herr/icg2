#include "EnumDictionary.hh"
#include "MemoryManagement/MemMgr.hh"
#include "Type/PrimitiveDataType.hh"
#include "Algorithm/CheckpointVisitor.hh"

#include "DataTypeTestSupport.hh"


#include "gtest/gtest.h"

class CheckpointVisitorTest : public ::testing::Test {
    protected:
    DataTypeInator dataTypeInator;
    EnumDictionary enumDictionary;
    MemMgr memMgr;

    CheckpointVisitorTest() {

    }

    ~CheckpointVisitorTest() {

    }
    void SetUp() {}
    void TearDown() {}
};

void verifyIntValue (const CheckpointVisitor::Leaf& leaf, std::string expected_name, int expected_value) {
    EXPECT_EQ(expected_name, leaf.name_stack.toString());

    IntegerValue * int_val = dynamic_cast <IntegerValue *> (leaf.value);
    ASSERT_TRUE(int_val != NULL);
    ASSERT_EQ (expected_value, int_val->getIntegerValue());
}

void verifyDoubleValue (const CheckpointVisitor::Leaf& leaf, std::string expected_name, double expected_value) {
    EXPECT_EQ(expected_name, leaf.name_stack.toString());

    FloatingPointValue * float_val = dynamic_cast <FloatingPointValue *> (leaf.value);
    ASSERT_TRUE(float_val != NULL);
    ASSERT_EQ (expected_value, float_val->getFloatingPointValue());
}

TEST_F(CheckpointVisitorTest, basic) {
    // ARRANGE
    PrimitiveDataType<int> int_data_type;
    int var_to_checkpoint = 100;

    // ACT
    CheckpointVisitor visitor("var_to_checkpoint", &var_to_checkpoint);
    visitor.go(&int_data_type);

    // ASSERT
    auto results = visitor.getResults();
    ASSERT_EQ(results.size(), 1);
    verifyIntValue(results[0], "var_to_checkpoint", var_to_checkpoint);
}

TEST_F(CheckpointVisitorTest, array) {
    // ARRANGE
    const DataType * data_type = dataTypeInator.resolve("int[5]");
    int var_to_checkpoint[5] = {1, 2, 3, 4, 5};

    // ACT
    CheckpointVisitor visitor("var_to_checkpoint", &var_to_checkpoint);
    visitor.go(data_type);

    // ASSERT
    auto results = visitor.getResults();
    ASSERT_EQ(results.size(), 5);
    for (int i = 0; i < 5; i++) {
        std::string var_name = "var_to_checkpoint[" + std::to_string(i) + "]";
        verifyIntValue(results[i], var_name, var_to_checkpoint[i]);
    }
}

TEST_F(CheckpointVisitorTest, composite1) {
    // ARRANGE
    addClassOneToTypeDictionary(&dataTypeInator);
    const DataType * data_type = dataTypeInator.resolve("ClassOne");
    ClassOne var_to_checkpoint = {.a = 5, .b = 1.5};

    // ACT
    CheckpointVisitor visitor("var_to_checkpoint", &var_to_checkpoint);
    visitor.go(data_type);

    // ASSERT
    auto results = visitor.getResults();
    ASSERT_EQ(results.size(), 2);
    verifyIntValue(results[0], "var_to_checkpoint.a", var_to_checkpoint.a);
    verifyDoubleValue(results[1], "var_to_checkpoint.b", var_to_checkpoint.b);
}

TEST_F(CheckpointVisitorTest, composite2) {
    // ARRANGE
    addClassOneToTypeDictionary(&dataTypeInator);
    addClassTwoToTypeDictionary(&dataTypeInator);

    const DataType * data_type = dataTypeInator.resolve("ClassTwo");
    ClassTwo var_to_checkpoint = {.x = 100, .y = 5.5, .c1 = {.a = 5, .b = 1.5}};

    // ACT
    CheckpointVisitor visitor("var_to_checkpoint", &var_to_checkpoint);
    visitor.go(data_type);

    // ASSERT
    auto results = visitor.getResults();
    ASSERT_EQ(results.size(), 4);
    verifyIntValue(results[0], "var_to_checkpoint.x", var_to_checkpoint.x);
    verifyDoubleValue(results[1], "var_to_checkpoint.y", var_to_checkpoint.y);
    verifyIntValue(results[2], "var_to_checkpoint.c1.a", var_to_checkpoint.c1.a);
    verifyDoubleValue(results[3], "var_to_checkpoint.c1.b", var_to_checkpoint.c1.b);
}

TEST_F(CheckpointVisitorTest, composite3) {
    // ARRANGE
    addClassThreeToTypeDictionary(&dataTypeInator);
    const DataType * data_type = dataTypeInator.resolve("ClassThree");

    ClassThree var_to_checkpoint = {.pos = {5.5, 6.6}, .vel = {7.7, 8.8}};

    // ACT
    CheckpointVisitor visitor("var_to_checkpoint", &var_to_checkpoint);
    visitor.go(data_type);

    // ASSERT
    auto results = visitor.getResults();
    ASSERT_EQ(results.size(), 4);
    verifyDoubleValue(results[0], "var_to_checkpoint.pos[0]", var_to_checkpoint.pos[0]);
    verifyDoubleValue(results[1], "var_to_checkpoint.pos[1]", var_to_checkpoint.pos[1]);
    verifyDoubleValue(results[2], "var_to_checkpoint.vel[0]", var_to_checkpoint.vel[0]);
    verifyDoubleValue(results[3], "var_to_checkpoint.vel[1]", var_to_checkpoint.vel[1]);
}