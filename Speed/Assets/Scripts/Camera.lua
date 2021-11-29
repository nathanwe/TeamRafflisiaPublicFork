--Camera

function Init()
	Set3rdPerson(false)
	Set3rdPersonDistance(10)
end

function Update(dt)
	UpdateCameraOld()
end

function HandleEvent(thingsToEffect, eventData)
end

function HandleEventPerEntity(e, categoriesToEffect, eventData)
end

function Destroy()
end