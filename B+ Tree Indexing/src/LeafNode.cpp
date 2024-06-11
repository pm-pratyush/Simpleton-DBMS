#include "RecordPtr.hpp"
#include "LeafNode.hpp"

LeafNode::LeafNode(const TreePtr &tree_ptr) : TreeNode(LEAF, tree_ptr) {
    this->data_pointers.clear();
    this->next_leaf_ptr = NULL_PTR;
    if(!is_null(tree_ptr))
        this->load();
}

//returns max key within this leaf
Key LeafNode::max() {
    auto it = this->data_pointers.rbegin();
    return it->first;
}

//inserts <key, record_ptr> to leaf. If overflow occurs, leaf is split
//split node is returned
//TODO: LeafNode::insert_key to be implemented
TreePtr LeafNode::insert_key(const Key &key, const RecordPtr &record_ptr) {
    TreePtr new_leaf = NULL_PTR; //if leaf is split, new_leaf = ptr to new split node ptr
    cout << "LeafNode::insert_key implemented" << endl;

    // Check if the key already exists
    if (this->data_pointers.find(key) != this->data_pointers.end()) {
        cout << "Key already exists" << endl;
        return new_leaf;
    }
    else {
        this->data_pointers.insert(pair<Key,RecordPtr>(key, record_ptr));
        this->size++;

        // Check if the leaf is full
        if (this->overflows()) {
            // Create a new leaf node
            auto temp_node = dynamic_cast<LeafNode*>(TreeNode::tree_node_factory(LEAF));
            new_leaf = temp_node->tree_ptr;
            
            temp_node->next_leaf_ptr = this->next_leaf_ptr;
            this->next_leaf_ptr = new_leaf;

            // Iterate through the current leaf node's data pointers and insert them into the new leaf node
            int i = 0;
            auto it = this->data_pointers.begin();
            for (; it != this->data_pointers.end(); it++) {
                if (i >= MIN_OCCUPANCY)
                    break;
                i++;
            }

            while(it != this->data_pointers.end()) {
                temp_node->data_pointers.insert(pair<Key,RecordPtr>(it->first, it->second));
                temp_node->size++;
                it = this->data_pointers.erase(it);
            }
            this->size = MIN_OCCUPANCY;

            temp_node->dump();
            delete temp_node;
        }
        this->dump();
        return new_leaf;
    }
}

// merge_sibling_node: merge the sibling node into the current node
void LeafNode::merge_sibling_node(TreeNode *sibling_node) {
    cout << "LeafNode::merge_sibling_node implemented" << endl;

    // Cast the sibling node to a leaf node
    auto sibling_leaf_node = dynamic_cast<LeafNode*>(sibling_node);

    // Iterate through the sibling node's data pointers and insert them into the current node
    for (auto it = sibling_leaf_node->data_pointers.begin(); it != sibling_leaf_node->data_pointers.end(); it++) {
        this->data_pointers.insert(pair<Key,RecordPtr>(it->first, it->second));
        this->size++;
    }

    // Handle next leaf pointer
    this->next_leaf_ptr = sibling_leaf_node->next_leaf_ptr;

    // Delete the sibling node
    sibling_leaf_node->delete_node();
    this->dump();
}

// left_redistribute: data pointer with MIN key from the leaf node to the left sibling node
void LeafNode::left_redistribute(TreeNode *right_sibling_node) {
    cout << "LeafNode::left_redistribute implemented" << endl;

    right_sibling_node->insert_key(this->data_pointers.rbegin()->first, this->data_pointers.rbegin()->second);
    this->delete_key(this->data_pointers.rbegin()->first);
}

// right_redistribute: data pointer with MAX key from the leaf node to the right sibling node
void LeafNode::right_redistribute(TreeNode *left_sibling_node) {
    cout << "LeafNode::right_redistribute implemented" << endl;

    left_sibling_node->insert_key(this->data_pointers.begin()->first, this->data_pointers.begin()->second);
    this->delete_key(this->data_pointers.begin()->first);
}

//key is deleted from leaf if exists
//TODO: LeafNode::delete_key to be implemented
void LeafNode::delete_key(const Key &key) {
    cout << "LeafNode::delete_key implemented" << endl;

    // Check if the key exists
    if (this->data_pointers.find(key) == this->data_pointers.end()) {
        cout << "Key does not exist" << endl;
        return;
    }
    else {
        this->data_pointers.erase(key);
        this->size--;
    }
    this->dump();
}

//runs range query on leaf
void LeafNode::range(ostream &os, const Key &min_key, const Key &max_key) const {
    BLOCK_ACCESSES++;
    for(const auto& data_pointer : this->data_pointers){
        if(data_pointer.first >= min_key && data_pointer.first <= max_key)
            data_pointer.second.write_data(os);
        if(data_pointer.first > max_key)
            return;
    }
    if(!is_null(this->next_leaf_ptr)){
        auto next_leaf_node = new LeafNode(this->next_leaf_ptr);
        next_leaf_node->range(os, min_key, max_key);
        delete next_leaf_node;
    }
}

//exports node - used for grading
void LeafNode::export_node(ostream &os) {
    TreeNode::export_node(os);
    for(const auto& data_pointer : this->data_pointers){
        os << data_pointer.first << " ";
    }
    os << endl;
}

//writes leaf as a mermaid chart
void LeafNode::chart(ostream &os) {
    string chart_node = this->tree_ptr + "[" + this->tree_ptr + BREAK;
    chart_node += "size: " + to_string(this->size) + BREAK;
    for(const auto& data_pointer: this->data_pointers) {
        chart_node += to_string(data_pointer.first) + " ";
    }
    chart_node += "]";
    os << chart_node << endl;
}

ostream& LeafNode::write(ostream &os) const {
    TreeNode::write(os);
    for(const auto & data_pointer : this->data_pointers){
        if(&os == &cout)
            os << "\n" << data_pointer.first << ": ";
        else
            os << "\n" << data_pointer.first << " ";
        os << data_pointer.second;
    }
    os << endl;
    os << this->next_leaf_ptr << endl;
    return os;
}

istream& LeafNode::read(istream& is){
    TreeNode::read(is);
    this->data_pointers.clear();
    for(int i = 0; i < this->size; i++){
        Key key = DELETE_MARKER;
        RecordPtr record_ptr;
        if(&is == &cin)
            cout << "K: ";
        is >> key;
        if(&is == &cin)
            cout << "P: ";
        is >> record_ptr;
        this->data_pointers.insert(pair<Key,RecordPtr>(key, record_ptr));
    }
    is >> this->next_leaf_ptr;
    return is;
}