---
title: 语法和语义
---

## 1. 概述

描述语言的难题之一是 **所有人都必须能理解这种描述** 。

- 设计者：描述是否准确清晰
- 实现者：必须确定语言的表达式、语句和程序单元如何构成以及执行的预期效果。
- 使用者：能够查阅语言参考手册，确定如何编写软件解决方案。

类似自然语言，程序语言描述分为 **语法和语义** 。

- 语法：语言的表达式、余具和程序单元的形式。
- 语义：这些表达式、语句和程序单元的意义。

有时候语法不同，但是代表的语义相同。例如 `count++` 和 `count=count+1` 。

语言是句子的集合，而句子是由字符表中的字符组成的串。英语的句子是由 26 个字母组成的串，而程序语言的句子是由 ASCII 字符组成的串。

语言有两种描述模型，分别是

1. 语言识别器：有限自动机、下推机。例如编译器。
2. 语言生成器：正则文法、上下文无关文法。例如形式定义。

## 2. 语法的形式化定义

**上下文无关文法** G 是一个四元组（V,T,P,S），其中

- V：**非终结符** 的非空有穷集，为了定义语言而引入的辅助符号
- T：**终结符** 的非空有穷集，可以出现在语言句子中，而且 $V\bigcap T = \emptyset$ 
- S：$S\in V$ ，为文法 G 的**开始符号** 。（start symbol）
- P：形如 $\alpha \rightarrow \beta$ 的产生式的非空又穷集合，其中 $\alpha\in V,\beta\in(V \bigcup T)^*$ 

---

语法定义需要严格地定义程序的复杂结构。在定义 Algol 60 语言时，Backus 提出了一种形式化的语法定义方式，称为 BNF 范式。

BNF 提供了两类符号：终结符和非终结符

- **终结符**是被定义的**语言的符号**，最终可以出现在程序中
- **非终结符**是为了定义语言的语法而引入的**辅助符号**。

一个非终结符表示语言中的一个语法类，例如

- 用 `<stmt>` 表示语句。
- 用 `<expr>` 表示表达式。

一个语言的 BNF 语法定义由**一组产生式（或规则）** 组成，本质上是一个**上下文无关文法** 。

产生式的形式是：左部→右部

- 左部：总是**一个**非终结符。
- 右部：用 $\mid$ 分隔的一个或多个终结符和非终结符的序列
- → 表示为 “定义为”，$\mid$ 表示 “或者” 。

一个简单的文法如下：

```c
<program> -> <stmts>
<stmts> -> <stmt> | <stmt>;<stmts>  // 用递归扫描列表
<stmt> -> <var> = <expr>
<var> -> a | b | c | d
<expr> -> <term>+<term> | <term>-<term>
<term> -> <var> | const
```

**推导** 是一个或一系列产生式的应用，最后得到一个全是终结符的语言。一个可能的推到如下

```c
<program>                // 所用产生式
=> <stmts>               // <program> -> <stmts>
=> <stmt>                // <stmts> -> <stmt>
=> <var> = <expr>        // <stmt> -> <var>=<expr>
=> a = <expr>            // <var> -> a
=> a = <term>+<term>     // <expr> -> <term>+<term>
=> a = <var>+<term>      // <term> -> <var>
=> a = b+<term>          // <var> -> b
=> a = b+const           // <term> -> const
```

句型：由开始符号经过推导得到的字符串。例如 `a=b+<term>` 

句子：只含有终结符的句型。例如 `a=b+const`

最左推导：每次总是替换句型**最左边**的非终结符。例如

```c
<program> => <stmts> => <stmt> => <var> = <expr>
 => a = <expr> => a = <term> + <term>=> a = <var> + <term>
 => a = b + <term> => a = b + const 
```

最右推导：每次总是替换句型**最右边**的非终结符。

```c
<program> => <stmts> => <stmt> => <var> = <expr>
=> <var> = <term> + <term> => <var> = <term> + const 
=> <var> = <var> + const => <var> = b + const => a = b + const
```

推导过程一直进行到句型不包含非终结符为止，即生成句子为止。

除了最左、最右推导，还有既不是最左也不是最右推导。

推导的顺序对文法生成的语言没有影响，但可以生成语言的不同句子。

穷举所有选择的组合，可以生成完整的语言。

同大多数语言一样，这种语言通常是无限的，无法在有限的时间内生成语言中的所有句子。

```c
<赋值> -> <标识符> = <表达式>
<标识符> -> A | B | C
<表达式> -> <标识符> + <表达式>
           | <标识符> + <表达式>
           | <标识符> * <表达式>
           | (<表达式>)
           | <标识符>
```

- 例子：利用上面的产生式，生成句子 A=B*(A+C) 的最左推导。

  ```
  <赋值> => <标识符> = <表达式>
        => A = <表达式>
        => A = <标识符> * <表达式>
        => A = B * <表达式>
        => A = B * (<表达式>)
        => A = B * (<标识符> + <表达式>)
        => A = B * (A + <表达式>)
        => A = B * (A + <标识符>)
        => A = B * (A + C)
  ```

