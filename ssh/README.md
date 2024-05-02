## ssh
A fake ssh emulator which when used, appears as if a user is connected to a real SSH session. Logs the username, IP, port, and password that the user uses to connect, as well as the commands they use.

### Usage
To use ssh, you will need to compile the Visual Studio Project Solution file. 
1. Begin by opening Microsoft Visual Studio 2019 or later, choose the ```Clone a repository```, paste in this repository's link
2. Either use the hotkey ```Ctrl+Shift+B```, or from the ```Build``` tab at the top, select the first option ```Build Solution```.

### Syntax
```ssh.exe [-p] [-l]```

### Switches
- ```-p```: Specify a port number to connect to, defaults to 22
- ```-l```: Specify a login name to log into on the target
