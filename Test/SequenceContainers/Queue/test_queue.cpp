#include "SequenceContainers/List/stl_list.h"
#include "SequenceContainers/PriorityQueue/stl_priority_queue.h"
#include "SequenceContainers/Queue/stl_queue.h"
#include <gtest/gtest.h>
#include <string>

using namespace ::MiniSTL;
using std::string;

class QueueTest : public testing::Test {
 protected:
  virtual void SetUp() {
    // set pre data
    qi.push(1);
    qf.push(1.0f);
    qd.push(1.0f);
    qb.push(BAR());
    qs.push("hello");
    qs.push("world");
    qqi.push(queue<int>());
  }
  // pre structure
  struct BAR {
    int a;
    double b;
  };
  queue<int> qi;
  queue<float> qf;
  queue<double> qd;
  queue<BAR> qb;
  queue<string> qs;
  queue<queue<int>> qqi;
};

TEST_F(QueueTest, DEFAULT_CTOR) {
  EXPECT_NO_THROW(queue<int>());
  EXPECT_NO_THROW(queue<float>());
  EXPECT_NO_THROW(queue<double>());
  EXPECT_NO_THROW(queue<string>());
  EXPECT_NO_THROW(queue<BAR>());
  EXPECT_NO_THROW(queue<queue<int>>());
}

TEST_F(QueueTest, COPY_CTOR) {
  EXPECT_NO_THROW(queue<int>{qi});
  EXPECT_NO_THROW(queue<float>{qf});
  EXPECT_NO_THROW(queue<double>{qd});
  EXPECT_NO_THROW(queue<string>{qs});
  EXPECT_NO_THROW(queue<BAR>{qb});
  EXPECT_NO_THROW(queue<queue<int>>{qqi});
}

TEST_F(QueueTest, MOVE_CTOR) {
  EXPECT_NO_THROW(queue<int>(MiniSTL::move(qi)));
  EXPECT_NO_THROW(queue<float>(MiniSTL::move(qf)));
  EXPECT_NO_THROW(queue<double>(MiniSTL::move(qd)));
  EXPECT_NO_THROW(queue<string>(MiniSTL::move(qs)));
  EXPECT_NO_THROW(queue<BAR>(MiniSTL::move(qb)));
  EXPECT_NO_THROW(queue<queue<int>>(MiniSTL::move(qqi)));
}

TEST_F(QueueTest, DTOR) {
  queue<int> *p = new queue<int>;
  p->push(1);
  int *p2i = &(p->front());
  EXPECT_EQ(*p2i, 1);
  delete p;
  EXPECT_NE(*p2i, 1);
}

TEST_F(QueueTest, COPYASSIGN_WITH_SELF) {
  queue<int> temp_qi;
  temp_qi.push(2);
  qi = temp_qi;
  EXPECT_EQ(qi.front(), 2);
  queue<float> temp_qf;
  temp_qf.push(2.0f);
  qf = temp_qf;
  EXPECT_EQ(qf.front(), 2.0f);
  queue<double> temp_qd;
  temp_qd.push(2.235f);
  qd = temp_qd;
  EXPECT_EQ(qd.front(), 2.235f);
  queue<string> temp_qs;
  temp_qs.push("cat");
  qs = temp_qs;
  EXPECT_EQ(qs.front(), "cat");
  queue<queue<int>> temp_qqi;
  temp_qqi.push(queue<int>());
  temp_qqi.front().push(1);
  qqi = temp_qqi;
  EXPECT_EQ(qqi.front().front(), 1);
}

TEST_F(QueueTest, MOVEASSIGN_WITH_SELF) {
  queue<int> temp_qi;
  temp_qi.push(2);
  qi = MiniSTL::move(temp_qi);
  EXPECT_EQ(qi.front(), 2);
  queue<float> temp_qf;
  temp_qf.push(2.0f);
  qf = temp_qf;
  EXPECT_EQ(qf.front(), 2.0f);
  queue<double> temp_qd;
  temp_qd.push(2.235f);
  qd = temp_qd;
  EXPECT_EQ(qd.front(), 2.235f);
  queue<string> temp_qs;
  temp_qs.push("cat");
  qs = temp_qs;
  EXPECT_EQ(qs.front(), "cat");
  queue<queue<int>> temp_qqi;
  temp_qqi.push(queue<int>());
  temp_qqi.front().push(1);
  qqi = temp_qqi;
  EXPECT_EQ(qqi.front().front(), 1);
}

