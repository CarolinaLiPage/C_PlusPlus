#include "skiplist.h"
#include "random.h"
#include <cassert>
#include <climits>
#include <cstdlib>
#include <iostream>
#include <unordered_map>

using namespace std;

// checks if the value needs to be inserted at a higher level
bool SkipList::shouldInsertAtHigherLevel() const
{
    return probability >= Random::random() % 100;
}

// you code goes here
SNode::SNode(int val)
{
    this->val = val;
//    next.push_back(nullptr);
}

// copy constructor
/*
SNode::SNode(const SNode &other)
{
    this->val = other.val;
    for(int i = other.next.size() - 1; i >= 0; i--) {
        if(other.next[i] == nullptr) {
            next.push_back(nullptr);
        } else if(i == other.next.size() - 1) {
            SNode *node = new SNode(*(other.next[i]));
            next.push_back(node);
        } else {
            // Look up node in level 0.

        }
    }
}
*/

SkipList::SkipList(int levels, int probability)
{
    this->levels = levels;
    this->probability = probability;
//    head = nullptr;
    head = new SNode();
    // SkipList starts out with a nullptr for each level.
    for(int i = 0; i < levels; i++)
    {
        head->next.push_back(nullptr);
    }
}

// copy constructor
SkipList::SkipList(const SkipList &other)
{
    SNode *curr = other.head;
    SNode *newCurr;
    this->levels = other.levels;
    this->probability = other.probability;

    head = new SNode();
    for(int i = 0; i < levels; i++)
    {
        head->next.push_back(nullptr);
    }
//    head = nullptr;
    newCurr = head;

    if(curr != nullptr)
    {
        curr = curr->next[levels - 1];  // Skip dummy node at start.
    }

    // Creating level 0 (highest index).
    while(curr != nullptr)
    {
// nullptr....
        newCurr->next[levels - 1] = new SNode();
        SNode *newNode = newCurr->next[levels - 1];
        newNode->val = curr->val;
//        cout << "Added val: " << curr->val << endl;
        for(int i = 0; i < levels; i++)
        {
            newNode->next.push_back(nullptr);
        }
        newCurr = newNode;
        curr = curr->next[levels - 1];
    }

    // Create other levels using pointers to level 0.
    for(int i = levels - 2; i >= 0; i--)
    {
        curr = other.head;
        newCurr = head;
        while(curr != nullptr)
        {
//            cout << "copy constructor at level " << i << " curr->val: " << curr->val << endl;

            // Lookup new node in level 0.
            SNode* next = head;
            while(next->val != curr->val)
            {
                next = next->next[levels - 1];
            }
            newCurr->next[i] = next;
//            cout << "copy constructor setting next at level " << i << " to " << newCurr->next[i]->val << endl;
//           }
            newCurr = newCurr->next[i];
            curr = curr->next[i];
        }
        newCurr->next[i] = nullptr;
    }
}

// destructor
SkipList::~SkipList()
{
    SNode *curr = head;
    SNode *prev = nullptr;

    while(curr != nullptr)
    {
        prev = curr;
        curr = curr->next[levels - 1];
        delete prev;
    }
}

// Add to list, assume no duplicates
void SkipList::add(int val)
{
    vector<SNode *> nodes = getBeforeNodes(val);
    SNode *temp = new SNode(val);
    for(int i = 0; i < levels; i++)
    {
        temp->next.push_back(nullptr);  // Add one nullptr per level.
    }

    for(int level = levels - 1; level >= 0; level--)
    {
        if(nodes[level] == nullptr)
        {
//            cout << "Adding at the head for level: " << level << endl;
/*
            if(head->next[level] == nullptr)
            {
                cout << "next value is nullptr" << endl;
            }
            else
            {
                cout << "next value is " << head->next[level]->val << endl;
            }
*/
            temp->next[level] = head->next[level];
            head->next[level] = temp; // First node at that level.
        }
        else
        {
            // Inserting a new node after the node at that level that came back from getBeforeNodes.
            temp->next[level] = nodes[level]->next[level];
            nodes[level]->next[level] = temp;
        }
        if(level != 0 && ! shouldInsertAtHigherLevel())
        {
//            cout << "DO NOT INSERT AT A HIGHER LEVEL " << val << " at level " << level << endl;
            break;
        }
//        else
//        {
//            cout << "DO INSERT AT A HIGHER LEVEL " << val << " at level " << level << endl;
//        }
    }

    /*
        if(head->next[0] == nullptr || head->next[0]->val > val)
        {
            SNode *temp = new SNode(val);
            temp->next[0] = head->next[0];
    //        if(head->next[0] != nullptr) {
    //            cout << "head->next[0]: " << head->next[0]->val << endl;
    //            }
            head->next[0] = temp;
    //        head->next[0]->next.push_back(nullptr);   // Done in node constructor now.
        }
        else
        {
            SNode *curr = head->next[0];
            SNode *prev = head;
            while(curr != nullptr && curr->val < val)
            {
                prev = curr;
                curr = curr->next[0];
            }
            if(curr != nullptr && curr->val == val) // Duplicate
            {
                return;
            }
            // Currently handles only level 1.
            SNode *newNode = new SNode(val);
            newNode->next[0] = prev->next[0];
            prev->next[0] = newNode;
    //        cout << "Added " << newNode->val << endl;

        }
    */
}

