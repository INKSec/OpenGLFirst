#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include <thread>

#include "helperFunctions.h"

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()"
"{\n"
"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n";

const char* fragmentShader2Source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()"
"{\n"
"    FragColor = vec4(0.0f, 0.0f, 0.2f, 1.0f);\n"
"}\n";

int main(void)
{

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    
    


    GLFWwindow* window{ glfwCreateWindow(800,600,"Tester", NULL, NULL) };

    if (window == NULL) {
        std::cerr << "Failed to create Window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    // sets coordinates for rendering
    glViewport(0, 0, 800, 600);



    // Shaders
    // --------------------------------------------------------
    // vertex shader

    // Create vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // check if vertex shader compilation is successfull
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader
    // create fragment shader
    unsigned int fragmentShader, fragmentShaderOrange;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    fragmentShaderOrange = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glShaderSource(fragmentShaderOrange, 1, &fragmentShader2Source, NULL);
    glCompileShader(fragmentShader);
    glCompileShader(fragmentShaderOrange);

    // check if fragment shader compilation is successfull
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    glGetShaderiv(fragmentShaderOrange, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link shaders
    unsigned int shaderProgram, shaderProgram2;
    shaderProgram = glCreateProgram();
    shaderProgram2 = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glAttachShader(shaderProgram2, vertexShader);
    glAttachShader(shaderProgram2, fragmentShaderOrange);
    glLinkProgram(shaderProgram);
    glLinkProgram(shaderProgram2);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    }
    glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    }
    
    // once the shaders were loaded we dont need them anymore
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(fragmentShaderOrange);


    // Set up vertex data
    // ------------------------------------------------------------

    float vertices[] = {
            0.5f, 0.5f, 0.0f, //top right
            0.5f, -0.5f, 0.0f, //bottom right
           -0.5f,  -0.5f, 0.0f, // bottom left
           -0.5f, 0.5f, 0.0f // top left
          

          
    };
    unsigned int indices[] = {
        0,1,3, // first triangle
        1,2,3, // second triangle
        
    };

    // temp
    float vertices2[] = {
           0.8f, 0.9f, 0.0f,
           0.7f, 0.8f, 0.0f,
           0.9f, 0.8f, 0.0f
    };
    
    // temp end

    unsigned int VBO[2], VAO[2], EBO;
    glGenVertexArrays(2, VAO);
    glGenBuffers(2, VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    glBindVertexArray(VAO[1]);	
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // because the vertex data is tightly packed we can also specify 0 as the vertex attribute's stride to let OpenGL figure it out
    glEnableVertexAttribArray(0);
    //glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {

        exitWindow(window);

        glClearColor(0.9f, 0.03f, 0.03f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO[0]);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

        glUseProgram(shaderProgram2);
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);


        glfwSwapBuffers(window);
        glfwPollEvents();

        
        

    }

    glDeleteVertexArrays(1, VAO);
    glDeleteBuffers(1, VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    
    glfwTerminate();

    
    return 0;
}