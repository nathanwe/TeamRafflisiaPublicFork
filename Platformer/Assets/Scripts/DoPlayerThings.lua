--Playerthings
local imguiControledEntity = -1
local airTime = {}
local accel = 100
local jumpspeed = 40
local decay = 200
local maxspeed = 20

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
	--update airTime
	if airTime[e] == nil then 
		airTime[e] = 0 
	end
	airTime[e] = airTime[e] + dt
	--roll
	data = {}
	data = GetRigidData(e)
	
	-- left speed cap
	if data.velocity.x > maxspeed then
		if data.velocity.x - decay*dt < maxspeed then
			AddPhysicsVelocity(e, maxspeed-data.velocity.x, 0, 0)
		else
			AddPhysicsVelocity(e, decay*dt, 0, 0)
		end
	end

	--right speed cap
	if data.velocity.x < -maxspeed then
		if data.velocity.x + decay*dt > -maxspeed then
			AddPhysicsVelocity(e, -maxspeed-data.velocity.x, 0, 0)
		else
			AddPhysicsVelocity(e, -decay*dt, 0, 0)
		end
	end

	--gravity increase, for more responsive movement
	AddPhysicsVelocity(e, 0, dt * -50, 0)
	
	--AddPhysicsVelocity(e, -data.velocity.x*(1-decay), 0, 0)

	AddRotation(e, 0,0,-data.velocity.x*dt*20)


end

function HandleEventPlayer(eventData)
	if eventData.type == 16 then
		ImguiText("Player")
		imguiControledEntity = eventData.e1
	end
	if eventData.type == 12 then
		airTime[eventData.e1] = 0
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
			--LOG_INFO("airTime = " .. airTime[e])
			if airTime[e] < .02 then
				AddPhysicsVelocity(e, 0, jumpspeed, 0)
			end
		end
		if eventData.stringData1 == "Down" then
			--AddPhysicsVelocity(e, 0, -accel * eventData.floatData1, 0)
		end
		if eventData.stringData1 == "Left" then
			AddPhysicsVelocity(e, accel * eventData.floatData1, 0, 0)
		end
		if eventData.stringData1 == "Right" then
			AddPhysicsVelocity(e, -accel * eventData.floatData1, 0, 0)
		end
	end
	
end
