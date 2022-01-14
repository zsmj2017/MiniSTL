#include <gtest/gtest.h>
#include <string>
#include "SequenceContainers/Vector/stl_vector.h"

using namespace ::MiniSTL;
using std::string;

class VectorTest : public testing::Test {
protected:
    virtual void SetUp() {
        // set pre data
        vi = {1, 2, 3, 4, 5};
        vf = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
        vd = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
        vb.resize(5);
        vs = {"hello", "hello"};
        vvi = {{1, 2, 3}, {1, 2, 3}};
    }
    // pre structure
    struct BAR {
        int a;
        double b;
    };
    vector<int> vi;
    vector<float> vf;
    vector<double> vd;
    vector<BAR> vb;
    vector<string> vs;
    vector<vector<int>> vvi;
};

TEST_F(VectorTest, DEFAULT_CTOR) {
    EXPECT_NO_THROW(vector<int>());
    EXPECT_NO_THROW(vector<float>());
    EXPECT_NO_THROW(vector<double>());
    EXPECT_NO_THROW(vector<string>());
    EXPECT_NO_THROW(vector<BAR>());
    EXPECT_NO_THROW(vector<vector<int>>());
}

TEST_F(VectorTest, CTOR_WITH_SIZE) {
    EXPECT_NO_THROW(vector<int>(5));
    EXPECT_NO_THROW(vector<float>(5));
    EXPECT_NO_THROW(vector<double>(5));
    EXPECT_NO_THROW(vector<string>(5));
    EXPECT_NO_THROW(vector<BAR>(5));
    EXPECT_NO_THROW(vector<vector<int>>(5));
}

TEST_F(VectorTest, CTOR_WITH_VALUE) {
    EXPECT_NO_THROW(vector<int>(5, 5));
    EXPECT_NO_THROW(vector<float>(5, 5.0f));
    EXPECT_NO_THROW(vector<double>(5, 5.0f));
    EXPECT_NO_THROW(vector<string>(5, "hello"));
    EXPECT_NO_THROW(vector<BAR>(5, BAR()));
    EXPECT_NO_THROW(vector<vector<int>>(5, {1, 2, 3}));
}

TEST_F(VectorTest, CTOR_WITH_INPUTITERATOR) {
    EXPECT_NO_THROW(vector<int>(vi.begin(), vi.end()));
    EXPECT_NO_THROW(vector<int>(vi.cbegin(), vi.cend()));
    EXPECT_NO_THROW(vector<float>(vf.begin(), vf.end()));
    EXPECT_NO_THROW(vector<float>(vf.cbegin(), vf.cend()));
    EXPECT_NO_THROW(vector<double>(vd.begin(), vd.end()));
    EXPECT_NO_THROW(vector<double>(vd.cbegin(), vd.cend()));
    EXPECT_NO_THROW(vector<BAR>(vb.begin(), vb.end()));
    EXPECT_NO_THROW(vector<BAR>(vb.cbegin(), vb.cend()));
    EXPECT_NO_THROW(vector<vector<int>>(vvi.begin(), vvi.end()));
    EXPECT_NO_THROW(vector<vector<int>>(vvi.cbegin(), vvi.cend()));
    // C API
    int ai[5] = {1, 2, 3, 4, 5};
    EXPECT_NO_THROW(vector<int>(ai, ai + 5));
    double ad[5] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    EXPECT_NO_THROW(vector<double>(ad, ad + 5));
    string as[5] = {"a", "b", "c", "d", "f"};
    EXPECT_NO_THROW(vector<string>(as, as + 5));
}

TEST_F(VectorTest, CTOR_WITH_INITIAL_LIST) {
    EXPECT_NO_THROW(vector<int>({1, 2, 3, 4, 5}));
    EXPECT_NO_THROW(vector<float>({1.0f, 2.0f, 3.0f, 4.0f, 5.0f}));
    EXPECT_NO_THROW(vector<double>({1.0f, 2.0f, 3.0f, 4.0f, 5.0f}));
    EXPECT_NO_THROW(vector<string>({"hello", "world"}));
    EXPECT_NO_THROW(vector<vector<int>>({{1, 2, 3}, {4, 5, 6}}));
}