TEST_F(QueueTest, TOP) {
  qi.push(2);
  EXPECT_EQ(qi.front(), 1);
  qi.push(3);
  EXPECT_EQ(qi.front(), 1);
  qf.push(2.0f);
  EXPECT_EQ(qf.front(), 1.0f);
  qf.push(3.0f);
  EXPECT_EQ(qf.front(), 1.0f);
  qd.push(2.0f);
  EXPECT_EQ(qd.front(), 1.0f);
  qd.push(3.0f);
  EXPECT_EQ(qd.front(), 1.0f);
  qs.push("cat");
  EXPECT_EQ(qs.front(), "hello");
  qs.push("dog");
  EXPECT_EQ(qs.front(), "hello");
  qqi.push(queue<int>());
  EXPECT_EQ(qqi.front(), queue<int>());
}

TEST_F(QueueTest, SIZE) {
  qi.push(2);
  EXPECT_EQ(qi.size(), 2);
  qi.push(3);
  EXPECT_EQ(qi.size(), 3);
  qf.push(2.0f);
  EXPECT_EQ(qf.size(), 2);
  qf.push(3.0f);
  EXPECT_EQ(qf.size(), 3);
  qd.push(2.0f);
  EXPECT_EQ(qd.size(), 2);
  qd.push(3.0f);
  EXPECT_EQ(qd.size(), 3);
  qs.push("cat");
  EXPECT_EQ(qs.size(), 3);
  qs.push("dog");
  EXPECT_EQ(qs.size(), 4);
  qqi.push(queue<int>());
  EXPECT_EQ(qqi.size(), 2);
}

TEST_F(QueueTest, COMPARATOR) {
  queue<int> temp_qi;
  temp_qi.push(1);
  EXPECT_TRUE(qi == temp_qi);
  queue<float> temp_qf;
  temp_qf.push(2.0f);
  EXPECT_TRUE(qf < temp_qf);
  queue<double> temp_qd;
  temp_qd.push(2.235f);
  EXPECT_TRUE(qd <= temp_qd);
  queue<string> temp_qs;
  temp_qs.push("cat");
  EXPECT_TRUE(qs > temp_qs);
  queue<queue<int>> temp_qqi;
  temp_qqi.push(queue<int>());
  temp_qqi.front().push(1);
  EXPECT_TRUE(qqi != temp_qqi);
}

TEST_F(QueueTest, PUSH_AND_POP) {
  qi.push(2);
  EXPECT_EQ(qi.size(), 2);
  EXPECT_EQ(qi.front(), 1);
  qi.pop();
  EXPECT_EQ(qi.size(), 1);
  EXPECT_EQ(qi.front(), 2);
  qf.push(2);
  EXPECT_EQ(qf.size(), 2);
  EXPECT_EQ(qf.front(), 1.0f);
  qf.pop();
  EXPECT_EQ(qf.size(), 1);
  EXPECT_EQ(qf.front(), 2.0f);
  qd.push(2);
  EXPECT_EQ(qd.size(), 2);
  EXPECT_EQ(qd.front(), 1.0f);
  qd.pop();
  EXPECT_EQ(qd.size(), 1);
  EXPECT_EQ(qd.front(), 2.0f);
  qs.push("cat");
  EXPECT_EQ(qs.size(), 3);
  EXPECT_EQ(qs.front(), "hello");
  qs.pop();
  EXPECT_EQ(qs.size(), 2);
  EXPECT_EQ(qs.front(), "world");
  qs.pop();
  EXPECT_EQ(qs.size(), 1);
  EXPECT_EQ(qs.front(), "cat");
  qqi.push(queue<int>());
  EXPECT_EQ(qqi.size(), 2);
  EXPECT_EQ(qqi.front(), queue<int>());
  qqi.pop();
  EXPECT_EQ(qqi.size(), 1);
  EXPECT_EQ(qqi.front(), queue<int>());
}

TEST_F(QueueTest, SWAP) {
  queue<int> temp_qi;
  temp_qi.push(2);
  swap(qi, temp_qi);
  EXPECT_EQ(qi.front(), 2);
  EXPECT_EQ(temp_qi.front(), 1);
  queue<float> temp_qf;
  temp_qf.push(2.0f);
  swap(qf, temp_qf);
  EXPECT_EQ(qf.front(), 2.0f);
  EXPECT_EQ(temp_qf.front(), 1.0f);
  queue<double> temp_qd;
  temp_qd.push(2.235f);
  swap(qd, temp_qd);
  EXPECT_EQ(qd.front(), 2.235f);
  EXPECT_EQ(temp_qd.front(), 1.0f);
  queue<string> temp_qs;
  temp_qs.push("cat");
  swap(qs, temp_qs);
  EXPECT_EQ(qs.front(), "cat");
  EXPECT_EQ(temp_qs.front(), "hello");
  queue<queue<int>> temp_qqi;
  temp_qqi.push(queue<int>());
  temp_qqi.front().push(1);
  swap(qqi, temp_qqi);
  EXPECT_EQ(qqi.front().front(), 1);
  EXPECT_EQ(temp_qqi.front(), queue<int>());
}

