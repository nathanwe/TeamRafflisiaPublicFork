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
	SetPosition(e, math.sin(timers[e])*2, math.cos(timers[e])*2, 0)
	timers[e] = timers[e] + dt
end

function HandleEventLion(EventData)
	if EventData.type == 5 then
		DestroyLion(EventData.e1)
	end
	if EventData.type == 6 then
		timers = {}
		directions = {}
	end
end

function HandleEventPerEntityLion(e, EventData)
	if EventData.type == 7 then
		DeleteEntity(e)
	end
end