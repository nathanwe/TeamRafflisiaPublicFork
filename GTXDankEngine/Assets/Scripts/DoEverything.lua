--doeverything
categoryNames = {}
deltaTime = 0
thisEntity = 0
gEventData = {}
GAME_PATH = ""

function DoStringWithErrorCheck(str)
	local tempfunc
	local errmsg
	tempfunc, errmsg = load(str)
	if (errmsg) then
		print(errmsg)
	else
		--print("string done ".. str)
		tempfunc()
	end
end


function Init()
	GAME_PATH = GetGamePath()
	LoadScript(GAME_PATH .."Assets/Scripts/CategoryList.lua")
	categoryNames = GetCategoryNames()
	for index,name in pairs(categoryNames) do
		LoadScript(GAME_PATH .."Assets/Scripts/Do"..name.."Things.lua")
		--DoStringWithErrorCheck("Update".. name .."(deltaTime, e)")
	end

end

function Update(dt)
	deltaTime = dt
	UpdateAllEntitys()
end

function UpdateEntity( e )
	local thisEntiysCategories = {}
	thisEntiysCategories = GetCategorysOfEntity(e)
	for index,cat in pairs(thisEntiysCategories) do
		if categoryNames[cat] ~= nil then
			thisEntity = e
			DoStringWithErrorCheck("Update".. categoryNames[cat] .."(deltaTime, thisEntity)")
		end
	end
end

function HandleEvent(thingsToEffect, eventData)
	for index,thing in pairs(thingsToEffect) do
		if categoryNames[thing] ~= nil then
			gEventData = eventData
			if eventData.type == 5 then
				DoStringWithErrorCheck("Destroy" .."(gEventData.e1)")
			elseif eventData.type == 6 then
				DoStringWithErrorCheck("Clear".. categoryNames[thing] .."s()")
			elseif eventData.type == 13 then
				DoStringWithErrorCheck("Save".. categoryNames[thing] .."s(gEventData.intData1)")
			elseif eventData.type == 14 then
				DoStringWithErrorCheck("Load".. categoryNames[thing] .."s(gEventData.intData1)")
			else
				DoStringWithErrorCheck("HandleEvent".. categoryNames[thing] .."(gEventData)")
			end
		end
	end
end

function HandleEventPerEntity(e, categoriesToEffect, eventData)
	for index,thing in pairs(categoriesToEffect) do
		thisEntity = e
		gEventData = eventData
		--print("HandleEventPerEntity for " .. e)
		DoStringWithErrorCheck("HandleEventPerEntity".. categoryNames[thing] .."(thisEntity, gEventData)")
	end		
end

function Destroy()

end

