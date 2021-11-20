--GoalZonethings
local timers = {}
local directions = {}
local imguiControledEntity = -1
local cycleTime = 5
local speed = 1

function SaveGoalZones( levelnum )
	levelstr = string.format("%i", levelnum)
	SaveIntFloatTableAsJson(timers, "/Assets/Levels/Level" .. levelstr .."GoalZoneTimerSave.json")
	SaveIntFloatTableAsJson(directions, "/Assets/Levels/Level" .. levelstr .."GoalZoneDirectionSave.json")
end

function LoadGoalZones( levelnum )
	levelstr = string.format("%i", levelnum)
	timers = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."GoalZoneTimerSave.json")
	directions = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."GoalZoneDirectionSave.json")
end

function ClearGoalZones()
	timers = {}
	directions = {}
end

function DestroyGoalZone(e)
	timers[e] = nil;
	directions[e] = nil
end


function UpdateGoalZone(dt, e)
	if timers[e] == nil then
		InitGoalZone(e)
	end

	timers[e] = timers[e] + dt
		if timers[e] > cycleTime then
			timers[e] = timers[e] - cycleTime
			directions[e] = directions[e] * -1
		end
	--AddToVQS(e, 0, 0, speed*directions[e]*dt)
end

function HandleEventGoalZone(eventData)
	if eventData.type == 5 then
		DestroyGoalZone(eventData.e1)
	end
	if eventData.type == 16 then
		ImguiText("GoalZone")
		ImguiControledFloat(0, "timers", timers[eventData.e1])
		ImguiControledFloat(1, "directions", directions[eventData.e1])
		imguiControledEntity = eventData.e1
	end
	if eventData.type == 17 then
		if imguiControledEntity ~= -1 then
			timers[imguiControledEntity] = GetImguiControledFloat(0)
			directions[imguiControledEntity] = GetImguiControledFloat(1)
		imguiControledEntity = -1
		end
	end
end

function HandleEventPerEntityGoalZone(e, eventData)
	if eventData.type == 7 then
		DeleteEntity(e)
	end
end

--custom functions

function InitGoalZone(e)
	timers[e] = e
	directions[e] = 1
end