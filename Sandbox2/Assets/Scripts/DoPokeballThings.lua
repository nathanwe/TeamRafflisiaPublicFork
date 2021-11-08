--pokeballthings

function InitPokeball(e)

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
		testtable = {}
		testtable[1] = -1
		testtable[3] = 3.14159
		print("about to save")
		SaveIntFloatTableAsJson(testtable, "/Assets/Levels/testLuaSave.json")
		tabletest = {}
		tabletest = LoadIntFloatTableFromJson("/Assets/Levels/testLuaSave.json")
		print(tabletest[1])
		print(tabletest[3])
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