TEST_F(VectorTest, COPY_CTOR) {
    EXPECT_NO_THROW(vector<int>(vi));
    EXPECT_NO_THROW(vector<float>(vf));
    EXPECT_NO_THROW(vector<double>(vd));
    EXPECT_NO_THROW(vector<string>(vs));
    EXPECT_NO_THROW(vector<BAR>(vb));
    EXPECT_NO_THROW(vector<vector<int>>(vvi));
}

TEST_F(VectorTest, MOVE_CTOR) {
    EXPECT_NO_THROW(vector<int>(std::move(vi)));
    EXPECT_NO_THROW(vector<float>(std::move(vf)));
    EXPECT_NO_THROW(vector<double>(std::move(vd)));
    EXPECT_NO_THROW(vector<string>(std::move(vs)));
    EXPECT_NO_THROW(vector<BAR>(std::move(vb)));
    EXPECT_NO_THROW(vector<vector<int>>(std::move(vvi)));
}

TEST_F(VectorTest, DTOR) {
    /*
    // It is not recommended in C++ to explicitly call the destructor manually.
    // Because this operation will cause repeated memory release.
    vector<int> temp_vi = {1,2,3};
    int* p = &temp_vi[0];
    temp_vi.~vector();
    EXPECT_FALSE(*p == 1);
    */
    auto *p = new vector<int>(5, 1);
    int *p2i = &((*p)[0]);
    EXPECT_EQ(*p2i, 1);
    delete p;
    EXPECT_NE(*p2i, 1);
}

TEST_F(VectorTest, COPYASSIGN_WITH_SELF) {
    vector<int> temp_vi(5, 2);
    vi = temp_vi;
    for (auto i : vi) EXPECT_EQ(i, 2);
    vector<float> temp_vf(5, 2.0f);
    vf = temp_vf;
    for (auto f : vf) EXPECT_EQ(f, 2.0f);
    vector<double> temp_vd(5, 2.235f);
    vd = temp_vd;
    for (auto d : vd) EXPECT_EQ(d, 2.235f);
    vector<string> temp_vs(5, "hello");
    vs = temp_vs;
    for (auto s : vs) EXPECT_EQ(s, "hello");
    vector<vector<int>> temp_vvi(5, {1, 2, 3});
    vvi = temp_vvi;
    for (auto vi : vvi) EXPECT_EQ(vi, vector<int>({1, 2, 3}));
}

TEST_F(VectorTest, COPYASSIGN_WITH_INITIAL_LIST) {
    vi = {2, 2, 2, 2, 2};
    for (auto i : vi) EXPECT_EQ(i, 2);
    vf = {2.0f, 2.0f, 2.0f, 2.0f, 2.0f};
    for (auto f : vf) EXPECT_EQ(f, 2.0f);
    vd = {2.235f};
    for (auto d : vd) EXPECT_EQ(d, 2.235f);
    vs = {"hello", "hello", "hello"};
    for (auto s : vs) EXPECT_EQ(s, "hello");
    vvi = {vector<int>({1, 2, 3}), vector<int>({1, 2, 3})};
    for (auto vi : vvi) EXPECT_EQ(vi, vector<int>({1, 2, 3}));
}

