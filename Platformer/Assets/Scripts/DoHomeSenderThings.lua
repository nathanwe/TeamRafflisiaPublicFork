--HomeSenderthings
local imguiControledEntity = -1
local cooldown = {}
function SaveHomeSenders( levelnum )
	levelstr = string.format("%i", levelnum)
	SaveIntFloatTableAsJson(cooldown, "/Assets/Levels/Level" .. levelstr .."/HomeSendercooldownSave.json")
end

function LoadHomeSenders( levelnum )
	levelstr = string.format("%i", levelnum)
	cooldown = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."/HomeSendercooldownSave.json")
end

function ClearHomeSenders()
	cooldown = {}
end

function DestroyHomeSender(e)
	cooldown[e] = nil
end

function UpdateHomeSender(dt, e)
	if cooldown[e] == nil then
		cooldown[e] = 0;
	end
	cooldown[e] = cooldown[e] + dt
end

function HandleEventHomeSender(eventData)

	if eventData.type == 16 then
		ImguiText("HomeSender")
		--ImguiControledFloat(0, "cooldown", cooldown[eventData.e1])
		imguiControledEntity = eventData.e1

	end
	if eventData.type == 17 then
		if imguiControledEntity ~= -1 then
			--cooldown[imguiControledEntity] = GetImguiControledFloat(0)
			--directions[imguiControledEntity] = GetImguiControledFloat(1)
			imguiControledEntity = -1
		end
	end
	if eventData.type == 12 then
		if cooldown[eventData.e1] > 1 then
			GoHomesBall()
			cooldown[eventData.e1] = 0
		end
	end
end



function HandleEventPerEntityHomeSender(e, eventData)

end
