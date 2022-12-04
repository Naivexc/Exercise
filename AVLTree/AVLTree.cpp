#include <iostream>
#include <vector>
#include <set>
#include "AVLTree.h"
int depth = 0;
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
struct A
{
    int x_;
    A(int x) : x_(x){};
    ~A() { std::cout << "Destructor Called:" << x_ << '\n'; }
};
bool operator<(const A &a, const A &b) { return a.x_ < b.x_; }
bool operator>(const A &a, const A &b) { return a.x_ > b.x_; }
bool operator==(const A &a, const A &b) { return a.x_ == b.x_; }
bool operator<=(const A &a, const A &b) { return a.x_ <= b.x_; }
bool operator>=(const A &a, const A &b) { return a.x_ >= b.x_; }
bool operator!=(const A &a, const A &b) { return a.x_ != b.x_; }
std::ostream &operator<<(std::ostream &out, const A &a)
{
    out << a.x_;
    return out;
}
void Test()
{
    int N = 0, M = 0, W = 0;
    std::cout << "please enter num of nodes:N\n";
    std::cin >> N;
    std::cout << "please enter range of val:M, M>=N\n";
    std::cin >> M;
    std::cout << "please enter num of nodes after delete:W,W<=N\n";
    std::cin >> W;
    AVLTree<A> tree;
    std::set<A> test;
    int cur = 0;
    for (int i = 0; i < N; ++i)
    {
        while (true)
        {
            cur = rand() % M;
            if (test.find(cur) == test.end())
                break;
        }
        std::cout << "insert " << cur << '\n';
        tree.insert(A(cur));
        test.insert(A(cur));
        if (tree.check())
            std::cout << "not failed\n";
    }
    for (int i = 0; i < N - W; ++i)
    {
        cur = rand() % test.size();
        auto iter = test.begin();
        while (cur)
        {
            ++iter;
            --cur;
        }
        std::cout << "delete " << iter->x_ << '\n';
        test.erase(iter);
        tree.erase(*iter);
        if (tree.check())
            std::cout << "not failed\n";
    }
}
int main()
{
    Test();
    return 0;
    AVLTree<int> tree;
    std::string cmd;
    std::vector<std::string> parsed_cmd;
    std::vector<int> init_vec{};
    std::set<int> test;
    for (int i = 0; i < init_vec.size(); ++i)
    {
        tree.insert(init_vec[i]);
        test.insert(init_vec[i]);
    }
    while (std::getline(std::cin, cmd))
    {
        cmd_parse(cmd, parsed_cmd);
        if (parsed_cmd[0] == "insert")
        {
            for (int i = 1; i < parsed_cmd.size(); ++i)
            {
                tree.insert(string_to_int(parsed_cmd[i]));
                test.insert(string_to_int(parsed_cmd[i]));
            }
        }
        else if (parsed_cmd[0] == "find")
        {
            continue;
        }
        else if (parsed_cmd[0] == "delete")
        {
            for (int i = 1; i < parsed_cmd.size(); ++i)
            {
                tree.erase(string_to_int(parsed_cmd[i]));
                test.erase(string_to_int(parsed_cmd[i]));
            }
        }
        else if (parsed_cmd[0] == "quit")
        {
            break;
        }
        else
        {
            std::cout << "wrong cmd\n";
        }
        if (tree.root_)
        {
            tree.print();
        }
        else
        {
            std::cout << "Empty\n";
        }
    }
    //  std::cout << tree.find("b")->val_ << '\n';
}