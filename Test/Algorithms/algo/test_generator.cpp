#include "Algorithms/algo/stl_algo.h"
#include "SequenceContainers/Vector/stl_vector.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class GeneratorTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};

static int cxxrand() { return rand(); }

class Fibonacci {
 public:
  Fibonacci() : v1(0), v2(1) {}
  inline int operator()();

 private:
  int v1;
  int v2;
};

inline int
Fibonacci::operator()() {
  int r = v1 + v2;
  v1 = v2;
  v2 = r;
  return v1;
}

TEST_F(GeneratorTest, gener0) {
  int numbers[10];
  generate(numbers, numbers + 10, cxxrand);
}

TEST_F(GeneratorTest, gener1) {
  vector<int> v1(10);
  Fibonacci generator;
  generate(v1.begin(), v1.end(), generator);

  ASSERT_TRUE(v1[0] == 1);
  ASSERT_TRUE(v1[1] == 1);
  ASSERT_TRUE(v1[2] == 2);
  ASSERT_TRUE(v1[3] == 3);
  ASSERT_TRUE(v1[4] == 5);
  ASSERT_TRUE(v1[5] == 8);
  ASSERT_TRUE(v1[6] == 13);
  ASSERT_TRUE(v1[7] == 21);
  ASSERT_TRUE(v1[8] == 34);
  ASSERT_TRUE(v1[9] == 55);
}
TEST_F(GeneratorTest, genern1) {
  vector<int> v1(10);
  generate_n(v1.begin(), v1.size(), cxxrand);
}
TEST_F(GeneratorTest, genern2) {
  vector<int> v1(10);
  Fibonacci generator;
  generate_n(v1.begin(), v1.size(), generator);

  ASSERT_TRUE(v1[0] == 1);
  ASSERT_TRUE(v1[1] == 1);
  ASSERT_TRUE(v1[2] == 2);
  ASSERT_TRUE(v1[3] == 3);
  ASSERT_TRUE(v1[4] == 5);
  ASSERT_TRUE(v1[5] == 8);
  ASSERT_TRUE(v1[6] == 13);
  ASSERT_TRUE(v1[7] == 21);
  ASSERT_TRUE(v1[8] == 34);
  ASSERT_TRUE(v1[9] == 55);
}
