# Butterflies

"Butterflies" is my final project for a university course in Computer Graphics. It is a variation upon a particle cannon. Next to continuously exploding fireworks, I implemented the option to transform the firework particles into fluttering butterflies. There are also sound effects and background music. I made use of OpenGL, GLUT and SDL libraries.

### Functionality

Upon running the executable, a window is created, in which five fireworks are constantly launching and exploding. This will go on forever, until the user presses ‘Esc’ or ‘q’, to quit the program. If the user presses ‘B’, the fireworks will change into butterflies. The butterflies will start flying in the direction of the (moving) mouse pointer. If the user moves the mouse faster, the butterflies will have higher velocity and vice versa. Pressing the left mouse button will cause the butterflies to fly closer. The right mouse button chases them away.
Another possibility is to move the mouse outside of the window area. If this happens, the butterflies lose sight of the mouse cursor and start ‘going crazy’.
By pressing ‘B’ again, the butterflies are released. The fireworks will start launching again.
It is also possible to move the camera, using the arrow keys. By pressing right shift + up or down arrow keys, the user can zoom in or out.

### Program structure

#### Firework class (main.cpp)
This class contains information for one firework. A firework consists of a specified number of particles, which move at a certain velocity in some direction. Before launching, the firework is given an initial velocity and launch direction. The upwards (y) velocity is counteracted by a gravitational force. When the firework speed reaches 0, the firework explodes. <br>
The different particles are each given a random velocity and direction. This creates the explosion.

#### Butterflies
The butterflies are created by the following functions: getMousePosition returns the coordinates of the mouse cursor. calculateBezier and calculateCircle each compute a curve that is used to draw the shapes and velocities of the butterflies. The function drawButterfly draws the actual butterfly, by using Bezier curves.

#### Updating and Drawing
There are two functions that are important for updating and rendering the model content. These are Update and DrawScene. Update calls the functions to move or explode the fireworks. It also processes user input, such as keys or mouse movement. DrawScene then takes these new values, and uses them to draw new content on the screen.

#### Video and Audio
For video, I used SDL to set up a video surface and create a window.
For audio, I made use of SDL mixer 1.2, which is an additional SDL library.

#### Main Loop
After initializing everything, the program goes into the main control loop, which keeps updating and rendering the fireworks and butterflies.

#### User Input
The files UserInput.cpp and UserInput.h handle the keys that the user presses, or the mouse movements that are made. SDL events are used to register the input.

#### Light and Materials
Light.cpp en light.h contain the functions to initialize the lights, and to define material properties for objects.

#### User Interface
To create a 2D user interface, interface.cpp and interface.h draw text on the screen. This is done using glutStrokeCharacter, with a font from the GLUT library.

