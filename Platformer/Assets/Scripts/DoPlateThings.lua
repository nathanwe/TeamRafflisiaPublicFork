--Platethings
local imguiControledEntity = -1
local blockStatus = {} -- not the blockStatus from doblockthings, that does nothing now
local lv0Pairs = {[11] = 4}
local lv1Pairs = {[18] = 7}
local lv2Pairs = {[10] = 3}
local lv3Pairs = {[10] = 3}
local pairs = {[0] = lv0Pairs, [1] = lv1Pairs, [2] = lv2Pairs, [3] = lv3Pairs}

function SavePlates( levelnum )
	levelstr = string.format("%i", levelnum)
end

function LoadPlates( levelnum )
	levelstr = string.format("%i", levelnum)
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
		--LOG_INFO("UpdatePlate blockStatus[e] = " .. blockStatus[e])
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
		--LOG_INFO("coltion between " .. eventData.e1 .. " and " .. eventData.e2)
		LOG_INFO("On Plate " .. eventData.e1)
		lvPairs = pairs[GetLevelNumber()]
		--LOG_INFO("level number = " ..GetLevelNumber())
		--LOG_INFO("box id" .. (pairs[GetLevelNumber()])[eventData.e1])
		if lvPairs[eventData.e1] ~= nil then --if on the list
			if blockStatus[lvPairs[eventData.e1]] == 0 then  -- if its in
				AddToVQS(lvPairs[eventData.e1], 0, 0, -6) --push it out
				blockStatus[lvPairs[eventData.e1]] = 1 --mark it out
				PlayAudioEvent("PlateActivate")
			end
		end
	end
end



function HandleEventPerEntityPlate(e, eventData)

end
