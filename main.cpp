#include "skiplist.h"
#include <cassert>
#include <iostream>
#include <sstream>

using namespace std;

// ************************************
// IMPORTANT: The below tests use Random::random to get random numbers, so
// adding more tests (or removing tests) will change how the SkipList is
// structured! Calling Random::random unnecessarily may also break tests.
// On the plus side, each run uses the same random numbers which makes
// debugginng easier.
// ************************************

// testing basic operations: operator<<, add, contains
void test1()
{
    stringstream outSS;
    SkipList skp;
    skp.add(3);
    outSS << skp;
    assert(outSS.str() == "[level: 1] 3-->nullptr\n");

    skp.add(9);
    outSS.str("");
    outSS << skp;
    assert(outSS.str() == "[level: 1] 3-->9-->nullptr\n");

    skp.add(1);
    outSS.str("");
    outSS << skp;
    assert(outSS.str() == "[level: 1] 1-->3-->9-->nullptr\n");

    skp.add(vector<int> {7, 5});
    outSS.str("");
    outSS << skp;
    assert(outSS.str() == "[level: 1] 1-->3-->5-->7-->9-->nullptr\n");

    assert(skp.contains(1) && skp.contains(7) && skp.contains(9));
    assert(!skp.contains(0) && !skp.contains(20));

    // TODO(student) check there are no memory leaks after test completed
    cout << "test1 done." << endl;
}

// testing the copy constructor for the SkipList
void test2()
{
    stringstream outSS;
    SkipList *skp1 = new SkipList;
    skp1->add(vector<int>{9, 1, 7, 5});

    outSS.str("");
    outSS << *skp1;
    assert(outSS.str() == "[level: 1] 1-->5-->7-->9-->nullptr\n");

    SkipList *skp2 = new SkipList(*skp1);
    delete skp1;
    outSS.str("");
    outSS << *skp2;
    assert(outSS.str() == "[level: 1] 1-->5-->7-->9-->nullptr\n");
    delete skp2;
    // TODO(student) check there are no memory leaks after test completed
    cout << "test2 done." << endl;
}

// testing SkipList with multiple levels
void test3()
{
    stringstream outSS;
    SkipList skp(3, 80);
    skp.add(vector<int>{9, 1, 7, 5, 3, 20});

    outSS << skp;
    assert(outSS.str() == "[level: 3] 7-->nullptr\n"
                          "[level: 2] 3-->7-->nullptr\n"
                          "[level: 1] 1-->3-->5-->7-->9-->20-->nullptr\n");

    skp.add(vector<int>{-20, 100});
    outSS.str("");
    outSS << skp;
    assert(outSS.str() ==
           "[level: 3] -20-->7-->100-->nullptr\n"
           "[level: 2] -20-->3-->7-->100-->nullptr\n"
           "[level: 1] -20-->1-->3-->5-->7-->9-->20-->100-->nullptr\n");

    // TODO(student) check that contains searches from top level down
    assert(skp.contains(1) && skp.contains(7) && skp.contains(9));
    assert(!skp.contains(0) && !skp.contains(200));
    assert(skp.contains(-20) && skp.contains(100));

    SkipList skp2(3, 30);
    skp2.add(vector<int>{9, 1, 7, 5, 3, 20});
    outSS.str("");
    outSS << skp2;
    assert(outSS.str() == "[level: 3] nullptr\n"
                          "[level: 2] 3-->5-->nullptr\n"
                          "[level: 1] 1-->3-->5-->7-->9-->20-->nullptr\n");

    assert(skp2.contains(3) && skp2.contains(5) && skp2.contains(20));
    assert(!skp2.contains(-3) && !skp2.contains(4) && !skp2.contains(200));
    // TODO(student) check there are no memory leaks after test completed
    cout << "test3 done." << endl;
}

// removing from multi-level SkipList
void test4()
{
    stringstream outSS;
    SkipList skp(3, 50);
    skp.add(vector<int>{9, 1, 7, 5, 3, 20});

	outSS << skp;
    assert(outSS.str() == "[level: 3] 20-->nullptr\n"
                          "[level: 2] 3-->5-->7-->9-->20-->nullptr\n"
                          "[level: 1] 1-->3-->5-->7-->9-->20-->nullptr\n");
    SkipList skp2(skp);
    assert(skp.remove(1));
    assert(!skp.remove(100));
    assert(skp.remove(9));

    outSS.str("");
    outSS << skp;
    assert(outSS.str() == "[level: 3] 20-->nullptr\n"
                          "[level: 2] 3-->5-->7-->20-->nullptr\n"
                          "[level: 1] 3-->5-->7-->20-->nullptr\n");

    // skp2 should be unchanged
    outSS.str("");
    outSS << skp2;
    assert(outSS.str() == "[level: 3] 20-->nullptr\n"
                          "[level: 2] 3-->5-->7-->9-->20-->nullptr\n"
                          "[level: 1] 1-->3-->5-->7-->9-->20-->nullptr\n");
    // TODO(student) check there are no memory leaks after test completed
    cout << "test4 done." << endl;
}

int main()
{
    test1();
    test2();
    test3();
    test4();
    cout << "Done." << endl;
    return 0;
}
