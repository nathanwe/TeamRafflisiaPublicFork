--HoneySlimethings
local imguiControledEntity = -1
local bouncyness = {}
local pitch=1.0
function SaveHoneySlimes( levelnum )
	levelstr = string.format("%i", levelnum)
	SaveIntFloatTableAsJson(bouncyness, "/Assets/Levels/Level" .. levelstr .."HoneySlimeBouncynessSave.json")
	--SaveIntFloatTableAsJson(directions, "/Assets/Levels/Level" .. levelstr .."HoneySlimeDirectionSave.json")
end

function LoadHoneySlimes( levelnum )
	levelstr = string.format("%i", levelnum)
	bouncyness = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."HoneySlimeBouncynessSave.json")
	--directions = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."HoneySlimeDirectionSave.json")
end

function ClearHoneySlimes()
	bouncyness = {}
end

function DestroyHoneySlime(e)
	bouncyness[e] = nil
end

function UpdateHoneySlime(dt, e)
	pitch = pitch + 0.01 * dt * 60
	if pitch > 1.0 then 
		pitch = 1.0
	end
	SetBGMPitch(pitch)
end

function HandleEventHoneySlime(eventData)

	if eventData.type == 16 then
		ImguiText("HoneySlime")
		ImguiControledFloat(0, "bouncyness", bouncyness[eventData.e1])
		imguiControledEntity = eventData.e1

	end
	if eventData.type == 17 then
		if imguiControledEntity ~= -1 then
			bouncyness[imguiControledEntity] = GetImguiControledFloat(0)
			--directions[imguiControledEntity] = GetImguiControledFloat(1)
			imguiControledEntity = -1
		end
	end
	if eventData.type == 12 then
		if bouncyness[eventData.e1] == nil then
			bouncyness[eventData.e1] = 1.0
		end
		x,y,z = GetPhysicsVelocity(eventData.e2)
		SetPhysicsVelocity(eventData.e2, x*bouncyness[eventData.e1], y*bouncyness[eventData.e1], z*bouncyness[eventData.e1])
		pitch = pitch - 0.02 * eventData.floatData1 * 60
		if pitch < 0.8 then 
			pitch = 0.8
		end
		SetBGMPitch(pitch)
	end
end



function HandleEventPerEntityHoneySlime(e, eventData)

end