TEST_F(VectorTest, MOVEASSIGN_WITH_SELF) {
    vector<int> temp_vi(5, 2);
    vi = std::move(temp_vi);
    for (auto i : vi) EXPECT_EQ(i, 2);
    for (auto i : temp_vi) EXPECT_NE(i, 2);
    vector<float> temp_vf(5, 2.0f);
    vf = std::move(temp_vf);
    for (auto f : vf) EXPECT_EQ(f, 2.0f);
    for (auto f : temp_vf) EXPECT_NE(f, 2.0f);
    vector<double> temp_vd(5, 2.235f);
    vd = std::move(temp_vd);
    for (auto d : vd) EXPECT_EQ(d, 2.235f);
    for (auto d : temp_vd) EXPECT_NE(d, 2.235f);
    vector<string> temp_vs(5, "hello");
    vs = std::move(temp_vs);
    for (auto s : vs) EXPECT_EQ(s, "hello");
    for (auto s : temp_vs) EXPECT_NE(s, "hello");
    vector<vector<int>> temp_vvi(5, {1, 2, 3});
    vvi = std::move(temp_vvi);
    for (auto vi : vvi) EXPECT_EQ(vi, vector<int>({1, 2, 3}));
    for (auto vi : temp_vvi) EXPECT_NE(vi, vector<int>({1, 2, 3}));
}

TEST_F(VectorTest, BEGIN_AND_END) {
    for (auto it = vi.begin(); it != vi.end(); ++it) {
        EXPECT_EQ(*it, it - vi.begin() + 1);
        *it = 5;
    }
    for (auto it = vi.cbegin(); it != vi.cend(); ++it) EXPECT_EQ(*it, 5);
    for (auto it = vi.rbegin(); it != vi.rend(); ++it) {
        EXPECT_EQ(*it, 5);
        *it = 3;
    }
    for (auto it = vi.crbegin(); it != vi.crend(); ++it) {
        EXPECT_EQ(*it, 3);
    }
    for (auto it = vf.begin(); it != vf.end(); ++it) {
        EXPECT_EQ(*it, it - vf.begin() + 1.0f);
        *it = 5.0f;
    }
    for (auto it = vf.cbegin(); it != vf.cend(); ++it) EXPECT_EQ(*it, 5.0f);
    for (auto it = vf.rbegin(); it != vf.rend(); ++it) {
        EXPECT_EQ(*it, 5.0f);
        *it = 3.0f;
    }
    for (auto it = vf.crbegin(); it != vf.crend(); ++it) {
        EXPECT_EQ(*it, 3.0f);
    }
    for (auto it = vd.begin(); it != vd.end(); ++it) {
        EXPECT_EQ(*it, it - vd.begin() + 1.0f);
        *it = 5.0f;
    }
    for (auto it = vd.cbegin(); it != vd.cend(); ++it) EXPECT_EQ(*it, 5.0f);
    for (auto it = vd.rbegin(); it != vd.rend(); ++it) {
        EXPECT_EQ(*it, 5.0f);
        *it = 3.0f;
    }
    for (auto it = vd.crbegin(); it != vd.crend(); ++it) {
        EXPECT_EQ(*it, 3.0f);
    }
    for (auto it = vs.begin(); it != vs.end(); ++it) {
        EXPECT_EQ(*it, "hello");
        *it = "world";
    }
    for (auto it = vs.cbegin(); it != vs.cend(); ++it) EXPECT_EQ(*it, "world");
    for (auto it = vs.rbegin(); it != vs.rend(); ++it) {
        EXPECT_EQ(*it, "world");
        *it = "hello";
    }
    for (auto it = vs.crbegin(); it != vs.crend(); ++it) {
        EXPECT_EQ(*it, "hello");
    }
    for (auto it = vvi.begin(); it != vvi.end(); ++it) {
        EXPECT_EQ(*it, vector<int>({1, 2, 3}));
        *it = {4, 5, 6};
    }
    for (auto it = vvi.cbegin(); it != vvi.cend(); ++it)
        EXPECT_EQ(*it, vector<int>({4, 5, 6}));
    for (auto it = vvi.rbegin(); it != vvi.rend(); ++it) {
        EXPECT_EQ(*it, vector<int>({4, 5, 6}));
        *it = {7, 8, 9};
    }
    for (auto it = vvi.crbegin(); it != vvi.crend(); ++it) {
        EXPECT_EQ(*it, vector<int>({7, 8, 9}));
    }
}

