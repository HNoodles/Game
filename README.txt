For Homework 5

Subdirectories

Section 1: Scripting, 2DPlatformerServer, 2DPlatformerClient
Section 2: SpaceInvadersServer, SpaceInvadersClient

These are all separate projects with their own main.cpp, so you can run them at the same time to achieve networking simulation. 
*The Scripting project is just for testing the functionality of scripting component. There is more sufficient description for this in the writeup. *

Compilation instructions

1. Static linking of SFML
2. Normal using of ZMQ
3. I modified the Rect.inl file of SFML, adding parenthesis to std::min and std::max functions in that file to avoid conflicts with min and max defined in windows.h included by ZMQ. I must modify it or I couldn't pass the compilation. The code after modify should be like (std::min) and (std::max). 
4. I'm working on debug mode, and it works fine. I'm not sure if release would also work well, but it should do. 
5. BE SURE TO START THE SERVER FIRST, AND THEN START CLIENTS, or the time on server and clients would be inconsistant. 
6. I added "C:\libs\duktape-2.5.0\src; C:\libs\dukglue\include\dukglue" to C++ include directories of my project configurations. Also, I added "_CRT_SECURE_NO_WARNINGS;" to C++ preprocessor definition. 

How to use

2D Platformer
W, A, D, or arrow keys for moving characters. 

Space Invaders
A, D for moving character
Space for firing bullets
	maximum 2 bullets existing simultaneously
	can only fire once every second

########################################################################################
For Homework 4

Subdirectories

The directories for section 1 and 2 are consistant. All clients and server are used from the beginning to the end of this assignment. 

Compilation instructions

1. Static linking of SFML
2. Normal using of ZMQ
3. I modified the Rect.inl file of SFML, adding parenthesis to std::min and std::max functions in that file to avoid conflicts with min and max defined in windows.h included by ZMQ. I must modify it or I couldn't pass the compilation. The code after modify should be like (std::min) and (std::max). 
4. I'm working on debug mode, and it works fine. I'm not sure if release would also work well, but it should do. 
5. BE SURE TO START THE SERVER FIRST, AND THEN START CLIENTS, or the time on server and clients would be inconsistant. 
6. On my laptop, I can run 2 clients simultaneously without affecting the game experenice too much, but 3 or 4 clients running at the same time will ruin everything. I'm not sure if the situation would be better if they are running on a better machine, and I think that 2 clients would be sufficient to test all the functionalities required. # About this, more detailed description is in the writeup. #

How to use

W, A, D, or arrow keys for moving characters. 
R, E for start recording or end recording replays. 
N, F, S for switching between normal, fast, slow time speed during replaying. 

########################################################################################
For Homework 3

Subdirectories

Section 1: ModeledGame
Section 2&3: NetworkedGameServer, NetworkedGameClient1, NetworkedGameClient2, NetworkedGameClient3, NetworkedGameClient4

These are all separate projects with their own main.cpp, so you can run them at the same time to achieve networking simulation. 

Compilation instructions

1. Static linking of SFML
2. Normal using of ZMQ
3. I modified the Rect.inl file of SFML, adding parenthesis to std::min and std::max functions in that file to avoid conflicts with min and max defined in windows.h included by ZMQ. I must modify it or I couldn't pass the compilation. The code after modify should be like (std::min) and (std::max). 
4. I'm working on debug mode, and it works fine. I'm not sure if release would also work well, but it should do. 

Description

The second and third section is a 2D platformer game with size of 1600 * 600. 
The side boundaries are on the left and right side of the window, whose size is 800 * 600. 
The boundaries are 100 away from the side of the window. 
There are death zones around the 1600 * 600 area, colliding with which will cause the character transferred back to above the static platform, where the spawn point stays. 

How to use

W, A, D, or arrow keys for moving characters. 
Ctrl + C: constant scaling
Ctrl + P: proportional scaling (default)
Ctrl + R: return to normal size of window and proportional mode

########################################################################################
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
