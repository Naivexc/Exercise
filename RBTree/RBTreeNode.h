#ifndef RB_TREE_NODE_H
#define RB_TREE_NODE_H
extern int depth;
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
    std::unique_ptr<RBTreeNode<KEY, VAL>> left_;
    std::unique_ptr<RBTreeNode<KEY, VAL>> right_;
    RBTreeNode(KEY key, VAL val, Color color);
    RBTreeNode *get_uncle();
    RBTreeNode *get_brother();
    std::unique_ptr<RBTreeNode<KEY, VAL>> &get_index_in_parent(); //返回父节点对其的索引指针;为根节点时返回nullptr
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
    if (parent_->left_.get() == this)
        return parent_->right_.get();
    else
        return parent_->left_.get();
}
template <typename KEY, typename VAL>
std::unique_ptr<RBTreeNode<KEY, VAL>> &RBTreeNode<KEY, VAL>::get_index_in_parent()
{
    if (!parent_)
        assert(false);
    if (parent_->left_.get() == this)
        return parent_->left_;
    return parent_->right_;
}
template <typename KEY, typename VAL>
void RBTreeNode<KEY, VAL>::print_middle()
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
    std::cout << "depth:" << depth << " key:" << key_ << " val:" << val_ << " color:" << (color_ == Color::black ? "black\n" : "red\n");
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
    if (gparent->left_.get() == parent_)
        return gparent->right_.get();
    return gparent->left_.get();
}

#endif
