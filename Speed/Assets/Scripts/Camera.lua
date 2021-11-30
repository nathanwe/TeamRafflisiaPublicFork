--Camera

function Init()
	SetCameraPitchYaw(0, 3.14)
	Set3rdPerson(true)
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