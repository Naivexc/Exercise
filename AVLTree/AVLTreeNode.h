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
    AVLTreeNode(T val) : val_(val), height_(1), parent_(nullptr), left_(nullptr), right_(nullptr) {}
    bool check_key();
    bool check_parent();
    bool check_height();
};
template <typename VAL>
bool AVLTreeNode<VAL>::check_key()
{
    if (left_ && !left_->check_key())
        return false;
    if (right_ && !right_->check_key())
        return false;
    if (left_ && left_->val_ >= val_ || right_ && right_->val_ <= val_)
        return false;
    return true;
}
template <typename VAL>
bool AVLTreeNode<VAL>::check_parent()
{
    if (left_ && !left_->check_parent())
        return false;
    if (right_ && !right_->check_parent())
        return false;
    if (left_ && left_->parent_ != this)
    {
        std::cout << left_.get() << '\n';
        std::cout << left_->parent_ << '\n';
        return false;
    }
    if (right_ && right_->parent_ != this)
    {
        std::cout << right_.get() << '\n';
        std::cout << right_->parent_ << '\n';
        return false;
    }
    return true;
}
template <typename VAL>
bool AVLTreeNode<VAL>::check_height()
{
    if (left_ && !left_->check_height())
        return false;
    if (right_ && !right_->check_height())
        return false;
    int left_height = 0, right_height = 0;
    if (left_)
        left_height = left_->height_;
    if (right_)
        right_height = right_->height_;
    if (abs(left_height - right_height) >= 2)
        return false;
    height_ = std::max(left_height, right_height) + 1;
    return true;
}
template <typename VAL>
std::ostream &operator<<(std::ostream &out, AVLTreeNode<VAL> *const &ptr)
{
    if (ptr == nullptr)
        std::cout << "val:null\n";
    else
        std::cout << "val:" << ptr->val_ << "\n";
    return out;
}
#endif