--Goalthings
local imguiControledEntity = -1
local winning = nil;

function SaveGoals( levelnum )
	levelstr = string.format("%i", levelnum)
end

function LoadGoals( levelnum )
	levelstr = string.format("%i", levelnum)
end

function ClearGoals()
	--bouncyness = {}
end

function DestroyGoal(e)
	--bouncyness[e] = nil
end

function UpdateGoal(dt, e)
	if winning ~= nil  then
		winning = winning -dt
	end
	if winning ~= nil and winning < 0 then
		winning = nil
		LoadNextLevel()
	end
end

function HandleEventGoal(eventData)

	if eventData.type == 16 then
		ImguiText("Goal")
		--ImguiControledFloat(0, "bouncyness", bouncyness[eventData.e1])
		imguiControledEntity = eventData.e1

	end
	if eventData.type == 17 then
		if imguiControledEntity ~= -1 then
			--bouncyness[imguiControledEntity] = GetImguiControledFloat(0)
			--directions[imguiControledEntity] = GetImguiControledFloat(1)
			imguiControledEntity = -1
		end
		if winning ~= nil then
			DrawText("YOU WIN!", 5, 50,350, 0,150,0)
		end
	end
	if eventData.type == 12 then
		LOG_INFO("you win loading next level")
		winning = 5
		--LoadNextLevel()
	end
end



function HandleEventPerEntityGoal(e, eventData)

end
