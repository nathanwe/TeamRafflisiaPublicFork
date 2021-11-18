--pokeballthings
local imguiControledEntity = -1

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

function HandleEventPokeball(eventData)
	if eventData.type == 16 then
		ImguiText("Pokeball")
		imguiControledEntity = eventData.e1
	end
end

function HandleEventPerEntityPokeball(e, eventData)
	if eventData.type == 4 then
		x,y,z = GetPosition(e)
		ent = CreateEntity("LION")
		SetPosition(ent,x,y,z)
	end
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
