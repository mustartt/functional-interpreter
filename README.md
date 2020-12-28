# functional-interpreter
A Functional Programming Language Interpreter written in C. The fundamental idea of this language is to express the functional nature of the language, as every function is a `λ` expression bound to an identifier. The language is non-typed and borrow many features of the Scheme Language, such as `list`, `cons`, `car`, `cdr`, and `struct`.

## Variables Declaration
A basic variable declaration is as follows
```
bind <identifier> (to, as, =, ->) <value>
```
the `bind` keyword binds the variable's `identifier` to the `value` using any of the keyword `to`, `as`, `=`, and `->`.
#### Examples
There are 5 basic literal types in the language: `num`, `float`, `string`, `list`, and `struct`.
``` python
bind pi to 3.1415926
bind num to 1234
bind str to "Hello World!"
bind array to list(1 2 3 4 5 6 7 8 9)
```

## The `λ` Expression
The language implement most syntax and features from Lambda Calculus. A `λ` expression in Lambda Calculus, such as the identity function
`λx. x` can be defined as 
``` python
lambda({x} -> x)
```
a `λ` expression with mulitple parameters `x` and `y` and a function `f`, in lambda calculus `λxy. f(x y)` can be expressed as
``` python
lambda({x y} -> f(x y))
```
#### Examples
In the language, `[]` and `{}` are interchangable for better readability in `λ` expressions.
``` python
> (lambda({x} -> x))(5)
5
> (lambda({x y} -> +(x y)))(2 3)
5
> {(lambda({x} -> lambda({y} -> +(x y))))(1)}(4)
5
```

## Function Declaration
In the language, all functions are a `function_identifier` bound to a `λ` expression using the `bind` keyword as follows.
```
bind <function_identifier> (as, to, =, ->) lambda({p_1 ... p_n} -> expression);
```
The `function_identifier` can contain all alphanumeric characters except keywords. All expressions should be postfixed by a `;` to indicate a seperate line.
#### Examples
``` python
# add(x y) calculates the sum of two numbers
# add: Num Num -> Num
bind add to lambda({x y} -> +(x y));

# !(n) calculates the n!
# !: Nat -> Nat
bind ! to lambda({x} -> 
	if(<=(x 1) 
		1 
		*(x !(-(x 1)))));  
```
and these functions can be called as
``` python
> add(2 3)
3
> !(5)
120
```

## Logical Operator and Decisions
Comparison in the language can produce boolean values `True` and `False`, and decisions can be performed based on the result using the `if` function. 
``` python
if(condition exp_true exp_false)
```
Use `and`, `or`, and `not` to combined logical result.

## Scope and Local Definition
### Local Definitions
One can define local definitions and mulitple subsequent routine in functions using the keyword `local` or `begin`. The syntax is
``` python
begin((def_1) ... (def_n) exp)
```
Note: In the global scope, the keyword `bind` does not require `()` around it, but in local definitions, `()` are required around binding to ensure that they are seperate parameters.

### Language Scope
The language implements scope, which means that local definitions can share same names as definitions from outer scopes. 
#### Example