// Add to list, assume no duplicates
void SkipList::add(const vector<int> &values)
{
    for(unsigned  i = 0; i < values.size(); i++)
    {
        add(values[i]);
//        cout << "After adding " << values[i] << endl;
//        cout << *this << endl;
//        cout << "end of after adding " << values[i] << endl;
    }
}

// return true if successfully removed
bool SkipList::remove(int val)
{
    vector<SNode *> nodes = getBeforeNodes(val);

    for(unsigned int i = 0; i < nodes.size(); i++)
    {
/*
        if(nodes[i] == nullptr)
        {
            cout << "i = " << i << " is nullptr" << endl;
        }
        else
        {
            cout << "i = " << i << " is val of " << nodes[i]->val << endl;
        }
*/
        if(nodes[i] != nullptr)
        {
            if(nodes[i]->next[i] != nullptr && nodes[i]->next[i]->val == val)
            {
                SNode *temp;
                if(i == nodes.size() - 1)
                {
                    temp = nodes[i]->next[i];
                }
                nodes[i]->next[i] = nodes[i]->next[i]->next[i];
                if(i == nodes.size() - 1)
                {
                    delete temp;
                    return true;
                }
            }
        }
        else
        {
            if(head->next[i] != nullptr && head->next[i]->val == val)
            {
                SNode *temp;
                if(i == nodes.size() - 1)
                {
                    temp = head->next[i];
                }
                head->next[i] = head->next[i]->next[i];
                if(i == nodes.size() - 1)
                {
                    delete temp;
                    return true;
                }
            }
        }
    }
    return false;
    /*
    SNode *curr = head->next[0];
    SNode *prev = nullptr;
    while(curr != nullptr && curr->val < val)
    {
        prev = curr;
        curr = curr->next[0];
    }
    if(curr != nullptr && curr->val == val) // Duplicate
    {
        // Currently only handles 1 level.
        prev->next[0] = curr->next[0];
        delete curr;
        return true;
    }
    return false;
    */
}

// return true if found in SkipList
// TODO
bool SkipList::contains(int val) const
{
    int level = 0;
    SNode* prev = head;
//    SNode* curr = nullptr;
    while(true)
    {
        // Find a good level.
        while((prev->next[level] == nullptr || prev->next[level]->val > val) && level < levels - 1)
        {
            level++;
        }
        SNode *curr = prev->next[level];
        if(curr == nullptr)
        {
            return false;
        }

        prev = curr;
        if(curr->val == val)
        {
            return true;
        }

        // Go right on level as far as possible.
        while(curr->next[level] != nullptr && curr->next[level]->val < val)
        {
            curr = curr->next[level];
        }
        prev = curr;
        if(curr->val == val)
        {
            return true;
        }
    }
    return false;
}

// Upper most level would be in index 0...
vector<SNode *> SkipList::getBeforeNodes(int val) const
{
    vector<SNode *> nodes;
    for(int level = 0; level < levels; level++)
    {
        if(head != nullptr)
        {
            SNode *temp = head->next[level];
            while(temp != nullptr && temp->next[level] != nullptr && temp->next[level]->val < val)
            {
                temp = temp->next[level];
            }
            if(temp != nullptr && temp->val < val)
            {
                nodes.push_back(temp);
            }
            else
            {
                nodes.push_back(nullptr);
            }
        }
        else
        {
            nodes.push_back(nullptr);
        }
    }
//    cout << "getBeforeNodes output adding: " << val << endl;
/*
    for(unsigned int i = 0; i < nodes.size(); i++)
    {
        if(nodes[i] == nullptr)
        {
            cout << "nullptr" << endl;
        }
        else
        {
            cout << nodes[i]->val << endl;
        }
    }
    cout << "end of getBeforeNodes output" << endl;
*/
    return nodes;
}

ostream &operator<<(ostream &out, const SkipList &skip)
{
    for(int level = 0; level < skip.levels; level++)
    {
        SNode *curr = skip.head->next[level];
        out << "[level: " << (skip.levels - level) <<"] ";
        while(curr != nullptr)
        {
            out << curr->val << "-->";
            curr = curr->next[level];
 /*
            if(curr != nullptr && curr == curr->next[level])
            {
                cout << "BUG *************************************************" << endl;
                break;
            }
*/
        }
        out << "nullptr" << endl << flush;
    }
    return out;
}

