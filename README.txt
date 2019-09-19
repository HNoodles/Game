Compilation instructions

I'm using static linking of SFML in my project, so there should be no need for .dll files. The executable files in the Debug or Release directory should work properly directly. 
If you need to compile my source code yourself, I think that it would be Ok either using dynamic linking or static linking. It should depend on your setting of project properties. Just pay attention that the Images directory should be in the same directory with the executable file generated. 
Also, I'm not using third-party libraries, so there should be no need for additional works on compiling. 

How to use

W, A, D, or arrow keys for moving characters. 
Ctrl + C: constant scaling
Ctrl + P: proportional scaling (default)
Ctrl + R: return to normal size of window and proportional mode
