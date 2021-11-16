#include "pch.h"
#include "LevelEditorSystem.h"
#include "../../Core/Engine.h"
#include "../../Components/ModelComponent/ModelComponent.h"
#include "../../Components/TransformComponent/TransformComponent.h"
#include "../../Components/MaterialComponent/MaterialComponent.h"
#include "../../Components/LightComponent/LightComponent.h"
#include "../../Components/PhysicsComponent/StillBodyComponent.h"
#include "../../Components/PhysicsComponent/MovingBodyComponent.h"
#include "../../Components/TagComponent/TagComponent.h"
#include "../../Components/GameLogicCategoryComponent/GameLogicCategoryComponent.h"

extern Engine engine;

//for dynamic resizeing of std::string field
//referenced from https://github.com/ocornut/imgui/issues/2035
static int InputTextCallback(ImGuiInputTextCallbackData* data)
{
	if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
	{
		// Resize string callback
		std::string* str = (std::string*)data->UserData;
		IM_ASSERT(data->Buf == str->c_str());
		str->resize(data->BufTextLen);
		data->Buf = (char*)str->c_str();
		return 1;
	}
	return 0;
}

void CopyButton(std::string buttonName, std::string dataToCopy)
{
	if (ImGui::Button(buttonName.c_str(), ImVec2(32, 20)))
	{
		if (!OpenClipboard(0))
		{
			return;
		}
		if (!EmptyClipboard())
		{
			return;
		}
		HGLOBAL hGlob = GlobalAlloc(GMEM_FIXED, 64);
		if(dataToCopy.size() > 0)
			strcpy_s((char*)hGlob, 64, (dataToCopy).c_str());
		if (::SetClipboardData(CF_TEXT, hGlob) == NULL)
		{
			CloseClipboard();
			GlobalFree(hGlob);
			return;
		}
		CloseClipboard();
	}
}

std::string PasteFromClipboard()
{
	if (!OpenClipboard(0))
	{
		return "";
	}
	HANDLE hData = GetClipboardData(CF_TEXT);
	if (hData == nullptr)
	{
		//error
		return "";
	}
	char* pszText = static_cast<char*>(GlobalLock(hData));
	if (pszText == nullptr)
	{
		//error
		return "";
	}
	std::string text(pszText);
	GlobalUnlock(hData);
	CloseClipboard();
	return text;
}

bool InputText(const char* label, std::string* str, ImGuiInputTextFlags flags)
{
	flags |= ImGuiInputTextFlags_CallbackResize;
	return ImGui::InputText(label, (char*)str->c_str(), str->capacity() + 1, flags, InputTextCallback, (void*)str);
}

static auto VectorGetter = [](void* vec, int idx, const char** out_text)
{
	auto& v = *static_cast<std::vector<std::string>*>(vec);
	if (idx < 0 || idx >= static_cast<int>(v.size())) 
	{ 
		return false; 
	}
	*out_text = v.at(idx).c_str();
	return true;
};


bool Combo(const char* label, int* currIndex, std::vector<std::string>& values)
{
	if (values.empty()) { return false; }
	return ImGui::Combo(label, currIndex, VectorGetter,
		static_cast<void*>(&values), values.size());
}

LevelEditorSystem::LevelEditorSystem()
{

}

bool LevelEditorSystem::Init()
{
	return true;
}

void LevelEditorSystem::Update(float timeStamp)
{
}

bool LevelEditorSystem::Destroy()
{
	return false;
}