TEST_F(VectorTest, FRONT_AND_BACK) {
    EXPECT_EQ(vi.front(), 1);
    vi.front() = 2;
    EXPECT_EQ(vi.front(), 2);
    EXPECT_EQ(vi.back(), 5);
    vi.back() = 2;
    EXPECT_EQ(vi.back(), 2);
    EXPECT_EQ(vf.front(), 1.0f);
    vf.front() = 2.0f;
    EXPECT_EQ(vf.front(), 2.0f);
    EXPECT_EQ(vf.back(), 5.0f);
    vf.back() = 2.0f;
    EXPECT_EQ(vf.back(), 2.0f);
    EXPECT_EQ(vd.front(), 1.0f);
    vd.front() = 2.0f;
    EXPECT_EQ(vd.front(), 2.0f);
    EXPECT_EQ(vd.back(), 5.0f);
    vd.back() = 2.0f;
    EXPECT_EQ(vd.back(), 2.0f);
    EXPECT_EQ(vs.front(), "hello");
    vs.front() = "world";
    EXPECT_EQ(vs.front(), "world");
    EXPECT_EQ(vs.back(), "hello");
    vs.back() = "world";
    EXPECT_EQ(vs.back(), "world");
    EXPECT_EQ(vvi.front(), vector<int>({1, 2, 3}));
    vvi.front() = {4, 5, 6};
    EXPECT_EQ(vvi.front(), vector<int>({4, 5, 6}));
    EXPECT_EQ(vvi.back(), vector<int>({1, 2, 3}));
    vvi.back() = {4, 5, 6};
    EXPECT_EQ(vvi.back(), vector<int>({4, 5, 6}));
}

TEST_F(VectorTest, RANDOMACCESS) {
    for (size_t i = 0; i != vi.size(); ++i) EXPECT_EQ(vi[i], i + 1);
    for (size_t i = 0; i != vf.size(); ++i) EXPECT_EQ(vf[i], i + 1.0f);
    for (size_t i = 0; i != vd.size(); ++i) EXPECT_EQ(vd[i], i + 1.0f);
    for (size_t i = 0; i != vs.size(); ++i) EXPECT_EQ(vs[i], "hello");
    for (size_t i = 0; i != vvi.size(); ++i)
        EXPECT_EQ(vvi[i], vector<int>({1, 2, 3}));
}

TEST_F(VectorTest, SIZE_AND_CAPACITY) {
    EXPECT_EQ(vi.size(), 5);
    EXPECT_EQ(vi.capacity(), 5);
    vi.resize(3);
    EXPECT_EQ(vi.size(), 3);
    EXPECT_EQ(vi.capacity(), 5);
    vi.resize(10, 5);
    for (size_t i = 3; i != 10; ++i) EXPECT_EQ(vi[i], 5);
    EXPECT_EQ(vi.size(), 10);
    EXPECT_EQ(vi.capacity(), 10);
    vi.reserve(3);
    EXPECT_NE(vi.size(), 3);
    EXPECT_NE(vi.capacity(), 3);
    vi.reserve(20);
    EXPECT_EQ(vi.size(), 10);
    EXPECT_EQ(vi.capacity(), 20);
    vi.shrink_to_fit();
    EXPECT_EQ(vi.size(), 10);
    EXPECT_EQ(vi.size(), vi.capacity());
}

TEST_F(VectorTest, COMPARATOR) {
    vector<int> temp_vi = {1, 2, 3, 4, 5};
    EXPECT_TRUE(temp_vi == vi);
    EXPECT_FALSE(temp_vi != vi);
    vector<float> temp_vf(vf);
    EXPECT_TRUE(temp_vf == vf);
    EXPECT_FALSE(temp_vf != vf);
    vector<double> temp_vd(vd.cbegin(), vd.cend());
    EXPECT_TRUE(temp_vd == vd);
    EXPECT_FALSE(temp_vd != vd);
    vector<string> temp_vs(std::move(vs));
    EXPECT_FALSE(temp_vs == vs);
    EXPECT_TRUE(temp_vs != vs);
    vector<vector<int>> temp_vvi({{1, 2, 3}, {1, 2, 3}});
    EXPECT_TRUE(temp_vvi == vvi);
    EXPECT_FALSE(temp_vvi != vvi);
}

