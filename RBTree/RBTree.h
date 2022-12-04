#ifndef RB_TREE_H
#define RB_TREE_H
#include <iostream>
#include <cassert>
#include <memory>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include "RBTreeNode.h"
#include <algorithm>
extern int depth;
template <typename VAL>
class RBTree
{
public:
    std::unique_ptr<RBTreeNode<VAL>> root_;
    RBTree();
    RBTreeNode<VAL> *find(VAL val);
    RBTreeNode<VAL> *insert(VAL val);
    RBTreeNode<VAL> *left_rotate(RBTreeNode<VAL> *ptr);
    RBTreeNode<VAL> *right_rotate(RBTreeNode<VAL> *ptr);
    void erase(VAL val);
    void adjust_for_insert(RBTreeNode<VAL> *ptr);
    void adjust_double_black_node(RBTreeNode<VAL> *ptr);
    bool is_black(RBTreeNode<VAL> *ptr)
    {
        return !ptr || ptr->color_ == Color::black;
    }
    void print();
    bool check();
};
template <typename VAL>
RBTree<VAL>::RBTree() : root_(nullptr)
{
}
template <typename VAL>
bool RBTree<VAL>::check()
{
    if (root_)
    {
        if (!root_->check_key())
        {
            std::cout << "error: key wrong\n";
            print();
            assert(false);
        }
        if (!root_->check_color().first)
        {
            std::cout << "error: color wrong\n";
            print();
            assert(false);
        }
        if (!root_->check_parent())
        {
            std::cout << "error: parent relation wrong\n";
            print();
            assert(false);
        }
    }
    else
    {
        std::cout << "Empty\n";
    }
    return true;
}
template <typename VAL>
void RBTree<VAL>::print()
{
    std::cout << "########## print ##########\n";
    static std::unordered_map<int, std::vector<RBTreeNode<VAL> *>> info;
    info.clear();
    std::queue<std::pair<RBTreeNode<VAL> *, int>> bfs;
    bfs.push(std::make_pair(root_.get(), 0));
    while (!bfs.empty())
    {
        std::pair<RBTreeNode<VAL> *, int> temp = bfs.front();
        bfs.pop();
        if (info.find(temp.second) == info.end())
            info.insert(std::make_pair(temp.second, std::vector<RBTreeNode<VAL> *>()));
        info.find(temp.second)->second.push_back(temp.first);
        if (temp.first == nullptr)
            continue;
        bfs.push(std::make_pair(temp.first->left_.get(), temp.second + 1));
        bfs.push(std::make_pair(temp.first->right_.get(), temp.second + 1));
    }
    std::vector<std::pair<int, std::vector<RBTreeNode<VAL> *>>> temp(info.begin(), info.end());
    auto cmp = [](const std::pair<int, std::vector<RBTreeNode<VAL> *>> &a, const std::pair<int, std::vector<RBTreeNode<VAL> *>> &b)
        -> bool
    { return a.first < b.first; };
    std::sort(temp.begin(), temp.end(), cmp);
    for (auto &vec : temp)
    {
        std::cout << "\ndepth:" << vec.first << '\n';
        for (auto &ptr : vec.second)
        {
            if (ptr == nullptr)
                std::cout << "val:null, "
                          << "color:black\n";
            else
                std::cout << "val:" << ptr->val_ << ", "
                          << "color:" << (ptr->color_ == Color::red ? "red\n" : "black\n");
        }
    }

    std::cout << std::endl;
}
template <typename VAL>
RBTreeNode<VAL> *RBTree<VAL>::find(VAL val)
{
    auto ptr = root_.get();
    while (ptr && ptr->val_ != val)
    {
        if (val < ptr->val_)
            ptr = ptr->left_.get();
        else
            ptr = ptr->right_.get();
    }
    return ptr;
}
template <typename VAL>
RBTreeNode<VAL> *RBTree<VAL>::left_rotate(RBTreeNode<VAL> *ptr)
{
    if (ptr->right_ == nullptr)
        assert(false);
    RBTreeNode<VAL> *right = ptr->right_.get();
    RBTreeNode<VAL> *parent = ptr->parent_;
    if (!parent)
    {
        root_.release();
        root_.reset(ptr->right_.release());
        root_->parent_ = nullptr;
        ptr->right_.reset(root_->left_.release());
        root_->left_.reset(ptr);
        ptr->parent_ = root_.get();
        if (ptr->right_)
            ptr->right_->parent_ = ptr;
    }
    else
    {
        auto &ref_ptr_in_parent = ptr->get_index_in_parent();
        ref_ptr_in_parent.release();
        ptr->right_.release();
        ref_ptr_in_parent.reset(right);
        right->parent_ = parent;
        ptr->right_.reset(right->left_.release());
        if (ptr->right_ != nullptr)
            ptr->right_->parent_ = ptr;
        right->left_.reset(ptr);
        ptr->parent_ = right;
    }
    return right;
}
template <typename VAL>
RBTreeNode<VAL> *RBTree<VAL>::right_rotate(RBTreeNode<VAL> *ptr)
{
    if (ptr->left_ == nullptr)
        assert(false);
    RBTreeNode<VAL> *left = ptr->left_.get();
    RBTreeNode<VAL> *parent = ptr->parent_;
    if (!parent)
    {
        root_.release();
        root_.reset(ptr->left_.release());
        root_->parent_ = nullptr;
        ptr->left_.reset(root_->right_.release());
        root_->right_.reset(ptr);
        ptr->parent_ = root_.get();
        if (ptr->left_)
            ptr->left_->parent_ = ptr;
    }
    else
    {
        auto &ref_ptr_in_parent = ptr->get_index_in_parent();
        ref_ptr_in_parent.release();
        ptr->left_.release();
        ref_ptr_in_parent.reset(left);
        left->parent_ = parent;
        ptr->left_.reset(left->right_.release());
        if (ptr->left_ != nullptr)
            ptr->left_->parent_ = ptr;
        left->right_.reset(ptr);
        ptr->parent_ = left;
    }
    return left;
}
template <typename VAL>
void RBTree<VAL>::adjust_for_insert(RBTreeNode<VAL> *ptr)
{
    if (ptr->parent_ == nullptr)
    {
        ptr->color_ = Color::black;
        return;
    }
    auto parent = ptr->parent_;
    if (parent->color_ == Color::black)
        return;
    if (parent->parent_ == nullptr)
    {
        parent->color_ = Color::black;
        return;
    }
    auto gparent = parent->parent_;
    auto uncle = ptr->get_uncle();
    if (parent == gparent->left_.get())
    {
        if (uncle && uncle->color_ == Color::red)
        {
            uncle->color_ = Color::black;
            parent->color_ = Color::black;
            gparent->color_ = Color::red;
            adjust_for_insert(gparent);
            return;
        }
        else
        {
            if (parent->left_.get() == ptr)
            {
                right_rotate(gparent);
                parent->right_->color_ = Color::red;
                parent->color_ = Color::black;
                return;
            }
            else
            {
                left_rotate(parent);
                right_rotate(gparent);
                gparent->color_ = Color::red;
                ptr->color_ = Color::black;
                return;
            }
        }
    }
    else
    {
        if (uncle && uncle->color_ == Color::red)
        {
            uncle->color_ = Color::black;
            parent->color_ = Color::black;
            gparent->color_ = Color::red;
            adjust_for_insert(gparent);
            return;
        }
        else
        {
            if (parent->right_.get() == ptr)
            {
                left_rotate(gparent);
                parent->left_->color_ = Color::red;
                parent->color_ = Color::black;
                return;
            }
            else
            {
                right_rotate(parent);
                left_rotate(gparent);
                gparent->color_ = Color::red;
                ptr->color_ = Color::black;
                return;
            }
        }
    }
}
template <typename VAL>
RBTreeNode<VAL> *RBTree<VAL>::insert(VAL val)
{
    auto ptr = root_.get();
    RBTreeNode<VAL> *ret = nullptr;
    RBTreeNode<VAL> *parent = nullptr;
    while (ptr)
    {
        if (val < ptr->val_)
        {
            parent = ptr;
            ptr = ptr->left_.get();
        }
        else if (val > ptr->val_)
        {
            parent = ptr;
            ptr = ptr->right_.get();
        }
        else
        {
            std::cout << "elements already exists\n";
            return nullptr;
        }
    }
    if (!parent)
    {
        //无节点
        root_ = std::make_unique<RBTreeNode<VAL>>(val, Color::black);
        return root_.get();
    }
    ret = new RBTreeNode(val, Color::red);
    if (val < parent->val_)
    {
        parent->left_.reset(ret);
        ret->parent_ = parent;
        adjust_for_insert(ret);
    }
    else if (val > parent->val_)
    {
        parent->right_.reset(ret);
        ret->parent_ = parent;
        adjust_for_insert(ret);
    }
    else
    {
        std::cout << "insert same element;ignore\n";
        delete ret;
        ret = nullptr;
    }
    return ret;
}
template <typename VAL>
void RBTree<VAL>::adjust_double_black_node(RBTreeNode<VAL> *ptr)
{
    if (ptr == root_.get())
        return;
    RBTreeNode<VAL> *parent = ptr->parent_;
    RBTreeNode<VAL> *brother = ptr->get_brother();
    if (parent->left_.get() == ptr)
    {
        if (is_black(brother))
        {
            //由平衡条件,此时兄弟节点不可能为nullptr
            if (is_black(brother->left_.get()) && is_black(brother->right_.get()))
            {
                brother->color_ = Color::red;
                if (parent->color_ == Color::red)
                {
                    parent->color_ = Color::black;
                    return;
                }
                else
                {
                    adjust_double_black_node(parent);
                    return;
                }
            }
            else if (!is_black(brother->left_.get()))
            {
                right_rotate(brother);
                left_rotate(parent);
                if (parent->color_ == Color::red)
                {
                    parent->color_ = Color::black;
                }
                else
                {
                    brother->parent_->color_ = Color::black;
                }
                return;
            }
            else
            {
                left_rotate(parent);
                if (parent->color_ == Color::black)
                {
                    brother->right_->color_ = Color::black;
                    return;
                }
                else
                {
                    parent->color_ = Color::black;
                    brother->right_->color_ = Color::black;
                    brother->color_ = Color::red;
                }
                return;
            }
        }
        else if (brother->color_ == Color::red)
        {
            if (parent->left_.get() == ptr)
            {
                left_rotate(ptr->parent_);
                parent->color_ = Color::red;
                brother->color_ = Color::black;
                adjust_double_black_node(ptr);
            }
            else
            {
                right_rotate(ptr->parent_);
                parent->color_ = Color::red;
                brother->color_ = Color::black;
                adjust_double_black_node(ptr);
            }
        }
    }
    if (parent->right_.get() == ptr)
    {
        if (is_black(brother))
        {
            //由平衡条件,此时兄弟节点不可能为nullptr
            if (is_black(brother->right_.get()) && is_black(brother->left_.get()))
            {
                brother->color_ = Color::red;
                if (parent->color_ == Color::red)
                {
                    parent->color_ = Color::black;
                    return;
                }
                else
                {
                    adjust_double_black_node(parent);
                    return;
                }
            }
            else if (!is_black(brother->right_.get()))
            {
                left_rotate(brother);
                right_rotate(parent);
                if (parent->color_ == Color::red)
                {
                    parent->color_ = Color::black;
                }
                else
                {
                    brother->parent_->color_ = Color::black;
                }
                return;
            }
            else
            {
                right_rotate(parent);
                if (parent->color_ == Color::black)
                {
                    brother->left_->color_ = Color::black;
                    return;
                }
                else
                {
                    parent->color_ = Color::black;
                    brother->left_->color_ = Color::black;
                    brother->color_ = Color::red;
                }
                return;
            }
        }
        else if (brother->color_ == Color::red)
        {
            if (parent->right_.get() == ptr)
            {
                right_rotate(ptr->parent_);
                parent->color_ = Color::red;
                brother->color_ = Color::black;
                adjust_double_black_node(ptr);
            }
            else
            {
                left_rotate(ptr->parent_);
                parent->color_ = Color::red;
                brother->color_ = Color::black;
                adjust_double_black_node(ptr);
            }
        }
    }
}
template <typename VAL>
void RBTree<VAL>::erase(VAL val)
{
    RBTreeNode<VAL> *ptr = root_.get();
    while (ptr && ptr->val_ != val)
    {
        if (ptr->val_ < val)
            ptr = ptr->right_.get();
        else
            ptr = ptr->left_.get();
    }
    if (!ptr)
    {
        std::cout << "don't find node to erase\n";
        return;
    }
    if (ptr->left_ && ptr->right_)
    {
        RBTreeNode<VAL> *qtr = ptr->right_.get();
        while (qtr->left_ != nullptr)
            qtr = qtr->left_.get();
        std::swap(ptr->val_, qtr->val_);
        ptr = qtr;
    }
    if (!ptr->left_ && !ptr->right_)
    {
        if (ptr->color_ == Color::red)
        {
            std::cout << ptr << '\n';
            std::cout << ptr->parent_ << '\n';
            if (ptr->parent_)
                ptr->get_index_in_parent().reset();
            else
                root_.reset();
            return;
        }
        else
        {
            adjust_double_black_node(ptr);
            if (ptr->parent_)
                ptr->get_index_in_parent().reset();
            else
                root_.reset();
            return;
        }
    }
    else if (ptr->left_ && !ptr->right_)
    {
        auto left = ptr->left_.get();
        if (ptr->color_ == Color::red)
        {
            if (ptr->parent_)
            {
                auto &ref_ptr_in_parent = ptr->get_index_in_parent();
                std::unique_ptr<RBTreeNode<VAL>> temp_u_ptr(ref_ptr_in_parent.release());
                ref_ptr_in_parent.reset(ptr->left_.release());
                ref_ptr_in_parent->parent_ = ptr->parent_;
                temp_u_ptr.reset();
            }
            else
            {
                std::unique_ptr<RBTreeNode<VAL>> temp_u_ptr(root_.release());
                root_.reset(ptr->left_.release());
                root_->parent_ = nullptr;
                temp_u_ptr.reset();
            }
            return;
        }
        else if (ptr->color_ == Color::black)
        {
            if (ptr->parent_)
            {
                auto &ref_ptr_in_parent = ptr->get_index_in_parent();
                std::unique_ptr<RBTreeNode<VAL>> temp_u_ptr(ref_ptr_in_parent.release());
                ref_ptr_in_parent.reset(ptr->left_.release());
                ref_ptr_in_parent->parent_ = ptr->parent_;
                temp_u_ptr.reset();
            }
            else
            {
                std::unique_ptr<RBTreeNode<VAL>> temp_u_ptr(root_.release());
                root_.reset(ptr->left_.release());
                root_->parent_ = nullptr;
                temp_u_ptr.reset();
            }
            if (left->color_ == Color::red)
            {
                left->color_ = Color::black;
                return;
            }
            else
            {
                adjust_double_black_node(left);
                return;
            }
        }
        else
            assert(false);
    }
    else if (!ptr->left_ && ptr->right_)
    {
        auto right = ptr->right_.get();
        if (ptr->color_ == Color::red)
        {
            if (ptr->parent_)
            {
                auto &ref_ptr_in_parent = ptr->get_index_in_parent();
                std::unique_ptr<RBTreeNode<VAL>> temp_u_ptr(ref_ptr_in_parent.release());
                ref_ptr_in_parent.reset(ptr->right_.release());
                ref_ptr_in_parent->parent_ = ptr->parent_;
                temp_u_ptr.reset();
            }
            else
            {
                std::unique_ptr<RBTreeNode<VAL>> temp_u_ptr(root_.release());
                root_.reset(ptr->right_.release());
                root_->parent_ = nullptr;
                temp_u_ptr.reset();
            }
            return;
        }
        else if (ptr->color_ == Color::black)
        {
            if (ptr->parent_)
            {
                auto &ref_ptr_in_parent = ptr->get_index_in_parent();
                std::unique_ptr<RBTreeNode<VAL>> temp_u_ptr(ref_ptr_in_parent.release());
                ref_ptr_in_parent.reset(ptr->right_.release());
                ref_ptr_in_parent->parent_ = ptr->parent_;
                temp_u_ptr.reset();
            }
            else
            {
                std::unique_ptr<RBTreeNode<VAL>> temp_u_ptr(root_.release());
                root_.reset(ptr->right_.release());
                root_->parent_ = nullptr;
                temp_u_ptr.reset();
            }
            if (right->color_ == Color::red)
            {
                right->color_ = Color::black;
                return;
            }
            else
            {
                adjust_double_black_node(right);
                return;
            }
        }
        else
            assert(false);
    }
    else
        assert(false);
}
#endif