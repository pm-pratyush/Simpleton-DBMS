# Simpleton-DBMS

A laughably-minimalist, integer-only, read-only Relational Database Management System that makes the author question why they ever bothered to write it up!

## Compilation Instructions

We use ``make`` to compile all the files and creste the server executable. ``make`` is used primarily in Linux systems, so those of you who want to use Windows will probably have to look up alternatives (I hear there are ways to install ``make`` on Windows). To compile

``cd`` into the SimpleRA directory

```
cd SimpleRA
```

``cd`` into the soure directory (called ``src``)

```
cd src
```

To compile

```
make clean
make
```

## To run

Post compilation, an executable names ``server`` will be created in the ``src`` directory

```
./server
```

## To setup your Git Repository

- Join the course github organisation using the invite link.
- Join or create a team corresponding to your team name on the organisation.
- Your repository will be initialised with a template code on succesful authorisation.
- Now clone your personal repo using ``git clone "your repo link"``.
- After you have cloned your repo to your system add the main repo as an upstream to your repo so whenever a change is pushed by TA's you can access it.
- To set it as upstream type ``git remote add upstream https://github.com/SimpleRA/SimpleRA.git``.
- Now it will be added as upstream.
- To fetch the changes made by TA's just type ``git pull upstream master --allow-unrelated-histories`` .
- Manually merge conflicts if there are any !!

## Git tutorials

