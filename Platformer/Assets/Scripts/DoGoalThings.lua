--Goalthings
local imguiControledEntity = -1
winningGoal = nil;
local isWin = false;

function SaveGoals( levelnum )
	levelstr = string.format("%i", levelnum)
end

function LoadGoals( levelnum )
	levelstr = string.format("%i", levelnum)
	winningGoal = nil
	isWin=false
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
			DrawText("YOU WIN", 3.5, GetCameraWidth()/2-500,GetCameraHeight()/2-50, 0,150/255,0)
			DrawText("Press Left Ctrl to Continue", 1, GetCameraWidth()/2-490,GetCameraHeight()/2+90, 0,150/255,0)
			if isWin == false then
				isWin=true
				PlayAudioEvent("Win")
			end
		end
	end
	if eventData.type == 12 then
		DeleteEntity(eventData.e2)
		if losingPlayer == nil then
			--LOG_INFO("you win loading next level")
			winningGoal = 99
		end
		--LoadNextLevel()
	end
	if eventData.type == 19 then
		if eventData.stringData1 == "Space" then
			--GoHomesBall()
		end
		if eventData.stringData1 == "Ctrl" then
			if winningGoal ~= nil then
				winningGoal = 0
			end
		end
	end
end



function HandleEventPerEntityGoal(e, eventData)

end
