For Homework 2

Subdirectories

Section 1: TimelinedGames
Section 2: Server, Client1, Client2, Client3
Section 3: NetworkedGameServer, NetworkedGameClient1, NetworkedGameClient2, NetworkedGameClient3

These are all separate projects with their own main.cpp, so you can run them at the same time to achieve networking simulation. 

Compilation instructions

1. Static linking of SFML
2. Normal using of ZMQ
3. I modified the Rect.inl file of SFML, adding parenthesis to std::min and std::max functions in that file to avoid conflicts with min and max defined in windows.h included by ZMQ. I must modify it or I couldn't pass the compilation. The code after modify should be like (std::min) and (std::max). 

How to use

P: pause/resume game
Ctrl + N: normal speed of time
Ctrl + S: slow speed of time
Ctrl + F: fast speed of time

W, A, D, or arrow keys for moving characters. 
Ctrl + C: constant scaling
Ctrl + P: proportional scaling (default)
Ctrl + R: return to normal size of window and proportional mode

########################################################################################
For Homework 1

Compilation instructions

I'm using static linking of SFML in my project, so there should be no need for .dll files. The executable files in the Debug or Release directory should work properly directly. 
If you need to compile my source code yourself, I think that it would be Ok either using dynamic linking or static linking. It should depend on your setting of project properties. Just pay attention that the Images directory should be in the same directory with the executable file generated. 
Also, I'm not using third-party libraries, so there should be no need for additional works on compiling. 

How to use

W, A, D, or arrow keys for moving characters. 
Ctrl + C: constant scaling
Ctrl + P: proportional scaling (default)
Ctrl + R: return to normal size of window and proportional mode
