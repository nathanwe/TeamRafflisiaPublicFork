--Starthings
local imguiControledEntity = -1
local starStatus = {} -- 0.0 = new, 1.0 = collected


function SaveStars( levelnum )
	levelstr = string.format("%i", levelnum)
	SaveIntFloatTableAsJson(starStatus, "/Assets/Levels/Level" .. levelstr .."/starStatusSave.json")
	
end

function LoadStars( levelnum )
	levelstr = string.format("%i", levelnum)
	starStatus = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."/starStatusSave.json")
end

function ClearStars()
	starStatus = {}
end

function DestroyStar(e)
	starStatus[e] = nil
end

function UpdateStar(dt, e)
	if starStatus[e] == nil then
		starStatus[e] = 0.0
	end
	if CheckAnyCollision(e) then
		starStatus[e] = 1.0
		SetColliderShape(e, 3)
		SetWireFrame(e, true)
	end
end

function HandleEventStar(eventData)

	if eventData.type == 16 then
		ImguiText("Star")
		imguiControledEntity = eventData.e1
		if ButtonImgui("Reset Star", 100,50) then
			starStatus[eventData.e1] = 0.0
			SetColliderShape(eventData.e1, 2)
			SetWireFrame(eventData.e1, false)
		end
		ImguiControledFloat(0, "starStatus", starStatus[eventData.e1])
	end
	if eventData.type == 17 then
		if imguiControledEntity ~= -1 then
			starStatus[imguiControledEntity] = GetImguiControledFloat(0)
			imguiControledEntity = -1
		end
		sum = 0
		for index,value in pairs(starStatus) do
			if value == 1.0 then
				sum = sum +1
			end
		end
		if sum == 1 then
			DrawText(sum.." Star Collected", .7, 710,60, 1,1,0)
		end
		if sum > 1 then
			DrawText(sum.." Stars Collected", .7, 710,60, 1,1,0)
		end
	end
	if eventData.type == 12 then
		--starStatus[eventData.e1] = 1.0
		--SetColliderShape(eventData.e1, 3)
		--SetWireFrame(eventData.e1, true)
	end
end



function HandleEventPerEntityStar(e, eventData)

end
