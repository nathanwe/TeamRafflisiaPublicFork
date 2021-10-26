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
		MakeLionByHand(x,y,z)
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
