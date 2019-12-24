ASK:

---

# TODO :

---

## part 2 docs-

### arg:

store a function argument  
fields: `char *id; int type;`  
functions:  
`arg *new_arg(char* id, int type)` - create and return new arg  
`void print_arg(arg* a)` - prints an arg

### var:

store variable information  
fiedls: `char *id; char *val; int type;`  
functions:  
`var *new_var(char *id, int type, char *val)` - create and return new var  
`void print_var(var *v)` - prints var

### func:

store function information  
fields: `char *id; struct arg **args; int args_count; int type;`  
functions:  
`func *new_func(char *id, int type)` - create and return new func  
`void print_func(struct func *f)` - prints a func

### sym_el:

stores either a function or var  
fields:`int type;(var or func) struct func *f; struct var *v;`  
functions:  
`sym_el *new_sym_el()` - return empty sym_el  
`void se_add_var(struct sym_el *s, struct var *v)` - adds var to an excisting sym_el  
`void se_add_func(struct sym_el *s, struct func *f)` - adds func to an excisting sym_el  
`char *se_get_id(struct sym_el *se)` - returns the id of an sym_el  
`int se_get_type(struct sym_el *se)` - return the type of an sym_el  
`int se_cmp_id(struct sym_el *se, char *id)` - compares the id  
`void print_se(struct sym_el *se)` - prints the sym_el

### sym_table:

linked list to store sym_el  
fields: `struct sym_el *data; struct sym_table *next;`  
functions:  
`sym_table *new_sym_table()` - create an empty sym_table  
`int st_is_declared(struct sym_table *st, char *id)` - checks if id declared in sym_table  
`void st_add_item(struct sym_table *st, struct sym_el *data)` - adds an item to sym_table  
`sym_el *st_find(struct sym_table *st, char *id)` - finds an sym_el in table (NULL if donst exist)  
`void print_st(struct sym_table *st)` - prints an sym_table

### code_stack:

stack that stores sym_tables  
fields: `struct sym_table *top; struct code_stack *next;`  
functions:  
`code_stack *new_code_stack()` - create an empty code_stack  
`void cs_push(struct code_stack* cs, struct sym_table *item)` - pushes sym_table to stack  
`sym_table *cs_pop(struct code_stack* cs)` - popping sym_table from stack  
`sym_el *cs_find(struct code_stack *cs, char *id)` - finds and returns sym_el in stack  
`void print_cs(struct code_stack* cs)` - prints code_stack
