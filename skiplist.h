/**
 * SkipList https://en.wikipedia.org/wiki/Skip_list
 * also used by MemSQL
 * https://www.singlestore.com/blog/what-is-skiplist-why-skiplist-index-for-memsql/
 * SkipList with a depth of 1 is similar to a linked list
 * Each item has a p percent chance of being at the next level up
 * If p is 50%,
 * All elements are inserted at the lowest Level (1)
 * 50% of the elements inserted in Level = 2
 * 50% of 50%, or 25% of the elements inserted in Level = 3
 * and so on
 *
 * If a Skip List has only 1 level
 * Insert O(n), Search O(n)
 * For Level > 1
 * Insert O(log n), Search O(log n)
 * Modifying p allows us to trade off search speed and storage cost
 *
 */

#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <iostream>
#include <vector>

using namespace std;

// only SkipList class can create SNode objects

class SkipList;

class SNode {
  // SkipList can access SNode's private variables
  friend class SkipList;

  // operator<< for printing can access val directly
  friend ostream &operator<<(ostream &out, const SkipList &skip);

private:
  // constructor
  explicit SNode(int val = 0);

  // copy constructor
//  SNode(const SNode &other);

  // data contained in the object
  int val;

  // link to Next SNode at each level
  vector<SNode *> next;
};

class SkipList {

  // display with level
  friend ostream &operator<<(ostream &out, const SkipList &skip);

private:
  // Number of levels in SkipList
  int levels;

  // probability of inserting at a higher level
  // as an integer between 0% and 100% (exclusive)
  int probability;

  // special head SNode that is always present
  SNode *head;

  // return true p% of time,
  // for p = 50, each node has a 50% chance of being at higher level
  bool shouldInsertAtHigherLevel() const;

  // collect all SNode* objects that come before this value at each level
  // used as a helper funnction to add and remove
  vector<SNode *> getBeforeNodes(int val) const;



public:
  // default SkipList has Depth of 1, just one doubly-linked list
  explicit SkipList(int levels = 1, int probability = 0);

  // copy constructor
  SkipList(const SkipList &other);

  // destructor
  virtual ~SkipList();

  // Add to list, assume no duplicates
  void add(int val);

  // Add to list, assume no duplicates
  void add(const vector<int> &values);

  // return true if successfully removed
  bool remove(int val);

  // return true if found in SkipList
  bool contains(int val) const;
};

#endif // ASS4_SKIPLIST_H
