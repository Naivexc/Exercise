#ifndef RB_TREE_NODE_H
#define RB_TREE_NODE_H
#include <memory>
#include <iostream>
#include <cassert>
extern int depth;
enum class Color
{
    red,
    black
};

template <typename VAL>
class RBTreeNode
{
public:
    VAL val_;
    Color color_;
    RBTreeNode *parent_;
    std::unique_ptr<RBTreeNode<VAL>> left_;
    std::unique_ptr<RBTreeNode<VAL>> right_;
    RBTreeNode(VAL val, Color color);
    RBTreeNode *get_uncle();
    RBTreeNode *get_brother();
    std::unique_ptr<RBTreeNode<VAL>> &get_index_in_parent(); //返回父节点对其的索引指针;为根节点时返回nullptr
    void print_middle();
    bool check_key();
    bool check_parent();
    std::pair<bool, int> check_color();
};
template <typename VAL>
std::ostream &operator<<(std::ostream &out, RBTreeNode<VAL> *const &ptr)
{
    if (ptr == nullptr)
        std::cout << "val:null, "
                  << "color:black\n";
    else
        std::cout << "val:" << ptr->val_ << ", "
                  << "color:" << (ptr->color_ == Color::red ? "red\n" : "black\n");
    return out;
}
template <typename VAL>
bool RBTreeNode<VAL>::check_key()
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
bool RBTreeNode<VAL>::check_parent()
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
std::pair<bool, int> RBTreeNode<VAL>::check_color()
{
    if (!parent_ && color_ == Color::red)
        return std::make_pair(false, 0);
    if (color_ == Color::red)
    {
        if (left_ && left_->color_ == Color::red || right_ && right_->color_ == Color::red)
            return std::make_pair(false, 0);
    }
    std::pair<bool, int> left_pair;
    std::pair<bool, int> right_pair;
    if (left_)
        left_pair = left_->check_color();
    else
        left_pair = std::make_pair(true, 1);
    if (!left_pair.first)
        return std::make_pair(false, 0);
    if (right_)
        right_pair = right_->check_color();
    else
        right_pair = std::make_pair(true, 1);
    if (!right_pair.first)
        return std::make_pair(false, 0);
    if (left_pair.second != right_pair.second)
        return std::make_pair(false, 0);
    if (color_ == Color::red)
        return std::make_pair(true, left_pair.second);
    else
        return std::make_pair(true, left_pair.second + 1);
}
template <typename VAL>
RBTreeNode<VAL> *RBTreeNode<VAL>::get_brother()
{
    if (!parent_)
    {
        std::cout << "error: root node has no brother node\n";
        return nullptr;
    }
    if (parent_->left_.get() == this)
        return parent_->right_.get();
    else
        return parent_->left_.get();
}
template <typename VAL>
std::unique_ptr<RBTreeNode<VAL>> &RBTreeNode<VAL>::get_index_in_parent()
{
    if (!parent_)
        assert(false);
    if (parent_->left_.get() == this)
        return parent_->left_;
    return parent_->right_;
}
template <typename VAL>
void RBTreeNode<VAL>::print_middle()
{
    ++depth;
    if (left_)
    {

        left_->print_middle();
    }
    else
    {
        std::cout << "depth:" << depth << " nullptr\n";
    }
    --depth;
    std::cout << "depth:" << depth << " val:" << val_ << " color:" << (color_ == Color::black ? "black\n" : "red\n");
    ++depth;
    if (right_)
    {

        right_->print_middle();
    }
    else
    {
        std::cout << "depth:" << depth << " nullptr\n";
    }
    --depth;
}

template <typename VAL>
RBTreeNode<VAL>::RBTreeNode(VAL val, Color color)
    : val_(val), color_(color), parent_(nullptr), left_(nullptr), right_(nullptr)
{
}
template <typename VAL>
RBTreeNode<VAL> *RBTreeNode<VAL>::get_uncle()
{
    if (parent_ == nullptr)
        assert(false);
    auto gparent = parent_->parent_;
    if (gparent == nullptr)
        assert(false);
    if (gparent->left_.get() == parent_)
        return gparent->right_.get();
    return gparent->left_.get();
}

#endif
