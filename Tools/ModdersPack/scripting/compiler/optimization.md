Optimization
------------

The execution speed of scripts is not typically important in an unmodded game, given the difference in performance between a modern computer and what Fallout was designed for. When you start adding mods to the mix there's the potential for problems again, since sfall's global script system means that you can have a large amount of scripts being run every single frame.

---
### sslc -O option

The sfall build of sslc supports a `-O` command line option to perform an optimization pass over the generated code. This isn't a magic make-my-code-go-faster bullet; most of what it does is very limited in scope. It's primary purpose was to strip out the procedures and variables which get automatically pulled into every script that includes **define.h**, whether you use them or not, and to do something about the additional variables that get created by `foreach` loops.

**There are several levels of optimization available:**
- `-O1` - Basic, only removes unreferenced globals variables and procedures, code itself remains untouched.
- `-O2` - Full, most code optimizations are on, but only those that were tested on complex scripts.
- `-O3` - Experimental, provides most efficiency, but tend to break some complex code due to bugs.

**The following optimizations are performed:**

- constant expression folding: if an expression depends only on values which are known at compile time, then the expression is replaced by its result.
  ```
  a := 2 + 2;  -> a := 4;
  ```

- constant variable initialization: All variables are initialised to some value, ('0', if you don't specify anything else,) so sslc attempts to make use of that fact to remove the first assignment to a variable if the first assignment is a constant expression.
  ```
  variable a;  -> variable a := 4;
  a := 4;      ->
  ```

- constant propagation: checks for values assigned to variables which can be computed at compile time, and replaces relevant references to the symbol by the constant. The original store is not removed by this optimization. Global variables are considered for this optimization only if they are not marked import or export, and are not assigned to anywhere in the script.
  ```
  a := 4;  -> a := 4;
  foo(a);  -> foo(4);
  ```

- dead code removal: Checks for and removes code which cannot be reached, either because it is hidden behind a return or because the argument to an if statement can be computed at compile time.
  ```
  if (True) then begin   -> display_msg("foo");
    display_msg("foo");  ->
  end else begin         ->
    display_msg("bar");  ->
  end                    ->
  ```

- unreferenced variable elimination: Checks for variables which are never referenced, and removes them. Also applies to global variables, as long as they are not marked for export.
  ```
  variable i, j, k;  -> variable i;
  i := 1;            -> i := 1;
  return;            -> return;
  ```

- unreferenced procedure elimination: Checks for any procedures which are never called, and removes them.
  ```
  procedure foo begin return "foo"; end   -> procedure foo begin return "foo"; end
  procedure bar begin return "bar"; end   -> procedure start begin
  procedure start begin                   ->   display_msg(foo);
    display_msg(foo);                     -> end
  end                                     ->
  ```

- dead store removal: Removes variable assignments if the result of the variable is unused, and if the expression used to compute the value of the variable is provably free of side effects. (See `pure` keyword)
  ```
  a := "moo";       -> a := "foo";
  a := "foo";       -> display_msg(a);
  display_msg(a);   ->
  a := "bar";       ->
  ```

- store combination: Where there are two stores in a row to the same variable, the two expressions are combined.
  ```
  var1 := var2; -> var1 := var2 + var3;
  var1 += var3; ->
  ```

- variable combination: Where usage regions of variables do not overlap, combine the variables to provide additional candidates for unreferenced variable elimination. Very useful for scripts containing multiple `foreach` loops, which generate 2 or 3 hidden variables each.
  ```
  a := "foo";       -> a := "foo";
  display_msg(a);   -> display_msg(a);
  b := "bar";       -> a := "bar";
  display_msg(b);   -> display_msg(a);
  ```

- namelist compression: Fallout stores the names of all file scope variables and procedures in a namelist which is saved into the .int. Any of these that are unreferenced can be removed, and the names of global variables can be modified to make them shorter.

---
### Writing your own code

- Don't have global scripts running any more often that you need them to. Not everything needs to be run every single frame.

- Never concat constant strings with the `+` operator, as it forces the operation to be done at runtime. The compiler can cope with constant strings being placed next to each other without the need for a `+`, which results in far more efficient code as the combination is done at lex time.
  ```
  #define GLOB_PREFIX "ts__"                    -> #define GLOB_PREFIX "ts__"

  procedure start begin                         -> procedure start begin
    set_sfall_global(GLOB_PREFIX + "foo1", 0);  ->   set_sfall_global(GLOB_PREFIX "foo1", 0);
  end                                           -> end
  ```

- Avoid function calls in `while` loops. Function calls are expensive in comparison to variable lookups, so it's more efficient to move the function call out of the loop and store the result in a variable.
  ```
  while i < len_array(array) do begin           -> tmp := len_array(array);
    ...                                         -> while i < tmp do begin
  end                                           ->   ...
                                                -> end
  ```

- Mark functions with `pure` or `inline` where relevant.

  * `pure` is a hint to the optimizer that a procedure has no side effects. (i.e. there's no way to tell that it's been called aside from its return value.) Pure procedures cannot modify global variables, or call any other procedure that isn't itself pure. Functions marked with pure can only be used in expressions (i.e. you cannot use the `call <procedure>` syntax to call them.) If there are non-pure terms in an expression, it prevents that expression being considered for dead store removal. Where no such optimizations can be performed, or if optimization is disabled, marking a procedure with pure will have no effect on the compiled code.

  * `inline` is an instruction to the compiler to replace calls to the marked procedure with a copy of the procedures code instead of having a separate call. Inlined procedures cannot use the `return` command, cannot be predefined, and cannot be used as part of an expression. Inlining if a procedure is only going to be called once is always a win, but if there are multiple calls to a procedure you will end up bloating the size of the generated code.
