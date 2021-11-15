--lightthings
local timers = {};
local directions = {}
local cycleTime = 2
local speed = 1


function SaveLight( levelnum )
	levelstr = string.format("%i", levelnum)
	SaveIntFloatTableAsJson(timers, "/Assets/Levels/Level" .. levelstr .."LightTimerSave.json")
	SaveIntFloatTableAsJson(directions, "/Assets/Levels/Level" .. levelstr .."LightDirectionSave.json")
end

function LoadLights( levelnum )
	levelstr = string.format("%i", levelnum)
	timers = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."LightTimerSave.json")
	directions = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."LightDirectionSave.json")
end

function ClearLights()
	timers = {}
	directions = {}
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

function HandleEventLight(EventData)

end

--custom functions


function InitLight(e)
	timers[e] = e
	directions[e] = 1
end