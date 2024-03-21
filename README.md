# dfor 740
Lorenzo Martorana's Repository for GMU DFOR 740. Contains executable binaries and source code to them.

## Taskkill
Terminates a specific process via PID or image name. 

### Usage
To use taskkill, download ```midTaskkill.exe``` and run it on a Windows Command Prompt.

### Syntax
```midTaskkill.exe [/pid <pid> | /im <image name>] [/f] [/t]```

### Switches
- ```/pid <pid>```: Specify Process ID of process to terminate.
- ```/im <image name>```: Specify image name of process to terminate.
- ```/f```: Forces the process to be terminated.
- ```/t```: Ends the process and all of its children.

## Tasklist
Displays a list of processes that are running on the computer and many specific details of them.

### Usage
To use tasklist, download ```midTasklist.exe``` and run it on a Windows Command Prompt.

### Syntax
```midTasklist.exe [/v] [/svc]```

### Switches
- ```/v```: Verbose information about the processes. Shows: status, spawner username, CPU time, and window title.
- ```/svc```: Lists all services for each process without truncation.
