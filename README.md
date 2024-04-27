# OpenGL-Project
This is a simple 3D graphics project using OpenGL.
SDL is used for making the window.

The demo consists with 3 parts.

### Ground
The ground is where the shadow of each objects are drawn.

### Light source object
The light source object indicates position of the light source. It is moving and showing dynamic shadow drawing.

### Pickable objects
There are four objects that you can pick up and move by holding and dragging your mouse.
- To enable picking up an object, I cast a line from the mouse position to object.
- If you try to move down them under the ground, it checks the collision and denies.
- The pickable objects have shadows, and shadows also follow the position of objects.
