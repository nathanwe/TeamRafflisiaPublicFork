--Translatethings
local imguiControledEntity = -1
local blockStatus = {} -- not the blockStatus from doblockthings, that does nothing now
local lv4Pairs = {[12] = 21}

local pairs = {[4] = lv4Pairs}

function SaveTranslates( levelnum )
	levelstr = string.format("%i", levelnum)
	--SaveIntFloatTableAsJson(timers, "/Assets/Levels/Level" .. levelstr .."TranslateTimerSave.json")
	--SaveIntFloatTableAsJson(directions, "/Assets/Levels/Level" .. levelstr .."TranslateDirectionSave.json")
end

function LoadTranslates( levelnum )
	levelstr = string.format("%i", levelnum)
	--timers = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."TranslateTimerSave.json")
	--directions = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."TranslateDirectionSave.json")
end

function ClearTranslates()

end

function DestroyTranslate(e)

end

function UpdateTranslate(dt, e)
	if blockStatus[e] == nil then
		blockStatus[e] = 0
	end
	if blockStatus[e] > 0 then
		blockStatus[e] = blockStatus[e] - dt
		--LOG_INFO("UpdateTranslate blockStatus[e] = " .. blockStatus[e])
		if blockStatus[e] <= 0 then
			blockStatus[e] = 0
			AddToVQS(e, 0, 0, 6)
		end
	end
end

function HandleEventTranslate(eventData)

	if eventData.type == 16 then
		ImguiText("Translate")
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
		--LOG_INFO("collision between " .. eventData.e1 .. " and " .. eventData.e2)
		LOG_INFO("On Translate " .. eventData.e1)
		lvPairsTranslate = pairs[GetLevelNumber()]
		--LOG_INFO("level number = " ..GetLevelNumber())
		--LOG_INFO("box id" .. (pairs[GetLevelNumber()])[eventData.e1])
		if lvPairsTranslate[eventData.e1] ~= nil then --if on the list
			if blockStatus[lvPairsTranslate[eventData.e1]] == 0 then  -- if its in
				AddToVQS(lvPairsTranslate[eventData.e1], 0, 0, -6) --push it out
				PlayAudioEvent("TranslateActivate")
			end
			blockStatus[lvPairsTranslate[eventData.e1]] = 1 --mark it out
		end
	end
end



function HandleEventPerEntityTranslate(e, eventData)

end
