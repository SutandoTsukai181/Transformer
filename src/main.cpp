#define GL_GLEXT_PROTOTYPES
#define GLFW_INCLUDE_GLU

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <Eigen/Dense>

#include <stdio.h>
#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "cpu_transformer.h"
#include "gpu_transformer.h"

//#include "shaders.h"

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

//unsigned int v, f, p;

//void setShaders() {
//    const char *vs,*fs;
//
//    v = glCreateShader(GL_VERTEX_SHADER);
//    f = glCreateShader(GL_FRAGMENT_SHADER);
//
//    vs = vertexShaderSource; //put the whole shader code in a string
//    fs = fragmentShaderSource;
//
//    const char * vv = vs;
//    const char * ff = fs;
//
//    glShaderSource(v, 1, &vv,NULL);
////    glShaderSource(f, 1, &ff,NULL);
//
////    free(vs);free(fs);
//
//    glCompileShader(v);
////    glCompileShader(f);
//
//    GLint success = 0;
////    glGetShaderiv(v, GL_COMPILE_STATUS, &success);
////    std::cout << "Compile status: " << success << std::endl;
////
////    glGetShaderiv(f, GL_COMPILE_STATUS, &success);
////    std::cout << "Compile status: " << success << std::endl;
//
//    GLint logSize = 0;
//    glGetShaderiv(v, GL_INFO_LOG_LENGTH, &logSize);
//
//    char* log = new char[logSize];
//    glGetShaderInfoLog(v, logSize, &logSize, &log[0]);
//
//    std::cout << log << std::endl;
//
//    p = glCreateProgram();
//
//    glAttachShader(p,v);
////    glAttachShader(p,f);
//
//    glLinkProgram(p);
//    glUseProgram(p);
//}


// Main code
int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Transformer", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    int version = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if (version == 0) {
        printf("Failed to initialize OpenGL context\n");
        return -1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Our state
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImVec4 model_color = ImVec4(0.0f, 1.0f, 0.0f, 1.00f);

    // Camera
    glMatrixMode(GL_PROJECTION);
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    bool isCpuMode = true;
    BaseTransformer *transformer = new CpuTransformer();
    transformer->setLocalTrans(true);

//    setShaders();
//    glUniformMatrix4fv(glGetUniformLocation(v, "transformationMatrix"), 1, GL_FALSE, );

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

