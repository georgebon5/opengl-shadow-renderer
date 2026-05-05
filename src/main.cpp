#define GL_SILENCE_DEPRECATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Scene.h"
#include "Camera.h"

static void framebufferSizeCallback(GLFWwindow*, int w, int h) {
    glViewport(0, 0, w, h);
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(800, 600, "Graphics I – Moving Objects", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Camera camera(12.0f, 0.0f, 20.0f);
    Scene  scene;

    bool  spacePrev = false;
    float lastTime  = static_cast<float>(glfwGetTime());

    while (!glfwWindowShouldClose(window)) {
        const float now = static_cast<float>(glfwGetTime());
        const float dt  = now - lastTime;
        lastTime = now;

        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        const bool spaceNow = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
        if (spaceNow && !spacePrev) scene.togglePause();
        spacePrev = spaceNow;

        camera.processInput(window, dt);
        scene.update(dt);

        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        int fbW, fbH;
        glfwGetFramebufferSize(window, &fbW, &fbH);
        scene.draw(camera, fbW, fbH);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
