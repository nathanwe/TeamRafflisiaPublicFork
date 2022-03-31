#include "pch.h"
#include "LevelEditorSystem.h"
#include "../../Core/Engine.h"
#include "../../Components/ModelComponent/ModelComponent.h"
#include "../../Components/TransformComponent/TransformComponent.h"
#include "../../Components/MaterialComponent/MaterialComponent.h"
#include "../../Components/LightComponent/LightComponent.h"
#include "../../Components/PhysicsComponent/StillBodyComponent.h"
#include "../../Components/PhysicsComponent/MovingBodyComponent.h"
#include "../../Components/PhysicsComponent/ColliderComponent.h"
#include "../../Components/TagComponent/TagComponent.h"
#include "../../Components/GameLogicCategoryComponent/GameLogicCategoryComponent.h"
#include "../PhysicsSystem/Raycasting/Raycasting.h"

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
	//level editor mode, assume it wouldnt change a lot so command pattern is not applied here
	if (engine.InputSys.IsLeftMouseTriggered())
	{
		if (ImGui::IsAnyItemHovered())
			return;
		auto e = RayCast(engine.GraphicsSys.camera, false);
		prevEntityID = entityID;
		if(e != -1)
			engine.LevelEditorSys.entityID = e;
		ChangeSelectedObject();
		//LOG_INFO("Selected Entity");
		//LOG_INFO(e);
	}
}

bool LevelEditorSystem::Destroy()
{
	return true;
}

