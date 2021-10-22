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
end
