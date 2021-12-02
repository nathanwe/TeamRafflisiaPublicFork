--Playerthings
--local timers = {}
--local directions = {}
local imguiControledEntity = -1
local speed = 10

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

function ClearPlayers()
	--timers = {}
	--directions = {}
end

function DestroyPlayer(e)
	--timers[e] = nil;
	--directions[e] = nil
end


function UpdatePlayer(dt, e)
	--if timers[e] == nil then
		--InitPlayer(e)
	--end
	x,y,z = GetPosition(e)
	if y < -50 then
		RestartGame()
		local AudioEventTable = {}
				AudioEventTable["type"] = 9 
				AudioEventTable["stringData1"] = "DeadEffect.wav" 
				AudioEventTable["floatData1"] = x
				AudioEventTable["floatData2"] = y
				AudioEventTable["floatData3"] = z
				AudioEventTable["floatData4"] = -6.0
				SendAudioEvent(AudioEventTable)
	end
end

function HandleEventPlayer(eventData)

	if eventData.type == 5 then
		DestroyPlayer(eventData.e1)
		
	end
	if eventData.type == 12 then
		categories = {}
		categories = GetCategorysOfEntity(eventData.e1)
		if categories[4] then
			LoadNextLevel()
				print("You Win!")
				x,y,z = GetPosition(eventData.e1)
				local AudioEventTable = {}
				AudioEventTable["type"] = 9 
				AudioEventTable["stringData1"] = "WinEffect.wav" 
				AudioEventTable["floatData1"] = x
				AudioEventTable["floatData2"] = y
				AudioEventTable["floatData3"] = z
				AudioEventTable["floatData4"] = -6.0 
				SendAudioEvent(AudioEventTable)
		end
		categories = GetCategorysOfEntity(eventData.e2)
		if categories[4] then
				LoadNextLevel()
				print("You Win!")
				x,y,z = GetPosition(eventData.e2)
				local AudioEventTable = {}
				AudioEventTable["type"] = 9 
				AudioEventTable["stringData1"] = "WinEffect.wav" 
				AudioEventTable["floatData1"] = x
				AudioEventTable["floatData2"] = y
				AudioEventTable["floatData3"] = z
				AudioEventTable["floatData4"] = -6.0 
				SendAudioEvent(AudioEventTable)
		end
	end
	if eventData.type == 16 then
		ImguiText("Player")
		--ImguiControledFloat(0, "timers", timers[eventData.e1])
		--ImguiControledFloat(1, "directions", directions[eventData.e1])
		imguiControledEntity = eventData.e1
	end
	--if eventData.type == 17 then
	--	if imguiControledEntity ~= -1 then
	--		timers[imguiControledEntity] = GetImguiControledFloat(0)
	--		directions[imguiControledEntity] = GetImguiControledFloat(1)
	--	imguiControledEntity = -1
	--	end
	--end
end

function HandleEventPerEntityPlayer(e, eventData)
	if eventData.type == 7 then
		DeleteEntity(e)
	end
	if eventData.type == 8 then
		camerax, cameray, cameraz = GetCameraOrientation()
		--print( "camera at", camerax, cameray, cameraz)
		mag = math.sqrt(camerax * camerax + cameraz * cameraz)
		newx = camerax/mag
		newz = cameraz/mag
		if eventData.stringData1 == "Up" then
			AddPhysicsVelocity(e, speed * eventData.floatData1 * newx, 0, speed * eventData.floatData1 * newz)
		end
		if eventData.stringData1 == "Down" then
			AddPhysicsVelocity(e, -speed * eventData.floatData1 * newx, 0, -speed * eventData.floatData1 * newz)
		end
		if eventData.stringData1 == "Left" then
			AddPhysicsVelocity(e, speed * eventData.floatData1 * newz, 0, -speed * eventData.floatData1 * newx)
		end
		if eventData.stringData1 == "Right" then
			AddPhysicsVelocity(e, -speed * eventData.floatData1 * newz, 0, speed * eventData.floatData1 * newx)
		end
	end
end

--custom functions

function InitPlayer(e)
	--timers[e] = e
	--directions[e] = 1
end