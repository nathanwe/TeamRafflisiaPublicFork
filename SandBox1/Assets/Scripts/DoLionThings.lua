--lionthings
local timers = {}
local directions = {}
local cycleTime = 5
local speed = 1


function InitLion(e)
	timers[e] = e
	directions[e] = 1
end

function DestroyLion(e)
	timers[e] = nil;
	directions[e] = nil
end


function UpdateLion(dt, e)
	if timers[e] == nil then
		InitLion(e)
	end
	timers[e] = timers[e] + dt
		if timers[e] > cycleTime then
			timers[e] = timers[e] - cycleTime
			directions[e] = directions[e] * -1
		end
	AddToVQS(e, 0, 0, speed*directions[e]*dt)
end

function HandleEventLion(EventData)
	if EventData.type == 5 then
		DestroyLion(EventData.e1)
	end
	if EventData.type == 6 then
		timers = {}
		directions = {}
	end
	if EventData.type == 13 then
		level = string.format("%i", EventData.intData1)
		SaveIntFloatTableAsJson(timers, "/Assets/Levels/Level" .. level .."LionTimerSave.json")
		SaveIntFloatTableAsJson(directions, "/Assets/Levels/Level" .. level .."LionDirectionSave.json")
	end
	if EventData.type == 14 then
		level = string.format("%i", EventData.intData1)
		timers = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. level .."LionTimerSave.json")
		directions = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. level .."LionDirectionSave.json")
	end
end

function HandleEventPerEntityLion(e, EventData)
	if EventData.type == 7 then
		DeleteEntity(e)
	end
end