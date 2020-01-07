// _fucntions
void codegen(struct node *tree);
void cg_handle_token(struct node *tree);
void cg_handle_children(struct node *tree);
char *freshLabel();
void cg_if_else(struct node *tree);