void LevelEditorSystem::UpdateGUI()
{
	prevEntityID = entityID;
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
				LOG_INFO("Game Object created");
				entityID = engine.GameObjectFac.CreateObject(names[entityCategoryIndex]);
			}
			else
			{
				LOG_INFO("Empty Game Object created");
				entityID = engine.EntitySys.CreateEntity();
			}
			ChangeSelectedObject();
		}
		ImGui::SameLine();
		//InputText("Entity Category: ", &entityCategory, ImGuiInputTextFlags_CallbackCharFilter);
		Combo("Entity Category", &entityCategoryIndex, names);

		if (ImGui::Button("Save Entity"))
		{
			auto* tag = TagComponentPool.GetComponentByEntity(entityID);
			if (tag == nullptr || tag->uniqueName.empty() || tag->category.empty())
			{
				LOG_INFO("Tag Component (Name/Category) cannot be empty.");
			}
			else
			{
				LOG_INFO("Game Object Saved");
				engine.GameObjectFac.SaveObject(entityID);
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Delete Entity"))
		{
			LOG_INFO("Game Object Deleted");
			engine.EntitySys.DestroyEntity(entityID);
		}

		if (ImGui::InputInt("Entity ID", &entityID))
		{
			ChangeSelectedObject();
		}

		if (!engine.EntitySys.IsEntityActive(entityID))
		{
			ImGui::End();
			return;
		}

		if (ImGui::CollapsingHeader("Name (Tag)"))
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
				LOG_INFO("Name tag is added to this entity automatically");
				TagComponentPool.Add(entityID);
			}
		}

		if (ImGui::CollapsingHeader("Transform"))
		{
			auto* transCom = TransformComponentPool.GetComponentByEntity(entityID);
			if (transCom != nullptr)
			{
				ImGui::DragFloat3("Position", &transCom->transform.position.x, 0.01f);
				ImGui::DragFloat3("Scale", &transCom->transform.scale.x, 0.001f);
				if (transCom->transform.scale.x < 0)
					transCom->transform.scale.x = 0;
				if (transCom->transform.scale.y < 0)
					transCom->transform.scale.y = 0;
				if (transCom->transform.scale.z < 0)
					transCom->transform.scale.z = 0;
				//ImGui::DragFloat4("Rotation", &transCom->transform.rotation.x, 0.01f);
				auto eulerAngle = glm::eulerAngles(transCom->transform.rotation) / glm::pi<float>() * 180.0f;
				ImGui::DragFloat("Euler Angle X", &eulerAngle.x, 1.0f);
				ImGui::DragFloat("Euler Angle Y", &eulerAngle.y, eulerAngleIncrement ? -1.0f : 1.0f);
				ImGui::DragFloat("Euler Angle Z", &eulerAngle.z, 1.0f);
				eulerAngleIncrement = eulerAngle.y > 90 || eulerAngle.y < -90 ? !eulerAngleIncrement : eulerAngleIncrement;
				transCom->transform.rotation = glm::quat(eulerAngle * glm::pi<float>() / 180.0f);
				eulerAngle = glm::eulerAngles(transCom->transform.rotation);
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
				CopyButton("CopyModelPath", modelCom->model->GetPointer()->path);
				ImGui::SameLine();
				if (ImGui::Button("PasteModelPath", ImVec2(32, 20)))
				{
					modelCom->model->GetPointer()->path = PasteFromClipboard();
				}
				ImGui::SameLine();
				if (ImGui::Button("RefreshModel", ImVec2(32, 20)))
				{
					auto path = modelCom->model->GetPointer()->path;
					if (!path.empty())
					{
						ModelComponentPool.Delete(entityID);
						ResourceHandle<Model>* model = ModelResourceManager.GetResourceHandle(path);
						ModelComponentPool.Add(entityID, model);
					}
					else
					{
						LOG_INFO("model path cannot be empty!");
					}
				}
				ImGui::SameLine();
				InputText("ModelPath",
					&modelCom->model->GetPointer()->path,
					ImGuiInputTextFlags_CallbackCharFilter);

				if (ImGui::Button("Remove Model"))
				{
					ModelComponentPool.Delete(entityID);
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
				CopyButton("CopyAlbedoPath", matCom->material.AlbedoPath);
				ImGui::SameLine();
				if (ImGui::Button("PasteAlbedoPath", ImVec2(32, 20)))
				{
					matCom->material.AlbedoPath = PasteFromClipboard();
				}
				ImGui::SameLine();
				InputText("AlbedoPath",
					&matCom->material.AlbedoPath,
					ImGuiInputTextFlags_CallbackCharFilter);

				CopyButton("CopyMetallicPath", matCom->material.MetallicPath);
				ImGui::SameLine();
				if (ImGui::Button("PasteMetallicPath", ImVec2(32, 20)))
				{
					matCom->material.MetallicPath = PasteFromClipboard();
				}
				ImGui::SameLine();
				InputText("MetallicPath",
					&matCom->material.MetallicPath,
					ImGuiInputTextFlags_CallbackCharFilter);


				ImGui::DragFloat("Alpha", &matCom->material.Alpha, 0.01f);

				if (ImGui::Button("Refresh Material"))
				{
					auto albedoPath = matCom->material.AlbedoPath;
					auto metallicPath = matCom->material.MetallicPath;

					if (!albedoPath.empty())
					{
						MaterialComponentPool.Delete(entityID);
						ResourceHandle<Texture>* albedo = TextureResourceManger.GetResourceHandle(albedoPath);
						ResourceHandle<Texture>* metallic = TextureResourceManger.GetResourceHandle(metallicPath);

						Material material = Material(albedo, metallic);
						material.AlbedoPath = albedoPath;
						material.MetallicPath = metallicPath;

						MaterialComponentPool.Add(entityID, material);
					}
				}

				if (ImGui::Button("Remove Material"))
				{
					MaterialComponentPool.Delete(entityID);
				}
			}
			else
			{
				if (ImGui::Button("Add Material"))
				{
					std::string path = "Assets/Defaults/BlackPurpleChecker.png";
					ResourceHandle<Texture>* dummy = TextureResourceManger.GetResourceHandle(path);
					Material material = Material(dummy, dummy);
					material.AlbedoPath = path;
					material.MetallicPath = path;
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

				if (ImGui::Button("Remove Light"))
				{
					LightComponentPool.Delete(entityID);
				}
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
		if (ImGui::CollapsingHeader("Collider"))
		{
			auto* collider = ColliderComponentPool.GetComponentByEntity(entityID);
			if (collider != nullptr)
			{
				ImGui::InputInt("Shape", reinterpret_cast<int*>(&collider->NarrowPhase.shape));
				if (collider->NarrowPhase.shape == Shape::SPHERE || collider->NarrowPhase.shape == Shape::NIL)
				{
					ImGui::Text("Sphere");
					ImGui::DragFloat("Radius", &collider->NarrowPhase.radius, 0.01f);
					if (collider->NarrowPhase.radius < 0)
						collider->NarrowPhase.radius = 0;
				}
				if (collider->NarrowPhase.shape == Shape::PLANE || collider->NarrowPhase.shape == Shape::NIL)
				{
					ImGui::Text("Plane");
					ImGui::DragFloat3("Normal", &collider->NarrowPhase.normal.x, 0.01f);
					ImGui::DragFloat("Magnitude", &collider->NarrowPhase.magnitude, 0.01f);
					ImGui::DragFloat3("p1", &collider->NarrowPhase.p1.x, 0.01f);
					ImGui::DragFloat3("p2", &collider->NarrowPhase.p2.x, 0.01f);
					ImGui::DragFloat3("p3", &collider->NarrowPhase.p3.x, 0.01f);
					ImGui::DragFloat3("p4", &collider->NarrowPhase.p4.x, 0.01f);
				}
				if (collider->NarrowPhase.shape == Shape::AABB || collider->NarrowPhase.shape == Shape::NIL)
				{
					ImGui::Text("AABB");
					ImGui::DragFloat3("MinPoint", &collider->NarrowPhase.minPoint.x, 0.01f);
					ImGui::DragFloat3("MaxPoint", &collider->NarrowPhase.maxPoint.x, 0.01f);
				}
			}
			else
			{
				if (ImGui::Button("Add Collider"))
				{
					ColliderComponentPool.Add(entityID);
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
					ImGui::DragFloat3("MinPoint", &stillBodyCom->BroadPhase.minPoint.x, 0.01f);
					ImGui::DragFloat3("MaxPoint", &stillBodyCom->BroadPhase.maxPoint.x, 0.01f);
				}

				if (ImGui::Button("Remove StillBody"))
				{
					StillBodyComponentPool.Delete(entityID);
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
					ImGui::DragFloat3("MinPoint", &movingBodyCom->BroadPhase.minPoint.x, 0.01f);
					ImGui::DragFloat3("MaxPoint", &movingBodyCom->BroadPhase.maxPoint.x, 0.01f);
				}

				ImGui::DragFloat("elasticity", &movingBodyCom->rigidBody.elasticity, 0.01f);
				ImGui::DragFloat("friction", &movingBodyCom->rigidBody.friction, 0.01f);
				ImGui::Checkbox("isGravity", &movingBodyCom->rigidBody.isGravity);
				ImGui::DragFloat("mass", &movingBodyCom->rigidBody.mass, 0.01f);
				ImGui::DragFloat3("velocity", &movingBodyCom->rigidBody.velocity.x, 0.01f);
				ImGui::DragFloat3("acceleration", &movingBodyCom->rigidBody.acceleration.x, 0.01f);
				ImGui::InputInt("collisionType", reinterpret_cast<int*>(&movingBodyCom->rigidBody.collisionType));

				if (ImGui::Button("Remove MovingBody"))
				{
					MovingBodyComponentPool.Delete(entityID);
				}
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
			if (gameLogicCom != nullptr)
			{
				for (int i = 1; i < static_cast<int>(GameLogicCategories::MAX_CATEGORIES); ++i)
				{
					
					if (gameLogicCom->categories.find(static_cast<GameLogicCategories>(i)) != gameLogicCom->categories.end())
					{
						if (ImGui::Button((std::string("remove game logic ") + std::to_string(i)).c_str()))
						{
							gameLogicCom->categories.erase((static_cast<GameLogicCategories>(i)));
						}
						Event ev = Event();
						ev.thingsToEffect.insert(static_cast<GameLogicCategories>(i));
						ev.type = EventType::SEND_DATA_TO_IMGUI;
						ev.e1 = entityID;
						engine.DoGameLogicScriptSys.HandleEvent(ev);
					}
					else
					{
						if (ImGui::Button((std::string("Add game logic ") + std::to_string(i)).c_str()))
						{
							gameLogicCom->categories.insert(static_cast<GameLogicCategories>(i));
						}
					}
				}
			}
			else
			{
				if (ImGui::Button("Add GameLogic"))
				{
					GameLogicCategoryComponentPool.Add(entityID, (std::vector<GameLogicCategories>()));
				}
			}
		}


		//add new component above here
	}
	ImGui::End();
}

void LevelEditorSystem::ChangeSelectedObject()
{
	if (engine.EntitySys.IsEntityActive(prevEntityID))
	{
		auto* mat = MaterialComponentPool.GetComponentByEntity(prevEntityID);
		if (mat != nullptr)
		{
			mat->material.wireMode = false;
		}
	}
	if (engine.EntitySys.IsEntityActive(entityID))
	{
		auto* mat = MaterialComponentPool.GetComponentByEntity(entityID);
		if (mat != nullptr)
		{
			mat->material.wireMode = true;
		}
	}
}

