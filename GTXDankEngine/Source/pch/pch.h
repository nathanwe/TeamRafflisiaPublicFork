// Graphics--------------------------------------------------
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>


// profiling-----------------------------------------------------
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>


// loading assets-------------------------------
#include <json/json.h>

#include <stb/stb_image.h>
//#include <yaml-cpp/anchor.h>
//#include <yaml-cpp/yaml.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/pbrmaterial.h>



//---------------------------------------------------------
// std
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cerrno>
#include <unordered_map>
#include <cstdint>
#include <chrono>
#include <thread>
#include <queue>


// UI
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"