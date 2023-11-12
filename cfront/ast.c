#include "ast.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

tnode_t *tn_append_child(tnode_t *parent, tnode_t *child) {
  assert(parent != NULL);
  if (parent->firstChild == NULL) {
    parent->firstChild = child;
    parent->lastChild = child;
    child->parent = parent;
  } else {
    parent->lastChild->nextSibling = child;
    child->prevSibling = parent->lastChild;
    parent->lastChild = child;
    child->parent = parent;
  }
  return parent;
}

tnode_t *tn_prepend_child(tnode_t *parent, tnode_t *child) {
  if (parent->firstChild == NULL) {
    parent->firstChild = child;
    parent->lastChild = child;
    child->parent = parent;
  } else {
    parent->firstChild->prevSibling = child;
    child->nextSibling = parent->firstChild;
    parent->firstChild = child;
    child->parent = parent;
  }
  return parent;
}

tnode_t *tn_remove_child(tnode_t *parent, tnode_t *child) {
  if (parent->firstChild == child) {
    parent->firstChild = child->nextSibling;
  }
  if (parent->lastChild == child) {
    parent->lastChild = child->prevSibling;
  }
  if (child->prevSibling != NULL) {
    child->prevSibling->nextSibling = child->nextSibling;
  }
  if (child->nextSibling != NULL) {
    child->nextSibling->prevSibling = child->prevSibling;
  }
  child->parent = NULL;
  child->prevSibling = NULL;
  child->nextSibling = NULL;
  return parent;
}

tnode_t *tn_replace_child(tnode_t *parent, tnode_t *oldChild,
                           tnode_t *newChild) {
  if (parent->firstChild == oldChild) {
    parent->firstChild = newChild;
  }
  if (parent->lastChild == oldChild) {
    parent->lastChild = newChild;
  }
  if (oldChild->prevSibling != NULL) {
    oldChild->prevSibling->nextSibling = newChild;
  }
  if (oldChild->nextSibling != NULL) {
    oldChild->nextSibling->prevSibling = newChild;
  }
  newChild->parent = parent;
  newChild->prevSibling = oldChild->prevSibling;
  newChild->nextSibling = oldChild->nextSibling;
  oldChild->parent = NULL;
  oldChild->prevSibling = NULL;
  oldChild->nextSibling = NULL;
  return parent;
}

tnode_t *tn_insert_before(tnode_t *parent, tnode_t *oldChild,
                           tnode_t *newChild) {
  if (parent->firstChild == oldChild) {
    parent->firstChild = newChild;
  }
  if (oldChild->prevSibling != NULL) {
    oldChild->prevSibling->nextSibling = newChild;
  }
  newChild->parent = parent;
  newChild->prevSibling = oldChild->prevSibling;
  newChild->nextSibling = oldChild;
  oldChild->prevSibling = newChild;
  return parent;
}

tnode_t *tn_insert_after(tnode_t *parent, tnode_t *oldChild,
                          tnode_t *newChild) {
  if (parent->lastChild == oldChild) {
    parent->lastChild = newChild;
  }
  if (oldChild->nextSibling != NULL) {
    oldChild->nextSibling->prevSibling = newChild;
  }
  newChild->parent = parent;
  newChild->prevSibling = oldChild;
  newChild->nextSibling = oldChild->nextSibling;
  oldChild->nextSibling = newChild;
  return parent;
}

node_t *make(type_t nodeType) {
  node_t *node = (node_t *)malloc(sizeof(node_t));
  node->nodeType = nodeType;
  TN_INIT(&node->treeNode);
  return node;
}

