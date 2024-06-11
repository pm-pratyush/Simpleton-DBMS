#include "InternalNode.hpp"

//creates internal node pointed to by tree_ptr or creates a new one
InternalNode::InternalNode(const TreePtr &tree_ptr) : TreeNode(INTERNAL, tree_ptr) {
    this->keys.clear();
    this->tree_pointers.clear();
    if (!is_null(tree_ptr))
        this->load();
}

//max element from tree rooted at this node
Key InternalNode::max() {
    Key max_key = DELETE_MARKER;
    TreeNode* last_tree_node = TreeNode::tree_node_factory(this->tree_pointers[this->size - 1]);
    max_key = last_tree_node->max();
    delete last_tree_node;
    return max_key;
}

//if internal node contains a single child, it is returned
TreePtr InternalNode::single_child_ptr() {
    if (this->size == 1)
        return this->tree_pointers[0];
    return NULL_PTR;
}

//inserts <key, record_ptr> into subtree rooted at this node.
//returns pointer to split node if exists
//TODO: InternalNode::insert_key to be implemented
TreePtr InternalNode::insert_key(const Key &key, const RecordPtr &record_ptr) {
    TreePtr new_tree_ptr = NULL_PTR;
    cout << "InternalNode::insert_key implemented" << endl;

    int i = 0, num_block_ptrs = this->size;
    while (i < num_block_ptrs - 1 && key > this->keys[i] && this->keys[i] != DELETE_MARKER) {
        i++;
    }

    // Get the child node
    auto child_node = TreeNode::tree_node_factory(this->tree_pointers[i]);
    new_tree_ptr = child_node->insert_key(key, record_ptr);

    if (!is_null(new_tree_ptr)) {
        // If the child node was split, then we need to insert the new key and tree pointer into this node
    
        this->keys.insert(this->keys.begin() + i, child_node->max());
        this->tree_pointers.insert(this->tree_pointers.begin() + i + 1, new_tree_ptr);
        this->size++;

        // Check for overflow
        if (this->overflows())
        {
            InternalNode* new_internal_node = dynamic_cast<InternalNode*>(TreeNode::tree_node_factory(INTERNAL));
            new_tree_ptr = new_internal_node->tree_ptr;

            // Copy the keys and tree pointers from the current node into the new node
            for (int j = MIN_OCCUPANCY; j < this->size - 1; j++) {
                new_internal_node->keys.push_back(this->keys[j]);
            }
            this->keys.resize(MIN_OCCUPANCY - 1);

            for (int j = MIN_OCCUPANCY; j < this->size; j++) {
                new_internal_node->tree_pointers.push_back(this->tree_pointers[j]);
                new_internal_node->size++;
            }
            this->tree_pointers.resize(MIN_OCCUPANCY);

            this->size = MIN_OCCUPANCY;
            
            new_internal_node->dump();
            delete new_internal_node;
        }
        else {
            new_tree_ptr = NULL_PTR;
        }
    }

    this->dump();
    return new_tree_ptr;
}

// merge_sibling_node: merge the sibling node into the current node
void InternalNode::merge_sibling_node(TreeNode *sibling_node) {
    cout << "InternalNode::merge_sibling_node implemented" << endl;

    InternalNode* sibling_internal_node = dynamic_cast<InternalNode*>(sibling_node);

    auto self_child_node = TreeNode::tree_node_factory(this->tree_pointers[this->size - 1]);
    this->keys.push_back(self_child_node->max());
    delete self_child_node;

    for (int i = 0; i < sibling_internal_node->size - 1; i++) {
        this->keys.push_back(sibling_internal_node->keys[i]);
    }
    for (int i = 0; i < sibling_internal_node->size; i++) {
        this->tree_pointers.push_back(sibling_internal_node->tree_pointers[i]);
    }
    this->size += sibling_internal_node->size;

    sibling_internal_node->delete_node();
    this->dump();
}

// left_redistribute: data pointer with MIN key from the leaf node to the left sibling node
void InternalNode::left_redistribute(TreeNode *right_sibling_node) {
    cout << "InternalNode::left_redistribute implemented" << endl;

    InternalNode* right_sibling_internal_node = dynamic_cast<InternalNode*>(right_sibling_node);
    right_sibling_internal_node->tree_pointers.insert(right_sibling_internal_node->tree_pointers.begin(), this->tree_pointers[this->size - 1]);

    auto right_sibling_child_node = TreeNode::tree_node_factory(right_sibling_internal_node->tree_pointers[0]);
    right_sibling_internal_node->keys.insert(right_sibling_internal_node->keys.begin(), right_sibling_child_node->max());
    right_sibling_internal_node->size++;
    delete right_sibling_child_node;

    this->tree_pointers.erase(this->tree_pointers.begin() + this->size - 1);
    this->keys.erase(this->keys.begin() + this->size - 2);
    this->size--;

    right_sibling_internal_node->dump();
    this->dump();
}

// right_redistribute: data pointer with MAX key from the leaf node to the right sibling node
void InternalNode::right_redistribute(TreeNode *left_sibling_node) {
    cout << "InternalNode::right_redistribute implemented" << endl;

    InternalNode* left_sibling_internal_node = dynamic_cast<InternalNode*>(left_sibling_node);

    auto left_sibling_child_node = TreeNode::tree_node_factory(left_sibling_internal_node->tree_pointers[left_sibling_internal_node->size - 1]);
    left_sibling_internal_node->keys.push_back(left_sibling_child_node->max());
    left_sibling_internal_node->tree_pointers.push_back(this->tree_pointers[0]);
    left_sibling_internal_node->size++;
    delete left_sibling_child_node;

    this->tree_pointers.erase(this->tree_pointers.begin());
    this->keys.erase(this->keys.begin());
    this->size--;

    left_sibling_internal_node->dump();
    this->dump();
}

