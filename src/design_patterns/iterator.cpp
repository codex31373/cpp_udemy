#include <iostream>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
// Iterator Coding Exercise
// Given a binary tree Node, implement preorder traversal
// Preorder: root -> left -> right
////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct Node
{
    T value;
    Node *left{nullptr}, *right{nullptr}, *parent{nullptr};

    Node(T value) : value(value) {}

    Node(T value, Node<T> *left, Node<T> *right) : value(value), left(left), right(right) {
        left->parent = right->parent = this;
    }

    // traverse the node and its children preorder
    // and put all the results into `result`
    void preorder_traversal(std::vector<Node<T>*>& result)
    {
        //Add current node first (root)
        result.push_back(this);

        //Then traverse left subtree
        if (left)
        {
            left->preorder_traversal(result);
        }

        //Then traverse right subtree
        if (right)
        {
            right->preorder_traversal(result);
        }
    }
};

int main(const int argc,const char *argv[])
{
    std::cout << "\033[92m" << "\nIterator : 5 May 2026\n"
              << "\033[0m" << std::endl;

    /*Test preorder traversal
           1
          / \
         2   3
        / \
       4   5
    
    Expected preorder: 1, 2, 4, 5, 3*/

    Node<int> n4{4}, n5{5}, n3{3};
    Node<int> n2{2, &n4, &n5};
    Node<int> n1{1, &n2, &n3};

    std::vector<Node<int>*> result;
    n1.preorder_traversal(result);

    std::cout << "Preorder traversal: ";
    for (auto* node : result)
    {
        std::cout << node->value << " ";
    }
    std::cout << "(expected: 1 2 4 5 3)" << std::endl;

        std::vector<std::string> names = {"Alice", "Bob", "Charlie"};
    std::vector<std::string>::iterator it{names.begin()};
    for ( ; it != names.end(); ++it) {
        std::cout << *it << " ";
    }
    for (auto&& name : names) {
        std::cout << name << " ";
    }

    return 0;
}