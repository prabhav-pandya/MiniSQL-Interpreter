# Mini SQL-Interpreter
An interpreter for a subset of SQL<br>
<a href="https://www.w3schools.com/cpp/"> ![C++](https://img.shields.io/badge/C%2B%2B-blue?style=for-the-badge&logo=c%2B%2B&logoColor=white)</a>
<a href="https://www.w3schools.com/cpp/"> ![IntelliJ](https://img.shields.io/badge/intellij-831299?style=for-the-badge&logo=intellij&logoColor=white)</a>
<img src="https://github.com/prabhav-pandya/MiniSQL-Interpreter/blob/master/banner.png">
<br><br>
## Description
The interpreter supports the following SQL commands:
* SELECT  
* CREATE TABLE
* DROP TABLE
* INSERT
* UPDATE
* DELETE
* HELP
<br>
Commands like: Select, Insert & Delete have support for an optional 'WHERE' command with specific type of conditional queries ( condition1 AND condition2 AND ...) or (condition1 OR condition2 OR ...).
<br><br>
Queries are *NOT* case sensitive except with indentifiers and aren't affected by redundant white spaces.
<br><br>
FOREIGN & PRIMARY KEYS are also supported.<br>
```
Example query := CREATE TABLE table_name (id INT PRIMARY KEY, name STR, dept INT FOREIGN KEY REFERENCES table_name table_column).
```

## Working of the Interpreter
The query goes through the following stages to display the final result:
* **Token Generation**: Every query is first passed to a Scanner class that breaks down the query into tokens and returns an array of tokens.
* **Parser**: The array of tokens is passed to a Parser class that ensures syntactical correctness of the query and returns an object containing all the information the interpreter will need in its final stage.
* **Interpreter**: Depending on the type of query, the interpreter creates, modifies, deletes or displays the requested table(s).
