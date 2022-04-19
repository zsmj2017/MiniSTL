#include "SequenceContainers/Vector/stl_vector.h"
#include "SmartPointer/unique_ptr.h"
#include <gtest/gtest.h>
#include <string>

using namespace ::MiniSTL;
using std::string;

class UniquePtrTest : public testing::Test {
 protected:
  void SetUp() override {}
};

TEST(UniquePtrTest, Ctor) {
  {
    unique_ptr<int> up(new int(5));
    EXPECT_TRUE(up);
  }
  {
    unique_ptr<int> up1(new int(5));
    unique_ptr<int> up2(move(up1));
    EXPECT_FALSE(up1);
    EXPECT_TRUE(up2);
  }
}

TEST(UniquePtrTest, Dtor) {
  {
    int *p = new int(5);
    {
      unique_ptr<int> up(p);
    }
    EXPECT_FALSE(*p == 5);
  }
}

TEST(UniquePtrTest, Opearor) {
  {
    unique_ptr<int> up1(new int(5));
    unique_ptr<int> up2 = move(up1);
    EXPECT_TRUE(up2);
  }
  {
    unique_ptr<string> up(new string("Hello,World"));
    EXPECT_EQ(*up, "Hello,World");
  }
  {
    unique_ptr<vector<int>> up(new vector<int>{1, 2, 3});
    EXPECT_EQ(up->front(), 1);
  }
}

TEST(UniquePtrTest, GETTER) {
  {
    vector<int> *vp = new vector<int>{1, 2, 3};
    unique_ptr<vector<int>> up(vp);
    EXPECT_EQ(up.get(), vp);
  }
}

TEST(UniquePtrTest, SETTER) {
  {
    unique_ptr<vector<int>> up(new vector<int>{1, 2, 3});
    up.reset();
    EXPECT_FALSE(up);
    up.reset(new vector<int>{4, 5, 6});
    EXPECT_TRUE(up);
  }
  {
    unique_ptr<string> up(new string("TEST UPTR'S RELEASE"));
    string *sp = up.release();
    EXPECT_FALSE(up);
    EXPECT_TRUE(sp);
    EXPECT_EQ(*sp, "TEST UPTR'S RELEASE");
  }
}