class Priority_Queue_Test : public testing::Test {
 protected:
  virtual void SetUp() {
    // set pre data
    pqvl = {4, 3, 2, 1, 5};
    pqdl = {2, 3, 4, 5, 1};
    pqvg = {6, 8, 9, 10, 7};
    pqdg = {6, 10, 9, 8, 7};
  };
  priority_queue<int, vector<int>, less<int>> pqvl;
  priority_queue<int, deque<int>, less<int>> pqdl;
  priority_queue<int, vector<int>, greater<int>> pqvg;
  priority_queue<int, deque<int>, greater<int>> pqdg;
};

TEST_F(Priority_Queue_Test, DEFAULT_CTOR) {
  EXPECT_NO_THROW((priority_queue<int, vector<int>, less<int>>{}));
  EXPECT_NO_THROW((priority_queue<int, deque<int>, less<int>>{}));
  EXPECT_NO_THROW((priority_queue<float, vector<float>, greater<float>>{}));
  EXPECT_NO_THROW((priority_queue<double, deque<double>, greater<double>>{}));
}

TEST_F(Priority_Queue_Test, CTOR_WITH_INITIALIZER_LIST) {
  EXPECT_NO_THROW(
      (priority_queue<int, vector<int>, less<int>>{1, 2, 3, 4, 5}));
  EXPECT_NO_THROW(
      (priority_queue<int, deque<int>, less<int>>{1, 2, 3, 4, 5}));
  EXPECT_NO_THROW(
      (priority_queue<float, vector<float>, greater<float>>{1, 2, 3, 4, 5}));
  EXPECT_NO_THROW((
      priority_queue<double, deque<double>, greater<double>>{1, 2, 3, 4, 5}));
}

TEST_F(Priority_Queue_Test, CTOR_WITH_INPUTITERATOR) {
  vector<int> temp_vi{1, 2, 3, 4, 5};
  list<int> temp_li{1, 2, 3, 4, 5};
  EXPECT_NO_THROW((priority_queue<int, vector<int>, less<int>>{
      temp_vi.begin(), temp_vi.end()}));
  EXPECT_NO_THROW((priority_queue<int, vector<int>, less<int>>{
      temp_li.begin(), temp_li.end()}));
  EXPECT_NO_THROW((priority_queue<int, deque<int>, less<int>>{
      temp_vi.begin(), temp_vi.end()}));
  EXPECT_NO_THROW((priority_queue<int, deque<int>, less<int>>{
      temp_li.begin(), temp_li.end()}));
  // C API
  int ai[5] = {1, 2, 3, 4, 5};
  EXPECT_NO_THROW((priority_queue<int, vector<int>, less<int>>{ai, ai + 5}));
  EXPECT_NO_THROW((priority_queue<int, deque<int>, less<int>>{ai, ai + 5}));
  EXPECT_NO_THROW(
      (priority_queue<int, deque<int>, greater<int>>{ai, ai + 5}));
  EXPECT_NO_THROW(
      (priority_queue<int, deque<int>, greater<int>>{ai, ai + 5}));
  double ad[5] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
  EXPECT_NO_THROW(
      (priority_queue<double, vector<double>, less<double>>{ad, ad + 5}));
  EXPECT_NO_THROW(
      (priority_queue<double, deque<double>, less<double>>{ad, ad + 5}));
  EXPECT_NO_THROW(
      (priority_queue<double, deque<double>, greater<double>>{ai, ai + 5}));
  EXPECT_NO_THROW(
      (priority_queue<double, deque<double>, greater<double>>{ai, ai + 5}));
}

TEST_F(Priority_Queue_Test, COPY_CTOR) {
  EXPECT_NO_THROW((priority_queue<int, vector<int>, less<int>>{pqvl}));
  EXPECT_NO_THROW((priority_queue<int, vector<int>, greater<int>>{pqvg}));
  EXPECT_NO_THROW((priority_queue<int, deque<int>, less<int>>{pqdl}));
  EXPECT_NO_THROW((priority_queue<int, deque<int>, greater<int>>{pqdg}));
}

