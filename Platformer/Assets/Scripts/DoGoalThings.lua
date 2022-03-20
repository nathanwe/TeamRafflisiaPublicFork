--Goalthings
local imguiControledEntity = -1
winningGoal = nil;

function SaveGoals( levelnum )
	levelstr = string.format("%i", levelnum)
end

function LoadGoals( levelnum )
	levelstr = string.format("%i", levelnum)
	winningGoal = nil
end

function ClearGoals()
	--cooldown = {}
end

function DestroyGoal(e)
	--cooldown[e] = nil
end

function UpdateGoal(dt, e)
	if winningGoal ~= nil  then
		winningGoal = winningGoal -dt
	end
	if winningGoal ~= nil and winningGoal < 0 then
		LoadNextLevel()
	end
end

function HandleEventGoal(eventData)

	if eventData.type == 16 then
		ImguiText("Goal")
		--ImguiControledFloat(0, "cooldown", cooldown[eventData.e1])
		imguiControledEntity = eventData.e1

	end
	if eventData.type == 17 then
		if imguiControledEntity ~= -1 then
			--cooldown[imguiControledEntity] = GetImguiControledFloat(0)
			--directions[imguiControledEntity] = GetImguiControledFloat(1)
			imguiControledEntity = -1
		end
		if winningGoal ~= nil then
			DrawText("YOU WIN", 3.5, 75,350, 0,150,0)
		end
	end
	if eventData.type == 12 then
		if losingPlayer == nil then
			LOG_INFO("you win loading next level")
			winningGoal = 5
		end
		--LoadNextLevel()
	end
end



function HandleEventPerEntityGoal(e, eventData)

end
