--pokeballthings

function InitPokeball(e)

end

function DestroyPokeball(e)

end

function UpdatePokeball(dt, e)

end

function HandleEventPokeball(EventData)
	if EventData.type == 0 then
		x,y,z = GetPosition(EventData.e1)
		MakeLionByHand(x,y,z)
	end
end

function HandleEventPerEntityPokeball(e, EventData)
	if EventData.type == 0 then
		x,y,z = GetPosition(e)
		MakeLionByHand(x,y,z)
	end
end
