# Medium renderer - Software renderer attempt

 This is a simple software renderer written in C++ in C style using SDL just for window management and writting pixels to the framebuffer.

Still a work in progress.

### Features in progress:
- [X] Perspective projection
- [X] Perspective correct texturing
- [X] Correct depth testing
- [X] Phong shading model
- [ ] Shadows
- [ ] Normal maps with tangent space

## Ambisious features to add if possible:
 When I make sure that the base is in a perfect state I'll try to move on to these later.
 Update: These features are still in mind for the next rewrite of this project.
- [ ] vectorizing all the operations of the renderer with SIMD
- [ ] global illumination with spherical harmonics
- [ ] SSAO (Screen Space Ambient Occlusion)
- thinking about and researching the rest of this list

### Neutral color preview
|                                     utah teapot                                       |           Suzan head   (the infamous CGI monkey) 
|:-------------------------------------------------------------------------------------:|:-----------------------------------------------------------------------------------:|
 |<img width="400" height="400" alt="Screenshot from 2025-11-12 22-41-41" src="https://github.com/user-attachments/assets/a788f242-fc90-4a3e-b6d2-96b2da31f104" />|<img width="400" height="400" alt="Screenshot from 2025-11-12 22-45-06" src="https://github.com/user-attachments/assets/82124547-29ea-468a-9203-93ab58eea96e" />|
 
|               Girl                                                                    |    Jack Daniel's Bottle                   |
|:-------------------------------------------------------------------------------------:|:-----------------------------------------------------------------------------------------:|
 |<img width="400" height="400" alt="girl" src="https://github.com/user-attachments/assets/1c0914d7-4c00-44a6-a3a7-dbee06098ae3" />|<img width="300" height="400" alt="jack" src="https://github.com/user-attachments/assets/7a7cb621-e022-4e89-ad89-7c3b80341461" />|

### Perspective correct texture mapping diffuse preview
|                                                                                       |                                                     |
|:-------------------------------------------------------------------------------------:|:-----------------------------------------------------------------------------------------:|
|<img width="500" height="520" alt="Screenshot from 2025-11-18 03-22-46" src="https://github.com/user-attachments/assets/ac9a2553-289c-490a-badc-41a88c5cf0ce" />|<img width="501" height="513" alt="Screenshot from 2025-11-18 03-25-17" src="https://github.com/user-attachments/assets/375a4d22-10e4-4637-a50d-37aa8a4d7662" />|
<img width="722" height="759" alt="Screenshot from 2025-11-18 04-10-30" src="https://github.com/user-attachments/assets/d714da9c-0693-4767-967c-b6629620bc32" />

### Meshes normals view

<img width="510" height="462" alt="suzan_n" src="https://github.com/user-attachments/assets/f95e65d6-3e41-4e04-b1ca-c6402d0cdfb2" />

### Phong shading

<img width="486" height="472" alt="suzen_l" src="https://github.com/user-attachments/assets/c8f1b640-f87c-4416-985e-58bb3a435f45" />





