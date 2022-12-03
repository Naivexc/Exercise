#ifndef AVL_TREE_NODE_H
#define AVL_TREE_NODE_H
#include <memory>
template <typename T>
class AVLTreeNode
{
public:
    T val_;
    int delta_; //左右子树高之差
    AVLTreeNode *parent_;
    std::unique_ptr<AVLTreeNode> left_;
    AVLTreeNode(T val) : val_(val), delta_(delta) {}
};
#endif