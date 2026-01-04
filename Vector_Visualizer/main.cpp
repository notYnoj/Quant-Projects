#include "../Math Algorithms/3DVector.hpp"
#include "../Math Algorithms/matrix.hpp"
#include "../cameras/target.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>
#include <cstdio>
#include <stdexcept>
#include <vector>
#include <cmath>


void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    static bool first_mouse = true;
    static double prevX, prevY;

    if(first_mouse){
        prevX = xpos;
        prevY = ypos;
        first_mouse = false; 
    }

    double xoffset = xpos - prevX;
    double yoffset = prevY - ypos;
    prevX = xpos;
    prevY = ypos;
    
    //use glfwGetWindowUserPointer cuz the mouse_callback has to be really specific lol bascially js returns cur camera
    TargetCamera* cam = static_cast<TargetCamera*>(glfwGetWindowUserPointer(window));
    if(cam){
        xoffset *= cam->sens.x;
        yoffset *= cam->sens.x;
        cam->theta.x += xoffset;   
        cam->phi.x += yoffset;     
        
        if(cam->phi.x > 179.0) cam->phi.x = 179.0;
        if(cam->phi.x < 1.0) cam->phi.x = 1.0;
        
        
        if(cam->theta.x > 360.0) cam->theta.x -= 360.0;
        if(cam->theta.x < 0.0) cam->theta.x += 360.0;
        
        cam->updateCamera();
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    TargetCamera* cam = static_cast<TargetCamera*>(glfwGetWindowUserPointer(window));
    if(cam){
        cam->radius.x -= yoffset * 0.25; 
        if(cam->radius.x < 1.0) cam->radius.x = 1.0;
        if(cam->radius.x > 20.0) cam->radius.x = 20.0;
        cam->updateCamera();
    }
}

void processWSADInput(GLFWwindow* window, TargetCamera& cam, double time){
    double velocity = cam.speed.x * time;
    //moving using the W S A D keys
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam.Position = cam.Position + (cam.front * velocity);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam.Position = cam.Position - (cam.front * velocity);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam.Position = cam.Position - (cam.right * velocity);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam.Position = cam.Position + (cam.right * velocity);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cam.Position = cam.Position + (cam.normalUp * velocity);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cam.Position = cam.Position - (cam.normalUp * velocity);
}

const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 view;
uniform mat4 projection;

out vec3 color;

void main() {
    gl_Position = projection * view * vec4(aPos, 1.0);
    color = aColor;
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
in vec3 color;
out vec4 FragColor;

void main() {
    FragColor = vec4(color, 1.0);
}
)";

//compile shader -> type + source (type comes from GL) source -> whatever u write above
//use glShaderSource to write to shader (make shader glCreateShader(type))
GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        fprintf(stderr, "Shader compilation failed: %s\n", infoLog);
        throw std::runtime_error("Shader compilation failed");
    }
    return shader;
}

GLuint createShaderProgram() {
    //Make shader (compile shader) -> make progam -> attach shaders -> link progam
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        fprintf(stderr, "Program linking failed: %s\n", infoLog);
        throw std::runtime_error("Program linking failed");
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return program;
}

struct GridData {
    GLuint VAO, VBO;
    int vertexCount;
};

