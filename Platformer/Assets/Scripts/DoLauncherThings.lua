--Launcherthings
local imguiControledEntity = -1
local velX = {}
local velY = {}
local velz = {}
function SaveLaunchers( levelnum )
	levelstr = string.format("%i", levelnum)
	SaveIntFloatTableAsJson(velX, "/Assets/Levels/Level" .. levelstr .."/LauncherVelXSave.json")
	SaveIntFloatTableAsJson(velY, "/Assets/Levels/Level" .. levelstr .."/LauncherVelYSave.json")
	SaveIntFloatTableAsJson(velZ, "/Assets/Levels/Level" .. levelstr .."/LauncherVelZSave.json")
end

function LoadLaunchers( levelnum )
	levelstr = string.format("%i", levelnum)
	velX = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."/LauncherVelXSave.json")
	velY = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."/LauncherVelYSave.json")
	velZ = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."/LauncherVelZSave.json")
end

function ClearLaunchers()
	velX = {}
	velY = {}
	velZ = {}
end

function DestroyLauncher(e)
	velX[e] = nil
	velY[e] = nil
	velZ[e] = nil
end

function UpdateLauncher(dt, e)
	
end

function HandleEventLauncher(eventData)

	if eventData.type == 16 then
		ImguiText("Launcher")
		ImguiControledFloat(0, "x", velX[eventData.e1])
		ImguiControledFloat(1, "y", velY[eventData.e1])
		ImguiControledFloat(2, "z", velZ[eventData.e1])
		imguiControledEntity = eventData.e1

	end
	if eventData.type == 17 then
		if imguiControledEntity ~= -1 then
			velX[imguiControledEntity] = GetImguiControledFloat(0)
			velY[imguiControledEntity] = GetImguiControledFloat(1)
			velZ[imguiControledEntity] = GetImguiControledFloat(2)
			--directions[imguiControledEntity] = GetImguiControledFloat(1)
			imguiControledEntity = -1
		end
	end
	if eventData.type == 12 then
		if velX[eventData.e1] == nil then
			velX[eventData.e1] = 0.0
			LOG_INFO("x was nil")
		end
		if velY[eventData.e1] == nil then
			velY[eventData.e1] = 0.0
			LOG_INFO("y was nil")
		end
		if velZ[eventData.e1] == nil then
			velZ[eventData.e1] = 0.0
			LOG_INFO("z was nil")
		end
		playerData = {}
		playerData = GetRigidData(eventData.e2)
		dotProduct = velX[eventData.e1]*playerData.velocity.x + velY[eventData.e1]*playerData.velocity.y + velZ[eventData.e1]*playerData.velocity.z
		if dotProduct > 0 then --  if you are moving tward the plate
			SetPhysicsVelocity(eventData.e2, velX[eventData.e1], velY[eventData.e1], velZ[eventData.e1])
			PlayAudioEvent("BallJump")
		end
	end
end



function HandleEventPerEntityLauncher(e, eventData)

end