语法树：推导的分层结构（或树型结构）表示。前面例子中的 `a=b+const` 的语法树如下

![语法树](/programming-language/img/grammarTree.png)

最左推导：最左遍历

最右推导：最右遍历（这两个特点可以用编程实现一下）

- 练习：考虑下列 BNF

  ```c
  <expr> -> <expr> + <expr> | <var>
  <var> -> a | b | c
  ```

  写出 a+b+c 的最左推导、最右推导和推导树，并说明有哪些句型。

  下面我写出最左推导，剩余自己写去

  ```c
  // 最左推导1                                // 最右推导
  <expr>                                    <expr>
  => <expr> + <expr>                        => <expr> + <expr>
  => <expr> + <expr> + <expr>               => <var> + <expr>
  => <var> + <expr> + <expr>                => a + <expr>
  => a + <expr> + <expr>                    => a + <expr> + <expr>
  => a + <var> + <expr>                     => a + <var> + <expr>
  => a + b + <expr>                         => a + b + <expr>
  => a + b + <var>                          => a + b + <var>
  => a + b + c                              => a + b + c
  ```

  以上两个最左推导中每一行的字符串都是句型。

---

二义性：文法生成的句型有两个或多个语法树。

- 例子：利用以下产生式生成句子 `const-const/const`

  ```c
  <expr> -> <expr> <op> <expr> | const 
  <op> -> / | - 
  ```

  可以生成两个语法树，如下

  ![二义性](/programming-language/img/ambiguity.png)

  这个例子说明了非终结符并没有优先级，导致推导过程中可以使用不同的组合。

因此，我们用不同的非终结符表示不同的优先级。利用如下产生式生成的句子 `const-const/const` 就是无二义性的

```c
<expr> -> <expr> - <term> | <term> 
<term> -> <term> / const | const 
```

![无二义性](/programming-language/img/noAmbiguity.png)

除了优先级，结合性也会出现二义性。

```c
二义：<expr> -> <expr> + <expr> | const 
非二义：<expr> -> <expr> + const | const 
```

- 设计一个描述布尔逻辑表达式的无二义性的 BNF（考虑优先级和结合性）

  ```c
  <bExpr> -> <bExpr> or <bTerm> | <bTerm>
  <bTerm> -> <bTerm> and <bFactor> | <bFactor>
  <bFactor>-> not <bFactor> | true | false 
  ```

  可以发现，产生式都是往右展开的，这样就避免了结合性的二义性，然后 or 的优先级最低，接着是 and，最后是 not 。

- 设计一个描述 if 语句的无二义性的 BNF 。

  ```c
  <stmt> -> <matched> | <unmatched>
  <matched> -> if <logic_expr> then <matched> else <matched> | any non-if statement
  <unmatched> -> if <logic_expr> then <stmt> | if <logic_expr> then <matched> else <unmatched>
  ```

  if 语句分为有 else 结尾（matched）和无 else 结尾（unmatched）的形式，分开讨论即可。

---

拓展的 BNF（EBNF）：增加可读性和可写性，并没有增加表达能力。

- 用圆括号表示多个选项

  ```c
  <expr> -> <expr> (+ | -) <term>
  ```

- 用方括号表示可选部分

  ```c
  <if_stmt> -> if <cond> then <stmt> [else <stmt>]
  ```

- 用花括号表示 0 次或多次重复

  ```c
  <ident> -> letter {letter | digit} 
  ```

BNF 和 EBNF 的功能上是一样的，所以它们之间是可以互相转换的。例如下面的 BNF 和 EBNF 都是表达同一套产生式。

```c
// BNF 右结合
<expr> -> <expr> + <term> 
               | <expr> - <term> 
               | <term> 
<term> -> <term> * <factor> 
               | <term> / <factor> 
               | <factor> 
// NBNF
<expr> -> <term> {(+ | -) <term>} 
<term> -> <factor> {(* | /) <factor>}
```

- EBNF 👉 BNF

  ```c
  // EBNF          // BNF
  S -> A{bA}       S -> A | SbA        
  A -> a[b]B       A -> aB | abB
  B -> (c|d)e      B -> ce | de
  ```

- 将之前描述布尔逻辑表达式的 BNF 转换为 EBNF 。

  ```c
  <bExpr> -> [<bExpr> or] <bTerm>  
  <bTerm> -> [<bTerm> and] <bFactor> 
  <bFactor>-> not <bFactor> | true | false
  ```

## 3. 属性文法

有些语言的结构特点很难用 BNF 描述，例如

- 类型兼容性规则：例如在 JAVA 中，浮点数不能赋值给整型变量，而反过来却是可以的（BNF 能做到，但难）
- 变量声明规则：变量必须在使用前先声明（BNF不能）

这类语言规则称为静态语义规则。

- 与程序执行过程中的意义只有间接关系。
- 在编译时进行分析。

Knuth 在 1968 年提出了属性文法。

- 添加静态语义属性的上下文无关文法。
- 既可以描述语法，也可以描述静态语义。

---

例如：形如 `id=id+id` 的赋值语句。

