--Playerthings
local imguiControledEntity = -1

local timer = 3
local spawnTime = 3

local level = 0
local score = 0

function SavePlayers( levelnum )
	levelstr = string.format("%i", levelnum)
	--SaveIntFloatTableAsJson(timers, "/Assets/Levels/Level" .. levelstr .."PlayerTimerSave.json")
	--SaveIntFloatTableAsJson(directions, "/Assets/Levels/Level" .. levelstr .."PlayerDirectionSave.json")
end

function LoadPlayers( levelnum )
	levelstr = string.format("%i", levelnum)
	--timers = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."PlayerTimerSave.json")
	--directions = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."PlayerDirectionSave.json")
end

function InitPlayer(e)
	print("test 123")
end

function ClearPlayers()

end

function DestroyPlayer(e)

end

function UpdatePlayer(dt, e)

	if(level == 0) then
		timer = timer - dt
		--print(timer)
		if(timer <= 0) then
			print("spawn")
			local e = CreateEntity("Target")
			SetPosition(e, -6.7, 0.5, -3.2)
			SetPhysicsVelocity(e, 10, 10, 0)
			timer = spawnTime
		end
	end

	if(score == 10) then
		print("next level")
		score = 0
		LoadNextLevel()
	end

end

function HandleEventPlayer(eventData)
	if eventData.type == 16 then
		ImguiText("Player")
		imguiControledEntity = eventData.e1
	end
	if eventData.type == 19 then
		if eventData.stringData1 == "Fire" then
			target = Raycast()
			print("target", target)
			if target ~= -1 then
				score = score + 1
				local EventTable = {}
				EventTable["type"] = 18
				EventTable["e1"] = target
				EventTable["thingsToEffect"] = {[1] = 2}
				SendEvent(EventTable)
			end
		end
	end
end

function HandleEventPerEntityPlayer(e, eventData)
	--if eventData.type == 8 then
	--	if eventData.stringData1 == "Up" then
	--		AddToVQS(e, 0, 10 *eventData.floatData1, 0)
	--	end
	--	if eventData.stringData1 == "Down" then
	--		AddToVQS(e, 0, -10 * eventData.floatData1, 0)
	--	end
	--	if eventData.stringData1 == "Left" then
	--		AddToVQS(e, -10 * eventData.floatData1, 0, 0)
	--	end
	--	if eventData.stringData1 == "Right" then
	--		AddToVQS(e, 10 * eventData.floatData1, 0, 0)
	--	end
	--end
end
