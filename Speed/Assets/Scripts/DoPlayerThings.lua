--Playerthings
local timers = {}
local directions = {}
local imguiControledEntity = -1
local cycleTime = 5
local speed = 1

function SavePlayers( levelnum )
	levelstr = string.format("%i", levelnum)
	SaveIntFloatTableAsJson(timers, "/Assets/Levels/Level" .. levelstr .."PlayerTimerSave.json")
	SaveIntFloatTableAsJson(directions, "/Assets/Levels/Level" .. levelstr .."PlayerDirectionSave.json")
end

function LoadPlayers( levelnum )
	levelstr = string.format("%i", levelnum)
	timers = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."PlayerTimerSave.json")
	directions = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."PlayerDirectionSave.json")
end

function ClearPlayers()
	timers = {}
	directions = {}
end

function DestroyPlayer(e)
	timers[e] = nil;
	directions[e] = nil
end


function UpdatePlayer(dt, e)
	if timers[e] == nil then
		InitPlayer(e)
	end

	timers[e] = timers[e] + dt
		if timers[e] > cycleTime then
			timers[e] = timers[e] - cycleTime
			directions[e] = directions[e] * -1
		end
	--AddToVQS(e, 0, 0, speed*directions[e]*dt)
end

function HandleEventPlayer(eventData)
	if eventData.type == 5 then
		DestroyPlayer(eventData.e1)
	end
	if eventData.type == 12 then
		if eventData.e1 == 1 and eventData.e2 ==5 then
			print("You Win!")
		end
		if eventData.e1 == 5 and eventData.e2 ==1 then
			print("You Win!")
		end
	end
	if eventData.type == 16 then
		ImguiText("Player")
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

function HandleEventPerEntityPlayer(e, eventData)
	if eventData.type == 7 then
		DeleteEntity(e)
	end
	if eventData.type == 8 then
		if eventData.stringData1 == "Up" then
			SetPhysicsVelocity(e, 0, 0, -10 * eventData.floatData1)
		end
		if eventData.stringData1 == "Down" then
			SetPhysicsVelocity(e, 0, 0, 10 * eventData.floatData1)
		end
		if eventData.stringData1 == "Left" then
			SetPhysicsVelocity(e, -10 * eventData.floatData1, 0, 0)
		end
		if eventData.stringData1 == "Right" then
			SetPhysicsVelocity(e, 10 * eventData.floatData1, 0, 0)
		end
	end
end

--custom functions

function InitPlayer(e)
	timers[e] = e
	directions[e] = 1
end