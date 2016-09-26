/* Pointer struct defines */
enum relational_operator
{
  LESS,
  LESS_OR_EQUAL,
  GREATER,
  GREATER_OR_EQUAL
};

enum equality_operator
{
  EQUAL,
  NOT_EQUAL
};

struct symbol_node
{
  char * name;

  double value;

  struct ast_node * function;
};

struct ast_node // for binary/unary operators and expression lists
{
  int node_type;

  struct ast_node * left;

  struct ast_node * right;
};

struct ast_relational_node // for relational operators
{
  int node_type;

  enum relational_operator operator;

  struct ast_node * left;

  struct ast_node * right;
};

struct ast_equality_node // for equality operators
{
  int node_type;

  enum equality_operator operator;

  struct ast_node * left;

  struct ast_node * right;
};

struct ast_function_node // for function calls
{
  int node_type;

  struct ast_node * arguments;

  struct symbol_node * symbol;
};

struct ast_symbol_reference_node // for symbol references
{
  int node_type;

  struct symbol_node * symbol;
};

struct ast_if_node // for "if/else" statements
{
  int node_type;

  struct ast_node * condition;

  struct ast_node * if_branch;

  struct ast_node * else_branch;
};

struct ast_while_node // for "while" statements
{
  int node_type;

  struct ast_node * condition;

  struct ast_node * while_branch;
};

struct ast_assignment_node // for assignment expressions
{
  int node_type;

  struct symbol_node * symbol;

  struct ast_node * value;
};

struct ast_number_node // for constant floating-point numbers
{
  int node_type;

  double value;
};
//--------------------------------------------------------

truct ast_node *
new_ast_node (int node_type,
              struct ast_node * left,
              struct ast_node * right)
{
  struct ast_node * ast_node =
    emalloc (sizeof (struct ast_node));

  ast_node->node_type = node_type;

  ast_node->left = left;
  ast_node->right = right;

  return ast_node;
}

struct ast_node *
new_ast_relational_node (enum relational_operator operator,
                         struct ast_node * left,
                         struct ast_node * right)
{
  struct ast_relational_node * ast_node =
    emalloc (sizeof (struct ast_relational_node));

  ast_node->node_type = 'R';

  ast_node->operator = operator;
  ast_node->left = left;
  ast_node->right = right;

  return (struct ast_node *) ast_node;
}

struct ast_node *
new_ast_equality_node (enum equality_operator operator,
                       struct ast_node * left,
                       struct ast_node * right)
{
  struct ast_equality_node * ast_node =
    emalloc (sizeof (struct ast_equality_node));

  ast_node->node_type = 'E';

  ast_node->operator = operator;
  ast_node->left = left;
  ast_node->right = right;

  return (struct ast_node *) ast_node;
}

struct ast_node *
new_ast_function_node (struct symbol_node * symbol,
                       struct ast_node * arguments)
{
  struct ast_function_node * ast_node =
    emalloc (sizeof (struct ast_function_node));

  ast_node->node_type = 'F';

  ast_node->arguments = arguments;
  ast_node->symbol = symbol;

  return (struct ast_node *) ast_node;
}

struct ast_node *
new_ast_symbol_reference_node (struct symbol_node * symbol)
{
  struct ast_symbol_reference_node * ast_node =
    emalloc (sizeof (struct ast_symbol_reference_node));

  ast_node->node_type = 'S';

  ast_node->symbol = symbol;

  return (struct ast_node *) ast_node;
}

struct ast_node *
new_ast_if_node (struct ast_node * condition,
                 struct ast_node * if_branch,
                 struct ast_node * else_branch)
{
  struct ast_if_node * ast_node =
    emalloc (sizeof (struct ast_if_node));

  ast_node->node_type = 'I';

  ast_node->condition = condition;
  ast_node->if_branch = if_branch;
  ast_node->else_branch = else_branch;

  return (struct ast_node *) ast_node;
}

struct ast_node *
new_ast_while_node (struct ast_node * condition,
                    struct ast_node * while_branch)
{
  struct ast_while_node * ast_node =
    emalloc (sizeof (struct ast_while_node));

  ast_node->node_type = 'W';

  ast_node->condition = condition;
  ast_node->while_branch = while_branch;

  return (struct ast_node *) ast_node;
}

struct ast_node *
new_ast_assignment_node (struct symbol_node * symbol,
                         struct ast_node * value)
{
  struct ast_assignment_node * ast_node =
    emalloc (sizeof (struct ast_assignment_node));

  ast_node->node_type = 'A';

  ast_node->symbol = symbol;
  ast_node->value = value;

  return (struct ast_node *) ast_node;
}

struct ast_node *
new_ast_number_node (double value)
{
  struct ast_number_node * ast_node =
    emalloc (sizeof (struct ast_number_node));

  ast_node->node_type = 'N';

  ast_node->value = value;

  return (struct ast_node *) ast_node;
}
