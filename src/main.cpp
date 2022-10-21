//SNMP
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include "../inc/snmpGet.h"

#include <string.h>
#include <iostream>
#include <bitset>
#include <vector>

//ImGui
#include "../imgui/imgui.h"
#include "../backends/imgui_impl_glfw.h"
#include "../backends/imgui_impl_opengl3.h"
#include <GLES2/gl2.h>
#include <GLFW/glfw3.h>

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void getPhaseActive(int phaseActive[16], char *ipPortString)
{
  for(int x=1; x < 16; x++)
  {
    char oid[34];
    sprintf(oid, ".1.3.6.1.4.1.1206.4.2.1.1.2.1.2.%d",x);
    int valor = snmpGet(oid, ipPortString);
    if(valor != 0)
      phaseActive[x-1] = 1;
    else
      phaseActive[x-1] = 0;
  }
}


int main()
{

  // Setup window
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit())
    return 1;

  // GL 3.0 + GLSL 130
  const char* glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  // Create window with graphics context
  GLFWwindow* window = glfwCreateWindow(650, 335, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
  if (window == NULL)
    return 1;
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // Enable vsync

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

  // Setup Dear ImGui style
  ImGui::StyleColorsLight();

  // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
  ImGuiStyle& style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
  {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  int phaseActive[16];
  getPhaseActive(phaseActive, "192.168.10.23:161");

  while (!glfwWindowShouldClose(window))
  {
        
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
        
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;

    static ImGuiInputTextFlags inputServer_flags = ImGuiInputTextFlags_CharsDecimal;
    inputServer_flags |= ImGuiInputTextFlags_CharsNoBlank;
    inputServer_flags |= ImGuiInputTextFlags_NoHorizontalScroll;

      static int IpControler[] = {192,168,10,23};
      static bool get = 0;
      static char SM[100];
      static int header = 0;
      {      
        ImGui::Begin("GET SM", NULL, window_flags);
        ImGui::SetWindowSize(ImVec2(629.0, 249.0));

        ImGui::InputInt4("IpV4", IpControler, inputServer_flags);
        for (int x = 0; x < 4; x++){
            if(IpControler[x] > 255)
              IpControler[x] = 255;
        }

        ImGui::InputText("##source", SM, IM_ARRAYSIZE(SM));        

        if(ImGui::Button("Start"))
        {
          header++;
          get = 1;
        } 
        ImGui::SameLine();
        if(ImGui::Button("Stop")) get = 0;  

        ImGui::End();
      }

      if(get)
      {
        {
          //for (int x = 0; x < header; x++){
            ImGui::Begin(SM, NULL, window_flags);                            

            ImGui::Text(SM);
            static char text[1024 * 16];
            ImGui::InputTextMultiline("##source", text, IM_ARRAYSIZE(text),
                                      ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16),
                                      ImGuiInputTextFlags_ReadOnly);
            ImGui::SetWindowSize(ImVec2(629.0, 249.0));

            static char ipS[30];
            sprintf(ipS, "%d.%d.%d.%d:%d", IpControler[0],
                    IpControler[1], IpControler[2], IpControler[3], 161);

            int greenInt  =  snmpGet(".1.3.6.1.4.1.1206.4.2.1.1.4.1.4.1", ipS);
            int yellowInt =  snmpGet(".1.3.6.1.4.1.1206.4.2.1.1.4.1.3.1", ipS);
            int redInt    =  snmpGet(".1.3.6.1.4.1.1206.4.2.1.1.4.1.2.1", ipS);

            int greenPhase[16];
            int yellowPhase[16];
            int redPhase[16];
            for(int i = 0; i<15; i++)
              greenPhase[i] = (greenInt>>i)&1;
            for(int i = 0; i<15; i++)
              yellowPhase[i] = (yellowInt>>i)&1;
            for(int i = 0; i<15; i++)
              redPhase[i] = (redInt>>i)&1;            

            sprintf(text, "greenPhase:  %u %u %u %u %u %u %u %u \r\n"
                          "yellowPhase: %u %u %u %u %u %u %u %u \r\n"
                          "redPhase:    %u %u %u %u %u %u %u %u \r\n"
                          "phaseActive: %u %u %u %u %u %u %u %u \r\n",
                          

                               greenPhase[7],  greenPhase[6],  greenPhase[5],  greenPhase[4],  greenPhase[3],  greenPhase[2],  greenPhase[1],  greenPhase[0],
                              yellowPhase[7], yellowPhase[6], yellowPhase[5], yellowPhase[4], yellowPhase[3], yellowPhase[2], yellowPhase[1], yellowPhase[0],
                                 redPhase[7],    redPhase[6],    redPhase[5],    redPhase[4],    redPhase[3],    redPhase[2],    redPhase[1],    redPhase[0],
                              phaseActive[7], phaseActive[6], phaseActive[5], phaseActive[4], phaseActive[3], phaseActive[2], phaseActive[1], phaseActive[0]);

            ImGui::End();
          //}
        } 
      }

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    	
    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
      GLFWwindow* backup_current_context = glfwGetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      glfwMakeContextCurrent(backup_current_context);
    }

    glfwSwapBuffers(window);
  }

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}