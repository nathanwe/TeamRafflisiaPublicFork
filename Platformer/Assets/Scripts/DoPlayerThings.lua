--Playerthings
local imguiControledEntity = -1
local airTime = {}
local speed = 10
local jumpspeed = 5

function SavePlayers( levelnum )
	levelstr = string.format("%i", levelnum)
	SaveIntFloatTableAsJson(airTime, "/Assets/Levels/Level" .. levelstr .."PlayerAirTimeSave.json")
	--SaveIntFloatTableAsJson(directions, "/Assets/Levels/Level" .. levelstr .."PlayerDirectionSave.json")
end

function LoadPlayers( levelnum )
	levelstr = string.format("%i", levelnum)
	airTime = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."PlayerAirTimeSave.json")
	--directions = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."PlayerDirectionSave.json")
end

function ClearPlayers()
	airTime = {}
end

function DestroyPlayer(e)
	airTime[e] = nil
end

function UpdatePlayer(dt, e)
	if airTime[e] == nil then 
		airTime[e] = 0 
	end
	airTime[e] = airTime[e] + dt
end

function HandleEventPlayer(eventData)
	if eventData.type == 16 then
		ImguiText("Player")
		imguiControledEntity = eventData.e1
	end
	if eventData.type == 12 then
		--LOG_INFO("coltion between " .. eventData.e1 .. " and " .. eventData.e2)
		local playerid = 69
		categoriesA = {}
		categoriesB = {}
		categoriesA = GetCategorysOfEntity(eventData.e1)
		categoriesB = GetCategorysOfEntity(eventData.e2)
		if categoriesA[1] or categoriesB[1] then --if player
			if categoriesA[1] then
				playerid = eventData.e1
			end
			if categoriesB[1] then
				playerid = eventData.e2
			end
			--LOG_INFO("On Plate " .. plateid)
			airTime[playerid] = 0
		end
	end
end



function HandleEventPerEntityPlayer(e, eventData)
	if eventData.type == 7 then
		DeleteEntity(e)
	end
	if eventData.type == 8 then
		camerax, cameray, cameraz = GetCameraOrientation()
		--print( "camera at", camerax, cameray, cameraz)
		--mag = math.sqrt(camerax * camerax + cameraz * cameraz)
		--newx = camerax/mag
		--newz = cameraz/mag

		if eventData.stringData1 == "Up" then
			LOG_INFO("airTime = " .. airTime[e])
			if airTime[e] < .3 then
				AddPhysicsVelocity(e, 0, speed * eventData.floatData1 * 5, 0)
			end
		end
		if eventData.stringData1 == "Down" then
			--AddPhysicsVelocity(e, 0, -speed * eventData.floatData1, 0)
		end
		if eventData.stringData1 == "Left" then
			AddPhysicsVelocity(e, speed * eventData.floatData1, 0, 0)
		end
		if eventData.stringData1 == "Right" then
			AddPhysicsVelocity(e, -speed * eventData.floatData1, 0, 0)
		end
	end
	
end
