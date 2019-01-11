#include <gtest/gtest.h>
#include "stl_queue.h"
#include <string>

using namespace::MiniSTL;
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
    struct BAR{ int a; double b; };
    queue<int> qi;
    queue<float> qf;
    queue<double> qd;
    queue<BAR> qb;
    queue<string> qs;
    queue<queue<int> > qqi;
};

TEST_F(QueueTest,DEFAULT_CTOR){
    EXPECT_NO_THROW(queue<int>());
    EXPECT_NO_THROW(queue<float>());
    EXPECT_NO_THROW(queue<double>());
    EXPECT_NO_THROW(queue<string>());
    EXPECT_NO_THROW(queue<BAR>());
    EXPECT_NO_THROW(queue<queue<int> >());
}

TEST_F(QueueTest,COPY_CTOR){
    EXPECT_NO_THROW(queue<int>(qi));
    EXPECT_NO_THROW(queue<float>(qf));
    EXPECT_NO_THROW(queue<double>(qd));
    EXPECT_NO_THROW(queue<string>(qs));
    EXPECT_NO_THROW(queue<BAR>(qb));
    EXPECT_NO_THROW(queue<queue<int> >(qqi));
}

TEST_F(QueueTest,MOVE_CTOR){
    EXPECT_NO_THROW(queue<int>(std::move(qi)));
    EXPECT_NO_THROW(queue<float>(std::move(qf)));
    EXPECT_NO_THROW(queue<double>(std::move(qd)));
    EXPECT_NO_THROW(queue<string>(std::move(qs)));
    EXPECT_NO_THROW(queue<BAR>(std::move(qb)));
    EXPECT_NO_THROW(queue<queue<int> >(std::move(qqi)));
}

TEST_F(QueueTest,DTOR){
    queue<int> *p = new queue<int>;
    p->push(1);
    int* p2i = &(p->front());
    EXPECT_EQ(*p2i,1);
    delete p;
    EXPECT_NE(*p2i,1);
}

TEST_F(QueueTest,COPYASSIGN_WITH_SELF){
    queue<int> temp_qi;temp_qi.push(2);
    qi = temp_qi;
    EXPECT_EQ(qi.front(),2);
    queue<float> temp_qf;temp_qf.push(2.0f);
    qf = temp_qf;
    EXPECT_EQ(qf.front(),2.0f);
    queue<double> temp_qd;temp_qd.push(2.235f);
    qd = temp_qd;
    EXPECT_EQ(qd.front(),2.235f);
    queue<string> temp_qs;temp_qs.push("cat");
    qs = temp_qs;
    EXPECT_EQ(qs.front(),"cat");
    queue<queue<int> > temp_qqi;temp_qqi.push(queue<int>());temp_qqi.front().push(1);
    qqi = temp_qqi;
    EXPECT_EQ(qqi.front().front(),1);
}

TEST_F(QueueTest,MOVEASSIGN_WITH_SELF){
    queue<int> temp_qi;temp_qi.push(2);
    qi = std::move(temp_qi);
    EXPECT_EQ(qi.front(),2);
    queue<float> temp_qf;temp_qf.push(2.0f);
    qf = temp_qf;
    EXPECT_EQ(qf.front(),2.0f);
    queue<double> temp_qd;temp_qd.push(2.235f);
    qd = temp_qd;
    EXPECT_EQ(qd.front(),2.235f);
    queue<string> temp_qs;temp_qs.push("cat");
    qs = temp_qs;
    EXPECT_EQ(qs.front(),"cat");
    queue<queue<int> > temp_qqi;temp_qqi.push(queue<int>());temp_qqi.front().push(1);
    qqi = temp_qqi;
    EXPECT_EQ(qqi.front().front(),1);
}

TEST_F(QueueTest,TOP){
    qi.push(2);EXPECT_EQ(qi.front(),1);
    qi.push(3);EXPECT_EQ(qi.front(),1);
    qf.push(2.0f);EXPECT_EQ(qf.front(),1.0f);
    qf.push(3.0f);EXPECT_EQ(qf.front(),1.0f);
    qd.push(2.0f);EXPECT_EQ(qd.front(),1.0f);
    qd.push(3.0f);EXPECT_EQ(qd.front(),1.0f);
    qs.push("cat");EXPECT_EQ(qs.front(),"hello");
    qs.push("dog");EXPECT_EQ(qs.front(),"hello");
    qqi.push(queue<int>());EXPECT_EQ(qqi.front(),queue<int>());
}

