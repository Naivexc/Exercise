#include <iostream>
#include <cassert>
enum class Color
{
    red,
    black
};
template <typename KEY, typename VAL>
class RBTreeNode
{
public:
    KEY key_;
    VAL val_;
    Color color_;
    RBTreeNode *parent_;
    RBTreeNode *left_;
    RBTreeNode *right_;
    RBTreeNode(KEY key, VAL val, Color color);
    RBTreeNode *get_uncle();
    RBTreeNode *get_brother();
    RBTreeNode *&get_index_in_parent(); //返回父节点对其的索引指针;为根节点时返回nullptr
    void print_middle();
    bool check_key();
    std::pair<bool, int> check_color();
};
template <typename KEY, typename VAL>
bool RBTreeNode<KEY, VAL>::check_key()
{
    if (left_ && !left_->check_key())
        return false;
    if (right_ && !right_->check_key())
        return false;
    if (left_ && left_->key_ >= key_ || right_ && right_->key_ <= key_)
        return false;
    return true;
}
template <typename KEY, typename VAL>
std::pair<bool, int> RBTreeNode<KEY, VAL>::check_color()
{
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
template <typename KEY, typename VAL>
RBTreeNode<KEY, VAL> *RBTreeNode<KEY, VAL>::get_brother()
{
    if (!parent_)
    {
        std::cout << "error: root node has no brother node\n";
        return nullptr;
    }
    if (parent_->left_ == this)
        return parent_->right_;
    else
        return parent_->left_;
}
template <typename KEY, typename VAL>
RBTreeNode<KEY, VAL> *&RBTreeNode<KEY, VAL>::get_index_in_parent()
{
    if (!parent_)
        assert(false);
    if (parent_->left_ == this)
        return parent_->left_;
    else
        return parent_->right_;
}
template <typename KEY, typename VAL>
class RBTree
{
public:
    RBTreeNode<KEY, VAL> *root;
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
    void print()
    {
    }
    bool check();
};
template <typename KEY, typename VAL>
RBTreeNode<KEY, VAL>::RBTreeNode(KEY key, VAL val, Color color)
    : key_(key), val_(val), color_(color), parent_(nullptr), left_(nullptr), right_(nullptr)
{
}
template <typename KEY, typename VAL>
RBTreeNode<KEY, VAL> *RBTreeNode<KEY, VAL>::get_uncle()
{
    if (parent_ == nullptr)
        assert(false);
    auto gparent = parent_->parent_;
    if (gparent == nullptr)
        assert(false);
    if (gparent->left_ == parent_)
    {
        return gparent->right_;
    }
    else
    {
        return gparent->left_;
    }
}
template <typename KEY, typename VAL>
RBTree<KEY, VAL>::RBTree() : root(nullptr)
{
}
template <typename KEY, typename VAL>
RBTreeNode<KEY, VAL> *RBTree<KEY, VAL>::find(KEY key)
{
    auto ptr = root;
    while (ptr && ptr->key_ != key)
    {
        if (key < ptr->key_)
            ptr = ptr->left_;
        else
            ptr = ptr->right_;
    }
    return ptr;
}
template <typename KEY, typename VAL>
RBTreeNode<KEY, VAL> *RBTree<KEY, VAL>::left_rotate(RBTreeNode<KEY, VAL> *ptr)
{
    if (ptr->right_ == nullptr)
        assert(false);
    auto right = ptr->right_;
    auto parent = ptr->parent_;
    ptr->right_ = right->left_;
    if (right->left_ != nullptr)
        right->left_->parent_ = ptr;
    ptr->parent_ = right;
    right->left_ = ptr;
    right->parent_ = parent;
    if (parent != nullptr)
    {
        if (parent->left_ == ptr)
            parent->left_ = right;
        else
            parent->right_ = right;
    }
    else
    {
        root = right;
    }
    return right;
}
template <typename KEY, typename VAL>
RBTreeNode<KEY, VAL> *RBTree<KEY, VAL>::right_rotate(RBTreeNode<KEY, VAL> *ptr)
{
    if (ptr->left_ == nullptr)
        assert(false);
    auto left = ptr->left_;
    auto parent = ptr->parent_;
    ptr->left_ = left->right_;
    if (left->right_ != nullptr)
        left->right_->parent_ = ptr;
    ptr->parent_ = left;
    left->right_ = ptr;
    left->parent_ = parent;
    if (parent != nullptr)
    {
        if (parent->right_ == ptr)
            parent->right_ = left;
        else
            parent->left_ = left;
    }
    else
    {
        root = left;
    }
    return left;
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
    if (parent == gparent->left_)
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
            if (parent->left_ == ptr)
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
            if (parent->right_ == ptr)
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
    auto ptr = root;
    RBTreeNode<KEY, VAL> *ret = nullptr;
    RBTreeNode<KEY, VAL> *parent = nullptr;
    while (ptr)
    {
        if (key < ptr->key_)
        {
            parent = ptr;
            ptr = ptr->left_;
        }
        else
        {
            parent = ptr;
            ptr = ptr->right_;
        }
    }
    if (!parent)
    {
        //无节点
        root = new RBTreeNode(key, val, Color::black);
        return root;
    }
    ret = new RBTreeNode(key, val, Color::red);
    if (key < parent->key_)
    {
        parent->left_ = ret;
        ret->parent_ = parent;
        adjust_for_insert(ret);
    }
    else if (key > parent->key_)
    {
        parent->right_ = ret;
        ret->parent_ = parent;
        adjust_for_insert(ret);
    }
    else
    {
        std::cout << "insert same element;ignore\n";
    }
    return ret;
}
template <typename KEY, typename VAL>
void RBTree<KEY, VAL>::adjust_double_black_node(RBTreeNode<KEY, VAL> *ptr)
{
    if (ptr == root)
        return;
    RBTreeNode<KEY, VAL> *parent = ptr->parent_;
    RBTreeNode<KEY, VAL> *brother = ptr->get_brother();
    if (parent->left_ == ptr)
    {
        if (is_black(brother))
        {
            //由平衡条件,此时兄弟节点不可能为nullptr
            if (is_black(brother->left_) && is_black(brother->right_))
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
            else if (!is_black(brother->left_))
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
            if (parent->left_ == ptr)
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
    if (parent->right_ == ptr)
    {
        if (is_black(brother))
        {
            //由平衡条件,此时兄弟节点不可能为nullptr
            if (is_black(brother->right_) && is_black(brother->left_))
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
            else if (!is_black(brother->right_))
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
            if (parent->right_ == ptr)
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
    RBTreeNode<KEY, VAL> *ptr = root;
    while (ptr && ptr->key_ != key)
    {
        if (ptr->key_ < key)
            ptr = ptr->right_;
        else
            ptr = ptr->left_;
    }
    if (!ptr)
    {
        std::cout << "don't find node to erase\n";
        return;
    }
    if (ptr->left_ && ptr->right_)
    {
        RBTreeNode<KEY, VAL> *qtr = ptr->right_;
        while (qtr->left_ != nullptr)
            qtr = qtr->left_;
        std::swap(ptr->key_, qtr->key_);
        std::swap(ptr->val_, qtr->val_);
        ptr = qtr;
    }
    if (!ptr->left_ && !ptr->right_)
    {
        if (ptr->color_ == Color::red)
        {
            if (ptr->parent_)
                ptr->get_index_in_parent() = nullptr;
            return;
        }
        else
        {
            adjust_double_black_node(ptr);
            if (ptr->parent_)
                ptr->get_index_in_parent() = nullptr;
            return;
        }
    }
    else if (ptr->left_ && !ptr->right_)
    {
        if (ptr->color_ == Color::red)
        {
            if (ptr->parent_)
                ptr->get_index_in_parent() = ptr->left_;
            ptr->left_->parent_ = ptr->parent_;
            return;
        }
        else if (ptr->color_ == Color::black)
        {
            if (ptr->parent_)
                ptr->get_index_in_parent() = ptr->left_;
            ptr->left_->parent_ = ptr->parent_;
            if (ptr->left_->color_ == Color::red)
            {
                ptr->left_->color_ = Color::black;
                return;
            }
            else
            {
                adjust_double_black_node(ptr->left_);
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
                ptr->get_index_in_parent() = ptr->right_;
            ptr->right_->parent_ = ptr->parent_;
            return;
        }
        else if (ptr->color_ == Color::black)
        {
            if (ptr->parent_)
                ptr->get_index_in_parent() = ptr->right_;
            ptr->right_->parent_ = ptr->parent_;
            if (ptr->right_->color_ == Color::red)
            {
                ptr->right_->color_ = Color::black;
                return;
            }
            else
            {
                adjust_double_black_node(ptr->right_);
                return;
            }
        }
        else
            assert(false);
    }
    else
        assert(false);
}