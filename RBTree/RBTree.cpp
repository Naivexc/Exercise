#include <iostream>
#include <vector>
#include "RBTree.h"
int depth = 0;
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
void cmd_parse(std::string cmd, std::vector<std::string> &cmd_parsed)
{
    cmd_parsed.clear();
    auto begin = 0;
    auto end = 0;
    while (begin < cmd.length())
    {
        while (begin < cmd.length() && cmd[begin] == ' ')
            ++begin;
        end = begin + 1;
        while (end < cmd.length() && cmd[end] != ' ')
            ++end;
        cmd_parsed.push_back(cmd.substr(begin, end - begin));
        begin = end;
    }
    return;
}
int string_to_int(std::string str)
{
    int ans = 0;
    int idx = 0;
    while (idx < str.length())
    {
        ans *= 10;
        ans += str[idx] - '0';
        ++idx;
    }
    return ans;
}
int main()
{
    RBTree<int, int> tree;
    std::string cmd;
    std::vector<std::string> parsed_cmd;
    std::vector<int> init_vec{9, 4, 14, 1, 6, 12, 18, 0, 2, 5, 7, 11, 13, 16, 19, 3, 8, 10, 15, 17};
    for (int i = 0; i < init_vec.size(); ++i)
        tree.insert(init_vec[i], init_vec[i]);
    while (std::getline(std::cin, cmd))
    {
        cmd_parse(cmd, parsed_cmd);
        if (parsed_cmd[0] == "insert")
        {
            for (int i = 1; i < parsed_cmd.size(); ++i)
                tree.insert(string_to_int(parsed_cmd[i]), string_to_int(parsed_cmd[i]));
        }
        else if (parsed_cmd[0] == "find")
        {
            auto ptr = tree.find(string_to_int(parsed_cmd[1]));
            if (ptr)
                std::cout << ptr->val_ << '\n';
            else
                std::cout << "don't find\n";
        }
        else if (parsed_cmd[0] == "delete")
        {
            for (int i = 1; i < parsed_cmd.size(); ++i)
                tree.erase(string_to_int(parsed_cmd[i]));
        }
        else if (parsed_cmd[0] == "quit")
        {
            break;
        }
        else
        {
            std::cout << "wrong cmd\n";
        }
        if (tree.root)
        {
            if (!tree.root->check_key())
            {
                std::cout << "error: key wrong\n";
                assert(false);
            }
            if (!tree.root->check_color().first)
            {
                std::cout << "error: color wrong\n";
                assert(false);
            }
            tree.root->print_middle();
        }
        else
        {
            std::cout << "Empty\n";
        }
    }
    //  std::cout << tree.find("b")->val_ << '\n';
}