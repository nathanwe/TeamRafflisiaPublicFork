--Windthings
local imguiControledEntity = -1
local blockStatus = {}
local airIntensity = 200

local lvl5Pairs = {[16] = 7}

local pairs = {[5] = lvl5Pairs}

function SaveWinds( levelnum )
	levelstr = string.format("%i", levelnum)
	SaveIntFloatTableAsJson(WindTime, "/Assets/Levels/Level" .. levelstr .."/WindWindTimeSave.json")
end

function LoadWinds( levelnum )
	levelstr = string.format("%i", levelnum)
	WindTime = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."/WindWindTimeSave.json")
end

function ClearWinds()
	WindTime = {}
end

function DestroyWind(e)
	WindTime[e] = nil
end

function UpdateWind(dt, e)
	if blockStatus[e] == nil then
		blockStatus[e] = 0
	end
	if blockStatus[e] > 0 then
		blockStatus[e] = blockStatus[e] - dt
		AddToVQS(e, -WindDist * dt/WindTime, 0, 0)
		--LOG_INFO("UpdateWind blockStatus[e] = " .. blockStatus[e])
		if blockStatus[e] <= 0 then
			blockStatus[e] = 0
			--AddToVQS(e, WindDist * dt-blockStatus[e]/WindTime, 0, 0) --correct back to start pos
		end
	end
end

function HandleEventWind(eventData)

	if eventData.type == 16 then
		ImguiText("Wind")
		ImguiControledFloat(0, "WindTime", WindTime[eventData.e1])
		imguiControledEntity = eventData.e1

	end
	if eventData.type == 17 then
		if imguiControledEntity ~= -1 then
			WindTime[imguiControledEntity] = GetImguiControledFloat(0)
			--directions[imguiControledEntity] = GetImguiControledFloat(1)
			imguiControledEntity = -1
		end
	end
	if eventData.type == 12 then
		if WindTime[eventData.e1] == nil then
			WindTime[eventData.e1] = 0.0
		end

		LOG_INFO("On Translate " .. eventData.e1)
		lvPairsWind = pairs[GetLevelNumber()]

		--[[
		AddToVQS(lvPairsWind[eventData.e1], airIntensity * eventData.floatData1, 0, 0) 
		]]--

	end
end



function HandleEventPerEntityWind(e, eventData)

end
