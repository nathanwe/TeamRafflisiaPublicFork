--pokeballthings
function SavePokeball( levelnum )
	levelstr = string.format("%i", levelnum)
	--SaveIntFloatTableAsJson(timers, "/Assets/Levels/Level" .. levelstr .."PokeballTimerSave.json")
	--SaveIntFloatTableAsJson(directions, "/Assets/Levels/Level" .. levelstr .."PokeballDirectionSave.json")
end

function LoadPokeballs( levelnum )
	levelstr = string.format("%i", levelnum)
	--timers = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."PokeballTimerSave.json")
	--directions = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."PokeballDirectionSave.json")
end

function ClearPokeballs()

end

function DestroyPokeball(e)

end

function UpdatePokeball(dt, e)

end

function HandleEventPokeball(EventData)

end

function HandleEventPerEntityPokeball(e, EventData)
	if EventData.type == 4 then
		x,y,z = GetPosition(e)
		ent = CreateEntity("LION")
		SetPosition(ent,x,y,z)
	end
	if EventData.type == 8 then
		if EventData.stringData1 == "Up" then
			AddToVQS(e, 0, 10 *EventData.floatData1, 0)
		end
		if EventData.stringData1 == "Down" then
			AddToVQS(e, 0, -10 * EventData.floatData1, 0)
		end
		if EventData.stringData1 == "Left" then
			AddToVQS(e, -10 * EventData.floatData1, 0, 0)
		end
		if EventData.stringData1 == "Right" then
			AddToVQS(e, 10 * EventData.floatData1, 0, 0)
		end
	end
end
