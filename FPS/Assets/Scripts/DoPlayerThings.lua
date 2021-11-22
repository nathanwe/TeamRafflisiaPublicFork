--Playerthings
local imguiControledEntity = -1

function SavePlayer( levelnum )
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
end

function HandleEventPerEntityPlayer(e, eventData)

	if eventData.type == 8 then
		if eventData.stringData1 == "Up" then
			AddToVQS(e, 0, 10 *eventData.floatData1, 0)
		end
		if eventData.stringData1 == "Down" then
			AddToVQS(e, 0, -10 * eventData.floatData1, 0)
		end
		if eventData.stringData1 == "Left" then
			AddToVQS(e, -10 * eventData.floatData1, 0, 0)
		end
		if eventData.stringData1 == "Right" then
			AddToVQS(e, 10 * eventData.floatData1, 0, 0)
		end
	end
end
