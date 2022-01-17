#include "AssociativeContainers/Map/stl_map.h"
#include "AssociativeContainers/Multimap/stl_multimap.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class MapTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};

// test cases from https://sourceforge.net/projects/stlport/
TEST_F(MapTest, map1) {
  typedef map<char, int, less<char>> maptype;
  maptype m;
  // Store mappings between roman numerals and decimals.
  m['l'] = 50;
  m['x'] = 20;// Deliberate mistake.
  m['v'] = 5;
  m['i'] = 1;
  //  cout << "m['x'] = " << m['x'] << endl;
  ASSERT_TRUE(m['x'] == 20);
  m['x'] = 10;// Correct mistake.
  ASSERT_TRUE(m['x'] == 10);
  ASSERT_TRUE(m['z'] == 0);
  //cout << "m['z'] = " << m['z'] << endl; // Note default value is added.
  ASSERT_TRUE(m.count('z') == 1);
  //cout << "m.count('z') = " << m.count('z') << endl;
  pair<maptype::iterator, bool> p = m.insert(pair<const char, int>('c', 100));
  ASSERT_TRUE(p.second);
  ASSERT_TRUE(p.first != m.end());
  ASSERT_TRUE((*p.first).first == 'c');
  ASSERT_TRUE((*p.first).second == 100);

  p = m.insert(pair<const char, int>('c', 100));
  ASSERT_TRUE(!p.second);// already existing pair
  ASSERT_TRUE(p.first != m.end());
  ASSERT_TRUE((*p.first).first == 'c');
  ASSERT_TRUE((*p.first).second == 100);
}

TEST_F(MapTest, mmap1) {
  typedef multimap<char, int, less<char>> mmap;
  mmap m;
  ASSERT_TRUE(m.count('X') == 0);

  m.insert(pair<const char, int>('X', 10));// Standard way.
  ASSERT_TRUE(m.count('X') == 1);

  m.insert(pair<const char, int>('X', 20));// jbuck: standard way
  ASSERT_TRUE(m.count('X') == 2);

  m.insert(pair<const char, int>('Y', 32));// jbuck: standard way
  mmap::iterator i = m.find('X');          // Find first match.
  pair<const char, int> p('X', 10);
  ASSERT_TRUE(*i == p);
  ASSERT_TRUE((*i).first == 'X');
  ASSERT_TRUE((*i).second == 10);
  i++;
  ASSERT_TRUE((*i).first == 'X');
  ASSERT_TRUE((*i).second == 20);
  i++;
  ASSERT_TRUE((*i).first == 'Y');
  ASSERT_TRUE((*i).second == 32);
  i++;
  ASSERT_TRUE(i == m.end());

  size_t count = m.erase('X');
  ASSERT_TRUE(count == 2);
}

TEST_F(MapTest, mmap2) {
  typedef pair<const int, char> pair_type;

  pair_type p1(3, 'c');
  pair_type p2(6, 'f');
  pair_type p3(1, 'a');
  pair_type p4(2, 'b');
  pair_type p5(3, 'x');
  pair_type p6(6, 'f');

  typedef multimap<int, char, less<int>> mmap;

  pair_type array[] = {p1, p2, p3, p4, p5, p6};

  mmap m(array + 0, array + 6);
  mmap::iterator i;
  i = m.lower_bound(3);
  ASSERT_TRUE((*i).first == 3);
  ASSERT_TRUE((*i).second == 'c');

  i = m.upper_bound(3);
  ASSERT_TRUE((*i).first == 6);
  ASSERT_TRUE((*i).second == 'f');
}

TEST_F(MapTest, iterators) {
  typedef map<int, char, less<int>> int_map;
  int_map imap;
  {
    int_map::iterator ite(imap.begin());
    int_map::const_iterator cite(imap.begin());
    ASSERT_TRUE(ite == cite);
    ASSERT_TRUE(!(ite != cite));
    ASSERT_TRUE(cite == ite);
    ASSERT_TRUE(!(cite != ite));
  }

  typedef multimap<int, char, less<int>> mmap;
  typedef mmap::value_type pair_type;

  pair_type p1(3, 'c');
  pair_type p2(6, 'f');
  pair_type p3(1, 'a');
  pair_type p4(2, 'b');
  pair_type p5(3, 'x');
  pair_type p6(6, 'f');

  pair_type array[] = {p1, p2, p3, p4, p5, p6};

  mmap m(array + 0, array + 6);

  {
    mmap::iterator ite(m.begin());
    mmap::const_iterator cite(m.begin());
    //test compare between const_iterator and iterator
    ASSERT_TRUE(ite == cite);
    ASSERT_TRUE(!(ite != cite));
    ASSERT_TRUE(cite == ite);
    ASSERT_TRUE(!(cite != ite));
  }

  // TODO::Need fix!!!
  // map and multimap iterators are NOT comparable
  {
    int_map::iterator mite(imap.begin());
    int_map::const_iterator mcite(imap.begin());
    mmap::iterator mmite(m.begin());
    mmap::const_iterator mmcite(m.begin());
    ASSERT_TRUE(!(mite == mmite));
    ASSERT_TRUE(!(mcite == mmcite));
    ASSERT_TRUE(mite != mmite);
    ASSERT_TRUE(mcite != mmcite);
    ASSERT_TRUE(!(mite == mmcite));
    ASSERT_TRUE(!(mite == mmcite));
    ASSERT_TRUE(mite != mmcite);
    ASSERT_TRUE(mite != mmcite);
  }

  mmap::reverse_iterator ri = m.rbegin();
  ASSERT_TRUE(ri != m.rend());
  ASSERT_TRUE(ri == m.rbegin());
  ASSERT_TRUE((*ri).first == 6);
  ASSERT_TRUE((*ri++).second == 'f');
  ASSERT_TRUE((*ri).first == 6);
  ASSERT_TRUE((*ri).second == 'f');

  mmap const &cm = m;
  mmap::const_reverse_iterator rci = cm.rbegin();
  ASSERT_TRUE(rci != cm.rend());
  ASSERT_TRUE((*rci).first == 6);
  ASSERT_TRUE((*rci++).second == 'f');
  ASSERT_TRUE((*rci).first == 6);
  ASSERT_TRUE((*rci).second == 'f');
}

