#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    //Error handling

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE){
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char *message = (char*)malloc(length*sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " <<
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        free(message);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    //Itialisation of shader
    unsigned int program = glCreateProgram();
    //Creating shader object
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}




int main(int argc, char *argv[])
{
    GLFWwindow* window;


    /*Initialize the library*/
    if (!glfwInit()){
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);



    if (glewInit() != GLEW_OK){
        std::cout << "Error!" << std::endl;
    }

    std::cout<< glGetString(GL_VERSION) << std::endl;

    /*Triangle Data*/
    float positions[6] = {
        -0.5f, -0.5f,
        0.0f, 0.5f,
        0.5f, -0.5f
    };

    /*Vertex Buffer*/
    unsigned int buffer;
    //Generate buffer object name on buffer (the ID)
    glGenBuffers(1, &buffer);
    //Sets state of buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    //Biinds data to currently selected state
    glBufferData(GL_ARRAY_BUFFER, 6*sizeof(float), positions, GL_STATIC_DRAW);

    //Enabling the AttribPointer
    glEnableVertexAttribArray(0);
    //Specifying the layout of the buffer
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0);

    std::string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "\n"
        "void main(){\n"
        "   gl_Position = position;\n"
        "}";

    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "\n"
        "void main(){\n"
        "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}";

    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}
