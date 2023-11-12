#pragma once

typedef enum _NodeType {
  N_LITERAL_INT,
  N_LITERAL_FLOAT,
  N_LITERAL_CHAR,
  N_LITERAL_STRING,
  N_LITERAL_BOOL,
  N_LITERAL_NULL,
  N_IDENTIFIER,
  N_SEMICOLON,
  N_COMMA,
  N_EQUAL,
  N_PLUS,
  N_MINUS,
  N_ASTERISK,
  N_SLASH,
  N_DOUBLE_AMPERSAND,
  N_DOUBLE_VERTICAL,
  N_DOT,
  N_EXCLAMATION,
  N_TYPE,
  N_LPAREN,
  N_RPAREN,
  N_LBRACKET,
  N_RBRACKET,
  N_LBRACE,
  N_RBRACE,
  N_STRUCT,
  N_RETURN,
  N_IF,
  N_ELSE,
  N_WHILE,
  N_DO,
  N_FOR,
  N_BREAK,
  N_CONTINUE,
  N_SWITCH,
  N_CASE,
  N_DEFAULT,

  N_PROGRAM,
  N_EXTDEFLIST,
  N_EXTDEF,
  N_EXTDECLIST,
  N_SPECIFIER,
  N_STRUCT_SPECIFIER,
  N_OPTTAG,
  N_TAG,
  N_VARDEC,
  N_FUNDEC,
  N_VARLIST,
  N_PARAMDEC,
  N_COMPST,
  N_STMTLIST,
  N_STMT,
  N_DEFLIST,
  N_DEF,
  N_DECLIST,
  N_DEC,
  N_EXP,
  N_RELOP,
  N_ARGS,

  N_CLASS,
  N_OPERATOR,
  N_DECLARATOR,
  N_DECLARATION,
  N_PARAMETER,
  N_ARGUMENT,
  N_BLOCK,
  N_CALL,
  N_CAST,
  N_SIZEOF,
  N_NEW,
  N_DELETE,
  N_ARRAY,
  N_INDEX,
  N_MEMBER,
  N_POINTER,
  N_DEREFERENCE,
  N_ADDRESS,
  N_ASSIGN,
  N_CONDITIONAL,
  N_PREINCREMENT,
  N_PREDECREMENT,
  N_POSTINCREMENT,
  N_POSTDECREMENT,
  N_NEGATE,
  N_NOT,
  N_BITWISE_NOT,
  N_ADD,
  N_SUBTRACT,
  N_MULTIPLY,
  N_DIVIDE,
  N_MODULO,
  N_BITWISE_AND,
  N_BITWISE_OR,
  N_BITWISE_XOR,
  N_BITWISE_LEFT_SHIFT,
  N_BITWISE_RIGHT_SHIFT,
  N_NOT_EQUAL,
  N_LESS_THAN,
  N_GREATER_THAN,
  N_LESS_THAN_OR_EQUAL,
  N_GREATER_THAN_OR_EQUAL,
  N_LOGICAL_AND,
  N_LOGICAL_OR,
  N_LOGICAL_XOR,
  N_ASSIGN_ADD,
  N_ASSIGN_SUBTRACT,
  N_ASSIGN_MULTIPLY,
  N_ASSIGN_DIVIDE,
  N_ASSIGN_MODULO,
  N_ASSIGN_BITWISE_AND,
  N_ASSIGN_BITWISE_OR,
  N_ASSIGN_BITWISE_XOR,
  N_ASSIGN_BITWISE_LEFT_SHIFT,
  N_ASSIGN_BITWISE_RIGHT_SHIFT,

  N_ERROR,
} type_t;

// This is a simplified version of the AST used in the C++ front end.
typedef struct tnode_t {
  struct tnode_t *parent;
  struct tnode_t *firstChild;
  struct tnode_t *lastChild;
  struct tnode_t *nextSibling;
  struct tnode_t *prevSibling;
} tnode_t;

#define container_of(ptr, type, member)                                        \
  (type *)((char *)ptr - (char *)&(((type *)0)->member))

