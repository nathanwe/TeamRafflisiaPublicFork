--lionthings
local timers = {};
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