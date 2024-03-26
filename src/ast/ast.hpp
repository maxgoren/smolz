#ifndef ast_hpp
#define ast_hpp
#include "../lexer/lexer.hpp"
enum NodeK {EXPRNODE, STMTNODE};
enum ExprK {ID_EXPR, OP_EXPR, CONST_EXPR, STRINGLIT_EXPR, LIST_EXPR, FUNC_EXPR};
enum StmtK {PRINT_STMT, IF_STMT, LOOP_STMT, ASSIGN_STMT, DEF_STMT, PUSH_STMT, POP_STMT, RETURN_STMT};

struct ASTNode {
    NodeK kind;
    union { ExprK expr; StmtK stmt; } type;
    struct { TOKENS tokenVal; string stringVal; double doubleVal; } data;
    ASTNode* next;
    ASTNode* left;
    ASTNode* mid;
    ASTNode* right;
};

ASTNode* makeExprNode(ExprK type, TOKENS token, string str);

ASTNode* makeStmtNode(StmtK type, TOKENS token, string str);

class ASTTracer {
    private:
        int depth;
        void printNode(ASTNode* node);
    public:
        ASTTracer();
        void traverse(ASTNode* node);

};

#endif