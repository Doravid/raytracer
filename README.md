## Project Overview

For this project I have two main goals. 
1. To work through the textbook [The Raytracer Challenge](http://raytracerchallenge.com/)
2. Convert the raytracer into a useable game engine.


## Output as of 07/03/2026:
### Textbook Progress
So far we have the linear algebra library done. We also have spheres, and scenes implemented from the book

### Game Engine Progress
The output of the raytracer is being shoved into Raylib. This is kind of cheating, since it kinda throws away some of the benefits of just writing C since Raylib uses OpenGL under the hood. 

Multithreading wasn't too hard and brought the performance up from 14fps to about 80fps at 720p. Which is pretty good all things considered. 

<img width="2053" height="1139" alt="image" src="https://github.com/user-attachments/assets/beb03f52-f9eb-4c6a-9d7d-c7eb9108579b" />
