#include "SequenceContainers/Vector/stl_vector.h"
#include "SmartPointer/shared_ptr.h"
#include <gtest/gtest.h>
#include <string>

using namespace ::MiniSTL;
using std::string;

class SharedPtrTest : public testing::Test {
 protected:
  void SetUp() override {}
};

TEST_F(SharedPtrTest, Ctor) {
  {
    shared_ptr<int> sp(new int(5));
    EXPECT_TRUE(sp);
  }
  {
    shared_ptr<vector<int>> sp1(new vector<int>{1, 2, 3});
    EXPECT_TRUE(sp1);
    shared_ptr<vector<int>> sp2(sp1);
    EXPECT_TRUE(sp2);
    shared_ptr<vector<int>> sp3 = sp2;
    EXPECT_TRUE(sp3);
  }
  {
    shared_ptr<vector<int>> sp1(new vector<int>{1, 2, 3});
    shared_ptr<vector<int>> sp2(MiniSTL::move(sp1));
    EXPECT_FALSE(sp1);
    EXPECT_TRUE(sp2);
  }
}

TEST_F(SharedPtrTest, Dtor) {
  {
    int *p = new int(5);
    shared_ptr<int> sp1(p);
    shared_ptr<int> sp2(sp1);
    sp1.~shared_ptr();
    EXPECT_TRUE(*p == 5);
  }
}

TEST_F(SharedPtrTest, Operator) {
  {
    shared_ptr<int> sp1(new int(5));
    shared_ptr<int> sp2((new int(6)));
    sp2 = sp1;
    EXPECT_TRUE(sp1);
    EXPECT_TRUE(sp2);
  }
  {
    shared_ptr<int> sp1(new int(5));
    shared_ptr<int> sp2((new int(6)));
    sp2 = MiniSTL::move(sp1);
    EXPECT_FALSE(sp1);
    EXPECT_TRUE(sp2);
  }
  {
    shared_ptr<string> sp(new string("Hello,World"));
    EXPECT_TRUE(sp);
    EXPECT_EQ(*sp, "Hello,World");
  }
  {
    shared_ptr<vector<int>> sp(new vector<int>{1, 2, 3});
    EXPECT_TRUE(sp);
    EXPECT_EQ(sp->front(), 1);
  }
}

TEST_F(SharedPtrTest, GETTER) {
  {
    vector<int> *vp = new vector<int>{1, 2, 3};
    shared_ptr<vector<int>> sp(vp);
    EXPECT_EQ(sp.get(), vp);
  }
  {
    shared_ptr<vector<int>> sp(nullptr);
    EXPECT_EQ(sp.use_count(), 0);
  }
  {
    vector<int> *vp = new vector<int>{1, 2, 3};
    shared_ptr<vector<int>> sp1(vp);
    EXPECT_TRUE(sp1.unique());
    shared_ptr<vector<int>> sp2(sp1);
    EXPECT_EQ(sp1.use_count(), 2);
    sp2.~shared_ptr();
    EXPECT_TRUE(sp1.unique());
  }
}

TEST_F(SharedPtrTest, SETTER) {
  {
    shared_ptr<vector<int>> sp(new vector<int>{1, 2, 3});
    sp.reset();
    EXPECT_FALSE(sp);
    sp.reset(new vector<int>{4, 5, 6});
    EXPECT_TRUE(sp);
  }
}