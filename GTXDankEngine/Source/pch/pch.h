#pragma warning(push)
#pragma warning(disable : 26812)
#pragma warning(disable : 26819)
#pragma warning(disable : 26451)
#pragma warning(disable : 26459)
#pragma warning(disable : 26495)
#pragma warning(disable : 6387)
// Graphics--------------------------------------------------
//windows has to be included before glad
#include <Windows.h>
#include <windows.h>

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
#include <json/json.h>


#include <stb/stb_image.h>


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/pbrmaterial.h>
#include <filesystem>


//-------------------------------------------------------
//sound
#include "fmod_studio.hpp"
#include "fmod.hpp"


//---------------------------------------------------------
// std
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
//#include <iostream>
#include <iomanip>
#include <cerrno>
#include <map>
#include <unordered_map>
#include <set>
#include <list>
#include <cstdint>
#include <chrono>
#include <queue>
#include <assert.h>

#include <thread>
#include<mutex>
#include<condition_variable>

#include <conio.h>
#include <Objbase.h>


// UI
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


// scripting
#include "../../Libraries/include/lua/lua.hpp"


#pragma warning(pop)
