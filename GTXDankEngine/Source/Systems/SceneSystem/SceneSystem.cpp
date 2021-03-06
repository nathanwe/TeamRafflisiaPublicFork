#include "pch.h"
#include "SceneSystem.h"
#include "../../Core/Engine.h"
#include "../Components/ModelComponent/ModelComponent.h"
#include "../Components/TransformComponent/TransformComponent.h"
#include "../Components/MaterialComponent/MaterialComponent.h"
#include "../Components/LightComponent/LightComponent.h"
#include "../Components/PhysicsComponent/StillBodyComponent.h"
#include "../Components/PhysicsComponent/MovingBodyComponent.h"
#include "../Components/GameLogicCategoryComponent/GameLogicCategoryComponent.h"

#include "../ProfileSystem/ProfileSystem.h"


extern Engine engine;

bool SceneSystem::Init()
{
    /// opens the file with the last played level
    if (std::filesystem::exists(GAME_PATH + std::string("Assets/Levels/lastLevel")))
    {
        std::ifstream llFile(GAME_PATH + std::string("Assets/Levels/lastLevel"));
        std::ostringstream interpolate;
        interpolate << llFile.rdbuf();
        lastSavedLevel = stoi(interpolate.str());
    }

    /// load credits
    if (std::filesystem::exists(std::string("Assets/Credits/credits")))
    {
        std::ifstream llFile(std::string("Assets/Credits/credits"));
        std::string line;
        while (std::getline(llFile,line))
        {
            creditsText.push_back(line);
        }
    }

    /// HARDCODED
    engine.GraphicsSys.GetMenuSystem().GetMenu("Level Select")->DeactivateLevelButtons(lastSavedLevel);
    
    for (int i = 0; std::filesystem::exists(GAME_PATH + std::string("Assets/Levels/Level") + std::to_string(i) + std::string("/level.json")); ++i)
    {
        auto* handle = SerializationResourceManager.GetResourceHandleNoThread(GAME_PATH + std::string("Assets/Levels/Level")+ std::to_string(i) + std::string("/level.json"));
        levels.push_back(handle->GetPointer()->data);
    }
    

    LoadScene(-3);

    engine.CommandSys.GetCommand("NextLevel").SetActionToExecute([&]()
        {
            LoadNextLevel();
        });
    engine.CommandSys.GetCommand("PreviousLevel").SetActionToExecute([&]()
        {
            LoadPreviousLevel();
        });

	return true;
}

void SceneSystem::SaveLastLevel() const
{
    std::ofstream llFile(GAME_PATH + std::string("Assets/Levels/lastLevel"));
    llFile << lastSavedLevel;
}

void SceneSystem::UnloadScene()
{
    engine.EntitySys.DeleteAllEntities();
    Event ev = Event(true);
    ev.type = EventType::DESTROY_ALL_ENTITIYS;
    engine.DoGameLogicScriptSys.HandleEvent(ev);
}

void SceneSystem::LoadScene(int level)
{
    UnloadScene();
    shouldLoadLevel = true;
    levelToLoad = level;
    
}

void SceneSystem::LoadCurrentLevel()
{
    LoadScene(currentLevel);
}

void SceneSystem::LoadNextLevel()
{
    if (currentLevel < int(levels.size()))
    {
        currentLevel++;
    }
    LoadScene(currentLevel);
}

void SceneSystem::LoadPreviousLevel()
{
    if (currentLevel > 0)
    {
        currentLevel--;
    }
    LoadScene(currentLevel);
}

void SceneSystem::SaveCurrentLevel()
{
    ordered_json levelJson;

    for (auto entity : engine.EntitySys.allocatedEntities)
    {
        levelJson[entity] = engine.GameObjectFac.SerializeObject(entity);
    }

    levels[currentLevel] = levelJson;

    std::ofstream outputStream(GAME_PATH + std::string("Assets/Levels/Level") + std::to_string(currentLevel) + std::string("/level.json"));
    outputStream << levels[currentLevel].dump(2);

    Event ev = Event(true);
    ev.type = EventType::SAVE_LUA;
    ev.intData1 = currentLevel;
    engine.DoGameLogicScriptSys.HandleEvent(ev);
}

void SceneSystem::SaveAsNewLevel()
{
    ordered_json levelJson;

    for (auto entity : engine.EntitySys.allocatedEntities)
    {
        levelJson[entity] = engine.GameObjectFac.SerializeObject(entity);
    }

    int newLevel = static_cast<int>(levels.size());
    levels.push_back(levelJson);

    std::ofstream outputStream(GAME_PATH + std::string("Assets/Levels/Level") + std::to_string(newLevel) + std::string("/level.json"));
    outputStream << levels[newLevel].dump(2);

    Event ev = Event(true);
    ev.type = EventType::SAVE_LUA;
    ev.intData1 = newLevel;
    engine.DoGameLogicScriptSys.HandleEvent(ev);

    LoadScene(newLevel);
}

