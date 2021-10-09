#pragma warning(push, 0)
// Graphics--------------------------------------------------
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/quaternion.hpp>


// profiling-----------------------------------------------------
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>


// loading assets-------------------------------
//#pragma warning(push)
//#pragma warning(disable:26819)
#include <json/json.h>
//#pragma warning(pop)

#include <stb/stb_image.h>
//#include <yaml-cpp/anchor.h>
//#include <yaml-cpp/yaml.h>


//#pragma warning(push)
//#pragma warning(disable:26812)
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/pbrmaterial.h>
//#pragma warning(pop)

//-------------------------------------------------------
//sound
//#pragma warning(push)
//#pragma warning(disable:26812)
#include "fmod_studio.hpp"
#include "fmod.hpp"
//#pragma warning(pop)

//---------------------------------------------------------
// std
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cerrno>
#include <map>
#include <unordered_map>
#include <set>
#include <list>
#include <cstdint>
#include <chrono>
#include <queue>

#include <thread>
#include<mutex>
#include<condition_variable>



// UI
//#pragma warning(push)
//#pragma warning(disable:26451)
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
//#pragma warning(pop)
#pragma warning(pop)