void LevelEditorSystem::UpdateGUI()
{
	ImGui::Begin("Level Editor");
	{
		std::string text = "Current Level: " + std::to_string(engine.SceneSys.GetCurrentLevel());
		ImGui::Text(text.c_str());
		if (ImGui::Button("Save Level"))
		{
			engine.SceneSys.SaveCurrentLevel();
		}
		ImGui::SameLine();
		if (ImGui::Button("Save as New Level"))
		{
			engine.SceneSys.SaveAsNewLevel();
		}
		ImGui::SameLine();
		if (ImGui::Button("Reset/Reload Level"))
		{
			engine.SceneSys.LoadCurrentLevel();
		}

		//todo optimization, use reference + only convert the list when changed
		auto names = engine.GameObjectFac.GetTemplateObjectNames();
		if (ImGui::Button("Add New Entity"))
		{
			if (entityCategoryIndex > 0)
			{
				entityID = engine.GameObjectFac.CreateObject(names[entityCategoryIndex]);
			}
			else
			{
				entityID = engine.EntitySys.CreateEntity();
			}
		}
		ImGui::SameLine();
		//InputText("Entity Category: ", &entityCategory, ImGuiInputTextFlags_CallbackCharFilter);
		Combo("Entity Category", &entityCategoryIndex, names);

		if (ImGui::Button("Save Entity"))
		{
			engine.GameObjectFac.SaveObject(entityID);
		}
		ImGui::SameLine();
		if (ImGui::Button("Delete Entity"))
		{
			engine.EntitySys.DestroyEntity(entityID);
		}

		ImGui::InputInt("Entity ID", &entityID);

		if (ImGui::CollapsingHeader("Tag"))
		{
			auto* tagCom = TagComponentPool.GetComponentByEntity(entityID);
			if (tagCom != nullptr)
			{
				InputText("Unique Name",
					&tagCom->uniqueName,
					ImGuiInputTextFlags_CallbackCharFilter);
				InputText("Category Name",
					&tagCom->category,
					ImGuiInputTextFlags_CallbackCharFilter);
			}
			else
			{
				if (ImGui::Button("Add Tag"))
				{
					TagComponentPool.Add(entityID);
				}
			}
		}

		if (ImGui::CollapsingHeader("Transform"))
		{
			auto* transCom = TransformComponentPool.GetComponentByEntity(entityID);
			if (transCom != nullptr)
			{
				ImGui::DragFloat3("Position", &transCom->transform.position.x, 0.01f);
				ImGui::DragFloat("Scale", &transCom->transform.scale, 0.001f);
				ImGui::DragFloat4("Rotation", &transCom->transform.rotation.x, 0.01f);
			}
			else
			{
				if (ImGui::Button("Add Transform"))
				{
					VQS vqs = VQS();
					TransformComponentPool.Add(entityID, vqs);
				}
			}
		}

		if (ImGui::CollapsingHeader("Model"))
		{
			auto* modelCom = ModelComponentPool.GetComponentByEntity(entityID);
			if (modelCom != nullptr)
			{

				InputText("ModelPath",
					&modelCom->model->GetPointer()->path,
					ImGuiInputTextFlags_CallbackCharFilter);
				ImGui::SameLine();
				CopyButton("CopyModelPath", modelCom->model->GetPointer()->path);
				ImGui::SameLine();
				if (ImGui::Button("PasteModelPath", ImVec2(32, 20)))
				{
					modelCom->model->GetPointer()->path = PasteFromClipboard();
				}
			}
			else
			{
				if (ImGui::Button("Add Model"))
				{
					ResourceHandle<Model>* model = ModelResourceManager.GetResourceHandle("Assets/Defaults/Sphere.obj");
					ModelComponentPool.Add(entityID, model);
				}
			}
		}

		if (ImGui::CollapsingHeader("Material"))
		{
			auto* matCom = MaterialComponentPool.GetComponentByEntity(entityID);
			if (matCom != nullptr)
			{
				InputText("AlbedoPath",
					&matCom->material.AlbedoPath,
					ImGuiInputTextFlags_CallbackCharFilter);
				ImGui::SameLine();
				CopyButton("CopyAlbedoPath", matCom->material.AlbedoPath);
				ImGui::SameLine();
				if (ImGui::Button("PasteAlbedoPath", ImVec2(32, 20)))
				{
					matCom->material.AlbedoPath = PasteFromClipboard();
				}

				InputText("MetallicPath",
					&matCom->material.MetallicPath,
					ImGuiInputTextFlags_CallbackCharFilter);
				ImGui::SameLine();
				CopyButton("CopyMetallicPath", matCom->material.MetallicPath);
				ImGui::SameLine();
				if (ImGui::Button("PasteMetallicPath", ImVec2(32, 20)))
				{
					matCom->material.MetallicPath = PasteFromClipboard();
				}

				InputText("NormalPath",
					&matCom->material.NormalPath,
					ImGuiInputTextFlags_CallbackCharFilter);
				ImGui::SameLine();
				CopyButton("CopyNormalPath", matCom->material.NormalPath);
				ImGui::SameLine();
				if (ImGui::Button("PasteNormalPath", ImVec2(32, 20)))
				{
					matCom->material.NormalPath = PasteFromClipboard();
				}

				InputText("RoughnessPath",
					&matCom->material.RoughnessPath,
					ImGuiInputTextFlags_CallbackCharFilter);
				ImGui::SameLine();
				CopyButton("CopyRoughnessPath", matCom->material.RoughnessPath);
				ImGui::SameLine();
				if (ImGui::Button("PasteRoughnessPath", ImVec2(32, 20)))
				{
					matCom->material.RoughnessPath = PasteFromClipboard();
				}

				ImGui::DragFloat("Alpha", &matCom->material.Alpha, 0.01f);
			}
			else
			{
				if (ImGui::Button("Add Material"))
				{
					std::string path = "Assets/Defaults/BlackPurpleChecker.png";
					ResourceHandle<Texture>* dummy = TextureResourceManger.GetResourceHandle(path);
					Material material = Material(dummy, dummy, dummy, dummy);
					material.AlbedoPath = path;
					material.MetallicPath = path;
					material.NormalPath = path;
					material.RoughnessPath = path;
					MaterialComponentPool.Add(entityID, material);
				}
			}
		}

		if (ImGui::CollapsingHeader("Light"))
		{
			auto* ligthCom = LightComponentPool.GetComponentByEntity(entityID);
			if (ligthCom != nullptr)
			{
				ImGui::DragFloat3("Color", &ligthCom->LightSource.Color.x, 0.01f);
				ImGui::DragFloat3("Intensity", &ligthCom->LightSource.Intensity.x, 0.01f);
				ImGui::DragFloat3("Target", &ligthCom->LightSource.Target.x, 0.01f);
				ImGui::InputInt("Type", reinterpret_cast<int*>(&ligthCom->LightSource.Type));
			}
			else
			{
				if (ImGui::Button("Add Light"))
				{
					Light light = Light();
					LightComponentPool.Add(entityID, light);
				}
			}
		}

		if (ImGui::CollapsingHeader("StillBody"))
		{
			auto* stillBodyCom = StillBodyComponentPool.GetComponentByEntity(entityID);
			if (stillBodyCom != nullptr)
			{
				ImGui::InputInt("Shape", reinterpret_cast<int*>(&stillBodyCom->BroadPhase.shape));
				if (stillBodyCom->BroadPhase.shape == Shape::SPHERE)
				{
					ImGui::DragFloat("Radius", &stillBodyCom->BroadPhase.radius, 0.01f);
				}
				else if (stillBodyCom->BroadPhase.shape == Shape::PLANE)
				{
					ImGui::DragFloat3("Normal", &stillBodyCom->BroadPhase.normal.x, 0.01f);
					ImGui::DragFloat("Magnitude", &stillBodyCom->BroadPhase.magnitude, 0.01f);
				}
				else if (stillBodyCom->BroadPhase.shape == Shape::AABB)
				{
					ImGui::DragFloat3("Normal", &stillBodyCom->BroadPhase.minPoint.x, 0.01f);
					ImGui::DragFloat3("Normal", &stillBodyCom->BroadPhase.max_point.x, 0.01f);
				}
			}
			else
			{
				if (ImGui::Button("Add StillBody"))
				{
					StillBodyComponentPool.Add(entityID);
				}
			}
		}

		if (ImGui::CollapsingHeader("MovingBody"))
		{
			auto* movingBodyCom = MovingBodyComponentPool.GetComponentByEntity(entityID);
			if (movingBodyCom != nullptr)
			{
				ImGui::InputInt("Shape", reinterpret_cast<int*>(&movingBodyCom->BroadPhase.shape));
				if (movingBodyCom->BroadPhase.shape == Shape::SPHERE)
				{
					ImGui::DragFloat("Radius", &movingBodyCom->BroadPhase.radius, 0.01f);
				}
				else if (movingBodyCom->BroadPhase.shape == Shape::PLANE)
				{
					ImGui::DragFloat3("Normal", &movingBodyCom->BroadPhase.normal.x, 0.01f);
					ImGui::DragFloat("Magnitude", &movingBodyCom->BroadPhase.magnitude, 0.01f);
				}
				else if (movingBodyCom->BroadPhase.shape == Shape::AABB)
				{
					ImGui::DragFloat3("Normal", &movingBodyCom->BroadPhase.minPoint.x, 0.01f);
					ImGui::DragFloat3("Normal", &movingBodyCom->BroadPhase.max_point.x, 0.01f);
				}

				ImGui::DragFloat("elasticity", &movingBodyCom->rigidBody.elasticity, 0.01f);
				ImGui::DragFloat("friction", &movingBodyCom->rigidBody.friction, 0.01f);
				ImGui::Checkbox("isGravity", &movingBodyCom->rigidBody.isGravity);
				ImGui::DragFloat("mass", &movingBodyCom->rigidBody.mass, 0.01f);
				ImGui::DragFloat3("velocity", &movingBodyCom->rigidBody.velocity.x, 0.01f);
				ImGui::DragFloat3("acceleration", &movingBodyCom->rigidBody.acceleration.x, 0.01f);
				ImGui::InputInt("collisionType", reinterpret_cast<int*>(&movingBodyCom->rigidBody.collisionType));

			}
			else
			{
				if (ImGui::Button("Add MovingBody"))
				{
					RigidBody rigidBody = RigidBody();
					MovingBodyComponentPool.Add(entityID, rigidBody);
					Collider* collider = new Collider();
					MovingBodyComponentPool.GetComponentByEntity(entityID)->BroadPhase = *collider;
				}
			}
		}

		if (ImGui::CollapsingHeader("GameLogic(unfinished)"))
		{
			auto* gameLogicCom = GameLogicCategoryComponentPool.GetComponentByEntity(entityID);
			for (auto& category : gameLogicCom->categories)
			{
				
			}
		}
	}
	ImGui::End();
}
