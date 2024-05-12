## ssh
A fake SSH emulator which when used, appears as if a user is connected to a real SSH session with a fake filesystem. Logs the username, IP, port, and password that the user uses to connect, as well as the commands they use.

### Usage
To use ssh, you will need to compile the Visual Studio Project Solution file. 
1. Begin by opening Microsoft Visual Studio 2019 or later, choose the ```Clone a repository```, paste in this repository's link, and select clone.
2. From the ```Build``` tab at the top, select the first option ```Build Solution```.
3. From the ```Git``` tab at the top, select ```Open in File Explorer```, then navigate through to ```x64/Debug/``` where ```ssh.exe``` and the resulting logs in ```ssh_log.txt``` are located.
4. In the address bar of File Explorer, highlight all then type ```cmd.exe``` to open a command prompt in the folder.
5. Follow the syntax below to use, available commands to use while "logged in" are below under **Available Commands**.

### Syntax
```ssh [-p <port number> | -l <login-name>]```

### Switches
- ```-p```: Specify a port number to connect to, defaults to 22
- ```-l```: Specify a login name to log into on the target, defaults to lorenzo

### Available commands (unless specified, takes no parameters<>)
- ```whoami```: reply with login-name user specified at command execution
- ```ver```: reply with a fake version number of the Windows OS
- ```hostname```: reply with a fake hostname of the computer
- ```dir```: reply with a listing of the current directory 
- ```cls```: clear the screen
- ```cd <directory>``` (can only read one directory at a time): change to a specified directory (case sensitive)
- ```echo <phrase>``` (only takes one word): reply with a phrase the user inputs
- ```exit```: quit the program