//deletes key from subtree rooted at this if exists
//TODO: InternalNode::delete_key to be implemented
void InternalNode::delete_key(const Key &key) {
    TreePtr new_tree_ptr = NULL_PTR;
    cout << "InternalNode::delete_key implemented" << endl;

    int i = 0, num_block_ptrs = this->size;
    while (i < num_block_ptrs - 1 && key > this->keys[i] && this->keys[i] != DELETE_MARKER) {
        i++;
    }

    // Get the child node
    auto child_node = TreeNode::tree_node_factory(this->tree_pointers[i]);
    child_node->delete_key(key);
    if (i < num_block_ptrs - 1) {
        this->keys[i] = child_node->max();
    }

    // Check if the child node underflows
    if (child_node->underflows()) {
        if (i != 0) {
            // Check if we can redistribute with the left sibling node
            auto left_sibling_node = TreeNode::tree_node_factory(this->tree_pointers[i - 1]);
            if (left_sibling_node->size + child_node->size >= 2 * MIN_OCCUPANCY) {
                // Redistribute with the left sibling node
                left_sibling_node->left_redistribute(child_node);
                this->keys[i - 1] = left_sibling_node->max();
            }
            else {
                // Merge with the left sibling node
                left_sibling_node->merge_sibling_node(child_node);
                this->tree_pointers.erase(this->tree_pointers.begin() + i);
                this->keys.erase(this->keys.begin() + i - 1);
                this->size--;
            }
            delete left_sibling_node;
        }
        else {
            // Check if we can redistribute with the right sibling node
            auto right_sibling_node = TreeNode::tree_node_factory(this->tree_pointers[i + 1]);
            if (right_sibling_node->size + child_node->size >= 2 * MIN_OCCUPANCY) {
                // Redistribute with the right sibling node
                right_sibling_node->right_redistribute(child_node);
                this->keys[i] = child_node->max();
            }
            else {
                // Merge with the right sibling node
                child_node->merge_sibling_node(right_sibling_node);
                this->tree_pointers.erase(this->tree_pointers.begin() + i + 1);
                this->keys.erase(this->keys.begin() + i);
                this->size--;
            }
            delete right_sibling_node;
        }
    }

    delete child_node;
    this->dump();
}

//runs range query on subtree rooted at this node
void InternalNode::range(ostream &os, const Key &min_key, const Key &max_key) const {
    BLOCK_ACCESSES++;
    for (int i = 0; i < this->size - 1; i++) {
        if (min_key <= this->keys[i]) {
            auto* child_node = TreeNode::tree_node_factory(this->tree_pointers[i]);
            child_node->range(os, min_key, max_key);
            delete child_node;
            return;
        }
    }
    auto* child_node = TreeNode::tree_node_factory(this->tree_pointers[this->size - 1]);
    child_node->range(os, min_key, max_key);
    delete child_node;
}

//exports node - used for grading
void InternalNode::export_node(ostream &os) {
    TreeNode::export_node(os);
    for (int i = 0; i < this->size - 1; i++)
        os << this->keys[i] << " ";
    os << endl;
    for (int i = 0; i < this->size; i++) {
        auto child_node = TreeNode::tree_node_factory(this->tree_pointers[i]);
        child_node->export_node(os);
        delete child_node;
    }
}

//writes subtree rooted at this node as a mermaid chart
void InternalNode::chart(ostream &os) {
    string chart_node = this->tree_ptr + "[" + this->tree_ptr + BREAK;
    chart_node += "size: " + to_string(this->size) + BREAK;
    chart_node += "]";
    os << chart_node << endl;

    for (int i = 0; i < this->size; i++) {
        auto tree_node = TreeNode::tree_node_factory(this->tree_pointers[i]);
        tree_node->chart(os);
        delete tree_node;
        string link = this->tree_ptr + "-->|";

        if (i == 0)
            link += "x <= " + to_string(this->keys[i]);
        else if (i == this->size - 1) {
            link += to_string(this->keys[i - 1]) + " < x";
        } else {
            link += to_string(this->keys[i - 1]) + " < x <= " + to_string(this->keys[i]);
        }
        link += "|" + this->tree_pointers[i];
        os << link << endl;
    }
}

ostream& InternalNode::write(ostream &os) const {
    TreeNode::write(os);
    for (int i = 0; i < this->size - 1; i++) {
        if (&os == &cout)
            os << "\nP" << i + 1 << ": ";
        os << this->tree_pointers[i] << " ";
        if (&os == &cout)
            os << "\nK" << i + 1 << ": ";
        os << this->keys[i] << " ";
    }
    if (&os == &cout)
        os << "\nP" << this->size << ": ";
    os << this->tree_pointers[this->size - 1];
    return os;
}

istream& InternalNode::read(istream& is) {
    TreeNode::read(is);
    this->keys.assign(this->size - 1, DELETE_MARKER);
    this->tree_pointers.assign(this->size, NULL_PTR);
    for (int i = 0; i < this->size - 1; i++) {
        if (&is == &cin)
            cout << "P" << i + 1 << ": ";
        is >> this->tree_pointers[i];
        if (&is == &cin)
            cout << "K" << i + 1 << ": ";
        is >> this->keys[i];
    }
    if (&is == &cin)
        cout << "P" << this->size;
    is >> this->tree_pointers[this->size - 1];
    return is;
}
