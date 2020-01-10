// _fucntions
void codegen(struct node *tree);
void cg_handle_token(struct node *tree);
void cg_handle_children(struct node *tree);
char *freshLabel();
char *freshVar();
int should_cpy_code(char *token);
void cpy_code(struct node *parent);
void write_code(struct node *tree);
int is_var(char *token);

// assignment
void cg_assignment(struct node *tree);
void check_for_assignment(struct node *tree);
void check_for_str_assignment(struct node *tree);

// condition
void cg_condition(struct node *tree);
void condition_lables(struct node *tree);
void cg_relop(struct node *tree);

// expression
void cg_expression(struct node *tree);
void cg_strchar(struct node *tree);
void cg_bool_exp(struct node *tree);
int is_relop(char *t);
void if_temp_needed(struct node *tree);
void cg_relop2(struct node *tree);

// function
void cg_function(struct node *tree);
void cg_main(struct node *tree);
void cg_return(struct node *tree);

// if_else
void cg_if_else(struct node *tree);
void labels_if_else(struct node *tree);
void gencode_if_else(struct node *tree);
void cg_if(struct node *tree);
void labels_if(struct node *tree);
void gencode_if(struct node *tree);
void if_else_2(struct node *tree);

// loops
void cg_while(struct node *tree);
void cg_for(struct node *tree);
void cg_do_while(struct node *tree);