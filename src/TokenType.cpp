
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

    // KEYWORDS
    AND, OR, SELECT, CREATE, TABLE, FROM, WHERE, UPDATE, INSERT, IN, HAVING, AS, QUIT,

    NULL_TOKEN
};

/*
 AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,
 */
