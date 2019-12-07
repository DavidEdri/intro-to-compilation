function int foo() { 
var int x;
var int* y;
var char* x;
var char z;
string y[10];
x = 5;
y = &x;
x = 6;
y = "foobar";
x = &y[5]; /* x points to 'r' */
z = *(x - 5); /* z is 'f' */
y = "barfoo"; /* z is still 'f', but x now points to 'o' */
 }

