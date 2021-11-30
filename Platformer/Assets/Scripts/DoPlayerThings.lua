--Playerthings
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

end

function DestroyPlayer(e)

end

function UpdatePlayer(dt, e)

end

function HandleEventPlayer(eventData)
	if eventData.type == 16 then
		ImguiText("Player")
		imguiControledEntity = eventData.e1
	end
	if eventData.type == 8 then
		if eventData.stringData1 == "Space" then
			target = Raycast()
			--print("target", target)
			if target ~= -1 then
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
			AddToVQS(e, speed * eventData.floatData1 * newx, speed * eventData.floatData1 * newz, 0)
		end
		if eventData.stringData1 == "Down" then
			AddToVQS(e, -speed * eventData.floatData1 * newx,  -speed * eventData.floatData1 * newz, 0)
		end
		if eventData.stringData1 == "Left" then
			AddToVQS(e, speed * eventData.floatData1 * newz, 0, -speed * eventData.floatData1 * newx)
		end
		if eventData.stringData1 == "Right" then
			AddToVQS(e, -speed * eventData.floatData1 * newz, 0, speed * eventData.floatData1 * newx)
		end
	end
end
