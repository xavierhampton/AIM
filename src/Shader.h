#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers
  
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader 
{
public:
//Program ID
    unsigned int ID;

    //Constructor to build shader
    Shader(const char* vertexPath, const char* fragmentPath);

    //Use the shader program
    void use();

    //Utility functions to set uniforms
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;

};
#endif // SHADER_H
