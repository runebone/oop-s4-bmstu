#ifndef __IMGUI_OPENGL3_GLFW_GUI_IMPL_H__
#define __IMGUI_OPENGL3_GLFW_GUI_IMPL_H__

#include "gui_impl.h"

#include "imgui_opengl3_glfw_ui_impl/includes.h"

class ImguiOpengl3GLFWGUI : public GUI
{
    // GLFW callback functions
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    static void process_input(GLFWwindow* window)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }

public:
    int run()
    {
        // Initialize GLFW
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create a GLFW window
        GLFWwindow* window = glfwCreateWindow(800, 600, "ImGui OpenGL3 GLFW Application", NULL, NULL);
        if (window == NULL)
        {
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);

        // Initialize GLAD
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            glfwTerminate();
            return -1;
        }

        // Setup ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;

        ImGui::StyleColorsDark();

        // Setup ImGui platform bindings
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");

        // Set GLFW callback functions
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        // Main loop
        while (!glfwWindowShouldClose(window))
        {
            process_input(window);

            // Start the ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // Create ImGui UI
            ImGui::Begin("Hello, ImGui!");
            ImGui::Text("This is an example application.");
            ImGui::End();

            // Rendering
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwTerminate();

        return 0;
    }
};

#endif // __IMGUI_OPENGL3_GLFW_GUI_IMPL_H__
