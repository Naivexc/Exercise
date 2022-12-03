#ifndef RB_TREE_H
#define RB_TREE_H
#include <iostream>
#include <cassert>
#include <memory>
#include "RBTreeNode.h"
extern int depth;
template <typename KEY, typename VAL>
class RBTree
{
public:
    std::unique_ptr<RBTreeNode<KEY, VAL>> root;
    RBTree();
    RBTreeNode<KEY, VAL> *find(KEY key);
    RBTreeNode<KEY, VAL> *insert(KEY key, VAL val);
    RBTreeNode<KEY, VAL> *left_rotate(RBTreeNode<KEY, VAL> *ptr);
    RBTreeNode<KEY, VAL> *right_rotate(RBTreeNode<KEY, VAL> *ptr);
    void erase(KEY key);
    void adjust_for_insert(RBTreeNode<KEY, VAL> *ptr);
    void adjust_double_black_node(RBTreeNode<KEY, VAL> *ptr);
    bool is_black(RBTreeNode<KEY, VAL> *ptr)
    {
        return !ptr || ptr->color_ == Color::black;
    }
};
template <typename KEY, typename VAL>
RBTree<KEY, VAL>::RBTree() : root(nullptr)
{
}
template <typename KEY, typename VAL>
RBTreeNode<KEY, VAL> *RBTree<KEY, VAL>::find(KEY key)
{
    auto ptr = root.get();
    while (ptr && ptr->key_ != key)
    {
        if (key < ptr->key_)
            ptr = ptr->left_.get();
        else
            ptr = ptr->right_.get();
    }
    return ptr;
}
template <typename KEY, typename VAL>
RBTreeNode<KEY, VAL> *RBTree<KEY, VAL>::left_rotate(RBTreeNode<KEY, VAL> *ptr)
{
    if (ptr->right_ == nullptr)
        assert(false);
    auto right_ptr = ptr->right_.get();
    ptr->get_index_in_parent().reset(ptr->right_.release());
    ptr->right_->parent_ = ptr->parent_;
    ptr->parent_ = right_ptr;
    if (right_ptr->left_)
        ptr->right_.reset(right_ptr->left_.get());
    ptr->right_->parent_ = ptr;
    right_ptr->left_.reset(ptr);
    return right_ptr;
}
template <typename KEY, typename VAL>
RBTreeNode<KEY, VAL> *RBTree<KEY, VAL>::right_rotate(RBTreeNode<KEY, VAL> *ptr)
{
    if (ptr->left_ == nullptr)
        assert(false);
    auto left_ptr = ptr->left_.get();
    ptr->get_index_in_parent().reset(ptr->left_.release());
    ptr->left_->parent_ = ptr->parent_;
    ptr->parent_ = left_ptr;
    if (left_ptr->right_)
        ptr->left_.reset(left_ptr->right_.get());
    ptr->left_->parent_ = ptr;
    left_ptr->right_.reset(ptr);
    return left_ptr;
}
template <typename KEY, typename VAL>
void RBTree<KEY, VAL>::adjust_for_insert(RBTreeNode<KEY, VAL> *ptr)
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
                parent->color_ = Color::black;
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
                parent->color_ = Color::black;
                return;
            }
        }
    }
}
template <typename KEY, typename VAL>
RBTreeNode<KEY, VAL> *RBTree<KEY, VAL>::insert(KEY key, VAL val)
{
    auto ptr = root.get();
    RBTreeNode<KEY, VAL> *ret = nullptr;
    RBTreeNode<KEY, VAL> *parent = nullptr;
    while (ptr)
    {
        if (key < ptr->key_)
        {
            parent = ptr;
            ptr = ptr->left_.get();
        }
        else
        {
            parent = ptr;
            ptr = ptr->right_.get();
        }
    }
    if (!parent)
    {
        //无节点
        root = std::make_unique<RBTreeNode<KEY, VAL>>(key, val, Color::black);
        return root.get();
    }
    ret = new RBTreeNode(key, val, Color::red);
    if (key < parent->key_)
    {
        parent->left_.reset(ret);
        ret->parent_ = parent;
        adjust_for_insert(ret);
    }
    else if (key > parent->key_)
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
template <typename KEY, typename VAL>
void RBTree<KEY, VAL>::adjust_double_black_node(RBTreeNode<KEY, VAL> *ptr)
{
    if (ptr == root.get())
        return;
    RBTreeNode<KEY, VAL> *parent = ptr->parent_;
    RBTreeNode<KEY, VAL> *brother = ptr->get_brother();
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
                    brother->color_ = Color::black;
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
                adjust_double_black_node(brother);
            }
            else
            {
                right_rotate(ptr->parent_);
                parent->color_ = Color::red;
                brother->color_ = Color::black;
                adjust_double_black_node(brother);
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
                    brother->color_ = Color::black;
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
                adjust_double_black_node(brother);
            }
            else
            {
                left_rotate(ptr->parent_);
                parent->color_ = Color::red;
                brother->color_ = Color::black;
                adjust_double_black_node(brother);
            }
        }
    }
}
template <typename KEY, typename VAL>
void RBTree<KEY, VAL>::erase(KEY key)
{
    RBTreeNode<KEY, VAL> *ptr = root.get();
    while (ptr && ptr->key_ != key)
    {
        if (ptr->key_ < key)
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
        RBTreeNode<KEY, VAL> *qtr = ptr->right_.get();
        while (qtr->left_ != nullptr)
            qtr = qtr->left_.get();
        std::swap(ptr->key_, qtr->key_);
        std::swap(ptr->val_, qtr->val_);
        ptr = qtr;
    }
    if (!ptr->left_ && !ptr->right_)
    {
        if (ptr->color_ == Color::red)
        {
            if (ptr->parent_)
                ptr->get_index_in_parent().release();
            else
                root.release();
            return;
        }
        else
        {
            adjust_double_black_node(ptr);
            if (ptr->parent_)
                ptr->get_index_in_parent().release();
            else
                root.release();
            return;
        }
    }
    else if (ptr->left_ && !ptr->right_)
    {
        if (ptr->color_ == Color::red)
        {
            if (ptr->parent_)
                ptr->get_index_in_parent().reset(ptr->left_.release());
            else
                root.reset(ptr->left_.release());
            ptr->left_->parent_ = ptr->parent_;
            return;
        }
        else if (ptr->color_ == Color::black)
        {
            if (ptr->parent_)
                ptr->get_index_in_parent().reset(ptr->left_.release());
            else
                root.reset(ptr->left_.release());
            ptr->left_->parent_ = ptr->parent_;
            if (ptr->left_->color_ == Color::red)
            {
                ptr->left_->color_ = Color::black;
                return;
            }
            else
            {
                adjust_double_black_node(ptr->left_.get());
                return;
            }
        }
        else
            assert(false);
    }
    else if (!ptr->left_ && ptr->right_)
    {
        if (ptr->color_ == Color::red)
        {
            if (ptr->parent_)
                ptr->get_index_in_parent().reset(ptr->right_.release());
            else
                root.reset(ptr->right_.release());
            ptr->right_->parent_ = ptr->parent_;
            return;
        }
        else if (ptr->color_ == Color::black)
        {
            if (ptr->parent_)
                ptr->get_index_in_parent().reset(ptr->right_.release());
            else
                root.reset(ptr->right_.release());
            ptr->right_->parent_ = ptr->parent_;
            if (ptr->right_->color_ == Color::red)
            {
                ptr->right_->color_ = Color::black;
                return;
            }
            else
            {
                adjust_double_black_node(ptr->right_.get());
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