//        ImGui::ShowDemoWindow();

        {
            ImGui::Begin("Transformer config");

            ImGui::SeparatorText("Model & mode");

            static bool fileLoadError = false;
            static char filepath[PATH_MAX];
            ImGui::InputText("File path", filepath, PATH_MAX);
            if (ImGui::Button("Load .off model")) {
                transformer->read(std::string(filepath));
            }

            if (transformer->isInitialized()) {
                static bool isLocalMode = transformer->isLocalTrans();

                if (isCpuMode) {
                    ImGui::Text("Current mode: CPU"); ImGui::SameLine();
                    if (ImGui::Button("Switch to GPU mode")) {
                        isCpuMode = false;
                        transformer->reset();

                        BaseTransformer* transformer1 = new GpuTransformer(transformer);
                        delete transformer;
                        transformer = transformer1;
                    }

                    if (ImGui::Checkbox("Use local transformations", &isLocalMode)) {
                        transformer->setLocalTrans(isLocalMode);
                    }
                } else {
                    ImGui::Text("Current mode: GPU"); ImGui::SameLine();
                    if (ImGui::Button("Switch to CPU mode")) {
                        isCpuMode = true;
                        transformer->reset();

                        BaseTransformer* transformer1 = new CpuTransformer(transformer);
                        delete transformer;
                        transformer = transformer1;

                        // Set local mode for CPU
                        isLocalMode = transformer->isLocalTrans();
                    }

                    ImGui::Text("Using local transformations on GPU");
                }

                if (ImGui::Button("RESET")) {
                    transformer->reset();
                }

                ImGui::SeparatorText("Colors");
                ImGui::ColorEdit3("Background color", (float*)&clear_color);
                ImGui::ColorEdit3("Model color", (float*)&model_color);

                // TRANSLATION
                ImGui::SeparatorText("Translation");

                static float transVec[3] = { 0.1, 0, 0 };
                ImGui::InputFloat3("vector", transVec);

                if (ImGui::Button("Apply##Trans")) {
                    transformer->translate(Eigen::Vector3f(transVec));
                }

                // ROTATION
                ImGui::SeparatorText("Rotation around parallel axis");

                static float angle = 90;
                ImGui::InputFloat("angle (deg)", &angle);

                static int axis = 1;
                ImGui::RadioButton("X Axis", &axis, 0); ImGui::SameLine();
                ImGui::RadioButton("Y Axis", &axis, 1); ImGui::SameLine();
                ImGui::RadioButton("Z Axis", &axis, 2);

                static float rotVec[2] = { 0, 0 };
                if (axis == 0) {
                    ImGui::InputFloat2("axis y, z components", rotVec);
                } else if (axis == 1) {
                    ImGui::InputFloat2("axis x, z components", rotVec);
                } else if (axis == 2) {
                    ImGui::InputFloat2("axis x, y components", rotVec);
                }

                if (ImGui::Button("Apply##Rot")) {
                    if (axis == 0) {
                        transformer->rotateAroundX(angle, rotVec[0], rotVec[1]);
                    } else if (axis == 1) {
                        transformer->rotateAroundY(angle, rotVec[0], rotVec[1]);
                    } else if (axis == 2) {
                        transformer->rotateAroundZ(angle, rotVec[0], rotVec[1]);
                    }
                }

                // ROTATION AXIS
                ImGui::SeparatorText("Rotation around arbitrary axis");

                static float angle2 = 90;
                ImGui::InputFloat("angle (deg)", &angle2);

                static float rotAxisP1[3] = { 0, 0, 0 };
                ImGui::InputFloat3("point 1", rotAxisP1);

                static float rotAxisP2[3] = { 0, 0, 1 };
                ImGui::InputFloat3("point 2", rotAxisP2);

                if (ImGui::Button("Apply##RotAxis")) {
                    transformer->rotateAroundArbitraryAxis(angle2, Eigen::Vector3f(rotAxisP1), Eigen::Vector3f(rotAxisP2));
                }

                // SCALING
                ImGui::SeparatorText("Scale with respect to fixed point");

                static float scaleFactor[3] = { 0.9, 0.9, 0.9 };
                ImGui::InputFloat3("scale factor", scaleFactor);

                static float scalePoint[3] = { 0, 0, 0 };
                ImGui::InputFloat3("point", scalePoint);

                if (ImGui::Button("Apply##Scale")) {
                    transformer->scaleFromPoint(Eigen::Vector3f(scaleFactor), Eigen::Vector3f(scalePoint));
                }

                // REFLECTION
                ImGui::SeparatorText("Reflection over an arbitrary plane");

                static float planePoint[3] = { 0, 0, 0 };
                ImGui::InputFloat3("point on plane", planePoint);

                static float planeNormal[3] = { 0, 1, 0 };
                ImGui::InputFloat3("plane normal", planeNormal);

                if (ImGui::Button("Apply##Reflect")) {
                    transformer->reflectOverPlane(Eigen::Vector3f(planePoint), Eigen::Vector3f(planeNormal));
                }

                // SHEARING
                ImGui::SeparatorText("Shearing");

                static int shearDir = 0;
                ImGui::RadioButton("X direction", &shearDir, 0); ImGui::SameLine();
                ImGui::RadioButton("Y direction", &shearDir, 1); ImGui::SameLine();
                ImGui::RadioButton("Z direction", &shearDir, 2);

                static float shearVec[2] = { 0.1, 0 };
                if (shearDir == 0) {
                    ImGui::InputFloat2("shear y, z components", shearVec);
                } else if (shearDir == 1) {
                    ImGui::InputFloat2("shear x, z components", shearVec);
                } else if (shearDir == 2) {
                    ImGui::InputFloat2("shear x, y components", shearVec);
                }

                if (ImGui::Button("Apply##Shear")) {
                    if (shearDir == 0) {
                        transformer->shearX(shearVec[0], shearVec[1]);
                    } else if (shearDir == 1) {
                        transformer->shearY(shearVec[0], shearVec[1]);
                    } else if (shearDir == 2) {
                        transformer->shearZ(shearVec[0], shearVec[1]);
                    }
                }

            }

            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

//        glUseProgram(p);
        glColor3f(model_color.x * clear_color.w, model_color.y * clear_color.w, model_color.z * clear_color.w);
        transformer->draw();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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
