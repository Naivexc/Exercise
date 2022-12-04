#ifndef AVL_TREE_H
#define AVL_TREE_H
#include "AVLTreeNode.h"
#include <cmath>
#include <memory>
#include <iostream>
enum class Side
{
    left,
    right,
    null
};
template <typename T>
class AVLTree
{
public:
    static Side last_side_ = Side::null;
    static Side last_last_side_ = Side::null;
    std::unique_ptr<AVLTreeNode<T>> root_;
    AVLTree() : root_(nullptr) {}
    AVLTreeNode<T> *insert(T val);
    int get_height(AVLTreeNode<T> *ptr)
    {
        if (!ptr)
            return 0;
        return ptr->height;
    }
    std::unique_ptr<AVLTreeNode<T>> &get_index_for_parent(AVLTreeNode<T> *ptr)
    {
        if (!ptr)
            assert(false);
        AVLTreeNode<T> parent = ptr->parent_;
        if (!parent)
            assert(false);
        if (parent->left_.get() == ptr)
            return parent->left_;
        else
            return parent->right_;
    }
    void left_rotate(AVLTreeNode<T> *ptr);
    void right_rotate(AVLTreeNode<T> *ptr);
};
template <typename T>
void AVLTree<T>::left_rotate(AVLTreeNode<T> *ptr)
{
    AVLTreeNode<T> *right = ptr->right_.get();
    if (!ptr->parent_)
    {
        // root
        root_.release();
        root_.reset(ptr->right_.release());
        root_->parent_ = nullptr;
        ptr->right_.reset(root_->left_.release());
        ptr->right_->parent_ = ptr;
        root_->left_.reset(ptr);
        ptr->parent_ = root_;
        return;
    }
    get_index_for_parent(ptr).release();
    get_index_for_parent(ptr).reset(ptr->right_.release());
    right->parent_ = ptr->parent_;
    ptr->right_.reset(right->left_.release());
    ptr->right_->parent_ = ptr;
    right->left_.reset(ptr);
    ptr->parent_ = right;
}
template <typename T>
void AVLTree<T>::right_rotate(AVLTreeNode<T> *ptr)
{
    AVLTreeNode<T> *left = ptr->left_.get();
    if (!ptr->parent_)
    {
        // root
        root_.release();
        root_.reset(ptr->left_.release());
        root_->parent_ = nullptr;
        ptr->left_.reset(root_->right_.release());
        ptr->left_->parent_ = ptr;
        root_->right_.reset(ptr);
        ptr->parent_ = root_;
        return;
    }
    get_index_for_parent(ptr).release();
    get_index_for_parent(ptr).reset(ptr->left_.release());
    left->parent_ = ptr->parent_;
    ptr->left_.reset(left->right_.release());
    ptr->left_->parent_ = ptr;
    left->right_.reset(ptr);
    ptr->parent_ = left;
}
template <typename T>
AVLTreeNode<T> *AVLTree<T>::insert(T val)
{
    auto ret = new AVLTreeNode<T>(val);
    if (!root_)
    {
        root_.reset(ret);
        return ret;
    }
    AVLTreeNode<T> *ptr = root_;
    AVLTreeNode<T> *parent = nullptr;
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
            std::cout << "element already exists\n";
            delete ret;
            return ptr;
        }
    }
    if (val < parent->val_)
    {
        // left tree
        parent->left_.reset(ret);
        ptr = parent->left_.get(); //用于更新height
        while (parent)
        {
            if (abs(get_height(parent->left_.get()) - get_height(parent->right_.get())) > 1)
                parent->height_ = get_height(ptr) + 1;
            parent = parent->parent_;
            ptr = ptr->parent_;
        }
    }
}
#endif