TEST_F(VectorTest, PUSH_AND_POP) {
    vi.push_back(6);
    EXPECT_EQ(vi.back(), 6);
    EXPECT_EQ(vi.size(), 6);
    EXPECT_EQ(vi.capacity(), 10);
    vi.pop_back();
    EXPECT_EQ(vi.back(), 5);
    EXPECT_EQ(vi.size(), 5);
    EXPECT_EQ(vi.capacity(), 10);
    vf.push_back(6.0f);
    EXPECT_EQ(vf.back(), 6.0f);
    EXPECT_EQ(vf.size(), 6);
    EXPECT_EQ(vf.capacity(), 10);
    vf.pop_back();
    EXPECT_EQ(vf.back(), 5.0f);
    EXPECT_EQ(vf.size(), 5);
    EXPECT_EQ(vf.capacity(), 10);
    vd.push_back(6.0f);
    EXPECT_EQ(vd.back(), 6.0f);
    EXPECT_EQ(vd.size(), 6);
    EXPECT_EQ(vd.capacity(), 10);
    vd.pop_back();
    EXPECT_EQ(vd.back(), 5.0f);
    EXPECT_EQ(vd.size(), 5);
    EXPECT_EQ(vd.capacity(), 10);
    vs.push_back("world");
    EXPECT_EQ(vs.back(), "world");
    EXPECT_EQ(vs.size(), 3);
    EXPECT_EQ(vs.capacity(), 4);
    vs.pop_back();
    EXPECT_EQ(vs.back(), "hello");
    EXPECT_EQ(vs.size(), 2);
    EXPECT_EQ(vs.capacity(), 4);
    vvi.push_back({4, 5, 6});
    EXPECT_EQ(vvi.back(), vector<int>({4, 5, 6}));
    EXPECT_EQ(vvi.size(), 3);
    EXPECT_EQ(vvi.capacity(), 4);
    vvi.pop_back();
    EXPECT_EQ(vvi.back(), vector<int>({1, 2, 3}));
    EXPECT_EQ(vvi.size(), 2);
    EXPECT_EQ(vvi.capacity(), 4);
}

TEST_F(VectorTest, ERASE) {
    vi.erase(vi.begin());
    EXPECT_EQ(vi.front(), 2);
    EXPECT_EQ(vi.size(), 4);
    EXPECT_EQ(vi.capacity(), 5);
    vi.erase(vi.end() - 1, vi.end());
    EXPECT_EQ(vi.back(), 4);
    EXPECT_EQ(vi.size(), 3);
    EXPECT_EQ(vi.capacity(), 5);
    vi.clear();
    EXPECT_EQ(vi.size(), 0);
    EXPECT_EQ(vi.capacity(), 5);
    vf.erase(vf.begin());
    EXPECT_EQ(vf.front(), 2.0f);
    EXPECT_EQ(vf.size(), 4);
    EXPECT_EQ(vf.capacity(), 5);
    vf.erase(vf.end() - 1, vf.end());
    EXPECT_EQ(vf.back(), 4.0f);
    EXPECT_EQ(vf.size(), 3);
    EXPECT_EQ(vf.capacity(), 5);
    vf.clear();
    EXPECT_EQ(vf.size(), 0);
    EXPECT_EQ(vf.capacity(), 5);
    vd.erase(vd.begin() + 1, vd.begin() + 2);
    EXPECT_EQ(vd[1], 3.0f);
    EXPECT_EQ(vd.size(), 4);
    EXPECT_EQ(vd.capacity(), 5);
    vs.erase(vs.begin() + 1, vs.begin() + 2);
    EXPECT_EQ(vs.size(), 1);
    EXPECT_EQ(vs.capacity(), 2);
    vvi.erase(vvi.begin() + 1, vvi.begin() + 2);
    EXPECT_EQ(vvi[1], vector<int>({1, 2, 3}));
    EXPECT_EQ(vvi.size(), 1);
    EXPECT_EQ(vvi.capacity(), 2);
}