TEST_F(Priority_Queue_Test, MOVE_CTOR) {
  EXPECT_NO_THROW(
      (priority_queue<int, vector<int>, less<int>>{MiniSTL::move(pqvl)}));
  EXPECT_NO_THROW(
      (priority_queue<int, vector<int>, greater<int>>{MiniSTL::move(pqvg)}));
  EXPECT_NO_THROW(
      (priority_queue<int, deque<int>, less<int>>{MiniSTL::move(pqdl)}));
  EXPECT_NO_THROW(
      (priority_queue<int, deque<int>, greater<int>>{MiniSTL::move(pqdg)}));
}

TEST_F(Priority_Queue_Test, DTOR) {
  priority_queue<int> *p = new priority_queue<int>;
  p->push(1);
  const int *p2i = &(p->top());
  EXPECT_EQ(*p2i, 1);
  delete p;
  EXPECT_NE(*p2i, 1);
}

TEST_F(Priority_Queue_Test, COPYASSIGN_WITH_SELF) {
  priority_queue<int> temp_qi;
  temp_qi.push(2);
  pqvl = temp_qi;
  EXPECT_EQ(pqvl.top(), 2);
  pqvl.pop();
  EXPECT_TRUE(pqvl.empty());
}

TEST_F(Priority_Queue_Test, MOVEASSIGN_WITH_SELF) {
  priority_queue<int> temp_qi;
  temp_qi.push(2);
  pqvl = MiniSTL::move(temp_qi);
  EXPECT_EQ(pqvl.top(), 2);
  pqvl.pop();
  EXPECT_TRUE(pqvl.empty());
}

TEST_F(Priority_Queue_Test, PUSH_POP_TOP) {
  EXPECT_EQ(pqvl.top(), 5);
  pqvl.pop();
  EXPECT_EQ(pqvl.top(), 4);
  pqvl.pop();
  EXPECT_EQ(pqvl.top(), 3);
  pqvl.pop();
  EXPECT_EQ(pqvl.top(), 2);
  pqvl.pop();
  EXPECT_EQ(pqvl.top(), 1);
  pqvl.pop();
  pqvl.push(5);
  EXPECT_EQ(pqvl.top(), 5);
  pqvl.push(4);
  EXPECT_EQ(pqvl.top(), 5);
  EXPECT_EQ(pqvg.top(), 6);
  pqvg.pop();
  EXPECT_EQ(pqvg.top(), 7);
  pqvg.pop();
  EXPECT_EQ(pqvg.top(), 8);
  pqvg.pop();
  EXPECT_EQ(pqvg.top(), 9);
  pqvg.pop();
  EXPECT_EQ(pqvg.top(), 10);
  pqvg.pop();
  pqvg.push(1);
  EXPECT_EQ(pqvg.top(), 1);
  pqvg.push(4);
  EXPECT_EQ(pqvg.top(), 1);
  EXPECT_EQ(pqdl.top(), 5);
  pqdl.pop();
  EXPECT_EQ(pqdl.top(), 4);
  pqdl.pop();
  EXPECT_EQ(pqdl.top(), 3);
  pqdl.pop();
  EXPECT_EQ(pqdl.top(), 2);
  pqdl.pop();
  EXPECT_EQ(pqdl.top(), 1);
  pqdl.pop();
  pqdl.push(5);
  EXPECT_EQ(pqdl.top(), 5);
  pqdl.push(4);
  EXPECT_EQ(pqdl.top(), 5);
  EXPECT_EQ(pqdg.top(), 6);
  pqdg.pop();
  EXPECT_EQ(pqdg.top(), 7);
  pqdg.pop();
  EXPECT_EQ(pqdg.top(), 8);
  pqdg.pop();
  EXPECT_EQ(pqdg.top(), 9);
  pqdg.pop();
  EXPECT_EQ(pqdg.top(), 10);
  pqdg.pop();
  pqdg.push(1);
  EXPECT_EQ(pqdg.top(), 1);
  pqdg.push(4);
  EXPECT_EQ(pqdg.top(), 1);
}

TEST_F(Priority_Queue_Test, SWAP) {
  priority_queue<int> temp{6, 7, 8, 9, 10};
  temp.swap(pqvl);
  EXPECT_EQ(temp.top(), 5);
  EXPECT_EQ(pqvl.top(), 10);
}