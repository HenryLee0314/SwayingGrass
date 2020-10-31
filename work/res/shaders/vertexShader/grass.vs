#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aEw;
layout (location = 2) in vec3 aEn;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vsFragPos;
out vec3 vsEw;
out vec3 vsEn;

void main()
{
	vsFragPos = vec3(model * vec4(aPos, 1.0));
	vsEw = aEw;
	vsEn = aEn;

    gl_Position = projection * view * model * vec4(aPos, 1.0);

    //gl_PointSize = 10.0;
}