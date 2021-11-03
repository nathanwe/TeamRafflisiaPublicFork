#pragma once

#include "pch.h"

using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

////////////////////////////////// INT
inline void to_json(ordered_json& j, const int& x) {
	j = x;
}

inline void from_json(const ordered_json& j, int& x) {
	x = j.get<int>();
}
/////////////////////////////////

///////////////////////////////// UNSIGNED INT
inline void to_json(ordered_json& j, const unsigned int& x) {
	j = x;
}

inline void from_json(const ordered_json& j, unsigned int& x) {
	x = j.get<unsigned int>();
}
/////////////////////////////////

///////////////////////////////// FLOAT
inline void to_json(ordered_json& j, const float& x) {
	j = x;
}

inline void from_json(const ordered_json& j, float& x) {
	x = j.get<float>();
}
/////////////////////////////////

///////////////////////////////// STRING
inline void to_json(ordered_json& j, const std::string& x) {
	j = x.c_str();
}

inline void from_json(const ordered_json& j, std::string& x) {
	x = j.get<std::string>();
}
/////////////////////////////////

///////////////////////////////// BOOL
inline void to_json(ordered_json& j, const bool& x) {
	j = x;
}

inline void from_json(const ordered_json& j, bool& x) {
	x = j.get<bool>();
}
/////////////////////////////////

///////////////////////////////// VEC2
inline void to_json(ordered_json& j, const glm::vec2& x) {
	j = {
		{"x",x.x},
		{"y",x.y},
	};
}

inline void from_json(const ordered_json& j, glm::vec2& x) {
	j.at("x").get_to(x.x);
	j.at("y").get_to(x.y);
}

///////////////////////////////// VEC3
inline void to_json(ordered_json& j, const glm::vec3& x) {
	j = {
		{"x",x.x},
		{"y",x.y},
		{"z",x.z},
	};
}

inline void from_json(const ordered_json& j, glm::vec3& x) {
	j.at("x").get_to(x.x);
	j.at("y").get_to(x.y);
	j.at("z").get_to(x.z);
}

///////////////////////////////// VEC4
inline void to_json(ordered_json& j, const glm::vec4& x) {
	j = {
		{"x",x.x},
		{"y",x.y},
		{"z",x.z},
		{"w",x.w},
	};
}

inline void from_json(const ordered_json& j, glm::vec4& x) {
	j.at("x").get_to(x.x);
	j.at("y").get_to(x.y);
	j.at("z").get_to(x.z);
	j.at("w").get_to(x.w);
}

///////////////////////////////// MAT4
inline void to_json(ordered_json& j, const glm::mat4& x) {
	j = {
		{x[0][0],x[1][0],x[2][0],x[3][0]},
		{x[0][1],x[1][1],x[2][1],x[3][1]},
		{x[0][2],x[1][2],x[2][2],x[3][2]},
		{x[0][3],x[1][3],x[2][3],x[3][3]}
	};
}

inline void from_json(const ordered_json& j, glm::mat4& x) {
	for (int k = 0; k < 4; ++k) {
		for (int l = 0; l < 4; ++l) {
			j.at(k).at(l).get_to(x[k][l]);
		}
	}
}

///////////////////////////////// quat
inline void to_json(ordered_json& j, const glm::quat& x) {
	j = {
		{"x",x.x},
		{"y",x.y},
		{"z",x.z},
		{"w",x.w},
	};
}

inline void from_json(const ordered_json& j, glm::quat& x) {
	j.at("x").get_to(x.x);
	j.at("y").get_to(x.y);
	j.at("z").get_to(x.z);
	j.at("w").get_to(x.w);
}

template<typename T>
inline void from_json(const ordered_json& j, T* arr) {
	std::vector<T> v;
	j.get_to(v);
	for (int i = 0; i < v.size(); i++)
	{
		arr[i] = v[i];
	}
}
