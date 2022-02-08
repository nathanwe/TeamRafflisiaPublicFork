--FaithPlatethings
local imguiControledEntity = -1
local velX = {}
local velY = {}
local velz = {}
function SaveFaithPlates( levelnum )
	levelstr = string.format("%i", levelnum)
	SaveIntFloatTableAsJson(velX, "/Assets/Levels/Level" .. levelstr .."FaithPlateVelXSave.json")
	SaveIntFloatTableAsJson(velY, "/Assets/Levels/Level" .. levelstr .."FaithPlateVelYSave.json")
	SaveIntFloatTableAsJson(velZ, "/Assets/Levels/Level" .. levelstr .."FaithPlateVelZSave.json")
	--SaveIntFloatTableAsJson(directions, "/Assets/Levels/Level" .. levelstr .."FaithPlateDirectionSave.json")
end

function LoadFaithPlates( levelnum )
	levelstr = string.format("%i", levelnum)
	velX = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."FaithPlateVelXSave.json")
	velY = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."FaithPlateVelYSave.json")
	velZ = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."FaithPlateVelZSave.json")
	--directions = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."FaithPlateDirectionSave.json")
end

function ClearFaithPlates()
	velX = {}
	velY = {}
	velZ = {}
end

function DestroyFaithPlate(e)
	velX[e] = nil
	velY[e] = nil
	velZ[e] = nil
end

function UpdateFaithPlate(dt, e)
	
end

function HandleEventFaithPlate(eventData)

	if eventData.type == 16 then
		ImguiText("FaithPlate")
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
		SetPhysicsVelocity(eventData.e2, velX[eventData.e1], velY[eventData.e1], velZ[eventData.e1])
		PlayAudioEvent("BallJump")
	end
end



function HandleEventPerEntityFaithPlate(e, eventData)

end
