// _fucntions
void codegen(struct node *tree);
void cg_handle_token(struct node *tree);
void cg_handle_children(struct node *tree);
char *freshLabel();
struct node* get_first_code(struct node *tree);

//assignment
void cg_assignment(struct node *tree);

// bool_exp
void cg_bool_exp(struct node *tree);
int is_relop(char *t);
void handle_relop(struct node *tree);

// if_else
void cg_if_else(struct node *tree);
