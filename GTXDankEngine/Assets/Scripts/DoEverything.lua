--doeverything
categoryNames = {[1] = "Pokeball", [3] = "Lion", [4] = "Light"}
deltaTime = 0
thisEntity = 0
gEventData = {}

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
	for index,name in pairs(categoryNames) do
		LoadScript("Assets/Scripts/Do"..name.."Things.lua")
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
		gEventData = eventData
		DoStringWithErrorCheck("HandleEvent".. categoryNames[thing] .."(gEventData)")
	end
end

function HandleEventPerEntity(e, categoriesToEffect, eventData)
	for index,thing in pairs(categoriesToEffect) do
		thisEntity = e
		gEventData = eventData
		DoStringWithErrorCheck("HandleEventPerEntity".. categoryNames[thing] .."(thisEntity, gEventData)")
	end		

end


