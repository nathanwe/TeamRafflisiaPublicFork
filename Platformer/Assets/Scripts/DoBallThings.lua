--Ballthings
local imguiControledEntity = -1
--local onlymake1 = true

function SaveBalls( levelnum )
	levelstr = string.format("%i", levelnum)
	--SaveIntFloatTableAsJson(timers, "/Assets/Levels/Level" .. levelstr .."BallTimerSave.json")
	--SaveIntFloatTableAsJson(directions, "/Assets/Levels/Level" .. levelstr .."BallDirectionSave.json")
end

function LoadBalls( levelnum )
	levelstr = string.format("%i", levelnum)
	--timers = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."BallTimerSave.json")
	--directions = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."BallDirectionSave.json")
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
	if y < 0 then
		ent = CreateEntity("ball")
		LOG_INFO("created", ent)
		DeleteEntity(e)
	end

end

function HandleEventBall(eventData)
	if eventData.type == 5 then
		DestroyBall(eventData.e1)
	end
	if eventData.type == 16 then
		ImguiText("Ball")
		--ImguiControledFloat(0, "timers", timers[eventData.e1])
		--ImguiControledFloat(1, "directions", directions[eventData.e1])
		imguiControledEntity = eventData.e1
	end
	if eventData.type == 17 then
		if imguiControledEntity ~= -1 then
			--timers[imguiControledEntity] = GetImguiControledFloat(0)
			--directions[imguiControledEntity] = GetImguiControledFloat(1)
		imguiControledEntity = -1
		end
	end
end

function HandleEventPerEntityBall(e, eventData)
	if eventData.type == 7 then
		DeleteEntity(e)
	end
end

--custom functions
