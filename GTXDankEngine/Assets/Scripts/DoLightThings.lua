--lightthings
local timers = {};
local directions = {}
local cycleTime = 2
local speed = 1


function InitLight(e)
	timers[e] = e
	directions[e] = 1
end

function DestroyLight(e)
	timers[e] = nil;
	directions[e] = nil
end


function UpdateLight (dt, e)
	if timers[e] == nil then
		InitLight(e)
	end
	timers[e] = timers[e] + dt
		if timers[e] > cycleTime then
			timers[e] = timers[e] - cycleTime
			directions[e] = directions[e] * -1
		end
	AddToVQS(e, 0, speed*directions[e]*dt, 0)
end