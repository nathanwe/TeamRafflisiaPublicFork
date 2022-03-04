--Glidethings
local imguiControledEntity = -1
local blockStatus = {}
local glideTime = {}
local glideDist = 40

local lvl5Pairs = {[16] = 7}

local pairs = {[5] = lvl5Pairs}

function SaveGlides( levelnum )
	levelstr = string.format("%i", levelnum)
	SaveIntFloatTableAsJson(glideTime, "/Assets/Levels/Level" .. levelstr .."/GlideglideTimeSave.json")
end

function LoadGlides( levelnum )
	levelstr = string.format("%i", levelnum)
	glideTime = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."/GlideglideTimeSave.json")
end

function ClearGlides()
	glideTime = {}
end

function DestroyGlide(e)
	glideTime[e] = nil
end

function UpdateGlide(dt, e)
	if blockStatus[e] == nil then
		blockStatus[e] = 0
	end
	if blockStatus[e] > 0 then
		blockStatus[e] = blockStatus[e] - dt
		AddToVQS(e, -glideDist * dt/glideTime, 0, 0)
		--LOG_INFO("UpdateGlide blockStatus[e] = " .. blockStatus[e])
		if blockStatus[e] <= 0 then
			blockStatus[e] = 0
			--AddToVQS(e, glideDist * dt-blockStatus[e]/glideTime, 0, 0) --correct back to start pos
		end
	end
end

function HandleEventGlide(eventData)

	if eventData.type == 16 then
		ImguiText("Glide")
		ImguiControledFloat(0, "glideTime", glideTime[eventData.e1])
		imguiControledEntity = eventData.e1

	end
	if eventData.type == 17 then
		if imguiControledEntity ~= -1 then
			glideTime[imguiControledEntity] = GetImguiControledFloat(0)
			--directions[imguiControledEntity] = GetImguiControledFloat(1)
			imguiControledEntity = -1
		end
	end
	if eventData.type == 12 then
		if glideTime[eventData.e1] == nil then
			glideTime[eventData.e1] = 0.0
		end

		LOG_INFO("On Translate " .. eventData.e1)
		lvPairsGlide = pairs[GetLevelNumber()]

		--[[
		--if on the list
		if lvPairsGlide[eventData.e1] ~= nil then 
			if blockStatus[lvPairsGlide[eventData.e1] ] == 0 then  
				AddToVQS(lvPairsGlide[eventData.e1], glideDist * eventData.floatData1, 0, 0) 
			end
			blockStatus[lvPairsGlide[eventData.e1] ] = 1 
		end
		]]--

	end
end



function HandleEventPerEntityGlide(e, eventData)

end
