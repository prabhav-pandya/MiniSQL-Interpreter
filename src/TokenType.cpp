
enum TokenType {
    // Single-character tokens.
    LEFT_PAREN, RIGHT_PAREN, SEMICOLON, STAR, COMMA, DOT, MINUS, PLUS, SLASH,

    // One or two character tokens.
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    // Data Types - it contains all STR, INT, etc. types
    TYPE,

    // Literals.
    IDENTIFIER, STRING, NUMBER,

    // Keywords
    AND, OR, TABLE, FROM, WHERE, IN, HAVING, AS, INTO, VALUES,
    SELECT, CREATE, UPDATE, INSERT, QUIT, DROP, DELETE, SET,

    // Help
    HELP, DESCRIBE, TABLES,

    NULL_TOKEN
};

/*
 AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,
 */
