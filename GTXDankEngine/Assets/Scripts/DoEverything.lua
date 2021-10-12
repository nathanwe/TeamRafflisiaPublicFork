--doeverything
categoryNames = {[3] = "Lion", [4] = "Light"}
deltaTime = 0
thisEntity = 0


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
