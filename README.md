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

## Output as of 10/20/2026:
School has started so I have been a bit busy. 
## Textbook Progress
Shadows were actually pretty easy to add since the textbook set us up nicely for it. 
I've done a bit of OOP to make it so the renderer supports more than one kind of primitive. I've also added in Planes. 
This also removes some of the floating point precision errors I was getting before with the thin spheres.

## Game Engine Progress
N/A, though with the new shadows / planes performance has dropped a fair bit to about 60fps. I think once the renderer is feature complete it will be easier to make some optimizations since I will know what can be cut.

<img width="547" height="353" alt="image" src="https://github.com/user-attachments/assets/4b6d450a-503c-47bb-a75e-8a08e2094137" />

