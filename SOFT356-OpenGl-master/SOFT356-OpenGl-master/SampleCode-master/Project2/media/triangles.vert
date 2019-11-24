
#version 400 core

layout( location = 0 ) in vec4 vPosition;
layout( location = 1 ) in vec4 vColour;
uniform mat4 mvp;

out vec4 fragColour;
void
main()
{
    gl_Position = mvp * vPosition;
	fragColour = vColour;
}
