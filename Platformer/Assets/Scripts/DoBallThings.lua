--Ballthings
local imguiControledEntity = -1
local homeX = {}
local homeY = {}
local homeZ = {}
local homeVelX = {}
local homeVelY = {}
local homeVelZ = {}


function SaveBalls( levelnum )
	levelstr = string.format("%i", levelnum)
	SaveIntFloatTableAsJson(homeX, "/Assets/Levels/Level" .. levelstr .."/BallHomeXSave.json")
	SaveIntFloatTableAsJson(homeY, "/Assets/Levels/Level" .. levelstr .."/BallHomeYSave.json")
	SaveIntFloatTableAsJson(homeZ, "/Assets/Levels/Level" .. levelstr .."/BallHomeZSave.json")
	SaveIntFloatTableAsJson(homeVelX, "/Assets/Levels/Level" .. levelstr .."/BallHomeVelXSave.json")
	SaveIntFloatTableAsJson(homeVelY, "/Assets/Levels/Level" .. levelstr .."/BallHomeVelYSave.json")
	SaveIntFloatTableAsJson(homeVelZ, "/Assets/Levels/Level" .. levelstr .."/BallHomeVelZSave.json")
end

function LoadBalls( levelnum )
	levelstr = string.format("%i", levelnum)
	homeX = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."/BallHomeXSave.json")
	homeY = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."/BallHomeYSave.json")
	homeZ = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."/BallHomeZSave.json")
	homeVelX = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."/BallHomeVelXSave.json")
	homeVelY = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."/BallHomeVelYSave.json")
	homeVelZ = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."/BallHomeVelZSave.json")
end

function ClearBalls()
	--timers = {}
	--directions = {}
end

function DestroyBall(e)
	--timers[e] = nil;
	--directions[e] = nil
end


function UpdateBall(dt, e)
	x,y,z = GetPosition(e)
	--print("UpdateBall", e, y)
	--if y < 0 then
	--	ent = CreateEntity("ball")
	--	LOG_INFO("created", ent)
	--	DeleteEntity(e)
	--end
	data = {}
	data = GetRigidData(e)
	AddRotation(e, 0,0,-data.velocity.x*dt*20)
end

function HandleEventBall(eventData)
	if eventData.type == 5 then
		DestroyBall(eventData.e1)
	end
	if eventData.type == 16 then
		ImguiText("Ball")
		ImguiControledFloat(0, "homeX", homeX[eventData.e1])
		ImguiControledFloat(1, "homeY", homeY[eventData.e1])
		ImguiControledFloat(2, "homeZ", homeZ[eventData.e1])
		ImguiControledFloat(3, "velx", homeVelX[eventData.e1])
		ImguiControledFloat(4, "vely", homeVelY[eventData.e1])
		ImguiControledFloat(5, "velz", homeVelZ[eventData.e1])
		imguiControledEntity = eventData.e1
	end
	if eventData.type == 17 then
		if imguiControledEntity ~= -1 then
			homeX[imguiControledEntity] = GetImguiControledFloat(0)
			homeY[imguiControledEntity] = GetImguiControledFloat(1)
			homeZ[imguiControledEntity] = GetImguiControledFloat(2)
			homeVelX[imguiControledEntity] = GetImguiControledFloat(3)
			homeVelY[imguiControledEntity] = GetImguiControledFloat(4)
			homeVelZ[imguiControledEntity] = GetImguiControledFloat(5)
			imguiControledEntity = -1
		end
	end
	if eventData.type == 19 then
		--LOG_INFO("pressed key")
		if eventData.stringData1 == "Space" then
			for index,value in pairs(homeX) do
				--LOG_INFO("moved" .. homeX[index] .. homeY[index] .. homeZ[index])
				SetPosition(index, homeX[index], homeY[index], homeZ[index])
				SetPhysicsVelocity(index, homeVelX[index], homeVelY[index], homeVelZ[index])
			end
		end
	end
end


function HandleEventPerEntityBall(e, eventData)
	if eventData.type == 7 then
		DeleteEntity(e)
	end
end

--custom functions