void print_node(node_t *node) {
  assert(node != NULL);
  switch (node->nodeType) {
  case N_LITERAL_INT: printf("INT: %d\n", node->intValue); break;
  case N_LITERAL_FLOAT: printf("FLOAT: %f\n", node->floatValue); break;
  case N_LITERAL_CHAR: printf("CHAR: %c\n", node->charValue); break;
  case N_LITERAL_STRING: printf("STRING: %s\n", node->strValue); break;
  case N_LITERAL_BOOL: printf("BOOL: %s\n", node->intValue ? "true" : "false"); break;
  case N_LITERAL_NULL: printf("NULL\n"); break;
  case N_IDENTIFIER: printf("ID: %s\n", node->strValue); break;
  case N_SEMICOLON: printf("SEMICOLON\n"); break;
  case N_COMMA: printf("COMMA\n"); break;
  case N_EQUAL: printf("EQUAL\n"); break;
  case N_PLUS: printf("PLUS\n"); break;
  case N_MINUS: printf("MINUS\n"); break;
  case N_ASTERISK: printf("ASTERISK\n"); break;
  case N_SLASH: printf("SLASH\n"); break;
  case N_DOUBLE_AMPERSAND: printf("DOUBLE_AMPERSAND\n"); break;
  case N_DOUBLE_VERTICAL: printf("DOUBLE_VERTICAL\n"); break;
  case N_DOT: printf("DOT\n"); break;
  case N_EXCLAMATION: printf("EXCLAMATION\n"); break;
  case N_TYPE: printf("TYPE: %s\n", node->strValue); break;
  case N_LPAREN: printf("LPAREN\n"); break;
  case N_RPAREN: printf("RPAREN\n"); break;
  case N_LBRACKET: printf("LBRACKET\n"); break;
  case N_RBRACKET: printf("RBRACKET\n"); break;
  case N_LBRACE: printf("LBRACE\n"); break;
  case N_RBRACE: printf("RBRACE\n"); break;
  case N_STRUCT: printf("STRUCT\n"); break;
  case N_RETURN: printf("RETURN\n"); break;
  case N_IF: printf("IF\n"); break;
  case N_ELSE: printf("ELSE\n"); break;
  case N_WHILE: printf("WHILE\n"); break;
  case N_DO: printf("DO\n"); break;
  case N_FOR: printf("FOR\n"); break;
  case N_BREAK: printf("BREAK\n"); break;
  case N_CONTINUE: printf("CONTINUE\n"); break;
  case N_SWITCH: printf("SWITCH\n"); break;
  case N_CASE: printf("CASE\n"); break;
  case N_DEFAULT: printf("DEFAULT\n"); break;

  case N_PROGRAM: printf("Program\n"); break;
  case N_EXTDEFLIST: printf("ExtDefList\n"); break;
  case N_EXTDEF: printf("ExtDef\n"); break;
  case N_EXTDECLIST: printf("ExtDecList\n"); break;
  case N_TYPESPEC: printf("Specifier\n"); break;
  case N_STRUCT_SPECIFIER: printf("StructSpecifier\n"); break;
  case N_OPTTAG: printf("OptTag\n"); break;
  case N_TAG: printf("Tag\n"); break;
  case N_VARDECL: printf("VarDec\n"); break;
  case N_FUNCDECL: printf("FunDec\n"); break;
  case N_PARAMLIST: printf("VarList\n"); break;
  case N_PARAMDEC: printf("ParamDec\n"); break;
  case N_COMPSTMT: printf("CompSt\n"); break;
  case N_STMTLIST: printf("StmtList\n"); break;
  case N_STMT: printf("Stmt\n"); break;
  case N_DEFLIST: printf("DefList\n"); break;
  case N_DEF: printf("Def\n"); break;
  case N_DECLIST: printf("DecList\n"); break;
  case N_DEC: printf("Dec\n"); break;
  case N_EXP: printf("Exp\n"); break;
  case N_RELOP: printf("RELOP\n"); break;
  case N_ARGS: printf("Args\n"); break;

  case N_ERROR: printf("ERROR\n"); break;
  default:
    printf("Unknown node type: %d\n", node->nodeType);
    break;
  }
}

void print_tree(node_t *node, int indent) {
  assert(node != NULL);
  for (int i = 0; i < indent; i++) {
    printf("| ");
  }
  print_node(node);
  tnode_t *child;
  TN_FOR_EACH_CHILD(child, &node->treeNode) {
    node_t *childNode = TN2NODE(child);
    print_tree(childNode, indent + 1);
  }
}