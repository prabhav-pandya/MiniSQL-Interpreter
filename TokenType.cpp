
enum TokenType {
    // Single-character tokens.
    LEFT_PAREN, RIGHT_PAREN, SEMICOLON, STAR,

    // One or two character tokens.
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    // Literals.
    IDENTIFIER, STRING, NUMBER,

    // KEYWORDS
    AND, OR, SELECT, CREATE, TABLE, FROM, WHERE, UPDATE, INSERT, IN, HAVING, AS,

    EOF
};

/*
 AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,
 */
