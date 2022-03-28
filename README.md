# Assembler-OpenU

The final assignment (#14) of the C course (20465) at The Open University.

Semester 2022 A

Done by Ben Avrahami and Gal Ben Artzi


# Getting Started 
The project was coded and compiled using Ubuntu, but it may run on all Linux versions.

## Usage

Use makefile to compile the project like this:

>   make

After preparing assembly files with an .as extension, open terminal and pass file names as arguments (without the file extensions) as following:

As for the files x.as, y.as, hello.as we will run:

>   assembler x y hello

The assembler will generate output files with the same filenames and the following extensions:

- `.ob` - Object file

- `.ent` - Entries file

- `.ext` - Externals file

An example of input and output files can be found under the 'tests' folder.


## Directives
A **_directive_** line of the following structure:

1. An **optional** preceding *label*. e.g. `PLACE1: `.
2. A _directive_: `.data`, `.string`, `.struct`, `.entry` or `.extern`.
3. Operands according to the type of the *directive*.

   ### `.data`
   This directive allocates memory in the data image for storing received integers later in memory (also increases the _data counter_ and updates the _symbol table_).
   The parameters of `.data` are valid integers (separated by a comma).  
   e.g. `LABEL1: .data      +7, -56, 4, 9`.
   
   ### `.string`
   This direcive receives a string as an operand and stores it in the data image. It stores all characters by their order in the string, encoded ny their *ASCII* values.
   e.g. `STRING1: .string "abcdef"` is a valid directive.

   ### `.entry`
   This directive outputs a received name of a *label* to the *symbol table*, so that later it will be recognized by other assembly files (and they would be able to use it).
   e.g. 
   ```
   ; file1.as
   .entry HELLO
   HELLO: add #1, r1 
   ```
   ### `.extern`
   This directive receives a name of a *label* as a parameter and declares the *label* as being external (defined in another file) and that the current file shall use it.  
   This way, the directive `.extern HELLO` in `file2.as` will match the `.entry` directive in the previous example.

