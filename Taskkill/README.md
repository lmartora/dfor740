## Taskkill
Terminates a specific process via PID or image name. 

### Usage
To use taskkill, download ```midTaskkill.exe``` from this repository and run it on a Windows Command Prompt.

### Syntax
```midTaskkill.exe [/pid <pid> | /im <image name>] [/f] [/t]```

### Switches
- ```/pid <pid>```: Specify Process ID of process to terminate.
- ```/im <image name>```: Specify image name of process to terminate.
- ```/f```: Forces the process to be terminated.
- ```/t```: Ends the process and all of its children.
