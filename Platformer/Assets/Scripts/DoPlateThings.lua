--Platethings
local imguiControledEntity = -1
local blockStatus = {} -- not the blockStatus from doblockthings, that does nothing now
local target = {}
local targetStatus = {}
local defaultSolid = {}
local lv0Pairs = {[11] = 4}
local lv3Pairs = {[8] = 3}
local lv4Pairs = {[8] = 3}
local lv5Pairs = {[18] = 7}
local lv6Pairs = {[10] = 3}
local lv7Pairs = {[10] = 3}
local lv8Pairs = {[8] = 7, [10] = 11}
local lv9Pairs = {[10] = 1}
local lv11Pairs = {[10] = 3, [48] = 52, [14] = 43}

local pairs1 = {[0] = lv0Pairs, [3] = lv3Pairs, [4] = lv4Pairs, [5] = lv5Pairs, [6] = lv6Pairs, [7] = lv7Pairs, [8] = lv8Pairs, [9] = lv9Pairs, [11] = lv11Pairs}

function SavePlates( levelnum )
	levelstr = string.format("%i", levelnum)
	SaveIntFloatTableAsJson(target, "/Assets/Levels/Level" .. levelstr .."/targetSave.json")
	SaveIntFloatTableAsJson(targetStatus, "/Assets/Levels/Level" .. levelstr .."/targetStatusSave.json")
	SaveIntFloatTableAsJson(defaultSolid, "/Assets/Levels/Level" .. levelstr .."/defaultSolidSave.json")
end

function LoadPlates( levelnum )
	levelstr = string.format("%i", levelnum)
	target = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."/targetSave.json")
	targetStatus = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."/targetStatusSave.json")
	defaultSolid = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."/defaultSolidSave.json")
	--lvPairs = pairs1[GetLevelNumber()]
	--if lvPairs ~= nil then
	--	for index, value in pairs(lvPairs) do
	--		target[index] = value
	--	end
	--end
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
	if targetStatus[e] > 0 then
		targetStatus[e] = targetStatus[e] - dt
		--LOG_INFO("UpdatePlate blockStatus[e] = " .. blockStatus[e])
		if targetStatus[e] <= 0 then
			targetStatus[e] = 0
			if defaultSolid[e] == 0 then
				LOG_INFO("make not wireframe")
				SetColliderShape(target[e], 2)
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
	end
	if eventData.type == 17 then
		if imguiControledEntity ~= -1 then
			target[imguiControledEntity] = GetImguiControledFloat(0)
			targetStatus[imguiControledEntity] = GetImguiControledFloat(1)
			defaultSolid[imguiControledEntity] = GetImguiControledFloat(2)
			imguiControledEntity = -1
		end
	end
	if eventData.type == 12 then
		LOG_INFO("On Plate " .. eventData.e1)
		LOG_INFO("targetstatus " ..targetStatus[eventData.e1])
		if targetStatus[eventData.e1] == 0 then  -- if its in
			if defaultSolid[eventData.e1] ~= 0 then
				LOG_INFO("make solid")
				SetColliderShape(target[eventData.e1], 2)
				SetWireFrame(target[eventData.e1], false)
			else
				LOG_INFO("make not solid")
				SetColliderShape(target[eventData.e1], 3)
				SetWireFrame(target[eventData.e1], true)
			end
			PlayAudioEvent("PlateActivate")
		end
		targetStatus[eventData.e1] = 1 --mark it out
	end
end



function HandleEventPerEntityPlate(e, eventData)

end
