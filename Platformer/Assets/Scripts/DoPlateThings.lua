--Platethings
local imguiControledEntity = -1
local blockStatus = {} -- not the blockStatus from doblockthings, that does nothing now
local target = {}
local targetStatus = {}
local defaultSolid = {}
local targetSolidShape = {}


function SavePlates( levelnum )
	levelstr = string.format("%i", levelnum)
	SaveIntFloatTableAsJson(target, "/Assets/Levels/Level" .. levelstr .."/targetSave.json")
	SaveIntFloatTableAsJson(targetStatus, "/Assets/Levels/Level" .. levelstr .."/targetStatusSave.json")
	SaveIntFloatTableAsJson(defaultSolid, "/Assets/Levels/Level" .. levelstr .."/defaultSolidSave.json")
	SaveIntFloatTableAsJson(targetSolidShape, "/Assets/Levels/Level" .. levelstr .."/targetSolidShapeSave.json")
end

function LoadPlates( levelnum )
	levelstr = string.format("%i", levelnum)
	target = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."/targetSave.json")
	targetStatus = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."/targetStatusSave.json")
	defaultSolid = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."/defaultSolidSave.json")
	targetSolidShape = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."/targetSolidShapeSave.json")
end

function ClearPlates()

end

function DestroyPlate(e)

end

function UpdatePlate(dt, e)
	if targetStatus[e] == nil then
		targetStatus[e] = 0
	end
	if defaultSolid[e] == nil then
		defaultSolid[e] = 0
	end
	if targetSolidShape[e] == nil then
		targetSolidShape[e] = 2
	end
	if targetStatus[e] > 0 then
		targetStatus[e] = targetStatus[e] - dt
		--LOG_INFO("UpdatePlate blockStatus[e] = " .. blockStatus[e])
		if targetStatus[e] <= 0 then
			targetStatus[e] = 0
			if defaultSolid[e] == 0 then
				LOG_INFO("make not wireframe")
				SetColliderShape(target[e], targetSolidShape[e])
				SetWireFrame(target[e], false)
			else
				LOG_INFO("make wireframe")
				SetColliderShape(target[e], 3)
				SetWireFrame(target[e], true)
			end
		end
	end
end

function HandleEventPlate(eventData)

	if eventData.type == 16 then
		ImguiText("Plate")
		imguiControledEntity = eventData.e1
		ImguiControledFloat(0, "target", target[eventData.e1])
		ImguiControledFloat(1, "targetStatus", targetStatus[eventData.e1])
		ImguiControledFloat(2, "defaultSolid", defaultSolid[eventData.e1])
		ImguiControledFloat(3, "targetSolidShape", targetSolidShape[eventData.e1])
	end
	if eventData.type == 17 then
		if imguiControledEntity ~= -1 then
			target[imguiControledEntity] = GetImguiControledFloat(0)
			targetStatus[imguiControledEntity] = GetImguiControledFloat(1)
			defaultSolid[imguiControledEntity] = GetImguiControledFloat(2)
			targetSolidShape[imguiControledEntity] = GetImguiControledFloat(3)
			imguiControledEntity = -1
		end
	end
	if eventData.type == 12 then
		LOG_INFO("On Plate " .. eventData.e1)
		LOG_INFO("targetstatus " ..targetStatus[eventData.e1])
		if targetStatus[eventData.e1] == 0 then  -- if its in
			if defaultSolid[eventData.e1] ~= 0 then
				LOG_INFO("make solid")
				SetColliderShape(target[eventData.e1], targetSolidShape[eventData.e1])
				SetWireFrame(target[eventData.e1], false)
			else
				LOG_INFO("make not solid")
				SetColliderShape(target[eventData.e1], 3)
				SetWireFrame(target[eventData.e1], true)
			end
			PlayAudioEvent("PlateActivate")
		end
		targetStatus[eventData.e1] = 0.3 --mark it out
	end
end



function HandleEventPerEntityPlate(e, eventData)

end