void SceneSystem::Update(float dt)
{
    PROFILE_THIS("Scene Update");
    
    if (!shouldLoadLevel && (currentLevel == -2 || currentLevel == -3))
    {
        PlayLogo(dt);
        return;
    }
    if (!shouldLoadLevel && currentLevel == levels.size())
    {
        PlayCredits(dt);
        return;
    }
    else if (shouldLoadLevel)
    {
        if (levelToLoad > lastSavedLevel && levelToLoad != levels.size())
        {
            /// HARDCODED
            Menu* levelMenu = engine.GraphicsSys.GetMenuSystem().GetMenu("Level Select");
            for (int i = lastSavedLevel; i <= levelToLoad; ++i)
            {
                if (levelMenu->GetButton("LEVEL " + std::to_string(i)) != nullptr)
                {
                    levelMenu->GetButton("LEVEL " + std::to_string(i))->active = true;
                }
            }

            lastSavedLevel = levelToLoad;
        }

        /// make sure cur level is updated
        currentLevel = levelToLoad;
        if (levelToLoad <= -2)
        {

            Event ev = Event(true);
            ev.type = EventType::LOAD_LUA;
            ev.intData1 = currentLevel;
            engine.DoGameLogicScriptSys.HandleEvent(ev);
            engine.CameraControlSys.HandleEvent(ev);

            // For Physics
            engine.PhysicsSys.UpdatePosition();
            engine.PhysicsSys.UpdateColliders();

            engine.GraphicsSys.GetParticleSystem().ResetEmitters();
            PlayLogo(dt);
        }
        else if (levelToLoad == levels.size())
        {

            Event ev = Event(true);
            ev.type = EventType::LOAD_LUA;
            ev.intData1 = currentLevel;
            engine.DoGameLogicScriptSys.HandleEvent(ev);
            engine.CameraControlSys.HandleEvent(ev);

            // For Physics
            engine.PhysicsSys.UpdatePosition();
            engine.PhysicsSys.UpdateColliders();

            engine.GraphicsSys.GetParticleSystem().ResetEmitters();
            PlayCredits(dt);
        }
        else if (levelToLoad == -1)
        {
            engine.GraphicsSys.GetMenuSystem().ToggleDisplay();
            engine.GraphicsSys.GetMenuSystem().SetCurrentMenu("Main");
        }
        else
        {
            ordered_json levelJson = levels[currentLevel];

            for (auto itr = levelJson.begin(); itr != levelJson.end(); ++itr)
            {
                ordered_json j = itr.value();
                Entity entity = engine.GameObjectFac.CreateObjectFromJson(j);
            }

            Event ev = Event(true);
            ev.type = EventType::LOAD_LUA;
            ev.intData1 = currentLevel;
            engine.DoGameLogicScriptSys.HandleEvent(ev);
            engine.CameraControlSys.HandleEvent(ev);

            // For Physics
            engine.PhysicsSys.UpdatePosition();
            engine.PhysicsSys.UpdateColliders();

            engine.GraphicsSys.GetParticleSystem().ResetEmitters();
        }
    }
    shouldLoadLevel = false;
}

bool SceneSystem::Destroy()
{
    return false;
}

int SceneSystem::GetCurrentLevel()
{
    return currentLevel;
}


void SceneSystem::PlayCredits(float dt)
{
    timer += dt;
    if (timer >= maxTime)
    {
        timer = 0.0f;
        ++curLine;
        if (curLine >= creditsText.size())
        {
            curLine = 0;
            engine.GraphicsSys.drawCredits = false;
            LoadScene(-1);
            return;
        }
    }
    /*
    std::string credit = creditsText[curLine];
//    std::replace(credit.begin(), credit.end(), '-', ' ');
    std::stringstream ss(credit);
    std::string name, role;
    std::getline(ss, name, '-');
    std::getline(ss, role, '-');
    engine.GraphicsSys.DrawCustomText(name, 2.3f, glm::vec2(engine.GraphicsSys.camera.width/2-400, 300), glm::vec3(1,0,0));
    engine.GraphicsSys.DrawCustomText(role, 1.5f, glm::vec2(engine.GraphicsSys.camera.width/2-700, 440), glm::vec3(1,0,0));*/
    engine.GraphicsSys.drawCredits = true;
    engine.GraphicsSys.credit = creditsText[curLine];
}

void SceneSystem::PlayLogo(float dt)
{
    timer += dt;
    if (currentLevel == -2)
    {
        if (timer >= 3.0f)
        {
            timer = 0.0f;
            engine.GraphicsSys.drawLogo = false;
            LoadNextLevel();
            return;
        }
        //engine.GraphicsSys.DrawLogo();
        engine.GraphicsSys.drawLogo = true;
    }
    else if (currentLevel == -3)
    {
        if (timer >= 3.0f)
        {
            timer = 0.0f;
            engine.GraphicsSys.drawCredits = false;
            LoadNextLevel();
            return;
        }
        engine.GraphicsSys.drawCredits = true;
        engine.GraphicsSys.credit = "TEAM RAFFLESIA PRODUCTION";
    }
}
