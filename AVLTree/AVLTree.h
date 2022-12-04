#ifndef AVL_TREE_H
#define AVL_TREE_H
#include "AVLTreeNode.h"
#include <cmath>
#include <memory>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <cassert>
#include <algorithm>
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
    std::unique_ptr<AVLTreeNode<T>> root_;
    AVLTree() : root_(nullptr) {}
    AVLTreeNode<T> *insert(T val);
    void erase(T val);
    void print();
    bool check();
    int get_height(AVLTreeNode<T> *ptr)
    {
        if (!ptr)
            return 0;
        return ptr->height_;
    }
    int reset_height(AVLTreeNode<T> *ptr)
    {
        ptr->height_ = std::max(get_height(ptr->left_.get()), get_height(ptr->right_.get())) + 1;
        return ptr->height_;
    }
    std::unique_ptr<AVLTreeNode<T>> &get_index_in_parent(AVLTreeNode<T> *ptr)
    {
        if (!ptr)
            assert(false);
        AVLTreeNode<T> *parent = ptr->parent_;
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
bool AVLTree<T>::check()
{
    if (root_)
    {
        if (!root_->check_key())
        {
            std::cout << "error: key wrong\n";
            print();
            assert(false);
        }
        if (!root_->check_height())
        {
            std::cout << "error: height wrong\n";
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
void AVLTree<VAL>::print()
{
    std::cout << "########## print ##########\n";
    static std::unordered_map<int, std::vector<AVLTreeNode<VAL> *>> info;
    info.clear();
    std::queue<std::pair<AVLTreeNode<VAL> *, int>> bfs;
    bfs.push(std::make_pair(root_.get(), 0));
    while (!bfs.empty())
    {
        std::pair<AVLTreeNode<VAL> *, int> temp = bfs.front();
        bfs.pop();
        if (info.find(temp.second) == info.end())
            info.insert(std::make_pair(temp.second, std::vector<AVLTreeNode<VAL> *>()));
        info.find(temp.second)->second.push_back(temp.first);
        if (temp.first == nullptr)
            continue;
        bfs.push(std::make_pair(temp.first->left_.get(), temp.second + 1));
        bfs.push(std::make_pair(temp.first->right_.get(), temp.second + 1));
    }
    std::vector<std::pair<int, std::vector<AVLTreeNode<VAL> *>>> temp(info.begin(), info.end());
    auto cmp = [](const std::pair<int, std::vector<AVLTreeNode<VAL> *>> &a, const std::pair<int, std::vector<AVLTreeNode<VAL> *>> &b)
        -> bool
    { return a.first < b.first; };
    std::sort(temp.begin(), temp.end(), cmp);
    for (auto &vec : temp)
    {
        std::cout << "\ndepth:" << vec.first << '\n';
        for (auto &ptr : vec.second)
        {
            if (ptr == nullptr)
                std::cout << "val:null\n";
            else
                std::cout << "val:" << ptr->val_ << "\n";
        }
    }
    std::cout << std::endl;
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
    AVLTreeNode<T> *ptr = root_.get();
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
        parent->left_->parent_ = parent;
        ptr = parent->left_.get(); //用于更新height

        while (parent)
        {
            int left_height = get_height(parent->left_.get());
            int right_height = get_height(parent->right_.get());
            int old_height = get_height(parent);
            if (left_height == right_height + 2)
            {
                if (get_height(ptr->left_.get()) == right_height + 1)
                {
                    right_rotate(parent);
                    parent = ptr->parent_;
                }
                else if (get_height(ptr->right_.get()) == right_height + 1)
                {
                    left_rotate(ptr);
                    reset_height(parent);
                    right_rotate(parent);
                    ptr = ptr->parent_;
                    parent = ptr->parent_;
                }
                else
                    assert(false);
            }
            else if (left_height + 2 == right_height)
            {
                if (get_height(ptr->right_.get()) == left_height + 1)
                {
                    left_rotate(parent);
                    parent = ptr->parent_;
                }
                else if (get_height(ptr->left_.get()) == left_height + 1)
                {
                    right_rotate(ptr);
                    reset_height(parent);
                    left_rotate(parent);
                    ptr = ptr->parent_;
                    parent = ptr->parent_;
                }
                else
                    assert(false);
            }
            else
            {
                ptr = parent;
                parent = ptr->parent_;
            }
            if (old_height == reset_height(ptr))
                break;
        }
    }
    else if (val > parent->val_)
    {
        // left tree
        parent->right_.reset(ret);
        parent->right_->parent_ = parent;
        ptr = parent->right_.get(); //用于更新height
        while (parent)
        {
            int right_height = get_height(parent->right_.get());
            int left_height = get_height(parent->left_.get());
            int old_height = get_height(parent);
            if (right_height == left_height + 2)
            {
                if (get_height(ptr->right_.get()) == left_height + 1)
                {
                    left_rotate(parent);
                    parent = ptr->parent_;
                }
                else if (get_height(ptr->left_.get()) == left_height + 1)
                {
                    right_rotate(ptr);
                    reset_height(parent);
                    left_rotate(parent);
                    ptr = ptr->parent_;
                    parent = ptr->parent_;
                }
                else
                    assert(false);
            }
            else if (right_height + 2 == left_height)
            {
                if (get_height(ptr->left_.get()) == right_height + 1)
                {
                    right_rotate(parent);
                    parent = ptr->parent_;
                }
                else if (get_height(ptr->right_.get()) == right_height + 1)
                {
                    left_rotate(ptr);
                    reset_height(parent);
                    right_rotate(parent);
                    ptr = ptr->parent_;
                    parent = ptr->parent_;
                }
                else
                    assert(false);
            }
            else
            {
                ptr = parent;
                parent = ptr->parent_;
            }
            int new_height = reset_height(ptr);
            if (old_height == new_height)
                break;
        }
    }
    else
        assert(false);
    return ret;
}
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
        if (ptr->right_)
            ptr->right_->parent_ = ptr;
        root_->left_.reset(ptr);
        ptr->parent_ = root_.get();
    }
    else
    {
        auto &ref_ptr_in_parent = get_index_in_parent(ptr);
        ref_ptr_in_parent.release();
        ptr->right_.release();
        ref_ptr_in_parent.reset(right);
        ref_ptr_in_parent->parent_ = ptr->parent_;
        ptr->right_.reset(right->left_.release());
        if (ptr->right_)
            ptr->right_->parent_ = ptr;
        right->left_.reset(ptr);
        ptr->parent_ = right;
    }
    reset_height(ptr);
    reset_height(right);
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
        if (ptr->left_)
            ptr->left_->parent_ = ptr;
        root_->right_.reset(ptr);
        ptr->parent_ = root_.get();
        return;
    }
    else
    {
        auto &ref_ptr_in_parent = get_index_in_parent(ptr);
        ref_ptr_in_parent.release();
        ptr->left_.release();
        ref_ptr_in_parent.reset(left);
        ref_ptr_in_parent->parent_ = ptr->parent_;
        ptr->left_.reset(left->right_.release());
        if (ptr->left_)
            ptr->left_->parent_ = ptr;
        left->right_.reset(ptr);
        ptr->parent_ = left;
    }
    reset_height(ptr);
    reset_height(left);
}
template <typename T>
void AVLTree<T>::erase(T val)
{
    AVLTreeNode<T> *ptr = root_.get();
    AVLTreeNode<T> *parent = nullptr;
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
        AVLTreeNode<T> *qtr = ptr->right_.get();
        while (qtr->left_ != nullptr)
            qtr = qtr->left_.get();
        std::swap(ptr->val_, qtr->val_);
        ptr = qtr;
    }
    if (!ptr->left_ && !ptr->right_)
    {
        if (!ptr->parent_)
        {
            root_.reset();
            return;
        }
        parent = ptr->parent_;
        get_index_in_parent(ptr).reset();
        ptr = nullptr;
    }
    else if (ptr->left_ && !ptr->right_)
    {
        if (!ptr->parent_)
        {
            auto temp_left_u_ptr = std::move(ptr->left_);
            root_.reset(temp_left_u_ptr.release());
            if (root_)
                root_->parent_ = nullptr;
            ptr = root_.get();
            parent = nullptr;
        }
        else
        {
            parent = ptr->parent_;
            std::unique_ptr<AVLTreeNode<T>> temp_left_u_ptr = std::move(ptr->left_);
            auto &ref_ptr_in_parent = get_index_in_parent(ptr);
            ref_ptr_in_parent.reset(temp_left_u_ptr.release());
            ref_ptr_in_parent->parent_ = parent;
            ptr = ref_ptr_in_parent.get();
        }
    }
    else if (ptr->right_ && !ptr->left_)
    {
        if (!ptr->parent_)
        {
            auto temp_right_u_ptr = std::move(ptr->right_);
            root_.reset(temp_right_u_ptr.release());
            if (root_)
                root_->parent_ = nullptr;
            ptr = root_.get();
            parent = nullptr;
        }
        else
        {
            parent = ptr->parent_;
            std::unique_ptr<AVLTreeNode<T>> temp_right_u_ptr = std::move(ptr->right_);
            auto &ref_ptr_in_parent = get_index_in_parent(ptr);
            ref_ptr_in_parent.reset(temp_right_u_ptr.release());
            ref_ptr_in_parent->parent_ = parent;
            ptr = ref_ptr_in_parent.get();
        }
    }
    else
        assert(false);
    while (parent)
    {
        int left_height = get_height(parent->left_.get());
        int right_height = get_height(parent->right_.get());
        int old_height = get_height(parent);
        if (left_height == right_height + 2)
        {
            ptr = parent->left_.get();
            if (get_height(ptr->left_.get()) == right_height + 1)
            {
                right_rotate(parent);
                parent = ptr->parent_;
            }
            else if (get_height(ptr->right_.get()) == right_height + 1)
            {
                left_rotate(ptr);
                reset_height(parent);
                right_rotate(parent);
                ptr = ptr->parent_;
                parent = ptr->parent_;
            }
            else
                assert(false);
        }
        else if (left_height + 2 == right_height)
        {
            ptr = parent->right_.get();
            if (get_height(ptr->right_.get()) == left_height + 1)
            {
                left_rotate(parent);
                parent = ptr->parent_;
            }
            else if (get_height(ptr->left_.get()) == left_height + 1)
            {
                right_rotate(ptr);
                reset_height(parent);
                left_rotate(parent);
                ptr = ptr->parent_;
                parent = ptr->parent_;
            }
            else
                assert(false);
        }
        else
        {
            ptr = parent;
            parent = ptr->parent_;
        }
        if (old_height == reset_height(ptr))
            break;
    }
}
#endif