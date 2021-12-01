--Playerthings
local imguiControledEntity = -1

local timer = 2.5
local spawnTime = 2.5
local entityToSpawn = 3

local score = 0
local level = 0
local gameOver = false

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
	level = GetLevelNumber()

	if(level == 0) then
		if(score == 1) then
			print("next level")
			score = 0
			LoadNextLevel()
		end
	end
	
	if(level == 1) then
		timer = timer - dt
		--print(timer)
		if(timer <= 0) then
			print("spawn")
			local e = CreateEntity("Target")
			SetPosition(e, -6.7, 0.5, -3.2)
			SetPhysicsVelocity(e, 10+math.random(5), 10+math.random(5), 0+math.random(5))
			timer = spawnTime
		end
		
		if(score == 8) then
			print("next level")
			score = 0
			LoadNextLevel()
			spawnTime = 0.5
			timer = 0.5
		end
	end
	
	if(level == 2) then
		if(score == 1) then
			print("next level")
			score = 0
			LoadNextLevel()
		end
	end
	
	if(level == 3) then
		timer = timer - dt
		--print(timer)
		if(timer <= 0) then
			print("spawn")
			local e = CreateEntity("Target")
			SetPosition(e, -5+math.random(5), 0+math.random(5), -3+math.random(5))
			SetScale(e, 0.5, 0.5, 0.5)
			SetPhysicsVelocity(e, 0, 0, 0)
			timer = spawnTime
		end
		
		if(score == 8) then
			print("next level")
			score = 0
			LoadNextLevel()
			spawnTime = 1
			timer = 1
		end
	end
	
	if(level == 4) then
		if(score == 1) then
			print("next level")
			score = 0
			LoadNextLevel()
		end
	end
	
	if(level == 5) then
		timer = timer - dt
		--print(timer)
		if(timer <= 0) then
			print("spawn")
			local e = CreateEntity("Target2")
			SetPosition(e, -2+math.random(5), 0+math.random(5), -15+math.random(5))
			SetScale(e, 0.5, 0.5, 0.5)
			SetPhysicsVelocity(e, 0, 0, 20)
			timer = spawnTime
		end

		if(score == 8) then
			print("next level")
			score = 0
			LoadNextLevel()
			spawnTime = 1
			timer = 1
		end
	end
	
	if(level == 6) then
		if(score == 1) then
			print("next level")
			score = 0
			LoadNextLevel()
		end
	end
	
	if(level == 7) then
		--timer = timer - dt
		--print(timer)
		--if(timer <= 0) then
			while(entityToSpawn > 0) do
				entityToSpawn = entityToSpawn - 1
				print("spawn")
				local e = CreateEntity("Target2")
				SetPosition(e, -2+math.random(5), 0+math.random(5), -15+math.random(5))
				SetScale(e, 0.5, 0.5, 0.5)
				SetPhysicsVelocity(e, 0, 0, 0)
			end
		--	timer = 200000000
		--end

		--if(score == 50) then
		--	print("next level")
		--	score = 0
		--	LoadNextLevel()
		--	spawnTime = 1
		--	timer = 1
		--end
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

				if(level == 7) then
					entityToSpawn = entityToSpawn + 1
				end
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
