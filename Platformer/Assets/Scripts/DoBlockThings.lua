--blockthings
local timers = {}
local blockStatus = {}
local imguiControledEntity = -1
--local totaloutblocks = {[1] = 0.0}

function SaveBlocks( levelnum )
	levelstr = string.format("%i", levelnum)
	--SaveIntFloatTableAsJson(totaloutblocks, "/Assets/Levels/Level" .. levelstr .."BlockTotalSave.json")
	SaveIntFloatTableAsJson(blockStatus, "/Assets/Levels/Level" .. levelstr .."BlockStatusSave.json")
end

function LoadBlocks( levelnum )
	levelstr = string.format("%i", levelnum)
	--totaloutblocks = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."BlockTotalSave.json")
	blockStatus = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."BlockStatusSave.json")
end

function ClearBlocks()
	--totaloutblocks = {[1] = 0.0 }
	timers = {}
	blockStatus = {}
end

function DestroyBlock(e)
	timers[e] = nil
	blockStatus[e] = nil
end


function UpdateBlock(dt, e)
	if blockStatus[e] == nil then
		InitBlock(e)
	end
end

function HandleEventBlock(eventData)
	if eventData.type == 5 then
		DestroyBlock(eventData.e1)
	end
	if eventData.type == 16 then
		ImguiText("Block")
		ImguiControledFloat(0, "timers", timers[eventData.e1])
		ImguiControledFloat(1, "blockStatus", blockStatus[eventData.e1])
		imguiControledEntity = eventData.e1
	end
	if eventData.type == 17 then
		if imguiControledEntity ~= -1 then
			timers[imguiControledEntity] = GetImguiControledFloat(0)
			blockStatus[imguiControledEntity] = GetImguiControledFloat(1)
		imguiControledEntity = -1
		end
	end
	if eventData.type == 18 then
		cats = {}
		cats = GetCategorysOfEntity(eventData.e1)
		if cats[2] then
			--print("shot")
			--if blockStatus[eventData.e1] == 0 then
			--	AddToVQS(eventData.e1, 0, 0, 6)
			--	blockStatus[eventData.e1] = 1
			--elseif blockStatus[eventData.e1] == 1 then
			--	AddToVQS(eventData.e1, 0, 0, -6)
			--	blockStatus[eventData.e1] = 0
			--end
		end
	end
end

function HandleEventPerEntityBlock(e, eventData)
	if eventData.type == 7 then
		DeleteEntity(e)
	end
end

--custom functions

function InitBlock(e)
	blockStatus[e] = 0
end

function totaloutblocks()
	total = 0;
	for x,y in pairs(blockStatus) do
		total = total+y
	end
	return total;
end
