// arg
arg *new_arg(char* id, int type);
arg_arr *new_arg_arr();
void arg_arr_add(struct arg_arr *a, struct arg *data);
int arg_is_declared(struct arg_arr *a, char *id);
void add_arg_to_f(struct func *f, struct node *tree, int type);
void ast_to_args(struct func *f, struct node *tree);
void print_arg(arg* a);
void print_arg_arr(arg_arr* a);
void args_to_st(struct func *f);
int num_of_args(struct arg_arr* args);
int count_tree_args(struct node* tree);
//int check_args_types(struct node* tree,arg_arr*);



// func
func *new_func(char *id, int type);
int func_type_to_int(char *type);
void ast_to_func(struct node* tree);
void print_func(struct func *f);
void validate_return(struct node *tree, struct func *f);
void validate_rets(struct node *tree, struct func *f);
void validate_ret(struct node *tree, struct func *f);

// var
var *new_var(char *id, int type, char *val);
void print_var(var *v);
int var_type_to_int(char *type);
void validate_var_decleration(struct node * tree, int type);

// sym_el
sym_el *new_sym_el();
void se_add_var(struct sym_el *s, struct var *v);
void se_add_func(struct sym_el *s, struct func *f);
char *se_get_id(struct sym_el *se);
int se_get_type(struct sym_el *se);
int se_cmp_id(struct sym_el *se, char *id);
void print_se(struct sym_el *se);

// sym_table
sym_table *new_sym_table();
int st_is_declared(struct sym_table *st, char *id);
void st_add_item(struct sym_table *st, struct sym_el *data);
sym_el *st_find(struct sym_table *st, char *id);
void print_st(struct sym_table *st);

// code_stack
code_stack *new_code_stack();
void cs_push(struct code_stack* cs, struct sym_table *item);
sym_table *cs_pop(struct code_stack* cs);
sym_el *cs_find(struct code_stack *cs, char *id);
void print_cs(struct code_stack* cs);

// _functions
void handle_children(struct node* tree);
void handle_code_block(struct node* tree, struct func *f);
void handle_token(struct node* tree);
struct node *tree_find(struct node *tree, char *id);
void validate_func_call(struct node* tree);