- [Basic github tutorial](https://youtu.be/SWYqp7iY_Tc)
- [Handling git merge conflicts](https://youtu.be/JtIX3HJKwfo)
- [What is git stash?](https://youtu.be/KLEDKgMmbBI)
- [The best way is documentation itself](https://docs.github.com/en)

## Instructions for Creating a Pull Request

- First fork the repo from the main code repository.
- Then commit your changes to this forked repository.
- To create a pull request go to the main repository and click on the pull request option.
- Kindly only submit your pull requests to the branch **student-pull-requests** only. We won't entertain pull requests to master branch.

## Project Phase - 1: Matrix Support in SimpleRA

### Individual Contributions

| Roll No.   | Team Member       | Contributions                                                                                                                                   |
| ---------- | ----------------- | ----------------------------------------------------------------------------------------------------------------------------------------------- |
| 2020101005 | Pratyush Mohanty  | A. LOAD MATRIX `<name>`<br />B. PRINT MATRIX `<`name `>`<br />C. EXPORT MATRIX `<`name `>`<br />D. RENAME MATRIX <`name`>           |
| 2021121001 | Debanil Chowdhury | A. LOAD MATRIX `<`name `>`<br />B. TRANSPOSE MATRIX `<`name `>`<br />C. RENAME MATRIX `<`name `>`<br />D. CHECK SYMMETRY <`name`> |

### Introduction

The goal of this project was to enhance the SimpleRA database management system to support matrices. This extension allows the system to handle square matrices of integers provided as .csv files. The implemented commands provide functionality for loading, printing, transposing, exporting, renaming matrices, checking for symmetry, and performing computations on matrices.

### Commands Implemented

1. **LOAD MATRIX `<name>`**
   * This command loads a matrix from a .csv file into the system.
   * Example: `LOAD MATRIX A`
2. **PRINT MATRIX `<name>`**
   * This command prints the contents of a matrix with the specified name.
   * Example: `PRINT MATRIX A`
3. **TRANSPOSE MATRIX `<name>`**
   * Transposes the specified matrix, creating a new matrix.
   * Example: `TRANSPOSE MATRIX A`
4. **EXPORT MATRIX `<name>`**
   * Exports the contents of a matrix to a .csv file with the specified name.
   * Example: `EXPORT MATRIX A`
5. **RENAME MATRIX `<old name> <new name>`**
   * Allows users to rename a matrix to a new name.
   * Example: `RENAME MATRIX A B`
6. **CHECKSYMMETRY <matrix_name>**
   * Checks whether a given matrix is symmetric (A == A') and returns a Boolean value.
   * Example: `CHECKSYMMETRY A`
7. **COMPUTE <matrix_name>**
   * Enables users to perform a computation (A - A') on matrices.
   * Example: `COMPUTE A - A'`

### Conclusion

The addition of matrix support to SimpleRA significantly enhances its capabilities, making it suitable for a wider range of data management tasks. Users can load, manipulate, and analyze square matrices seamlessly. This extension opens up opportunities for various applications, such as numerical analysis and scientific computing, within the SimpleRA environment.

### Learnings on Page and Block Structure from the Matrix Extension Project

#### 1. Data Organization

* **Matrix Storage** : Storing matrices efficiently requires considering their square structure. We need to decide how to map the two-dimensional matrix into pages and blocks while optimizing for read and write operations.

#### 2. File Formats

* **CSV as Storage** : CSV files can serve as a convenient storage format for matrices, but transforming them into an efficient in-memory structure for DBMS operations requires careful consideration of how data is read, parsed, and organized.

#### 3. Page and Block Management

* **Memory Allocation** : When dealing with large matrices, efficient memory allocation and management are crucial. Loading entire matrices into memory may not always be feasible, and thus, we need strategies for partial loading and handling.

#### 4. Performance Trade-offs

* **Optimizations** : Different matrix operations (e.g., transpose, computation) may require different data layouts or indexing structures for optimal performance. Balancing these trade-offs is essential to ensure efficient execution.

#### 5. Metadata Handling

* **Metadata Storage** : We need to decide how to store and manage metadata efficiently. This can include storing information about matrix dimensions, data types, and other relevant properties.

#### 6. Query Optimization

* **Query Planning** : For matrix-specific operations, such as checking symmetry or performing computations, query planning and optimization need to be adapted to handle these operations efficiently.

#### 7. Scalability

* **Scalability Challenges** : Dealing with large matrices (up to n x n with n <= 10^4) presents scalability challenges. Strategies for handling such large data efficiently must be developed.

In conclusion, extending our DBMS to support matrices has deepened our understanding of the intricacies of page and block structures within a database. We've learned that matrix data, due to its unique characteristics, requires thoughtful adaptations to traditional database storage and retrieval mechanisms to ensure efficient and performant operations. These learnings will inform our future endeavors in database management and system optimization.

## Project Phase - 2: Implement External Sorting, JOIN, GROUP BY and ORDER Commands

### Individual Contributions

| Roll No.   | Team Member       | Contributions                                 |
| ---------- | ----------------- | --------------------------------------------- |
| 2021121001 | Debanil Chowdhury | Implemented Task 1 (External Sorting)         |
| 2020101005 | Pratyush Mohanty  | Implemented Task 2 (JOIN, GROUP BY and ORDER) |

### Introduction

The goal of this project was to enhance the SimpleRA database management system to support external sorting, and to implement JOIN, GROUP BY and ORDER commands. This extension allows the system to handle sorting of tables even if they are too large to store in RAM. The implemented commands provide functionality for JOIN, GROUP BY and ORDER commands, which are similar to the commands of corresponding name in SQL.

### Commands Implemented

1. **SORT `<table_name>` BY `<column_name1, column_name2,..., column_namek>` IN `<ASC|DESC, ASC|DESC,..., ASC|DESC`**
   * This command externally sorts the specified columns `<column_name1, column_name2,..., column_namek>` in a table specified by `<table_name>`, in the order specified by `<ASC|DESC, ASC|DESC,..., ASC|DESC`, with each column having an order specified.
   * Example: `SORT items BY price, discount IN ASC, DESC`
2. **`<new_relation_name>` <- JOIN `<tablename1>, <tablename2>` ON `<column1> <bin_op> <column2>`**
   * This command executes a JOIN operation where the resultant table is specified by `<new_relation_name>`. The input tables are `<tablename1>, <tablename2>`. The columns used for comparison are `<column1> <column2>` and the way of comparison is `<bin_op>`.
   * Example: `failed-students <- JOIN courses, students ON minimum-pass-marks <= student-marks`
3. **`<new_table>` <- GROUP BY `<grouping_attribute>` FROM `<table_name>` HAVING `<aggregate(attribute)> <bin_op> <attribute_value>` RETURN `<aggregate_func(attribute)>`**
   * This command groups the rows in table `<table_name>` according to the attribute `<grouping_attribute>`. Then the rows whose attributes satisfy a certain condition specified by `<aggregate(attribute)>` are taken and the result of the function `<aggregate_func(attribute)>` applied to those attributes are output into the resultant table `<new_table>`
   * Example: `where-to-watch-movie <- GROUP BY rating FROM released-movies HAVING MIN(ticket-price) < budget RETURN MIN(distance-to-hall)`
4. **`<new_table>` <- ORDER BY `<attribute>` ASC|DESC ON `<table_name>`**
   * This command sorts the column `<attribute>` from table `<table_name>` according to the order specified and stores the sorted table as table `<new_table>`.
   * Example: `courses-to-take <- ORDER BY price ASC ON online-courses`

### Conclusion

The addition of external sorting support to SimpleRA significantly enhances its capabilities, making it possible to sort tables whose storage space is larger than RAM. Furthermore, with JOIN, GROUP BY and ORDER commands, the SimpleRA system is now suitable for a wider range of data manipulation tasks.

## Project Phase - 3: B+Tree Indexing

### Individual Contributions

| Roll No.   | Team Member       | Contributions       |
| ---------- | ----------------- | ------------------- |
| 2020101005 | Pratyush Mohanty  | Insertion, Analysis |
| 2021121001 | Debanil Chowdhury | Deletion, Analysis  |

### Objective

The objective of this phase is to implement a B+Tree index. The disk-based index is built on top of an unordered heap file organization. We assume records are unique 1-sized tuples consisting of non-negative integers (records consist of only one attribute which is a key attribute).

### Commands Implemented

1. **INSERT `<key>`**
   * Inserts key into heap and B+Tree.
   * Example: `INSERT 5`
2. **DELETE `<key>`**
   * Deletes key from heap and B+Tree.
   * Example: `DELETE 5`
3. **RANGE `<key1>` `<key2>`**
   * Performs a range query to retrieve keys within the range [key1, key2]. The results are written in file data/range_[key1_key2].txt. This command also prints the number of block accesses needed to perform this query using the B+Tree and just using the heap.
   * Example: `RANGE 1 10`
4. **SOURCE `<file>`**
   * Executes all commands in file data/`<file>`.
   * Example: `SOURCE commands.txt`
5. **EXPORT**
   * Exports heap into the file heap.md and B+Tree into the file data/bptree.md.
   * Example: `EXPORT`
6. **QUIT**
   * Quits the command line interpreter.
   * Example: `QUIT`

### B+Tree Structure

The B+Tree index is a tree structure consisting of tree nodes (class TreeNode) - internal nodes (class InternalNode) and leaf nodes (class LeafNode) satisfying the following properties:

#### Internal Nodes

* An internal node consists of keys and tree pointers.
* All the elements in the subtree rooted at Pi are greater than Ki − 1 and less than or equal to Ki. All the keys in the internal node are sorted (K1 < K2 < ... < Kq-1).
* Every internal node, except the root node, should have at least ⌈f/2⌉ and at most f children (tree pointers), where f is the fanout of the B+Tree (FANOUT = 3).

#### Leaf Nodes

* A leaf consists of <K, data_pointer> pairs, where K is the key or value of the indexing field, and data_pointer points to the location of the record on the disk.
* Every leaf points to the next leaf.
* Every leaf node (except the root node) should have at least ⌈f/2⌉ and at most f children, represented as pairs of <key, record pointers>, where f is the fanout of the B+Tree.

![img](./docs/chart.gif)

### Insertion

In this part, two function definitions are completed. The first is performing insert in leaf nodes.

1. **TreePtr LeafNode::insert_key(Key key, RecordPtr record_ptr)**
   * Implementing insertion in the leaf node is straightforward - if there is space in the leaf, we insert the <key, record_ptr> pair and write the leaf back to the disk. If there is no space in the leaf (overflow), split the leaf into two and return a tree pointer pointing to the new split leaf.
   * When a leaf node n is split to create a new leaf node split_node, node n retains the smallest (key) ⌈f/2⌉ <key, RecordPtr> pairs and the rest are inserted into split_node.

The second function to be implemented is insertion within internal nodes

1. **TreePtr InternalNode::insert_key(Key key, RecordPtr record_ptr)**
   * This insert function should provide two capabilities - (1) It first finds the appropriate child to insert the <key, record_ptr> into and (2) If the chosen child returns a split node, then the insert function should include this new node as a new child; if the inclusion of the new child causes a split then the new split node should be returned.
   * When an internal node n is split to create a new node split_node, node n retains the first ⌈f/2⌉ children and the rest are inserted into split_node.

### Deletion

Deletion is a little more complicated than insertion. Again there are two functions to be implemented - deletion in the leaf nodes and internal nodes.

1. **TreePtr LeafNode::delete_key(Key key)**
   * If the key exists, delete <key, RecordPtr> pair.
   * Merging and redistribution will happen through the parent node.
2. **TreePtr InternalNode::delete_key(Key key)**
   * Finds the appropriate child to delete the key from and checks if the chosen child now underflows. If it underflows, then perform the appropriate redistribute/merge operations as per the preference detailed above.

### Analysis

The RANGE command has been implemented for you. When RANGE is called, it prints the number of node accesses needed when the B+Tree is used and when it is not.

In this section, the following steps are completed:

1. Generate a source file where numbers 1-100 are inserted in some random order.
2. Plot the distribution of block accesses needed with and without the B+Tree when calling RANGE i i+1 on every valid i as two separate traces (with and without).

This graph is provided for any 3 different values of FANOUT and submitted as a PDF in the data folder of the repository.

### Conclusion

The implementation of B+Tree indexing in SimpleRA significantly improves its performance for range queries, offering efficient and scalable data retrieval. This phase of the project provided valuable insights into the complexity of indexing structures and their impact on database performance.