#define TN_INIT(node)                                                          \
  do {                                                                         \
    (node)->parent = NULL;                                                     \
    (node)->firstChild = NULL;                                                 \
    (node)->lastChild = NULL;                                                  \
    (node)->nextSibling = NULL;                                                \
    (node)->prevSibling = NULL;                                                \
  } while (0)

tnode_t *tn_append_child(tnode_t *parent, tnode_t *child);
tnode_t *tn_prepend_child(tnode_t *parent, tnode_t *child);
tnode_t *tn_remove_child(tnode_t *parent, tnode_t *child);
tnode_t *tn_replace_child(tnode_t *parent, tnode_t *oldChild,
                          tnode_t *newChild);
tnode_t *tn_insert_before(tnode_t *parent, tnode_t *oldChild,
                          tnode_t *newChild);
tnode_t *tn_insert_after(tnode_t *parent, tnode_t *oldChild, tnode_t *newChild);

#define TN_FOR_EACH_CHILD(child, parent)                                       \
  for (child = (parent)->firstChild; child != NULL; child = child->nextSibling)

typedef struct node_t {
  type_t nodeType;
  tnode_t treeNode;
  int lineno;
  union {
    int intValue;
    float floatValue;
    char charValue;
    char *strValue;
  };
} node_t;

#define TN2NODE(tn) container_of(tn, node_t, treeNode)

node_t *make(type_t nodeType);

static inline node_t *makei(type_t nodeType, int intValue) {
  node_t *node = make(nodeType);
  node->intValue = intValue;
  return node;
}
static inline node_t *makef(type_t nodeType, float floatValue) {
  node_t *node = make(nodeType);
  node->floatValue = floatValue;
  return node;
}
static inline node_t *makec(type_t nodeType, char charValue) {
  node_t *node = make(nodeType);
  node->charValue = charValue;
  return node;
}
static inline node_t *makes(type_t nodeType, char *strValue) {
  node_t *node = make(nodeType);
  node->strValue = strValue;
  return node;
}

static inline void appendn(node_t *parent, node_t *child) {
  tn_append_child(&(parent)->treeNode, &(child)->treeNode);
}

static inline void prependn(node_t *parent, node_t *child) {
  tn_prepend_child(&(parent)->treeNode, &(child)->treeNode);
}

static inline void append(node_t *parent, type_t nodeType) {
  appendn(parent, make(nodeType));
}

static inline void appendi(node_t *parent, type_t nodeType, int intValue) {
  appendn(parent, makei(nodeType, intValue));
}

static inline void appendf(node_t *parent, type_t nodeType, float floatValue) {
  appendn(parent, makef(nodeType, floatValue));
}

static inline void appendc(node_t *parent, type_t nodeType, char charValue) {
  appendn(parent, makec(nodeType, charValue));
}

static inline void appends(node_t *parent, type_t nodeType, char *strValue) {
  appendn(parent, makes(nodeType, strValue));
}

static inline void prepend(node_t *parent, type_t nodeType) {
  prependn(parent, make(nodeType));
}

static inline void remove_child(node_t *parent, node_t *child) {
  tn_remove_child(&(parent)->treeNode, &(child)->treeNode);
}

static inline void replace_child(node_t *parent, node_t *oldChild,
                                 node_t *newChild) {
  tn_replace_child(&(parent)->treeNode, &(oldChild)->treeNode,
                   &(newChild)->treeNode);
}

static inline void insert_before(node_t *parent, node_t *oldChild,
                                 node_t *newChild) {
  tn_insert_before(&(parent)->treeNode, &(oldChild)->treeNode,
                   &(newChild)->treeNode);
}

static inline void insert_after(node_t *parent, node_t *oldChild,
                                node_t *newChild) {
  tn_insert_after(&(parent)->treeNode, &(oldChild)->treeNode,
                  &(newChild)->treeNode);
}

void print_node(node_t *node);
void print_tree(node_t *node, int indent);
