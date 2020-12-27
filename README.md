# functional-interpreter
A Functional Programming Language Interpreter written in C. The fundamental idea of this language is to express the functional nature of the language, as every function is a `λ` expression bound to an identifier. The language is non-typed and borrow many features of the Scheme Language, such as `list`, `cons`, `car`, `cdr`, and `struct`.

### Variables Declaration
A basic variable declaration is as follows
```
bind <identifier> (to, as, =, ->) <value>
```
the `bind` keyword binds the variable's `identifier` to the `value` using any of the keyword `to`, `as`, `=`, and `->`.
##### Examples
There are 5 basic literal types in the language: `num`, `float`, `string`, `list`, and `struct`.
```
bind pi to 3.1415926
bind num to 1234
bind str to "Hello World!"
bind array to list(1 2 3 4 5 6 7 8 9)
```
