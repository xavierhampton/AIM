#version 330 core
layout (location = 0) in vec3 aPos;   
layout (location = 1) in vec3 aColor; 
  
out vec3 ourColor; 
uniform float greenColor;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    ourColor = vec3(aColor.x, greenColor, aColor.z); 
}  