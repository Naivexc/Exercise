#ifndef AVL_TREE_NODE_H
#define AVL_TREE_NODE_H
#include <memory>

template <typename T>
class AVLTreeNode
{
public:
    T val_;
    int height_; //左右子树高之差
    AVLTreeNode *parent_;
    std::unique_ptr<AVLTreeNode> left_;
    std::unique_ptr<AVLTreeNode> right_;
    AVLTreeNode(T val) : val_(val), height_(0), parent_(nullptr), left_(nullptr), right_(nullptr) {}
};
#endif