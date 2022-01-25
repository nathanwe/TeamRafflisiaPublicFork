--Platethings
local imguiControledEntity = -1
local blockStatus = {} -- not the blockStatus from doblockthings, that does nothing now
local lv0Pairs = {[12] = 4}
local lv1Pairs = {[18] = 7}
local pairs = {[0] = lv0Pairs, [1] = lv1Pairs}
local lvPairs = {}
function SavePlates( levelnum )
	levelstr = string.format("%i", levelnum)
	--SaveIntFloatTableAsJson(timers, "/Assets/Levels/Level" .. levelstr .."PlateTimerSave.json")
	--SaveIntFloatTableAsJson(directions, "/Assets/Levels/Level" .. levelstr .."PlateDirectionSave.json")
end

function LoadPlates( levelnum )
	levelstr = string.format("%i", levelnum)
	--timers = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."PlateTimerSave.json")
	--directions = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."PlateDirectionSave.json")
end

function ClearPlates()

end

function DestroyPlate(e)

end

function UpdatePlate(dt, e)
	if blockStatus[e] == nil then
		blockStatus[e] = 0
	end
	if blockStatus[e] > 0 then
		blockStatus[e] = blockStatus[e] - dt
		LOG_INFO("UpdatePlate blockStatus[e] = " .. blockStatus[e])
		if blockStatus[e] <= 0 then
			blockStatus[e] = 0
			AddToVQS(e, 0, 0, 6)
		end
	end
end

function HandleEventPlate(eventData)

	if eventData.type == 16 then
		ImguiText("Plate")
		imguiControledEntity = eventData.e1

	end
	if eventData.type == 17 then
		if imguiControledEntity ~= -1 then
			--timers[imguiControledEntity] = GetImguiControledFloat(0)
			--directions[imguiControledEntity] = GetImguiControledFloat(1)
			imguiControledEntity = -1
		end
	end
	if eventData.type == 12 then
		LOG_INFO("coltion between " .. eventData.e1 .. " and " .. eventData.e2)
		local plateid = 69
		categoriesA = {}
		categoriesB = {}
		categoriesA = GetCategorysOfEntity(eventData.e1)
		categoriesB = GetCategorysOfEntity(eventData.e2)
		if (categoriesA[1] and categoriesB[2]) or (categoriesA[2] and categoriesB[1]) then --if player/block
			if (categoriesA[1] and categoriesB[2]) then
				plateid = eventData.e2
			end
			if (categoriesA[2] and categoriesB[1]) then
				plateid = eventData.e1
			end
			LOG_INFO("On Plate " .. plateid)
			lvPairs = pairs[GetLevelNumber()]
			--LOG_INFO("level number = " ..GetLevelNumber())
			--LOG_INFO("box id" .. (pairs[GetLevelNumber()])[plateid])
			if lvPairs[plateid] ~= nil then --if on the list
				if blockStatus[lvPairs[plateid]] == 0 then  -- if its in
					AddToVQS(lvPairs[plateid], 0, 0, -6) --push it out
					blockStatus[lvPairs[plateid]] = 1 --mark it out
				end
			end
		end
		
	end

end



function HandleEventPerEntityPlate(e, eventData)
	LOG_INFO("handleing plate event")
	if eventData.type == 7 then
		DeleteEntity(e)
	end
	if eventData.type == 12 then
		LOG_INFO("coltion between " .. eventData.e1 .. " and " .. eventData.e2)
		if e == eventData.e1 or e == eventData.e2 then --if collide with me
			categories = {}
			categoriesA = GetCategorysOfEntity(eventData.e1)
			categoriesB = GetCategorysOfEntity(eventData.e2)
			if categoriesA[1] or categoriesB[1] then --if collide with player
				LOG_INFO("onPlate")
				lvpairs = pairs[GetLevelNumber()]
				if lvpairs[e] ~= nil then --if on the list
					if blockStatus[lvpairs[e]] == 0 then-- if its in
						AddToVQS(lvpairs[e], 0, 0, -6) --push it out
						blockStatus[lvpairs[e]] = 1 --mark it out
					end
				end
			end
		end
	end
end
