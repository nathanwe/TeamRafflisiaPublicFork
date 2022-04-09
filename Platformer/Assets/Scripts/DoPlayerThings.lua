--Playerthings
local imguiControledEntity = -1
local airTime = {}
local decaying = {}
local accel = 100
local jumpspeed = 25
local decay = 200
local maxspeed = 35
local hardcap = 40
losingPlayer = nil
local levelTitles = {[1] = "LV1 WASD", [2] = "LV2 Space",
	[3] = "LV3 The Button", [4] = "LV4 Bouncy Trampoline", [5] = "LV5 Slow and Steady",
	[6] = "LV7 Through the Air", [7] = "LV7 The Cage", [8] = "LV8 Friction Physics",
	[9] = "LV9 Go Fast!", [10] = "LV10 Stoplight", [11] = "LV11 Up The Stairs"}

function SavePlayers( levelnum )
	levelstr = string.format("%i", levelnum)
	SaveIntFloatTableAsJson(airTime, "/Assets/Levels/Level" .. levelstr .."/PlayerAirTimeSave.json")
	SaveIntFloatTableAsJson(decaying, "/Assets/Levels/Level" .. levelstr .."/PlayerDecayingSave.json")
end

function LoadPlayers( levelnum )
	levelstr = string.format("%i", levelnum)
	airTime = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."/PlayerAirTimeSave.json")
	decaying = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."/PlayerDecayingSave.json")
	losingPlayer = nil
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
	--update losing
	if losingPlayer ~= nil then
		losingPlayer = losingPlayer - dt
	end

	--roll
	data = {}
	data = GetRigidData(e)
	AddRotation(e, 0,0,-data.velocity.x*dt*20)

	if (decaying[e] == 1.0) then
		-- left speed cap
		if data.velocity.x > maxspeed then
			if data.velocity.x > hardcap then AddPhysicsVelocity(e, hardcap-data.velocity.x, 0, 0) end

			if data.velocity.x - decay*dt < maxspeed then
				AddPhysicsVelocity(e, maxspeed-data.velocity.x, 0, 0)
			else
				AddPhysicsVelocity(e, decay*dt, 0, 0)
			end
		end

		--right speed cap
		if data.velocity.x < -maxspeed then
			if data.velocity.x < -hardcap then AddPhysicsVelocity(e, -hardcap-data.velocity.x, 0, 0) end

			if data.velocity.x + decay*dt > -maxspeed then
				AddPhysicsVelocity(e, -maxspeed-data.velocity.x, 0, 0)
			else
				AddPhysicsVelocity(e, -decay*dt, 0, 0)
			end
		end
	end
	--gravity increase, for more responsive movement
	AddPhysicsVelocity(e, 0, dt * -50, 0)
	
	--AddPhysicsVelocity(e, -data.velocity.x*(1-decay), 0, 0)
	if losingPlayer == nil and data.position.y < -20 and winningGoal == nil then
		losingPlayer = 99
	end
		if losingPlayer ~= nil and losingPlayer < 0 then
		RestartLevel()
	end
end

function HandleEventPlayer(eventData)
	if eventData.type == 16 then
		ImguiText("Player")
		imguiControledEntity = eventData.e1
	end
	if eventData.type == 17 then
		if imguiControledEntity ~= -1 then
			--bouncyness[imguiControledEntity] = GetImguiControledFloat(0)
			--directions[imguiControledEntity] = GetImguiControledFloat(1)
			imguiControledEntity = -1
		end
		levelnum = GetLevelNumber()
		if levelTitles[levelnum] ~= nil then
			DrawText(levelTitles[levelnum], 0.5, 10, 40, 254/255, 252/255, 208/255)
		end
		if losingPlayer ~= nil then
			DrawText("TRY AGAIN", 3.25, 30,350, 150/255,0,0)
			DrawText("Press Space", 1, 30,550, 150/255,0,0)
		end
	end
	if eventData.type == 12 then --collison
		if airTime[eventData.e1] > 0.5 then
			x,y,z = GetPosition(eventData.e1)
			dx,dy,dz = GetMovementDirection(eventData.e1)
			dist = GetRadius(eventData.e1)
			nx,ny,nz = GetPlaneNormal(eventData.e2)

			if nx then
				x = x - nx * dist
			end
			if ny then
				y = y - ny * dist
			end
			if nz then
				z = z - nz * dist
			end
            --                 pos      dir   spr  part
			--SendParticleEvent(2, x,y,z, dx,dy,dz, 1,   "")
		end
		airTime[eventData.e1] = 0
		types = {}
		--print("types of other object")
		--print(types[4])
		types = GetCategorysOfEntity(eventData.e2)
		if types[4] == nil then --its not a faith plate
			decaying[eventData.e1] = 1.0
			--LOG_INFO("decaying")
		else 
			decaying[eventData.e1] = 0.0
			--LOG_INFO("not decaying")
		end
	end

	if eventData.type == 12 then	-- TODO invoking AddPhysicsVelocity causes the program to return "body not found"
	    -- LOG_INFO("friction")
		local fric = 20

		local e = eventData.e1
		local data = {}
		data = GetRigidData(e)


		if data.velocity.x > 0 then
			if data.velocity.x - fric * eventData.floatData1 < 0 then
				SetPhysicsVelocity(e, 0, data.velocity.y, data.velocity.z)
			else
				AddPhysicsVelocity(e, -fric * eventData.floatData1, 0, 0)
			end
		end

		if data.velocity.x < 0 then
			if data.velocity.x + fric * eventData.floatData1 > 0 then
				SetPhysicsVelocity(e, 0, data.velocity.y, data.velocity.z)
			else
				AddPhysicsVelocity(e, fric * eventData.floatData1, 0, 0)
			end
		end
		
	end
		
	if eventData.type == 19 then
		if eventData.stringData1 == "Space" then
			if losingPlayer ~= nil then
				losingPlayer = 0
			end
		end
	end

end



function HandleEventPerEntityPlayer(e, eventData)
	data = {}
	data = GetRigidData(e)

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

			-- Gravity reduction while holding up
			if data.velocity.y > 0 then
				AddPhysicsVelocity(e, 0, eventData.floatData1 * 30, 0)
			end

			-- Jump
			if airTime[e] < .02 then
				if data.velocity.y < jumpspeed then
					AddPhysicsVelocity(e, 0, jumpspeed-data.velocity.y, 0)
					PlayAudioEvent("BallJump")
				end
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

