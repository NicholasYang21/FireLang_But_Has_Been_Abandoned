## Syntax Doc

This file contains all the syntax of FireLang.

#### 1.primary

---

status: `building`

Description:
    
> The basic unit in FireLang.
>
> It can be defined like:

```
primary ::= INTEGER 
            | FLOAT
            | BOOLEAN
            | CHAR
            | STRING
            | "(" expression ")"
            ;
```