# OpenGL-Project
This is simple 3D graphics project with OpenGL. I also used SDL.

It consists with 3 parts, which are ground, light source object and picking up object.

Ground is where shadow of each objects are drawn.

Light source object indicates position of light source and it is moving, so shows drawing shadow is dynamic.

Picking up objects are four, and you can pick up and move them with mouse holding and dragging.
- To pick up object, I cast a line from mouse position to object.
- If you try to move down them under ground, it checks collision and prevent it.
- They have shadows, and shadows also follow the position of object.