TEST_F(VectorTest, INSERT) {
    vi.insert(vi.begin());
    EXPECT_EQ(vi.front(), 0);
    EXPECT_EQ(vi.size(), 6);
    EXPECT_EQ(vi.capacity(), 10);
    vi.insert(vi.begin() + 1, 5);
    EXPECT_EQ(vi[1], 5);
    EXPECT_EQ(vi.size(), 7);
    EXPECT_EQ(vi.capacity(), 10);
    vector<int> temp_vi = {1, 2, 3};
    vi.insert(vi.end(), temp_vi.begin(), temp_vi.end());
    EXPECT_EQ(vi.back(), 3);
    EXPECT_EQ(vi.size(), 10);
    EXPECT_EQ(vi.capacity(), 10);
    vf.insert(vf.begin());
    EXPECT_EQ(vf.front(), 0.0f);
    EXPECT_EQ(vf.size(), 6);
    EXPECT_EQ(vf.capacity(), 10);
    vf.insert(vf.begin() + 1, 5.0f);
    EXPECT_EQ(vf[1], 5.0f);
    EXPECT_EQ(vf.size(), 7);
    EXPECT_EQ(vi.capacity(), 10);
    vector<float> temp_vf = {1.0, 2.0, 3.0, 4.0};
    vf.insert(vf.end(), temp_vf.begin(), temp_vf.end());
    EXPECT_EQ(vf.back(), 4.0f);
    EXPECT_EQ(vf.size(), 11);
    EXPECT_EQ(vf.capacity(), 14);
}

TEST_F(VectorTest, ASSIGN) {
    vi.assign(5, 2);
    for (auto i : vi) EXPECT_EQ(i, 2);
    vector<int> temp_vi = {1, 2};
    vi.assign(temp_vi.begin(), temp_vi.end());
    EXPECT_EQ(vi, temp_vi);
    EXPECT_EQ(vi.size(), 2);
    EXPECT_EQ(vi.capacity(), 5);
    vi.assign({3, 3, 3, 3, 3, 3});
    for (auto i : vi) EXPECT_EQ(i, 3);
    EXPECT_EQ(vi.size(), 6);
    EXPECT_EQ(vi.capacity(), 6);
    vf.assign(5, 2.0f);
    for (auto f : vf) EXPECT_EQ(f, 2.0f);
    vector<float> temp_vf = {1.0f, 2.0f};
    vf.assign(temp_vf.begin(), temp_vf.end());
    EXPECT_EQ(vf, temp_vf);
    EXPECT_EQ(vf.size(), 2);
    EXPECT_EQ(vf.capacity(), 5);
    vf.assign({3.1f, 3.1f, 3.1f, 3.1f, 3.1f, 3.1f});
    for (auto f : vf) EXPECT_EQ(f, 3.1f);
    EXPECT_EQ(vf.size(), 6);
    EXPECT_EQ(vf.capacity(), 6);
    vs.assign(5, "hi");
    for (auto s : vs) EXPECT_EQ(s, "hi");
    vector<string> temp_vs = {"world"};
    vs.assign(temp_vs.begin(), temp_vs.end());
    EXPECT_EQ(vs, temp_vs);
    EXPECT_EQ(vs.size(), 1);
    EXPECT_EQ(vs.capacity(), 5);
    vs.assign({"cat", "cat", "cat"});
    for (auto s : vs) EXPECT_EQ(s, "cat");
    EXPECT_EQ(vs.size(), 3);
    EXPECT_EQ(vs.capacity(), 5);
}