TEST_F(MapTest, equal_range) {
  typedef map<char, int, less<char>> maptype;
  {
    maptype m;
    m['x'] = 10;

    pair<maptype::iterator, maptype::iterator> ret;
    ret = m.equal_range('x');
    ASSERT_TRUE(ret.first != ret.second);
    ASSERT_TRUE((*(ret.first)).first == 'x');
    ASSERT_TRUE((*(ret.first)).second == 10);
    ASSERT_TRUE(++(ret.first) == ret.second);
  }
  {
    {
      maptype m;

      maptype::iterator i = m.lower_bound('x');
      ASSERT_TRUE(i == m.end());

      i = m.upper_bound('x');
      ASSERT_TRUE(i == m.end());

      pair<maptype::iterator, maptype::iterator> ret;
      ret = m.equal_range('x');
      ASSERT_TRUE(ret.first == ret.second);
      ASSERT_TRUE(ret.first == m.end());
    }

    {
      const maptype m;
      pair<maptype::const_iterator, maptype::const_iterator> ret;
      ret = m.equal_range('x');
      ASSERT_TRUE(ret.first == ret.second);
      ASSERT_TRUE(ret.first == m.end());
    }
  }
}

TEST_F(MapTest, template_methods) {
  struct Key {
    Key() : m_data(0) {}
    Key(int data) : m_data(data) {}

    int m_data;
  };

  struct KeyCmp {
    bool operator()(Key lhs, Key rhs) const { return lhs.m_data < rhs.m_data; }

    bool operator()(Key lhs, int rhs) const { return lhs.m_data < rhs; }

    bool operator()(int lhs, Key rhs) const { return lhs < rhs.m_data; }
  };
  {
    typedef map<Key, int, KeyCmp> Container;
    typedef Container::value_type value;
    Container cont;
    cont.insert(value(Key(1), 1));
    cont.insert(value(Key(2), 2));
    cont.insert(value(Key(3), 3));
    cont.insert(value(Key(4), 4));

    ASSERT_TRUE(cont.count(Key(1)) == 1);
    ASSERT_TRUE(cont.count(1) == 1);
    ASSERT_TRUE(cont.count(5) == 0);

    ASSERT_TRUE(cont.find(2) != cont.end());
    ASSERT_TRUE(cont.lower_bound(2) != cont.end());
    ASSERT_TRUE(cont.upper_bound(2) != cont.end());
    ASSERT_TRUE(cont.equal_range(2) != make_pair(cont.begin(), cont.end()));

    Container const &ccont = cont;
    ASSERT_TRUE(ccont.find(2) != ccont.end());
    ASSERT_TRUE(ccont.lower_bound(2) != ccont.end());
    ASSERT_TRUE(ccont.upper_bound(2) != ccont.end());
    ASSERT_TRUE(ccont.equal_range(2) != make_pair(ccont.end(), ccont.end()));
  }

  {
    typedef multimap<Key, int, KeyCmp> Container;
    typedef Container::value_type value;
    Container cont;
    cont.insert(value(Key(1), 1));
    cont.insert(value(Key(2), 2));
    cont.insert(value(Key(3), 3));
    cont.insert(value(Key(4), 4));

    ASSERT_TRUE(cont.count(Key(1)) == 1);
    ASSERT_TRUE(cont.count(1) == 1);
    ASSERT_TRUE(cont.count(5) == 0);

    ASSERT_TRUE(cont.find(2) != cont.end());
    ASSERT_TRUE(cont.lower_bound(2) != cont.end());
    ASSERT_TRUE(cont.upper_bound(2) != cont.end());
    ASSERT_TRUE(cont.equal_range(2) != make_pair(cont.begin(), cont.end()));

    Container const &ccont = cont;
    ASSERT_TRUE(ccont.find(2) != ccont.end());
    ASSERT_TRUE(ccont.lower_bound(2) != ccont.end());
    ASSERT_TRUE(ccont.upper_bound(2) != ccont.end());
    ASSERT_TRUE(ccont.equal_range(2) != make_pair(ccont.end(), ccont.end()));
  }
}