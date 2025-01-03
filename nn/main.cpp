#include "include/glad/glad.h"
#include "include/GLFW/glfw3.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void proccess_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Version 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1200, 600, "Squishy Shell", NULL, NULL);

    if (window == NULL) {
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        return 1;
    }

    glViewport(0, 0, 1200, 600);  

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while (!glfwWindowShouldClose(window)) {
        proccess_input(window);

        glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}