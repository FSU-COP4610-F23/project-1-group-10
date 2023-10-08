# Shell

[Our shell gives the ability to view a user's shell info, execute simple commands,
check if errors exist in commands, and run commands concurrently in the background. ]

## Group Members
- **Roderick Shaw**: rks21b@fsu.edu
- **Fernando Parra**: fap20g@fsu.edu
- **Sofia Sanchez**: srs20h@fsu.edu
## Division of Labor

## BEFORE
### Part 1: Prompt
- **Responsibilities**: [Print the user, machine, and path info from their envirnoment.]
- **Assigned to**: Roderick Shaw

### Part 2: Environment Variables
- **Responsibilities**: [Parse all environment variables appropriately]
- **Assigned to**: Fernando Parra, Roderick Shaw, Sofia Sanchez

### Part 3: Tilde Expansion
- **Responsibilities**: [Parse ~ as a home environment variable]
- **Assigned to**: Fernando Parra, Roderick Shaw, Sofia Sanchez

### Part 4: $PATH Search
- **Responsibilities**: [Find proper paths for each command]
- **Assigned to**: Fernando Parra, Roderick Shaw, Sofia Sanchez

### Part 5: External Command Execution
- **Responsibilities**: [Execute each command by forking them]
- **Assigned to**: Fernando Parra, Roderick Shaw, Sofia Sanchez

### Part 6: I/O Redirection
- **Responsibilities**: [Allow execution functionally with files as input and output]
- **Assigned to**: Fernando Parra, Roderick Shaw, Sofia Sanchez

### Part 7: Piping
- **Responsibilities**: [Allow piping execution]
- **Assigned to**: Fernando Parra, Roderick Shaw, Sofia Sanchez

### Part 8: Background Processing
- **Responsibilities**: [Let commands run concurrently by executing commands w/ '&' in the background]
- **Assigned to**: Fernando Parra, Roderick Shaw, Sofia Sanchez

### Part 9: Internal Command Execution
- **Responsibilities**: [Implement 3 commands: exit, cd PATH, and jobs]
- **Assigned to**: Fernando Parra, Roderick Shaw, Sofia Sanchez

### Part 10: External Timeout Executable
- **Responsibilities**: [Implement mytimeout command]
- **Assigned to**: Fernando Parra, Roderick Shaw, Sofia Sanchez

### Extra Credit
- **Responsibilities**: [Optional code if we have time to implement them]
- **Assigned to**: Nobody

## AFTER
### Part 1: Prompt
- **Responsibilities**: [Print the user, machine, and path info from their envirnoment.]
- **Assigned to**: Roderick Shaw

### Part 2: Environment Variables
- **Responsibilities**: [Parse all environment variables appropriately]
- **Assigned to**: Fernando Parra

### Part 3: Tilde Expansion
- **Responsibilities**: [Parse ~ as a home environment variable]
- **Assigned to**: Fernando Parra, Roderick Shaw

### Part 4: $PATH Search
- **Responsibilities**: [Find proper paths for each command]
- **Assigned to**: Sofia Sanchez

### Part 5: External Command Execution
- **Responsibilities**: [Execute each command by forking them]
- **Assigned to**: Roderick Shaw

### Part 6: I/O Redirection
- **Responsibilities**: [Allow execution functionally with files as input and output]
- **Assigned to**: Sofia Sanchez

### Part 7: Piping
- **Responsibilities**: [Allow piping execution]
- **Assigned to**: Fernando Parra

### Part 8: Background Processing
- **Responsibilities**: [Let commands run concurrently by executing commands w/ '&' in the background]
- **Assigned to**: Roderick Shaw

### Part 9: Internal Command Execution
- **Responsibilities**: [Implement 3 commands: exit, cd PATH, and jobs]
- **Assigned to**: 

### Part 10: External Timeout Executable
- **Responsibilities**: [Implement mytimeout command]
- **Assigned to**: Sofia Sanchez

### Extra Credit
- **Responsibilities**: [Optional code if we have time to implement them]
- **Assigned to**: Fernando Parra

## File Listing
```
shell/
│
├── src/
│ |── shell.c
│ |── bgProcessor.c
│ |── envirVar.c
│ |── mytimeout.c
│ |── ioRedirection.c
│ |── lexer.c
│ |── listOfCommands.c
│ |── pathSearch.c
│ |── piping.c
│ └── prompt.c
│
├── include/
│ |── lexer.h
| └── bgProcessor.h
│
├── README.md
└── Makefile
```
## How to Compile & Execute

### Requirements
- **Compiler**: gcc -std=c99 nameOfFile -o whatYouWantTheExecutableToBeNamed
- **Dependencies**: None needed to be downloaded

### Compilation
```bash
make
```
This will build the executable alongside the makefile and other folders 
in this directory. It will also create the bin and obj folders in the same
directory.
### Execution
```bash
make run
```
This will run the program and start off with a slightly different prompt than
before. 
## Bugs
- **Bug 1**: Printing out the command line gives a segmentation fault.
- **Bug 2**: Piping doesn't seem to be going into the background.
- **Bug 3**: Visual prompt bug with cd. It works, but it doesn't reflect that in prompt.

## Extra Credit
- **Extra Credit 1**: [Support unlimited number of pipes]

## Considerations
[Tilde expansion, external command execution, and internal commands are 
inside shell.c because they were too small for another file.]
