### Implement External Sorting, JOIN, GROUP BY and ORDER commands - Team 11 - Project Phase - 2

#### Individual Contributions:

| Roll No.   | Team Member       | Contributions                                                                                                                             |
| ---------- | ----------------- | ----------------------------------------------------------------------------------------------------------------------------------------- |
| 2021121001 | Debanil Chowdhury | Implemented Task 1 (External Sorting) |
| 2020101005 | Pratyush Mohanty  | Implemented Task 2 (JOIN, GROUP BY and ORDER) |
| 2020111024 | Amey Kunte        | - |

#### Introduction

The goal of this project was to enhance the SimpleRA database management system to support external sorting, and to implement JOIN, GROUP BY and ORDER commands. This extension allows the system to handle sorting of tables even if they are too large to store in RAM. The implemented commands provide functionality for JOIN, GROUP BY and ORDER commands, which are similar to the commands of corresponding name in SQL.

#### Commands Implemented

1. **SORT `<table_name>` BY `<column_name1, column_name2,..., column_namek>` IN `<ASC|DESC, ASC|DESC,..., ASC|DESC`**
   * This command externally sorts the specified columns `<column_name1, column_name2,..., column_namek>` in a table specified by `<table_name>`, in the order specified by `<ASC|DESC, ASC|DESC,..., ASC|DESC`, with each column having an order specified.
   * Example: `SORT items BY price, discount IN ASC, DESC`
2. **`<new_relation_name>` <- JOIN `<tablename1>, <tablename2>` ON `<column1> <bin_op> <column2>`**
   * This command executes a JOIN operation where the resultant table is specified by `<new_relation_name>`. The input tables are `<tablename1>, <tablename2>`. The columns used for comparision are `<column1> <column2>` and the way of comparision is `<bin_op>`.
   * Example: `failed-students <- JOIN courses, students ON minimum-pass-marks <= student-marks`
3. **`<new_table>` <- GROUP BY `<grouping_attribute>` FROM `<table_name>` HAVING `<aggregate(attribute)> <bin_op> <attribute_value>` RETURN `<aggregate_func(attribute)>`**
   * This command groups the rows in table `<table_name>` according to the attribute `<grouping_attribute>`. Then the rows whose attributes satisfy a certain condition specified by `<aggregate(attribute)>` are taken and the result of the function `<aggregate_func(attribute)>` applied to those attributes are output into the resultant table `<new_table>`
   * Example: `where-to-watch-movie <- GROUP BY rating FROM released-movies HAVING MIN(ticket-price) < budget RETURN MIN(distance-to-hall)`
4. **`<new_table>` <- ORDER BY `<attribute>` ASC|DESC ON `<table_name>`**
   * This command sorts the column `<attribute>` from table `<table_name>` according to the order specified and stores the sorted table as table `<new_table>`.
   * Example: `courses-to-take <- ORDER BY price ASC ON online-courses`


#### Conclusion

The addition of external sorting support to SimpleRA significantly enhances its capabilities, making it possible to sort tables whose storage space is larger than RAM. Furthermore, with JOIN, GROUP BY and ORDER commands, the SimpleRA system is now suitable for a wider range of data manipulation tasks.

