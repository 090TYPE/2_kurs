#include <iostream>
#include <algorithm>
#include <vector>

struct node {
    node(const int& value) : data(value), height(1), left(nullptr), right(nullptr) {}

    int data;
    unsigned char height;
    node* left;
    node* right;
};

unsigned char get_height(node* s) {
    if (s != nullptr) {
        return s->height;
    }
    else {
        return 0;
    }
}

int balance_factor(node* s) {
    return get_height(s->right) - get_height(s->left);
}

void fix_height(node* s) {
    auto lhs = get_height(s->left);
    auto rhs = get_height(s->right);
    s->height = std::max(lhs, rhs) + 1;
}

class tree {
public:
    tree();

    ~tree() = default;

    void insert(const int& value);

    void erase(const int& value);

    friend std::ostream& operator<<(std::ostream& out, const tree& rhs);

private:
    node* root;
    unsigned char height;
    size_t nodes_count;

    void show(node* s) const;

    void get_all_nodes(node* s, std::vector<node*>& data) const;

    node* rotate_right(node* s);

    node* rotate_left(node* s);

    node* balance_tree(node* s);

    node* _insert(node* s, const int& value);

    node* findmin(node* s);

    node* removemin(node* s);

    node* remove(node* s, const int& value);
};

node* tree::rotate_right(node* s) {
    node* tmp = s->left;
    s->left = tmp->right;
    tmp->right = s;
    fix_height(s);
    fix_height(tmp);
    return tmp;
}

node* tree::rotate_left(node* s) {
    node* tmp = s->right;
    s->right = tmp->left;
    tmp->left = s;
    fix_height(s);
    fix_height(tmp);
    return tmp;
}

node* tree::balance_tree(node* s) {
    fix_height(s);
    if (balance_factor(s) == 2) {
        if (balance_factor(s->right) < 0) {
            s->right = rotate_right(s);
        }
        return rotate_left(s);
    }
    if (balance_factor(s) == -2) {
        if (balance_factor(s->left) > 0) {
            s->left = rotate_left(s->left);
        }
        return rotate_right(s);
    }
    return s;
}

node* tree::_insert(node* s, const int& value) {
    if (s == nullptr) {
        return new node(value);
    }
    else if (value < s->data) {
        s->left = _insert(s->left, value);
    }
    else {
        s->right = _insert(s->right, value);
    }
    return balance_tree(s);
}

void tree::insert(const int& value) {
    root = _insert(root, value);
}

std::ostream& operator<<(std::ostream& out, const tree& rhs) {
    rhs.show(rhs.root);
    return out;
}

void tree::show(node* s) const {
    std::vector<node*> data;
    get_all_nodes(root, data);
    std::sort(data.begin(), data.end(), [](node* lhs, node* rhs) { return lhs->height > rhs->height; });
    int it = 0;
    unsigned char prev_height = 0;
    for (const auto& elem : data) {
        if (prev_height != elem->height) {
            std::cout << "\n" << '\n';
        }
        else {
            std::cout << "\t";
        }
        for (int i = 0; i + 1 < elem->height; ++i) {
            std::cout << '\t';
        }
        std::cout << elem->data << ", " << static_cast<int>(elem->height);
        ++it;
        prev_height = elem->height;
        if (it == 2) {
            std::cout << '\t';
            it = 0;
        }
    }
}

tree::tree() : root(nullptr), height(0), nodes_count(0) {}

void tree::get_all_nodes(node* s, std::vector<node*>& data) const {
    if (s == nullptr) {
        return;
    }
    data.push_back(s);
    get_all_nodes(s->left, data);
    get_all_nodes(s->right, data);
}

void tree::erase(const int& value) {
    root = remove(root, value);
}

node* tree::findmin(node* s) {
    if (s->left != nullptr) {
        return findmin(s->left);
    }
    else {
        return s;
    }
}

node* tree::removemin(node* s) {
    if (s->left == nullptr) {
        return s->right;
    }
    s->left = removemin(s->left);
    return balance_tree(s);
}

node* tree::remove(node* s, const int& value) {
    if (s == nullptr) {
        return nullptr;
    }
    else if (value < s->data) {
        s->left = remove(s->left, value);
    }
    else if (value > s->data) {
        s->right = remove(s->right, value);
    }
    else {
        node* lhs = s->left;
        node* rhs = s->right;
        delete s;

        node* min = findmin(rhs);
        min->right = removemin(rhs);
        min->left = lhs;
        return balance_tree(min);
    }
    return balance_tree(s);
}

int main() {


    tree obj;
    obj.insert(110);
    obj.insert(120);
    obj.insert(130);
    obj.insert(140);
    obj.insert(160);
    obj.insert(150);
    obj.insert(180);

    std::cout << obj << '\n';

    obj.erase(140);

    std::cout << obj;

    return 0;
}
