#include <GL/glew.h> // Include before GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>

#include <iostream>
#include <cassert>

#include "Renderer.h"
#include "Tests/TestClearColor.h"

constexpr float WINDOW_HEIGHT = 540.0f;
constexpr float WINDOW_WIDTH = 960.0f;

static GLFWwindow* InitOpenGL()
{
  // Initialize the OpenGL library.
  if (glfwInit() == GLFW_FALSE)
    return nullptr;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create a windowed mode window and it's OpenGL context
  GLFWwindow* window;
  window = glfwCreateWindow((int)WINDOW_WIDTH, (int)WINDOW_HEIGHT, "Hello World", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return nullptr;
  }

  // Create the OpenGL context.
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  // Init GLEW. Should be called after creating a valid OpenGL context.
  if (glewInit() == GLEW_OK)
  {
    std::cout << "Initialised GLEW - " << glGetString(GL_VERSION) << std::endl;
  }
  else
  {
    std::cout << "Error initialising glew!" << std::endl;
  }

  return window;
}

int main(void)
{
  GLFWwindow* window = InitOpenGL();
  assert(window);

  OpenGLCall(glEnable(GL_BLEND));
  OpenGLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  Renderer renderer;

  ImGui::CreateContext();
  ImGui_ImplGlfwGL3_Init(window, true);
  ImGui::StyleColorsDark();

  test::Test* currentTest = nullptr;
  test::TestMenu* testMenu = new test::TestMenu(currentTest);
  currentTest = testMenu;

  testMenu->RegisterTest<test::ClearColor>("Clear Color");

  // Loop until the user closes the window
  while (!glfwWindowShouldClose(window))
  {
    OpenGLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    renderer.Clear();

    ImGui_ImplGlfwGL3_NewFrame();

    if (currentTest) 
    {
      currentTest->OnUpdate(0.0f);
      currentTest->OnRender();

      ImGui::Begin("Test");
      if (currentTest != testMenu && ImGui::Button("<--"))
      {
        delete currentTest;
        currentTest = testMenu;
      }
      currentTest->OnImGuiRender();
      ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  if (testMenu != currentTest)
    delete testMenu;
  delete currentTest;

  ImGui_ImplGlfwGL3_Shutdown();
  ImGui::DestroyContext();
  glfwTerminate();

  return 0;
}