TEST_F(QueueTest,SIZE){
    qi.push(2);EXPECT_EQ(qi.size(),2);
    qi.push(3);EXPECT_EQ(qi.size(),3);
    qf.push(2.0f);EXPECT_EQ(qf.size(),2);
    qf.push(3.0f);EXPECT_EQ(qf.size(),3);
    qd.push(2.0f);EXPECT_EQ(qd.size(),2);
    qd.push(3.0f);EXPECT_EQ(qd.size(),3);
    qs.push("cat");EXPECT_EQ(qs.size(),3);
    qs.push("dog");EXPECT_EQ(qs.size(),4);
    qqi.push(queue<int>());EXPECT_EQ(qqi.size(),2);
}

TEST_F(QueueTest,COMPARATOR){
    queue<int> temp_qi;temp_qi.push(1);
    EXPECT_TRUE(qi == temp_qi);
    queue<float> temp_qf;temp_qf.push(2.0f);
    EXPECT_TRUE(qf < temp_qf);
    queue<double> temp_qd;temp_qd.push(2.235f);
    EXPECT_TRUE(qd <= temp_qd);
    queue<string> temp_qs;temp_qs.push("cat");
    EXPECT_TRUE(qs > temp_qs);
    queue<queue<int> > temp_qqi;temp_qqi.push(queue<int>());temp_qqi.front().push(1);
    EXPECT_TRUE(qqi != temp_qqi);
}

TEST_F(QueueTest,PUSH_AND_POP){
    qi.push(2);EXPECT_EQ(qi.size(),2);EXPECT_EQ(qi.front(),1);
    qi.pop();EXPECT_EQ(qi.size(),1);EXPECT_EQ(qi.front(),2);
    qf.push(2);EXPECT_EQ(qf.size(),2);EXPECT_EQ(qf.front(),1.0f);
    qf.pop();EXPECT_EQ(qf.size(),1);EXPECT_EQ(qf.front(),2.0f);
    qd.push(2);EXPECT_EQ(qd.size(),2);EXPECT_EQ(qd.front(),1.0f);
    qd.pop();EXPECT_EQ(qd.size(),1);EXPECT_EQ(qd.front(),2.0f);
    qs.push("cat");EXPECT_EQ(qs.size(),3);EXPECT_EQ(qs.front(),"hello");
    qs.pop();EXPECT_EQ(qs.size(),2);EXPECT_EQ(qs.front(),"world");
    qs.pop();EXPECT_EQ(qs.size(),1);EXPECT_EQ(qs.front(),"cat");
    qqi.push(queue<int>());EXPECT_EQ(qqi.size(),2);EXPECT_EQ(qqi.front(),queue<int>());
    qqi.pop();EXPECT_EQ(qqi.size(),1);EXPECT_EQ(qqi.front(),queue<int>());
}

TEST_F(QueueTest,SWAP){
    queue<int> temp_qi;temp_qi.push(2);
    swap(qi,temp_qi);
    EXPECT_EQ(qi.front(),2);EXPECT_EQ(temp_qi.front(),1);
    queue<float> temp_qf;temp_qf.push(2.0f);
    swap(qf,temp_qf);
    EXPECT_EQ(qf.front(),2.0f);EXPECT_EQ(temp_qf.front(),1.0f);
    queue<double> temp_qd;temp_qd.push(2.235f);
    swap(qd,temp_qd);
    EXPECT_EQ(qd.front(),2.235f);EXPECT_EQ(temp_qd.front(),1.0f);
    queue<string> temp_qs;temp_qs.push("cat");
    swap(qs,temp_qs);
    EXPECT_EQ(qs.front(),"cat");EXPECT_EQ(temp_qs.front(),"hello");
    queue<queue<int> > temp_qqi;temp_qqi.push(queue<int>());temp_qqi.front().push(1);
    swap(qqi,temp_qqi);
    EXPECT_EQ(qqi.front().front(),1);EXPECT_EQ(temp_qqi.front(),queue<int>());
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
    return 0;
}