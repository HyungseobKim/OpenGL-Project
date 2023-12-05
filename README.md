# OpenGL-Project
This is a simple 3D graphics project with OpenGL. SDL is used for this project too.

It consists with 3 parts, which are the ground, the light source object and pickiable objects.

The ground is where the shadow of each objects are drawn.

The light source object indicates position of the light source. It is moving, so it shows that drawing shadows is dynamic.

There are four objects that you can pick up and move by holding and dragging your mouse.
- To enable picking up an object, I cast a line from the mouse position to object.
- If you try to move down them under the ground, it checks the collision and denies.
- The pickable objects have shadows, and shadows also follow the position of objects.