//this doesn't work <3 
GridData createGrid(double size = 10.0, int divisions = 20) {
    std::vector<double> vertices;
    double step = size / divisions;
    double halfSize = size / 2.0;
    for (int i = 0; i <= divisions; ++i) {
        double z = -halfSize + i * step;
        double r = (i == divisions / 2) ? 1.0f : 0.3f;
        double g = (i == divisions / 2) ? 1.0f : 0.3f;
        double b = (i == divisions / 2) ? 1.0f : 0.3f;
        vertices.push_back(-halfSize); vertices.push_back(0.0f); vertices.push_back(z);
        vertices.push_back(r); vertices.push_back(g); vertices.push_back(b);
        vertices.push_back(halfSize); vertices.push_back(0.0f); vertices.push_back(z);
        vertices.push_back(r); vertices.push_back(g); vertices.push_back(b);
    }
    for (int i = 0; i <= divisions; ++i) {
        double x = -halfSize + i * step;
        double r = (i == divisions / 2) ? 1.0f : 0.3f;
        double g = (i == divisions / 2) ? 1.0f : 0.3f;
        double b = (i == divisions / 2) ? 1.0f : 0.3f;
        vertices.push_back(x); vertices.push_back(0.0f); vertices.push_back(-halfSize);
        vertices.push_back(r); vertices.push_back(g); vertices.push_back(b);
        vertices.push_back(x); vertices.push_back(0.0f); vertices.push_back(halfSize);
        vertices.push_back(r); vertices.push_back(g); vertices.push_back(b);
    }
    
    GridData grid;
    grid.vertexCount = vertices.size() / 6;
    glGenVertexArrays(1, &grid.VAO);
    glGenBuffers(1, &grid.VBO);
    glBindVertexArray(grid.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, grid.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(double), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 6 * sizeof(double), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 6 * sizeof(double), (void*)(3 * sizeof(double)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    return grid;
}

Matrix createPerspectiveMatrix(double fov, double aspect, double near, double far) {
    Matrix proj(4, 4);
    double tanHalfFov = tan(fov / 2.0f);
    proj.append(0, 0, 1.0f / (aspect * tanHalfFov));
    proj.append(0, 1, 0.0f);
    proj.append(0, 2, 0.0f);
    proj.append(0, 3, 0.0f);
    proj.append(1, 0, 0.0f);
    proj.append(1, 1, 1.0f / tanHalfFov);
    proj.append(1, 2, 0.0f);
    proj.append(1, 3, 0.0f);
    proj.append(2, 0, 0.0f);
    proj.append(2, 1, 0.0f);
    proj.append(2, 2, -(far + near) / (far - near));
    proj.append(2, 3, -(2.0f * far * near) / (far - near));
    proj.append(3, 0, 0.0f);
    proj.append(3, 1, 0.0f);
    proj.append(3, 2, -1.0f);
    proj.append(3, 3, 0.0f);
    return proj;
}

void setMatrixUniform(GLuint program, const char* name, const Matrix& mat) {
    GLint location = glGetUniformLocation(program, name);
    if (location == -1) {
        fprintf(stderr, "Warning: uniform '%s' not found\n", name);
        return;
    }
    float data[16];
    for (int col = 0; col < 4; ++col) {
        for (int row = 0; row < 4; ++row) {
            data[col * 4 + row] = static_cast<float>(mat.get(row, col));
        }
    }
    
    glUniformMatrix4fv(location, 1, GL_FALSE, data);
}

static void error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    (void)scancode;
    (void)mods;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}


int main() {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        throw std::runtime_error("GLFW was not initialized correctly");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Vector Visualizer", nullptr, nullptr);

    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    glEnable(GL_DEPTH_TEST);
    GLuint shaderProgram = createShaderProgram();
    
    GridData grid = createGrid(10.0f, 20);
    TargetCamera cam(
        {0, 0, 0},
        Radius{10.0},
        Theta{0.0},
        Phi{90.0},
        Speed{2.0},
        Sens{0.3}
    );
    double prevTime = 0.0;
    //Basically we can use our mouse after this I think
    glfwSetWindowUserPointer(window, &cam);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    Matrix projection = createPerspectiveMatrix((45.0f * (M_PI / 180.0)), 800.0f / 600.0f, 0.1f, 100.0f);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        double curTime = glfwGetTime();
        double time = curTime - prevTime;
        prevTime = curTime;
        processWSADInput(window, cam, time);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Vec3D target = cam.Position + cam.front;
        Matrix view = Matrix::lookAt(cam.Position, target, cam.up);
        glUseProgram(shaderProgram);
        setMatrixUniform(shaderProgram, "view", view);
        setMatrixUniform(shaderProgram, "projection", projection);
        glBindVertexArray(grid.VAO);
        glDrawArrays(GL_LINES, 0, grid.vertexCount);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