- id 的类型是整数或者浮点数
- `+` 号两边的 id 的类型必须一样
- `=` 左边 id 的类型必须匹配右边表达式的类型
- 右边的表达式的类型必须匹配它的期望类型

而其 BNF 如下，并不能进行类型判断。

```c
<ass_stmt>-><var>=<expr>
<expr> -> <var> + <var> 
<var> -> id 
```

为了满足上述要求，我们需要知道

- 表达式或变量的实际类型
- 表达式的期望类型

这些需要求解的信息称之为语义属性，简称属性。属性文法是具有以下附加特性的文法。

每个文法符号 X 都有一个属性集 A(X) 。

- 综合属性：沿语法树自底向上传递的语义信息。
- 继承属性：沿语法树自顶向下、从左往右传递语义信息。
- 本质属性：叶子节点（即终结符）的综合属性。

每条产生式有一个计算属性的语义函数集。

每条产生式有一个分析静态语义可能为空的谓词函数集。

在前面的例子中，实际类型是与 `<var>` 、`<expr>` 相关的综合属性，以及 id 的本质属性。期望类型是与 `<expr>` 相关的继承属性。

我们完善前面的 BNF 产生式，得到如下

```c
<ass_stmt>-><var>=<expr>
函数：<expr>.expected_type = <var>.actual_type    // 从左往右继承属性
<expr> -> <var>[1] + <var>[2]
函数：<expr>. actual_type = <var>[1].actual_type   // 自底向上综合属性
谓词： <var>[1]. actual_type == <var>[2].actual_type
      <expr>. expected_type == <expr>.actual_type
<var> -> id
函数：<var>.actual_type = id.actual_type          // 自底向上的综合属性
     id.actual_type = lookup(符号表,id)           // 本质属性一开始就要声明
```

关于属性的判断的时候，自己的脑子里面要有一颗文法树。

属性文法的计算就是语法树的装饰过程。

只有综合属性：自底向上

只有继承属性：自顶向下（从左到右）

两者都有：自底向上和自顶向下

个人理解：不论哪种形式遍历得到属性，最后都要校验一次，看看会不会冲突，如果冲突说明代码出现问题了。

- 下面是一个属性文法检测冲突的例子

  ```c
  int A;
  A = 2+3.5
  ```

  首先 `+` 号两边类型冲突，但是整型数可以转为浮点数，接着 `=` 两边出现冲突，由于浮点数不能转换为整型数，检测出语法有误。

  ![属性文法检测](/programming-language/img/attribute.png)

## 4. 动态语义

动态语义：编程语言中表达式、语句和程序单元的意义。目前没有被普遍接受的统一的形式。常用的三种语义描述如下

- 操作语义
- 指称语义
- 公理语义

### 4.1 操作语义

用抽象的方法描述语句的 **执行效果** ，以免语义依赖于语言实现所用的具体计算机。大概步骤如下：

1. 设计一个抽象机及其状态。
2. 定义每一个语句（或抽象机指令）对状态的改变。

人工智能之父 John McCarthy 用 Lisp 语言本身定义了 Lisp 语言的执行过程（语义）。

- 例如 C 语言的 for 结构：

  ```c
  for(expr1;expr2;expr3){
  	...
  }
  ```

  意义（虚拟机指令）：

  ```c
  		expr1;
  loop:	if expr2 == 0 goto out
  		...
  		expr3;
  		goto loop
  out:	...	
  ```

描述机器状态最简单的形式化模型：从合法名字集合到 ”值“ 的映射。

其形式化定义为：$S: Name \rightarrow Value U \lbrace \perp \rbrace$

其中 S 表示一个状态，Name 表示名字的集合，Value 表示 "值" 的集合，$\lbrace\perp\rbrace$ 表示无定义的一些符号。

- 例如当 x 取值 3，y 取值 5，z 取值 0 时的状态为

  ```c
  {x -> 3, y -> 5, z -> 0}
  ```

---

接下来我们用下面的实例语言的 BNF 来描述不同语句的执行效果。

```c
<stmt> -> SKIP
	| <var> = val
	| <stmt>;<stmt>
	| IF <cond> THEN <stmt> ELSE <stmt>
	| WHILE <cond> DO <stmt> END

```

1. SKIP 语句的操作语义：SKIP 语句不做任何操作

   ```c
   SKIP, S => S
   ```

2. `<stmt>, S1 => S2` ：表示在状态 S1 下执行语句 `<stmt>` 后，状态改变为 S2 。 

3. 赋值语句 `<val>=val` 的操作语义：将变量 `<var>` 的值改为 `val` ，其他变量不变。例如 

   ```c
   x=2,{x -> 3, y -> 5, z -> 0} => {x -> 2, y -> 5, z -> 0}
   x=2, {y->1} => {x -> 2, y-> 1}
   ```

如果有多个语句（复合语句），则先执行语句1，再执行语句2

```c
stmt1, S => S1	stmt2, S1 => S2
等价于
stmt1;stmt2, S => S2
```

例如

```c
x=2; z=3, {x->1} => {x->2, z->3}
```

- IF 语句的操作语义：如果条件为真，则执行语句1，否则执行语句2



{{site.math}}